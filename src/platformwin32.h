/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <platform.h>

struct PlatformWin32 : Platform {
    const std::string_view default_font_file = "c:\\Windows\\Fonts\\arial.ttf";

    PlatformWin32();
    virtual auto init() -> void override;
    virtual auto done() -> void override;
    virtual auto open_window(int x, int y, int width, int height, const std::string title)
        -> std::shared_ptr<PlatformWindow> override;
    virtual auto show_window(std::shared_ptr<PlatformWindow> window) -> void override;
    virtual auto invalidate(PlatformWindow &window) -> void;
    virtual auto main_loop() -> void override;
};

using ThePlatform = PlatformWin32;
