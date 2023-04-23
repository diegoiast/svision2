#pragma once

#include <memory>
#include <map>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <platform.h>

struct PlatformWindowX11;

class PlatformX11
{
    std::map<Window, std::shared_ptr<PlatformWindowX11>> windows;
    Display *dpy = nullptr;
    int screen = -1;
    Atom wmDeleteMessage;

public:
    bool exit_loop = false;

    bool close_on_last_window = true;
    auto init() -> void;
    auto done() -> void;
    auto open_window(int x, int y, int width, int height, const std::string title) -> std::shared_ptr<PlatformWindow>;
    void main_loop();
};
