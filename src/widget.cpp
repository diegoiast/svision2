/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "widget.h"
#include "platform.h"
#include "theme.h"

#include <spdlog/spdlog.h>

static auto point_in_rect(Position p, Size s, int x, int y) -> bool {
    if (x < p.x)
        return false;
    if (y < p.y)
        return false;
    if (p.x + s.width < x)
        return false;
    if (p.y + s.height < y)
        return false;
    return true;
}

auto WidgetCollection::add(std::shared_ptr<Widget> widget, PlatformWindow *window)
    -> std::shared_ptr<Widget> {
    widgets.push_back(widget);
    widget->window = window;
    if (widget->focus_index < 0) {
        if (window) {
            // FIXME - this seems wierd, should I be manipulating window's internals...?
            widget->focus_index = max_focus_index;
            widget->theme = window->theme;
        } else {
            widget->focus_index = max_focus_index;
        }
        max_focus_index++;
    }
    spdlog::info("New sub widget {}", fmt::ptr(widget.get()));
    return widget;
}

auto WidgetCollection::on_mouse(const EventMouse &event) -> void {
    auto widget_under_mouse = std::shared_ptr<Widget>();
    for (auto w : this->widgets) {
        auto child_event = event;
        if (point_in_rect(w->position, w->content.size, event.x, event.y)) {
            child_event.x = event.x - w->position.x;
            child_event.y = event.y - w->position.y;
            child_event.is_local = true;
            widget_under_mouse = w;
        }

        if (child_event.is_local || w->read_external_mouse_events) {
            w->on_mouse(child_event);
            switch (event.type) {
            case MouseEvents::Release:
                   on_mouse_release(event, w);
                break;
            case MouseEvents::Press:
                on_mouse_press(event, w);
                break;
            case MouseEvents::MouseMove:
                // TODO: is this the correct event?
                on_mouse_press(event, w);
                break;
            case MouseEvents::Unknown:
                break;
            }
        }
    }

    if (last_overed_widget != widget_under_mouse) {
        if (last_overed_widget) {
            last_overed_widget->on_mouse_leave();
        }
        last_overed_widget = widget_under_mouse;
        if (last_overed_widget) {
            last_overed_widget->on_mouse_enter();
        }
    }
}

auto WidgetCollection::on_mouse_release(const EventMouse &event, std::shared_ptr<Widget> w)
    -> void {
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
    w->on_mouse_click(click_event);
}

auto WidgetCollection::on_mouse_press(const EventMouse &event, std::shared_ptr<Widget> w) -> void {
    if (!point_in_rect(w->position, w->content.size, event.x, event.y)) {
        return;
    }

    auto local_event = event;
    local_event.is_local = true;
    local_event.x = event.x - w->position.x;
    local_event.y = event.y - w->position.y;
    if (last_overed_widget && last_overed_widget != w) {
        spdlog::info("Mouse focus changed from {} to {}", fmt::ptr(last_overed_widget.get()),
                     fmt::ptr(w.get()));
        last_overed_widget->on_mouse_leave();
        w->needs_redraw |= last_overed_widget->needs_redraw;
    }
    if (!w->mouse_over) {
        w->on_mouse_enter();
    }
    if (event.type == MouseEvents::MouseMove) {
        w->on_hover(local_event);
    } else {
        w->on_mouse_click(local_event);
    }
    last_overed_widget = w;

    if (event.type == MouseEvents::Press) {
        if (!w->has_focus) {
            w->invalidate();

            if (!w->parent) {
                // TODO - how should we handle this on sub widgets?
                // ask window for focus - only for op level widgets
                w->window->focus_widget(w);
            }
        }
    }
}

auto WidgetCollection::focus_next_widget() -> void {
    auto last_focus_index = -1;

    if (focused_widget)
        last_focus_index = focused_widget->focus_index;

    std::shared_ptr<Widget> best_focus_widget;
    std::shared_ptr<Widget> least_focus_widget;
    for (auto w : this->widgets) {
        if (!w->can_focus) {
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
        } else
            spdlog::info("No widget in focus");
    }

    focus_widget(best_focus_widget);
}

