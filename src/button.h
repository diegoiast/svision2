/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <functional>
#include <widget.h>

struct Button : Widget {
    enum class States {
        Normal,
        Hovered,
        ClickedInside,
        ClickedOutside,
    };

    States state;
    std::string text;
    std::function<void()> on_button_click;

    Button(Position pp, Size size, std::string text);
    Button(Position pp, Size size, std::string text, std::function<void()> on_button_click);

    virtual auto draw() -> void override;
    virtual auto on_hover(const EventMouse &event) -> void override;
    virtual auto on_mouse_enter() -> void override;
    virtual auto on_mouse_leave() -> void override;
    virtual auto on_mouse_click(const EventMouse &event) -> void override;
};
