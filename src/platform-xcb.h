/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <map>
#include <memory>

#include <xcb/xcb.h>

#include <platform.h>

struct PlatformWindowXCB;

struct PlatformXCB : Platform {
    std::map<xcb_window_t, std::shared_ptr<PlatformWindowXCB>> windows;

    virtual auto init() -> void override;
    virtual auto done() -> void override;
    virtual auto open_window(int x, int y, int width, int height, const std::string title)
        -> std::shared_ptr<PlatformWindow> override;
    virtual auto show_window(std::shared_ptr<PlatformWindow> window) -> void override;
    virtual auto invalidate(PlatformWindow &window) -> void override;
    virtual auto main_loop() -> void override;

  private:
    xcb_connection_t *connection;
    xcb_screen_t *screen;
};

using ThePlatform = PlatformXCB;