auto WidgetCollection::focus_previous_widget() -> void {
    auto last_focus_index = -1;

    if (focused_widget)
        last_focus_index = focused_widget->focus_index;

    std::shared_ptr<Widget> best_focus_widget;
    std::shared_ptr<Widget> least_focus_widget;
    for (auto w : this->widgets) {
        if (!w->can_focus) {
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
        } else
            spdlog::info("No widget in focus");
    }

    focus_widget(best_focus_widget);
}

auto WidgetCollection::focus_widget(std::shared_ptr<Widget> widget) -> void {
    if (focused_widget && widget == focused_widget) {
        return;
    }

    if (!widget->can_focus) {
        spdlog::info("Not focusing widget, has it has a no focus policy");
        return;
    }

    if (this->focused_widget) {
        spdlog::info("widget {} lost focus", focused_widget->focus_index);
        this->focused_widget->on_focus_change(false);
        this->focused_widget->has_focus = false;
        // TODO - we had this code - is it needed? probably not
        // last_focus_index = this->focus_widget->focus_index;
        widget->needs_redraw |= focused_widget->needs_redraw;
        this->focused_widget.reset();
    }

    spdlog::info("widget {} got focus", widget->focus_index);
    widget->on_focus_change(true);
    widget->has_focus = true;
    // TODO - we had this code - is it needed? probably not
    // last_focus_index = widget->focus_index;
    focused_widget = widget;
}

Widget::Widget(Position position, Size size, uint32_t color) {
    this->position = position;
    this->content.background_color = color;
    this->content.resize(size);
}

Widget::~Widget() {}

auto Widget::invalidate() -> void {
    this->needs_redraw = true;
    if (this->window) {
        window->invalidate();
    }
    if (this->parent) {
        parent->invalidate();
    }
}

auto Widget::draw() -> void {
    for (auto w : widgets.widgets) {
        if (w->needs_redraw) {
            w->draw();
            w->needs_redraw = false;
        }
        content.draw(w->position, w->content);
    }
}

auto Widget::on_mouse(const EventMouse &event) -> void { widgets.on_mouse(event); }

auto Widget::on_hover(const EventMouse &event) -> void {}

auto Widget::on_mouse_enter() -> void {}

auto Widget::on_mouse_leave() -> void {}

auto Widget::on_mouse_click(const EventMouse &event) -> void {}

auto Widget::get_theme() -> std::shared_ptr<Theme> {
    if (theme)
        return theme;
    if (parent && parent->theme)
        return parent->theme;
    return window->theme;
}

PlatformWindow::~PlatformWindow() { spdlog::info("Window done"); }

auto PlatformWindow::draw() -> void {
    if (content.background_color != 0)
        content.fill_rect(0, 0, content.size.width, content.size.height, content.background_color);
    else
        theme->draw_window_background(content);

    for (auto w : widgets.widgets) {
        if (w->needs_redraw) {
            w->draw();
            w->needs_redraw = false;
        }
        content.draw(w->position, w->content);

        if (w->has_focus) {
            if (theme->needs_frame_for_focus()) {
                auto offset = 3;
                auto p = w->position - offset;
                auto s = w->content.size + offset * 2;
                content.draw_rectangle(p.x, p.y, s.width, s.height, 0x808080, 0x808080);
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
            if (event.is_shift_pressed())
                focus_previous_widget();
            else
                focus_next_widget();
            break;
        default:
            if (widgets.focused_widget) {
                widgets.focused_widget->on_keyboard(event);
                needs_redraw |= widgets.focused_widget->needs_redraw;
            }
            break;
        }
    }
}

auto PlatformWindow::on_mouse(const EventMouse &event) -> void { widgets.on_mouse(event); }

auto PlatformWindow::invalidate() -> void {
    assert(platform);
    platform->invalidate(*this);
};

auto PlatformWindow::on_close() -> void {
    for (auto w : widgets.widgets) {
        if (w) {
            w->on_remove();
        }
    }
    widgets.widgets.clear();
}
