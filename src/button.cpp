/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <button.h>
#include <spdlog/spdlog.h>
#include <theme.h>

auto start_repeate_timer(Button *button) -> void {
    if (!button->is_autorepeat) {
        spdlog::debug("this NOT is a autorepeat button! FAIL!!!");
        return;
    }

    switch (button->repeat_state) {
    case RepeatState::Normal:
        button->repeat_state = RepeatState::WaitForFirstRepeat;
        button->click_timer = std::make_shared<Timer>(button->autorepeat_start, false, [button]() {
            if (button->state == ButtonStates::ClickedInside) {
                button->repeat_state = RepeatState::Repeating;
                if (button->on_button_click)
                    button->on_button_click();
                button->click_timer->stop();

                // We need a new timer, with different timeout.
                // re-use existing timer. I hope this works.
                button->click_timer->repeating = true;
                button->click_timer->millies = button->autorepeat_millies;
                button->click_timer->initialize();
                button->click_timer->start();
            }
        });
        button->click_timer->start();
        break;
    case RepeatState::WaitForFirstRepeat:
        if (button->on_button_click)
            button->on_button_click();

        button->click_timer->stop();
        button->click_timer.reset();

        button->repeat_state = RepeatState::Repeating;
        button->click_timer = std::make_shared<Timer>(button->autorepeat_millies, true, [button]() {
            if (button->repeat_state != RepeatState::WaitForFirstRepeat) {
                return;
            }

            if (button->state != ButtonStates::ClickedInside) {
                button->repeat_state = RepeatState::Repeating;
                if (button->on_button_click)
                    button->on_button_click();
            }
        });
        button->click_timer->start();
        break;
    case RepeatState::Repeating:
        break;
    }
}

Button::Button(Position pp, Size size, std::string text, bool is_default,
               std::function<void()> on_button_click)
    : Widget(pp, size, 0) {
    this->text = text;
    this->is_default = is_default;
    this->on_button_click = on_button_click;
    this->can_focus = true;
}

auto Button::draw() -> void {
    get_theme()->draw_button(content, has_focus, is_default, is_enabled, state, text);
}

auto Button::on_hover(const EventMouse &event) -> void {
    // default implementation demands redraw, we
    // don't need this by default
}

auto Button::on_mouse_enter() -> void {
    mouse_over = true;
    switch (state) {
    case ButtonStates::ClickedInside:
        state = ButtonStates::ClickedOutside;
        invalidate();
        break;
    case ButtonStates::ClickedOutside:
        state = ButtonStates::ClickedInside;
        invalidate();
        break;
    case ButtonStates::Hovered:
        break;
    case ButtonStates::Normal:
        state = ButtonStates::Hovered;
        invalidate();
        break;
    }
}

auto Button::on_mouse_leave() -> void {
    mouse_over = false;

    switch (state) {
    case ButtonStates::ClickedInside:
        state = ButtonStates::ClickedOutside;
        invalidate();
        break;
    case ButtonStates::ClickedOutside:
        state = ButtonStates::ClickedInside;
        invalidate();
        break;
    case ButtonStates::Hovered:
        state = ButtonStates::Normal;
        invalidate();
        break;
    case ButtonStates::Normal:
        break;
    }
}

auto Button::on_mouse_click(const EventMouse &event) -> void {
    if (!is_enabled) {
        Widget::on_mouse_click(event);
        return;
    }

    if (!event.is_local) {
        Widget::on_mouse_click(event);
        return;
    }

    switch (state) {
    case ButtonStates::ClickedInside:
        if (!event.pressed) {
            state = ButtonStates::Hovered;
            switch (event.button) {
            case 1:
                // if the button has auto repeat state, unclicking
                // will trigger the event, unless a single click has been emited
                if (repeat_state != RepeatState::Repeating && on_button_click) {
                    on_button_click();
                }
                break;
            default:
                break;
            }
            invalidate();
        }
        break;
    case ButtonStates::ClickedOutside:
        if (event.pressed) {
            state = ButtonStates::ClickedInside;
            invalidate();
        } else {
            state = ButtonStates::Normal;
            invalidate();
            spdlog::debug("Button click aborted");
        }
        break;
    case ButtonStates::Hovered:
        if (event.pressed) {
            state = ButtonStates::ClickedInside;
            invalidate();
        }
        if (is_autorepeat) {
            if (event.pressed) {
                start_repeate_timer(this);
            } else {
                spdlog::info("Stopped auto repeat timer");
                click_timer.reset();
                repeat_state = RepeatState::Normal;
            }
        }
        break;
    case ButtonStates::Normal:
        if (event.pressed) {
            state = ButtonStates::ClickedInside;
            invalidate();
        }
        break;
    }

    if (!event.pressed && click_timer) {
        click_timer->stop();
        click_timer.reset();
        repeat_state = RepeatState::Normal;
    }
}

auto Button::on_focus_change(bool new_state) -> void { invalidate(); }

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

auto Button::set_auto_repeat(int64_t repeat_millies, int64_t repeat_start) -> void {
    this->is_autorepeat = true;
    this->autorepeat_millies = repeat_millies;
    this->autorepeat_start = repeat_start;
}

auto Button::disable_auto_repeat() -> void {
    this->is_autorepeat = false;
    this->autorepeat_millies = 0;
    this->autorepeat_start = 0;
    this->click_timer.reset();
}
