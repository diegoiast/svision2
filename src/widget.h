#pragma once

#include <bitmap.h>
#include <events.h>
#include <list>
#include <string>
#include <memory>

struct Widget
{
    uint32_t background_color;
    Bitmap content;
    Position position;
    bool mouse_over = false;
    bool needs_redraw = false;

    Widget(Position pp, Size size, uint32_t color);
    virtual auto draw() -> void;
    virtual auto on_hover(const EventMouse &ev) -> void;
    virtual auto on_mouse_enter() -> void;
    virtual auto on_mouse_leave() -> void;
};

struct PlatformWindow
{
    std::string title;
    Bitmap content;
    uint32_t background_color = 0;
    std::list<std::shared_ptr<Widget>> widgets;
    std::shared_ptr<Widget> last_overed_widget;
    bool needs_redraw = false;

    virtual ~PlatformWindow()
    {
    }

    virtual auto draw() -> void;

    virtual auto on_keyboard(const EventKeyboard &) -> void
    {
        // by default, do nothing
    }

    virtual auto on_mouse(const EventMouse &) -> void;

    virtual auto on_resize(const EventResize &) -> void
    {
        // by default, do nothing
    }

    virtual auto can_close() -> bool
    {
        return true;
    }

    auto add(std::shared_ptr<Widget> w)
    {
        widgets.push_back(w);
    }
};
