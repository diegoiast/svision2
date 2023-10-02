/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <functional>

#include <events.h>

enum class ButtonStates {
    Normal,
    Hovered,
    ClickedInside,
    ClickedOutside,
};

struct AbstractButtonState {
    ButtonStates state = ButtonStates::Normal;

    auto on_mouse_enter() -> void;
    auto on_mouse_leave() -> void;
    auto on_mouse_click(const EventMouse &event) -> EventPropagation;
    auto on_keyboard(const EventKeyboard &event, const std::function<void()> &on_button_click) -> EventPropagation;
};
