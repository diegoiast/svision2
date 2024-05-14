/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "widget.h"
#include "layout.h"
#include "platform.h"
#include "theme.h"

#include <spdlog/spdlog.h>

static auto point_in_rect(Position p, Size s, int x, int y) -> bool {
    if (x < p.x) {
        return false;
    }
    if (y < p.y) {
        return false;
    }
    if (p.x + s.width < x) {
        return false;
    }
    if (p.y + s.height < y) {
        return false;
    }
    return true;
}

static auto dump_widget(const std::shared_ptr<Widget> w) -> void {
    auto hint = w->size_hint();
    auto padding = w->get_padding();
    spdlog::info("-------- widget = class = {}, index {}", typeid(*w).name(), w->focus_index);
    spdlog::info("size      = {}x{}", w->content.size.width, w->content.size.height);
    spdlog::info("position  = {}x{}", w->position.x, w->position.y);
    spdlog::info("size hint = {}x{}", hint.width, hint.height);
    spdlog::info("padding   = {}x{}", padding.get_horizontal(), padding.get_horizontal());
    spdlog::info("=====================");
}

auto WidgetCollection::add(std::shared_ptr<Widget> widget, PlatformWindow *window)
    -> std::shared_ptr<Widget> {
    widgets.push_back(widget);
    if (window == nullptr) {
        spdlog::warn("Adding widget without window");
    }
    widget->window = window;
    if (widget->focus_index < 0) {
        if (window) {
            widget->focus_index = max_focus_index;
            widget->theme = window->main_widget.theme;
        } else {
            widget->focus_index = max_focus_index;
        }
        max_focus_index++;
    }
    return widget;
}

// TODO - if we move this widget collection back into widget, the last argument is no longer needed
auto WidgetCollection::on_mouse(const EventMouse &event, Widget &myself) -> EventPropagation {
    auto widget_under_mouse = std::shared_ptr<Widget>();
    auto result = EventPropagation::propagate;

    // TODO - this just traverses in reverse order, we need to order widgets by z-index instead
    for (auto it = widgets.rbegin(); it != widgets.rend(); ++it) {
        auto w = *it;
        if (!w->is_visible()) {
            continue;
        }

        auto child_event = event;
        child_event.is_local = false;
        if (point_in_rect(w->position, w->content.size, event.x, event.y)) {
            child_event.x = event.x - w->position.x;
            child_event.y = event.y - w->position.y;
            child_event.is_local = true;
            widget_under_mouse = w;
        }

        if (child_event.is_local || w->read_external_mouse_events) {
            // propagate the event to sub-widgets
            auto b = w->on_mouse(child_event);
            if (b == EventPropagation::handled) {
                result = EventPropagation::handled;
            }

            // not handled by sub-widgets, send this event to the widget itself
            switch (event.type) {
            case MouseEvents::Release: {
                b = on_mouse_release(event, w);
                if (b == EventPropagation::handled) {
                    result = EventPropagation::handled;
                }
                break;
            }
            case MouseEvents::Press: {
                b = on_mouse_press(event, w);
                if (b == EventPropagation::handled) {
                    result = EventPropagation::handled;
                }
                break;
            }
            case MouseEvents::MouseMove: {
                // TODO: event name is bad, as this also handles mouse move
                b = on_mouse_press(event, w);
                if (b == EventPropagation::handled) {
                    result = EventPropagation::handled;
                }
                break;
            }
            case MouseEvents::Unknown:
                break;
            }
        }
        if (result == EventPropagation::handled) {
            break;
        }
    }
    if (last_overed_widget != widget_under_mouse) {
        if (last_overed_widget) {
            last_overed_widget->mouse_over = false;
            last_overed_widget->on_mouse_leave();
        }
        last_overed_widget = widget_under_mouse;
        if (last_overed_widget) {
            last_overed_widget->mouse_over = true;
            last_overed_widget->on_mouse_enter();
        } else {
            if (myself.window) {
                myself.window->set_cursor(myself.get_cursor());
            }
        }
    }
    return result;
}

auto WidgetCollection::on_mouse_release(const EventMouse &event, std::shared_ptr<Widget> w)
    -> EventPropagation {
    auto click_event = event;
    // is the release event is inside the widget -
    // we send him a "local event", with local coordinates.
    // otherwise - a "global event" - means the coordinates are relative
    // to the container window.
    if (point_in_rect(w->position, w->content.size, event.x, event.y)) {
        click_event.x = event.x - w->position.x;
        click_event.y = event.y - w->position.y;
        click_event.is_local = true;
    }
    return w->on_mouse_click(click_event);
}

