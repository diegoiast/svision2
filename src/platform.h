#pragma once

#include <bitmap.h>
#include <widget.h>
#include <events.h>

#include <string>
#include <memory>
#include <list>

struct PlatformWindow
{
    std::string title;
    Bitmap content;
    uint32_t background_color = 0;
    std::list<std::shared_ptr<Widget>> widgets;

    virtual ~PlatformWindow()
    {
    }

    virtual auto draw() -> void
    {
        content.fill_rect(0, 0, content.size.width, content.size.height, background_color);
        for (auto w : widgets) {
            w->draw();
            content.draw(w->position, w->content);
        }
    }

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

class Platform
{
public:
    bool exit_loop = false;

    bool close_on_last_window = true;
    virtual auto init() -> void = 0;
    virtual auto done() -> void = 0;
    virtual auto open_window(int x, int y, int width, int height, const std::string title) -> std::shared_ptr<PlatformWindow> = 0;
    virtual auto main_loop() -> void = 0;
};
