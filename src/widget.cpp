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

Widget::Widget(Position position, Size size, uint32_t color) {
    this->position = position;
    this->content.background_color = color;
    this->content.resize(size);
}

Widget::~Widget() {}


auto Widget::invalidate() -> void
{
    this->needs_redraw = true;
    if (this->window) {
        window->invalidate();
    }
    if (this->parent) {
        parent->invalidate();
    }
}

auto Widget::draw() -> void {
    for (auto w : widgets) {
        if (w->needs_redraw) {
            w->draw();
            w->needs_redraw = false;
        }
        content.draw(w->position, w->content);
    }
}

auto Widget::on_hover(const EventMouse &event) -> void {
}

auto Widget::on_mouse_enter() -> void {
}

auto Widget::on_mouse_leave() -> void {
}

auto Widget::on_mouse_click(const EventMouse &event) -> void {
}

auto Widget::add(std::shared_ptr<Widget> widget) -> std::shared_ptr<Widget>
{
    widgets.push_back(widget);
    widget->theme = theme;
    widget->window = this->window;
    widget->parent = this;
    if (widget->focus_index < 0) {
        if (window) {
            // TODO - this seems wierd, should I be manipulating window's internals...?
            widget->focus_index = window->max_focus_index;
            window->max_focus_index++;
        } else {
            widget->focus_index = focus_index+1;
        }
    }
    spdlog::info("New sub widget {}", fmt::ptr(widget.get()));
    return widget;
}

PlatformWindow::~PlatformWindow() { spdlog::info("Window done"); }

auto PlatformWindow::select_next_widget() -> void {
    auto last_focus_index = -1;

    if (focus_widget)
        last_focus_index = focus_widget->focus_index;

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
        if (focus_widget) {
            spdlog::info("No best focus, keeping {}", focus_widget->focus_index);
        } else
            spdlog::info("No widget in focus");
    }

    select_widget(best_focus_widget);
}

auto PlatformWindow::select_previous_widget() -> void
{
    auto last_focus_index = -1;

    if (focus_widget)
        last_focus_index = focus_widget->focus_index;

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
        if (focus_widget) {
            spdlog::info("No best focus, keeping {}", focus_widget->focus_index);
        } else
            spdlog::info("No widget in focus");
    }

    select_widget(best_focus_widget);

}

auto PlatformWindow::select_widget(std::shared_ptr<Widget> widget) -> void {
    if (widget == focus_widget) {
        return;
    }

    if (!widget->can_focus) {
        spdlog::info("Not focusing widget, has it has a no focus policy");
        return;
    }

    if (this->focus_widget) {
        spdlog::info("widget {} lost focus", focus_widget->focus_index);
        this->focus_widget->on_focus_change(false);
        this->focus_widget->has_focus = false;
        //        last_focus_index = this->focus_widget->focus_index;
        this->needs_redraw |= this->focus_widget->needs_redraw;
        this->focus_widget.reset();
    }

    spdlog::info("widget {} got focus", widget->focus_index);
    widget->on_focus_change(true);
    widget->has_focus = true;
    //    last_focus_index = widget->focus_index;
    needs_redraw |= widget->needs_redraw;
    focus_widget = widget;
}

auto PlatformWindow::draw() -> void {
    if (background_color != 0)
        content.fill_rect(0, 0, content.size.width, content.size.height, background_color);
    else
        theme->draw_window_background(content);

    for (auto w : widgets) {
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
                select_previous_widget();
            else
                select_next_widget();
            break;
        default:
            if (focus_widget) {
                focus_widget->on_keyboard(event);
                needs_redraw |= focus_widget->needs_redraw;
            }
            break;
        }
    }
}

auto PlatformWindow::on_mouse(const EventMouse &event) -> void {
    bool found_widget = false;

    for (auto w : this->widgets) {
        switch (event.type) {
        case MouseEvents::Release: {
            auto click_event = event;
            // is the release event is inside the widget -
            // we send him a "local event", with local coordinates.
            // othewse - a "global event" - means the coordinates are relative
            // to the container window.
            if (point_in_rect(w->position, w->content.size, event.x, event.y)) {
                click_event.x = event.x - w->position.x;
                click_event.y = event.y - w->position.y;
                click_event.is_local = true;
            }
            w->on_mouse_click(click_event);
            found_widget = true;
        } break;

        case MouseEvents::Press:
        case MouseEvents::MouseMove:
            if (point_in_rect(w->position, w->content.size, event.x, event.y)) {
                auto move_press_event = event;
                move_press_event.is_local = true;
                move_press_event.x = event.x - w->position.x;
                move_press_event.y = event.y - w->position.y;
                if (last_overed_widget && last_overed_widget != w) {
                    spdlog::info("Mouse focus changed from {} to {}",
                                 fmt::ptr(last_overed_widget.get()), fmt::ptr(w.get()));
                    last_overed_widget->on_mouse_leave();
                    needs_redraw |= last_overed_widget->needs_redraw;
                }
                if (!w->mouse_over) {
                    w->on_mouse_enter();
                }

                if (event.type == MouseEvents::MouseMove) {
                    w->on_hover(move_press_event);
                } else {
                    w->on_mouse_click(move_press_event);
                }
                last_overed_widget = w;
                found_widget = true;

                if (move_press_event.is_local && event.type == MouseEvents::Press) {
                    select_widget(w);
                }
            }
            break;

        default:
            break;
        }

        needs_redraw |= w->needs_redraw;
    }

    if (!found_widget && last_overed_widget) {
        last_overed_widget->on_mouse_leave();
        last_overed_widget.reset();
    }
}

auto PlatformWindow::add(std::shared_ptr<Widget> widget) -> std::shared_ptr<Widget> {
    widgets.push_back(widget);
    widget->theme = theme;
    widget->window = this;
    if (widget->focus_index < 0) {
        widget->focus_index = max_focus_index;
        max_focus_index++;
    }

    for (auto w : widget->widgets) {
        if (w->focus_index < 0) {
            w->focus_index = max_focus_index;
            max_focus_index++;
        }
        w->theme = theme;
        w->window = this;
    }

    spdlog::info("New widget {}", fmt::ptr(widget.get()));
    return widget;
}

auto PlatformWindow::invalidate() -> void {
    assert(platform);
    platform->invalidate(*this);
};

auto PlatformWindow::on_close() -> void {
    for (auto w : widgets) {
        if (w) {
            w->on_remove();
        }
    }
    widgets.clear();
}