auto WidgetCollection::on_mouse_press(const EventMouse &event, std::shared_ptr<Widget> w)
    -> EventPropagation {
    if (!point_in_rect(w->position, w->content.size, event.x, event.y)) {
        return EventPropagation::propagate;
    }

    EventPropagation result;
    auto local_event = event;
    local_event.is_local = true;
    local_event.x = event.x - w->position.x;
    local_event.y = event.y - w->position.y;
    if (last_overed_widget && last_overed_widget != w) {
        /*
        spdlog::info("Mouse focus changed from {} to {}", fmt::ptr(last_overed_widget.get()),
                     fmt::ptr(w.get()));
        */
        last_overed_widget->on_mouse_leave();
        last_overed_widget->mouse_over = false;
        last_overed_widget->needs_redraw = true;
        w->needs_redraw |= last_overed_widget->needs_redraw;
    }
    if (!w->mouse_over) {
        w->mouse_over = true;
        w->on_mouse_enter();

        if (w->window) {
            auto cursor = w->get_cursor();
            w->window->set_cursor(cursor);
        } else {
            spdlog::error("Widget without window!");
        }
    }
    if (event.type == MouseEvents::MouseMove) {
        w->on_hover(local_event);
        result = EventPropagation::handled;
    } else {
        if (!w->has_focus && w->window) {
            w->window->focus_widget(w);
        }
        result = w->on_mouse_click(local_event);
        if (debug && event.button == 2) {
            dump_widget(w);
        }
    }
    last_overed_widget = w;
    return result;
}

auto WidgetCollection::focus_next_widget() -> void {
    auto last_focus_index = -1;

    if (focused_widget) {
        last_focus_index = focused_widget->focus_index;
    }

    std::shared_ptr<Widget> best_focus_widget;
    std::shared_ptr<Widget> least_focus_widget;
    for (auto &w : this->widgets) {
        if (!w->can_focus || !w->is_visible()) {
            continue;
        }

        if (!least_focus_widget) {
            spdlog::info("Widget {} is now first (*)", w->focus_index);
            least_focus_widget = w;
        } else {
            if (w->focus_index < least_focus_widget->focus_index) {
                least_focus_widget = w;
                spdlog::info("Widget {} is now first", w->focus_index);
            }
        }

        if (w->focus_index <= last_focus_index) {
            continue;
        }
        if (!best_focus_widget) {
            best_focus_widget = w;
        } else {
            if (w->focus_index < best_focus_widget->focus_index) {
                best_focus_widget = w;
            }
        }
    }

    if (!best_focus_widget) {
        best_focus_widget = least_focus_widget;
        if (focused_widget) {
            spdlog::info("No best focus, keeping {}", focused_widget->focus_index);
        } else {
            spdlog::info("No widget in focus");
        }
    }

    focus_widget(best_focus_widget);
}

auto WidgetCollection::focus_previous_widget() -> void {
    auto last_focus_index = -1;

    if (focused_widget) {
        last_focus_index = focused_widget->focus_index;
    }

    std::shared_ptr<Widget> best_focus_widget;
    std::shared_ptr<Widget> least_focus_widget;
    for (auto &w : this->widgets) {
        if (!w->can_focus || !w->is_visible()) {
            continue;
        }

        if (!least_focus_widget) {
            spdlog::info("Widget {} is now last (*)", w->focus_index);
            least_focus_widget = w;
        } else {
            if (w->focus_index >= least_focus_widget->focus_index) {
                least_focus_widget = w;
                spdlog::info("Widget {} is now last", w->focus_index);
            }
        }

        if (w->focus_index >= last_focus_index) {
            continue;
        }
        if (!best_focus_widget) {
            best_focus_widget = w;
        } else {
            if (w->focus_index > best_focus_widget->focus_index) {
                best_focus_widget = w;
            }
        }
    }

    if (!best_focus_widget) {
        best_focus_widget = least_focus_widget;
        if (focused_widget) {
            spdlog::info("No best focus, keeping {}", focused_widget->focus_index);
        } else {
            spdlog::info("No widget in focus");
        }
    }

    focus_widget(best_focus_widget);
}

