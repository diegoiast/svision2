/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "buttonstates.h"
#include <spdlog/spdlog.h>

auto AbstractButtonState::on_mouse_enter() -> void {
    switch (state) {
    case ButtonStates::ClickedInside:
        // ???
        // state = ButtonStates::ClickedOutside;
        break;
    case ButtonStates::ClickedOutside:
        state = ButtonStates::ClickedInside;
        break;
    case ButtonStates::Hovered:
        break;
    case ButtonStates::Normal:
        state = ButtonStates::Hovered;
        break;
    }
}

auto AbstractButtonState::on_mouse_leave() -> void {
    switch (state) {
    case ButtonStates::ClickedInside:
        state = ButtonStates::ClickedOutside;
        break;
    case ButtonStates::ClickedOutside:
        state = ButtonStates::ClickedInside;
        break;
    case ButtonStates::Hovered:
        state = ButtonStates::Normal;
        break;
    case ButtonStates::Normal:
        break;
    }
}

auto AbstractButtonState::on_mouse_click(const EventMouse &event) -> EventPropagation {
    auto result = EventPropagation::propagate;

    switch (state) {
    case ButtonStates::ClickedInside:
        result = EventPropagation::handled;
        if (!event.pressed) {
            state = ButtonStates::Hovered;
        }
        break;
    case ButtonStates::ClickedOutside:
        if (event.pressed) {
            state = ButtonStates::ClickedInside;
            result = EventPropagation::handled;
        } else {
            state = ButtonStates::Normal;
            spdlog::debug("Button click aborted");
        }
        break;
    case ButtonStates::Hovered:
        if (event.pressed) {
            result = EventPropagation::handled;
            state = ButtonStates::ClickedInside;
        }
        break;
    case ButtonStates::Normal:
        if (event.pressed) {
            state = ButtonStates::ClickedInside;
        }
        break;
    }
    return result;
}

auto AbstractButtonState::on_keyboard(const EventKeyboard &event,
                                      const std::function<void()> &on_button_click)
    -> EventPropagation {
    if (event.keydown) {
        if (event.key == KeyCodes::Enter || event.key == KeyCodes::Return ||
            event.key == KeyCodes::Space) {
            // TODO - animate the button click
            if (on_button_click) {
                on_button_click();
            }
            return EventPropagation::handled;
        }
    }
    return EventPropagation::propagate;
};
