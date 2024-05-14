/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <mousecursors.h>

#include <memory>
#include <string_view>

struct Theme;
struct PlatformWindow;
struct FontProvider;
struct FileLoader;
struct ImageLoader;

struct Platform {
    bool exit_loop = false;
    bool close_on_last_window = true;
    std::string_view default_font_file = "";
    std::shared_ptr<Theme> default_theme = nullptr;
    std::shared_ptr<FontProvider> default_font = nullptr;
    std::shared_ptr<ImageLoader> image_loader = nullptr;

    auto init() -> void;
    virtual auto platform_init() -> void = 0;
    virtual auto done() -> void = 0;
    virtual auto open_window(int x, int y, int width, int height, const std::string_view title)
        -> std::shared_ptr<PlatformWindow> = 0;
    virtual auto show_window(std::shared_ptr<PlatformWindow> window) -> void = 0;

    // TODO: Should I pass the shared pointer, to keep API consistent?
    virtual auto clear_cursor_cache() -> void = 0;
    virtual auto set_cursor(PlatformWindow &window, MouseCursor cursor) -> void = 0;
    virtual auto invalidate(PlatformWindow &window) -> void = 0;
    virtual auto main_loop() -> void = 0;
};
