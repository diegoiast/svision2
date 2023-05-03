/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <map>
#include <memory>

#include <X11/XKBlib.h>
#include <X11/Xlib.h>
#include <X11/keysym.h>

#include <platform.h>

struct PlatformWindowX11;

struct PlatformX11 : Platform {
    std::map<Window, std::shared_ptr<PlatformWindowX11>> windows;
    Display *dpy = nullptr;
    int screen = -1;
    Atom wmDeleteMessage = None;

    virtual auto init() -> void override;
    virtual auto done() -> void override;
    virtual auto open_window(int x, int y, int width, int height, const std::string title)
        -> std::shared_ptr<PlatformWindow> override;
    virtual auto show_window(std::shared_ptr<PlatformWindow> window) -> void override;
    virtual auto invalidate(PlatformWindow& window) -> void override;
    virtual auto main_loop() -> void override;
};

using ThePlatform = PlatformX11;
