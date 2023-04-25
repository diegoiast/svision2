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
    auto color = content.background_color;

    if (mouse_over)
    {
        // color = MakeColor(0xe0,0xa0,0xa0);
        // color = Lighter(content.background_color, 0.1);
        color = Lighter(content.background_color, 0.1);
    }

    content.fill_rect(0, 0, content.size.width, content.size.height, color);
}

auto Widget::on_hover(const EventMouse &event) -> void
{
    // spdlog::info("Widget: Mouse over: {}x{}", event.x, event.y);
}

auto Widget::on_mouse_enter() -> void
{
    mouse_over = true;
    spdlog::info("Widget: mouse entered!");
    needs_redraw = true;
}

auto Widget::on_mouse_leave() -> void
{
    mouse_over = false;
    spdlog::info("Widget: mouse leave!");
    needs_redraw = true;
}

auto PlatformWindow::draw() -> void
{
    content.fill_rect(0, 0, content.size.width, content.size.height, background_color);
    for (auto w : widgets)
    {
        if (w->needs_redraw)
        {
            w->draw();
            w->needs_redraw = false;
        }
        content.draw(w->position, w->content);
    }
    needs_redraw = false;
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
                spdlog::info("Focus changed from {} to {}", fmt::ptr(last_overed_widget.get()), fmt::ptr(w.get()));
                last_overed_widget->on_mouse_leave();
                needs_redraw |= last_overed_widget->needs_redraw;
            }
            if (!w->mouse_over)
            {
                w->on_mouse_enter();
            }

            w->on_hover(local);
            last_overed_widget = w;
            found_widget = true;
        }
        needs_redraw |= w->needs_redraw;
    }

    if (!found_widget && last_overed_widget)
    {
        last_overed_widget->on_mouse_leave();
        last_overed_widget.reset();
    }
}
