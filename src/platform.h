#pragma once

#include <string>
#include <memory>

struct Event
{
};

struct EventKeyboard : public Event
{
    int modifiers;
    int key;
    bool keydown;
};

struct EventMouse : public Event
{
    bool pressed = false;
    int button = -1;
    int x = -1;
    int y = -1;
};

struct PlatformWindow
{
    uint32_t *buf;
    std::string title;

    virtual auto on_keyboard(const EventKeyboard &) -> void
    {
        // by default, do nothing
    }
    virtual auto on_mouse(const EventMouse &) -> void
    {
        // by default, do nothing
    }

    virtual auto can_close() -> bool
    {
        return true;
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
