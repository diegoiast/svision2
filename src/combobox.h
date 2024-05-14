/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <functional>
#include <string>
#include <vector>

#include <widget.h>

struct Button;
struct ListView;
struct ComboboxList;

struct Combobox : Widget {
    std::shared_ptr<ComboboxList> popup_list = {};
    std::vector<std::string_view> strings = {};
    std::function<void(Combobox &, int)> on_item_selected = {};

    int selected_item = 0;

    Combobox(Position position, int width, const std::vector<std::string_view> &strings);
    Combobox(const std::vector<std::string_view> &strings);
    auto get_value() const -> std::string_view;
    auto get_item() const -> int { return selected_item; }
    virtual auto draw() -> void override;
    virtual auto on_mouse(const EventMouse &) -> EventPropagation override;
    virtual auto on_focus_change(bool new_state) -> void override;
    virtual auto on_keyboard(const EventKeyboard &) -> EventPropagation override;
    virtual auto size_hint() const -> Size override;
    virtual auto on_resize() -> void override;

    auto show_popup() -> void;
    auto set_active_index(int index) -> void;
    auto get_active_index() const -> int { return selected_item; }
    auto set_items(const std::vector<std::string_view> &new_strings) -> void;

  private:
    std::shared_ptr<Button> popup_button = nullptr;
};
