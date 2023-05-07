/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <button.h>
#include <spdlog/spdlog.h>
#include <theme.h>

Button::Button(Position pp, Size size, std::string text, bool is_default,
               std::function<void()> on_button_click)
    : Widget(pp, size, 0) {
    this->text = text;
    this->is_default = is_default;
    this->on_button_click = on_button_click;
    this->can_focus = true;
}

auto Button::draw() -> void { theme->draw_button(content, has_focus, is_default, state, text); }

auto Button::on_hover(const EventMouse &event) -> void {
    // default implementation demands redraw, we
    // don't need this by default
}

auto Button::on_mouse_enter() -> void {
    mouse_over = true;
    switch (state) {
    case ButtonStates::ClickedInside:
        state = ButtonStates::ClickedOutside;
        needs_redraw = true;
        break;
    case ButtonStates::ClickedOutside:
        state = ButtonStates::ClickedInside;
        needs_redraw = true;
        break;
    case ButtonStates::Hovered:
        break;
    case ButtonStates::Normal:
        state = ButtonStates::Hovered;
        needs_redraw = true;
        break;
    }
}

auto Button::on_mouse_leave() -> void {
    mouse_over = false;

    switch (state) {
    case ButtonStates::ClickedInside:
        state = ButtonStates::ClickedOutside;
        needs_redraw = true;
        break;
    case ButtonStates::ClickedOutside:
        state = ButtonStates::ClickedInside;
        needs_redraw = true;
        break;
    case ButtonStates::Hovered:
        state = ButtonStates::Normal;
        needs_redraw = true;
        break;
    case ButtonStates::Normal:
        break;
    }
}

auto Button::on_mouse_click(const EventMouse &event) -> void {
    switch (state) {
    case ButtonStates::ClickedInside:
        if (!event.pressed && event.button == 1) {
            if (event.is_local) {
                state = ButtonStates::Hovered;
                if (on_button_click) {
                    on_button_click();
                }
            } else {
                state = ButtonStates::Normal;
            }
            needs_redraw = true;
        }

        break;
    case ButtonStates::ClickedOutside:
        if (event.pressed) {
            state = ButtonStates::ClickedInside;
            needs_redraw = true;
        } else {
            state = ButtonStates::Normal;
            needs_redraw = true;
            spdlog::debug("Button click aborted");
        }
        break;
    case ButtonStates::Hovered:
        if (event.pressed) {
            state = ButtonStates::ClickedInside;
            needs_redraw = true;
        }
        break;
    case ButtonStates::Normal:
        if (event.pressed) {
            state = ButtonStates::ClickedInside;
            needs_redraw = true;
        }
        break;
    }
}

auto Button::on_focus_change(bool new_state) -> void { needs_redraw = true; }

auto Button::on_keyboard(const EventKeyboard &event) -> void {
    if (event.keydown) {
        if (event.key == KeyCodes::Enter || event.key == KeyCodes::Return ||
            event.key == KeyCodes::Space) {
            // TODO - animate the button click
            if (on_button_click) {
                on_button_click();
            }
        }
    }
};
