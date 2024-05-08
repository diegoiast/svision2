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
            if (button->state.state == ButtonStates::ClickedInside) {
                button->repeat_state = RepeatState::Repeating;
                if (button->on_button_click)
                    button->on_button_click(*button);
                button->click_timer->stop();

                // We need a new timer, with different timeout.
                // reuse existing timer. I hope this works.
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
            button->on_button_click(*button);

        button->click_timer->stop();
        button->click_timer.reset();

        button->repeat_state = RepeatState::Repeating;
        button->click_timer = std::make_shared<Timer>(button->autorepeat_millies, true, [button]() {
            if (button->repeat_state != RepeatState::WaitForFirstRepeat) {
                return;
            }

            if (button->state.state != ButtonStates::ClickedInside) {
                button->repeat_state = RepeatState::Repeating;
                if (button->on_button_click)
                    button->on_button_click(*button);
            }
        });
        button->click_timer->start();
        break;
    case RepeatState::Repeating:
        break;
    }
}

Button::Button(std::string_view text, bool is_default,
               std::function<void(Button &)> on_button_click)
    : Button(Position{}, Size{}, text, false, on_button_click) {
    this->padding_style = PaddingStyle::Button;
    this->is_default = is_default;
}

Button::Button(Position pp, Size size, std::string_view text, bool is_default,
               std::function<void(Button &)> on_button_click)
    : Widget(pp, size, 0) {
    this->text = text;
    this->is_default = is_default;
    this->on_button_click = on_button_click;
    this->can_focus = true;
    this->padding_style = PaddingStyle::Button;
}

auto Button::draw() -> void {
    get_theme()->draw_button(content, has_focus, is_default, is_enabled, has_frame, state.state,
                             text);
}

auto Button::on_hover(const EventMouse &) -> void {
    // default implementation demands redraw, we don't need this by default
}

auto Button::on_mouse_enter() -> void {
    mouse_over = true;
    state.on_mouse_enter();
    invalidate();
}

auto Button::on_mouse_leave() -> void {
    mouse_over = false;
    state.on_mouse_leave();
    invalidate();
}

auto Button::on_mouse_click(const EventMouse &event) -> EventPropagation {
    if (!is_enabled || !event.is_local) {
        return Widget::on_mouse_click(event);
    }

    auto result = state.on_mouse_click(event);

    switch (state.state) {
    case ButtonStates::ClickedInside:
        switch (event.button) {
        case 1:
            if (is_autorepeat) {
                if (event.pressed) {
                    start_repeate_timer(this);
                } else {
                    spdlog::info("Stopped auto repeat timer");
                    click_timer.reset();
                    repeat_state = RepeatState::Normal;
                }
            }
            invalidate();
            break;
        }
        break;
    case ButtonStates::ClickedOutside:
        invalidate();
        break;
    case ButtonStates::Hovered:
        // this means a button has been released
        if (result == EventPropagation::handled) {
            switch (event.button) {
            case 1:
                if (repeat_state != RepeatState::Repeating && on_button_click) {
                    on_button_click(*this);
                }
                break;
            default:
                break;
            }
            invalidate();
        }
        break;
    case ButtonStates::Normal:
        break;
    }

    if (!event.pressed && click_timer) {
        click_timer->stop();
        click_timer.reset();
        repeat_state = RepeatState::Normal;
    }
    return result;
}

auto Button::on_focus_change(bool new_state) -> void {
    invalidate();
    (void)(new_state);
}

auto Button::on_keyboard(const EventKeyboard &event) -> EventPropagation {

    if (state.on_keyboard(event) == EventPropagation::handled) {
        on_button_click(*this);
        return EventPropagation::handled;
    }
    return Widget::on_keyboard(event);
}

Size Button::size_hint() const {
    // TODO: Size of text is not correct. We also need to calculate the yMin and yMax for example
    auto s = get_theme()->font.text_size(text);
    auto p = get_padding();
    if (auto_shrink) {
        return {s.width + p.get_horizontal(), s.height + p.get_vertical()};
    }
    return {0, s.height + p.get_vertical()};
}

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
