#include "widget.h"
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

auto Widget::draw() -> void {
        auto color = content.background_color;

        if (mouse_over) {
                color = Lighter(content.background_color, 0.1);
        }

        content.fill_rect(0, 0, content.size.width, content.size.height, color);

        if (mouse_over) {
                auto str =
                    fmt::format("{} Position = {}x{} ", state_pressed ? "*" : " ", pos.x, pos.y);
                content.write_fixed(Position{4, 4}, str, MakeColor(0xf, 0xf, 0));
        } else {
                if (state_pressed)
                        content.write_fixed_char(Position{4, 4}, '*', MakeColor(0x0, 0x0, 0));
        }
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

auto PlatformWindow::draw() -> void {
        content.fill_rect(0, 0, content.size.width, content.size.height, background_color);
        for (auto w : widgets) {
                if (w->needs_redraw) {
                        w->draw();
                        w->needs_redraw = false;
                }
                content.draw(w->position, w->content);
        }
        needs_redraw = false;
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
                                                     fmt::ptr(last_overed_widget.get()),
                                                     fmt::ptr(w.get()));
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