auto WidgetCollection::focus_widget(std::shared_ptr<Widget> widget) -> void {
    if (focused_widget && widget == focused_widget) {
        return;
    }

    if (!widget->can_focus || !widget->is_visible()) {
        spdlog::info("Not focusing widget, has it has a no focus policy");
        return;
    }

    if (this->focused_widget) {
        spdlog::info("widget {} lost focus", focused_widget->focus_index);
        this->focused_widget->on_focus_change(false);
        this->focused_widget->has_focus = false;
        this->focused_widget->needs_redraw = true;
        // TODO - we had this code - is it needed? probably not
        // last_focus_index = this->focus_widget->focus_index;
        widget->needs_redraw |= focused_widget->needs_redraw;
        this->focused_widget.reset();
    }

    spdlog::info("widget {} got focus", widget->focus_index);
    widget->on_focus_change(true);
    widget->has_focus = true;
    widget->is_widget_visible = true;
    widget->needs_redraw = true;
    // TODO - we had this code - is it needed? probably not
    // last_focus_index = widget->focus_index;
    this->focused_widget = widget;
}

Widget::Widget() : Widget(0) {}

Widget::Widget(uint32_t color) {
    this->position = {};
    this->content.background_color = color;
}

Widget::Widget(Position position, Size size, uint32_t color) {
    this->position = position;
    this->content.background_color = color;
    this->content.resize(size);
}

Widget::~Widget() {}

auto Widget::invalidate() -> void {
    if (this->needs_redraw) {
        return;
    }
    this->needs_redraw = true;
    if (this->parent && !this->parent->needs_redraw) {
        this->parent->invalidate();
    }
    if (this->window && !this->window->needs_redraw) {
        this->window->invalidate();
    }
}

auto Widget::draw() -> void {
    if (draw_background) {
        if (content.background_color != 0) {
            content.fill(content.background_color);
        } else {
            get_theme()->draw_widget_background(content, has_focus);
        }
    }
    for (auto &w : widgets.widgets) {
        if (!w->is_visible()) {
            continue;
        }
        if (w->needs_redraw) {
            w->draw();
            w->needs_redraw = false;
        }
        content.draw(w->position, w->content);
    }

    auto frame_proxy = this->frame;
    auto actual_theme = get_theme();
    if (can_focus && actual_theme->modify_frame_on_hover()) {
        // Setting hover frame works only on selectable widgets
        if (frame_proxy.style == FrameStyles::Normal ||
            frame_proxy.style == FrameStyles::Reversed) {
            if (this->has_focus) {
                // TODO - are we missing another frame style?
                frame_proxy.style = FrameStyles::Hover;
            } else if (this->mouse_over) {
                frame_proxy.style = FrameStyles::Hover;
            }
        }
    }
    if (frame_proxy.style != FrameStyles::NoFrame) {
        actual_theme->draw_frame(content, {0, 0}, content.size, frame_proxy.style,
                                 frame_proxy.size);
    }
}

auto Widget::on_mouse(const EventMouse &event) -> EventPropagation {
    return widgets.on_mouse(event, *this);
}

auto Widget::on_hover(const EventMouse &event) -> void {
    for (auto &w : widgets.widgets) {
        w->on_hover(event);
    }
};

auto Widget::on_mouse_enter() -> void {
    for (auto &w : widgets.widgets) {
        w->on_mouse_enter();
    }
    // TODO - this is not needed strictly, we can remove when we fix listview
    invalidate();
}

auto Widget::on_mouse_leave() -> void {
    for (auto &w : widgets.widgets) {
        w->on_mouse_leave();
    }
    // TODO - this is not needed strictly, we can remove when we fix listview
    invalidate();
}

auto Widget::on_mouse_click(const EventMouse &event) -> EventPropagation {
    (void)(event);
    return EventPropagation::propagate;
};

auto Widget::on_focus_change(bool new_state) -> void { (void)(new_state); };

auto Widget::on_keyboard(const EventKeyboard &) -> EventPropagation {
    return EventPropagation::propagate;
};

auto Widget::on_remove() -> void{};

auto Widget::on_resize() -> void {
    if (layout) {
        layout->relayout({0, 0}, this->content.size);
    }
}

auto Widget::get_theme() const -> std::shared_ptr<Theme> {
    if (theme) {
        return theme;
    }
    auto p = parent;
    while (p) {
        if (p->theme) {
            return p->theme;
        }
        p = p->parent;
    }
    if (window) {
        return window->main_widget.theme;
    }
    return {};
}

