#include "widget.h"
#include <spdlog/spdlog.h>

static auto point_in_rect(Position p, Size s, int x, int y) -> bool
{
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

Widget::Widget(Position position, Size size, uint32_t color)
{
    this->position = position;
    this->content.background_color = color;
    this->content.resize(size);
}

auto Widget::draw() -> void
{
    content.fill_rect(0, 0, content.size.width, content.size.height, content.background_color);
}

auto Widget::on_hover(const EventMouse &event) -> void
{
    // spdlog::info("Widget: Mouse over: {}x{}", event.x, event.y);
}

auto Widget::on_mouse_enter() -> void
{
    mouse_over = true;
    spdlog::info("Widget: mouse entered!");
}

auto Widget::on_mouse_leave() -> void
{
    mouse_over = false;
    spdlog::info("Widget: mouse leave!");
}

auto PlatformWindow::draw() -> void
{
    {
        content.fill_rect(0, 0, content.size.width, content.size.height, background_color);
        for (auto w : widgets)
        {
            w->draw();
            content.draw(w->position, w->content);
        }
    }
}

auto PlatformWindow::on_mouse(const EventMouse &event) -> void
{
    bool found_widget = false;
    for (auto w : this->widgets)
    {
        if (point_in_rect(w->position, w->content.size, event.x, event.y))
        {
            auto local = event;
            local.x = event.x - w->position.x;
            local.y = event.y - w->position.y;
            if (last_overed_widget && last_overed_widget != w)
            {
                last_overed_widget->on_mouse_leave();
            }
            if (!w->mouse_over)
            {

                w->on_mouse_enter();
            }

            w->on_hover(local);
            last_overed_widget = w;
            found_widget = true;
        }
    }

    if (!found_widget && last_overed_widget)
    {
        last_overed_widget->on_mouse_leave();
        last_overed_widget.reset();
    }
}
