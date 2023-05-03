/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "widget.h"
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

Widget::~Widget()
{
}

auto Widget::draw() -> void {
    auto color = content.background_color;

    if (mouse_over) {
        color = Lighter(content.background_color, 0.1);
    }

    content.fill(color);

    if (mouse_over) {
        auto str = fmt::format("{} Position = {}x{} ", state_pressed ? "*" : " ", pos.x, pos.y);
        content.write_fixed(Position{4, 4}, str, MakeColor(0xf, 0xf, 0));
    } else {
        if (state_pressed)
            content.write_fixed_char(Position{4, 4}, '*', MakeColor(0x0, 0x0, 0));
    }

    content.line(10, 23, 30, 23, 0);
    content.line(10, 24, 30, 24, 0xffffff);
    content.line(10, 25, 30, 25, 0);
    content.line(10, 26, 30, 26, 0xffffff);

    // content.line(0, 0, content.size.width-1, 0, 0);

    content.line(10, 23, 70, 27, 0xff3388);
    content.draw_bezier(20, 2, 40, 36, 160, 37, 0x2f2af8);
    content.line_thikness(5, 23, 175, 37, 5, 0x9933fe);
    content.draw_rounded_rectangle(10, 5, 150, 30, 10, 0xffaaaa, 0xffaaaa);
}

auto Widget::on_hover(const EventMouse &event) -> void {
    // spdlog::info("Widget: Mouse over: {}x{}", event.x, event.y);
    pos.x = event.x;
    pos.y = event.y;
    this->needs_redraw = true;
}

auto Widget::on_mouse_enter() -> void {
    mouse_over = true;
    spdlog::info("Widget: mouse entered!");
    needs_redraw = true;
}

auto Widget::on_mouse_leave() -> void {
    mouse_over = false;
    spdlog::info("Widget: mouse leave!");
    needs_redraw = true;
}

auto Widget::on_mouse_click(const EventMouse &event) -> void {
    if (event.type == MouseEvents::Press) {
        spdlog::info("Click inside widget, {}", fmt::ptr(this));
        state_pressed = true;
    } else if (event.type == MouseEvents::Release) {
        if (event.is_local)
            spdlog::info("Click release inside widget, {}", fmt::ptr(this));
        else if (state_pressed)
            spdlog::info("Click release outside widget, {}", fmt::ptr(this));
        state_pressed = false;
        unclick_inside = event.is_local;
    }
    needs_redraw = true;
}

PlatformWindow::~PlatformWindow() {
    spdlog::info("Window done");
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
        case KeyCodes::Tab: {
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
            if (best_focus_widget != focus_widget) {
                if (focus_widget) {
                    spdlog::info("widget {} lost focus", focus_widget->focus_index);
                    focus_widget->on_focus_change(false);
                    focus_widget->has_focus = false;
                    last_focus_index = focus_widget->focus_index;
                    needs_redraw |= focus_widget->needs_redraw;
                    focus_widget.reset();
                }

                spdlog::info("widget {} got focus", best_focus_widget->focus_index);
                best_focus_widget->on_focus_change(true);
                best_focus_widget->has_focus = true;
                last_focus_index = best_focus_widget->focus_index;
                needs_redraw |= best_focus_widget->needs_redraw;
                focus_widget = best_focus_widget;
            }
        } break;
        default:
            if (focus_widget) {
                focus_widget->on_keyboard(event);
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
                auto local = event;
                local.is_local = true;
                local.x = event.x - w->position.x;
                local.y = event.y - w->position.y;
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
                    w->on_hover(local);
                } else {
                    w->on_mouse_click(local);
                }
                last_overed_widget = w;
                found_widget = true;
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

auto PlatformWindow::add(std::shared_ptr<Widget> widget) -> std::shared_ptr<Widget>
{
    widgets.push_back(widget);
    widget->theme = theme;
    if (widget->focus_index < 0) {
        widget->focus_index = max_focus_index;
        max_focus_index++;
    }
    spdlog::info("New widget {}", fmt::ptr(widget.get()));
    return widget;
}

auto PlatformWindow::on_close() -> void
{
    for (auto w : widgets) {
        if (w) {
            w->on_remove();
        }
    }
    widgets.clear();
}
