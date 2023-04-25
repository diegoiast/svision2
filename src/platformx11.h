#pragma once

#include <memory>
#include <map>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <platform.h>

struct PlatformWindowX11;

class PlatformX11 : public Platform
{
    std::map<Window, std::shared_ptr<PlatformWindowX11>> windows;
    Display *dpy = nullptr;
    int screen = -1;
    Atom wmDeleteMessage = None;

public:
    virtual auto init() -> void override;
    virtual auto done() -> void override;
    virtual auto main_loop() -> void override;
    virtual auto open_window(int x, int y, int width, int height, const std::string title) -> std::shared_ptr<PlatformWindow> override;
};

using ThePlatform = PlatformX11;