auto Widget::get_cursor() const -> MouseCursor {
    if (mouse_cursor != MouseCursor::Inherit) {
        return mouse_cursor;
    }
    auto p = parent;
    while (p) {
        if (p->mouse_cursor != MouseCursor::Inherit) {
            return p->mouse_cursor;
        }
        p = p->parent;
    }
    return MouseCursor::Inherit;
}

auto Widget::show() -> void {
    if (is_visible()) {
        return;
    }
    is_widget_visible = true;
    invalidate();
}

auto Widget::hide() -> void {
    if (!is_visible()) {
        return;
    }
    is_widget_visible = false;
    invalidate();
}

auto Widget::get_padding() const -> LayoutParams {
    if (padding.is_valid()) {
        return padding;
    }
    auto t = get_theme();
    if (t) {
        return t->get_padding(padding_style);
    }
    return {};
}

PlatformWindow::PlatformWindow() {
    main_widget.layout = std::make_shared<VerticalLayout>();
    main_widget.layout->padding.set_vertical(5);
    main_widget.layout->padding.set_horizontal(5);
    main_widget.mouse_cursor = MouseCursor::Normal;
    main_widget.window = this;
}

PlatformWindow::~PlatformWindow() { spdlog::info("Window done"); }

auto PlatformWindow::set_cursor(MouseCursor cursor) -> void {
    static auto currentCursor = MouseCursor::Inherit;
    if (overrideCursor != MouseCursor::Inherit) {
        cursor = overrideCursor;
    }

    assert(cursor != MouseCursor::Inherit);
    if (cursor == currentCursor) {
        return;
    }

    if (platform) {
        platform->set_cursor(*this, cursor);
    } else {
        spdlog::error("Window without platform!");
    }
    currentCursor = cursor;
}

auto PlatformWindow::set_override_cursor(MouseCursor cursor) -> void {
    if (this->overrideCursor == cursor) {
        return;
    }

    this->overrideCursor = cursor;
    set_override_cursor(cursor);
}

auto PlatformWindow::draw() -> void {
    if (main_widget.content.background_color != 0) {
        main_widget.content.fill(main_widget.content.background_color);
    } else {
        main_widget.theme->draw_window_background(main_widget.content);
    }

    for (auto &w : main_widget.widgets.widgets) {
        if (!w->is_visible()) {
            continue;
        }
        if (w->needs_redraw) {
            w->draw();
            w->needs_redraw = false;
        }
        main_widget.content.draw(w->position, w->content);
        if (w->has_focus) {
            if (main_widget.theme->needs_frame_for_focus()) {
                auto offset = 3;
                auto p = w->position - offset;
                auto s = w->content.size + offset * 2;
                main_widget.content.draw_rectangle(p.x, p.y, s.width, s.height, 0x808080, 0x808080);
            }
        }
    }
    needs_redraw = false;
}

auto PlatformWindow::on_keyboard(const EventKeyboard &event) -> void {
    if (event.keydown) {
        switch (event.key) {
        /*
        TODO - find button with defaults, and send it this event
        TODO - handle shortcuts on buttons
        case KeyCodes::Enter:
            break;
        case KeyCodes::Escape:
            break;
        */
        case KeyCodes::Tab:
            if (event.is_shift_pressed()) {
                focus_previous_widget();
            } else {
                focus_next_widget();
            }
            break;
        default:
            if (main_widget.widgets.focused_widget) {
                main_widget.widgets.focused_widget->on_keyboard(event);
                needs_redraw |= main_widget.widgets.focused_widget->needs_redraw;
            }
            break;
        }
    }
}

auto PlatformWindow::on_mouse(const EventMouse &event) -> void { main_widget.on_mouse(event); }

auto PlatformWindow::on_resize(const EventResize &event) -> void {
    spdlog::info("New window size: {}x{}", event.size.width, event.size.height);
    if (main_widget.layout) {
        main_widget.layout->relayout({0, 0}, event.size);
        invalidate();
    }
}

auto PlatformWindow::invalidate() -> void {
    assert(platform);
    this->needs_redraw = true;
    platform->invalidate(*this);
};

auto PlatformWindow::on_close() -> void {
    for (auto &w : main_widget.widgets.widgets) {
        if (w) {
            w->on_remove();
        }
    }
    main_widget.widgets.widgets.clear();
}
