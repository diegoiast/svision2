/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <checkbox.h>
#include <spdlog/spdlog.h>
#include <theme.h>

Checkbox::Checkbox(Position p, int length, std::string text,
                   std::function<void(Checkbox &)> on_checkbox_change)
    : Widget(p, {}, 0) {
    this->text = text;
    this->on_checkbox_change = on_checkbox_change;
    this->can_focus = true;
    this->content.resize({length, 22});
}

auto Checkbox::draw() -> void {
    get_theme()->draw_checkbox(content, has_focus, is_enabled, is_checked, state.state, text, shape,
                               get_padding());
}

auto Checkbox::on_hover(const EventMouse &event) -> void {
    // default implementation demands redraw, we don't need this by default
}

auto Checkbox::on_mouse_enter() -> void {
    mouse_over = true;
    state.on_mouse_enter();
    invalidate();
}

auto Checkbox::on_mouse_leave() -> void {
    mouse_over = false;
    state.on_mouse_leave();
    invalidate();
}

auto Checkbox::on_mouse_click(const EventMouse &event) -> EventPropagation {
    if (!is_enabled || !event.is_local) {
        return Widget::on_mouse_click(event);
    }

    auto result = state.on_mouse_click(event);

    switch (state.state) {
    case ButtonStates::ClickedInside:
        invalidate();
        break;
    case ButtonStates::ClickedOutside:
        invalidate();
        break;
    case ButtonStates::Hovered:
        // this means a Checkbox has been released
        if (result == EventPropagation::handled) {
            switch (event.button) {
            case 1:
                toggle();
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

    return result;
}

auto Checkbox::on_focus_change(bool new_state) -> void { invalidate(); }

auto Checkbox::on_keyboard(const EventKeyboard &event) -> EventPropagation {
    if (state.on_keyboard(event, [this]() { toggle(); }) == EventPropagation::propagate) {
        return EventPropagation::handled;
    }
    return Widget::on_keyboard(event);
}

auto Checkbox::size_hint() const -> Size {
    auto p = get_padding();
    auto hint = Size{};
    auto font_height = this->get_theme()->font.text_size("X").height;
    hint.height = font_height + p.get_vertical();
    return hint;
}

auto Checkbox::toggle() -> bool {
    this->is_checked = !this->is_checked;
    if (on_checkbox_change) {
        on_checkbox_change(*this);
    }
    invalidate();
    return is_checked;
}

auto Checkbox::set_checked(EventPropagation propagate) -> void {
    if (is_checked) {
        return;
    }
    is_checked = true;
    if (propagate == EventPropagation::propagate && on_checkbox_change) {
        on_checkbox_change(*this);
    }
    invalidate();
}

auto Checkbox::set_unchecked(EventPropagation propagate) -> void {
    if (!is_checked) {
        return;
    }
    is_checked = false;
    if (propagate == EventPropagation::propagate && on_checkbox_change) {
        on_checkbox_change(*this);
    }
    invalidate();
}
