/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <functional>

#include <buttonstates.h>
#include <memory>
#include <timer.h>
#include <widget.h>

struct Checkbox : Widget {
    bool is_enabled = true;
    bool is_checked = true;

    std::string text;
    std::function<void()> on_checkbox_click;
    AbstractButtonState state;

    Checkbox(Position pp, int length, std::string text, std::function<void()> on_button_click = {});

    virtual auto draw() -> void override;
    virtual auto on_hover(const EventMouse &event) -> void override;
    virtual auto on_mouse_enter() -> void override;
    virtual auto on_mouse_leave() -> void override;
    virtual auto on_mouse_click(const EventMouse &event) -> EventPropagation override;
    virtual auto on_focus_change(bool new_state) -> void override;
    virtual auto on_keyboard(const EventKeyboard &) -> EventPropagation override;

    auto toggle() -> bool;
};
