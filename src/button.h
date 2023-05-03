/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <functional>

#include <buttonstates.h>
#include <widget.h>

struct Button : Widget {
    ButtonStates state = ButtonStates::Normal;
    bool is_default = false;
    std::string text;
    std::function<void()> on_button_click;

    Button(Position pp, Size size, std::string text, bool is_default = false,
           std::function<void()> on_button_click = {});

    virtual auto draw() -> void override;
    virtual auto on_hover(const EventMouse &event) -> void override;
    virtual auto on_mouse_enter() -> void override;
    virtual auto on_mouse_leave() -> void override;
    virtual auto on_mouse_click(const EventMouse &event) -> void override;
    virtual auto on_focus_change(bool new_state) -> void override;
    virtual auto on_keyboard(const EventKeyboard &) -> void override;
};
