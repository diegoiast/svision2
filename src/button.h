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

enum class RepeatState { Normal, WaitForFirstRepeat, Repeating };

struct Button : Widget {
    bool is_default = false;
    bool is_enabled = true;
    bool auto_shrink = false;
    bool has_frame = true;
    std::shared_ptr<Bitmap> icon;

    std::string text;
    std::function<void(Button &)> on_button_click;
    AbstractButtonState state;

    // TODO this part can be extracted into another helper class to be re-used.
    std::shared_ptr<Timer> click_timer;
    RepeatState repeat_state = RepeatState::Normal;
    bool is_autorepeat = false;
    int64_t autorepeat_millies = 500;
    int64_t autorepeat_start = autorepeat_millies;

    // TODO add support for buttons with images

    Button(Position pp, Size size, std::string_view text, bool is_default = false,
           std::function<void(Button &)> on_button_click = {});

    Button(std::string_view text, bool is_default = false,
           std::function<void(Button &)> on_button_click = {});

    virtual auto draw() -> void override;
    virtual auto on_hover(const EventMouse &event) -> void override;
    virtual auto on_mouse_enter() -> void override;
    virtual auto on_mouse_leave() -> void override;
    virtual auto on_mouse_click(const EventMouse &event) -> EventPropagation override;
    virtual auto on_focus_change(bool new_state) -> void override;
    virtual auto on_keyboard(const EventKeyboard &) -> EventPropagation override;
    virtual auto size_hint() const -> Size override;

    auto set_auto_repeat(int64_t repeat_millies, int64_t repeat_start = 500) -> void;
    auto disable_auto_repeat() -> void;

    auto set_is_default(bool new_state) -> std::shared_ptr<Button> {
        this->is_default = new_state;
        this->needs_redraw = true;
        return std::dynamic_pointer_cast<Button>(this->shared_from_this());
    }

    auto set_is_enabled(bool new_state) -> std::shared_ptr<Button> {
        this->is_enabled = new_state;
        this->needs_redraw = true;
        return std::dynamic_pointer_cast<Button>(this->shared_from_this());
    }

    auto set_auto_shrink(bool new_state) -> std::shared_ptr<Button> {
        this->auto_shrink = new_state;
        this->needs_redraw = true;
        return std::dynamic_pointer_cast<Button>(this->shared_from_this());
    }

    auto set_has_frame(bool new_state) -> std::shared_ptr<Button> {
        this->has_frame = new_state;
        this->needs_redraw = true;
        return std::dynamic_pointer_cast<Button>(this->shared_from_this());
    }

    auto set_on_click(std::function<void(Button &)> handler) -> std::shared_ptr<Button> {
        this->on_button_click = handler;
        return std::dynamic_pointer_cast<Button>(this->shared_from_this());
    }

    auto set_icon(std::shared_ptr<Bitmap> icon) {
        this->icon = icon;
        return std::dynamic_pointer_cast<Button>(this->shared_from_this());
    }
};
