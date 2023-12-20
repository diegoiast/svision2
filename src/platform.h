/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <list>
#include <memory>
#include <string>

struct Theme;
struct PlatformWindow;
struct FontProvider;

struct Platform {
    bool exit_loop = false;
    bool close_on_last_window = true;
    std::shared_ptr<Theme> default_theme;
    std::shared_ptr<FontProvider> default_font;

    virtual auto init() -> void = 0;
    virtual auto done() -> void = 0;
    virtual auto open_window(int x, int y, int width, int height, const std::string title)
        -> std::shared_ptr<PlatformWindow> = 0;
    virtual auto show_window(std::shared_ptr<PlatformWindow> window) -> void = 0;

    // TODO: I would like to pass the shared pointer, to keep API consistent
    virtual auto invalidate(PlatformWindow &window) -> void = 0;
    virtual auto main_loop() -> void = 0;
};
