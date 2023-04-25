#include "platform.h"
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

auto PlatformWindow::on_mouse(const EventMouse &event) -> void
{
    for (auto w : this->widgets)
    {
        if (point_in_rect(w->position, w->content.size, event.x, event.y))
        {
            auto local = event;
            local.x = event.x - w->position.x;
            local.y = event.y - w->position.y;
            w->on_hover(local);
        }
    }
}