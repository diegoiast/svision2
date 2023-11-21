/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <string>
#include <vector>

#include <widget.h>

class ListView;

struct Combobox : Widget {
    std::shared_ptr<ListView> popup_list = {};
    std::vector<std::string> strings;
    int selected_item = -1;

    Combobox(Position position, int width, const std::vector<std::string> strings);
    auto get_value() const -> std::string;
    auto get_item() const -> int { return selected_item; }
    virtual auto draw() -> void override;
    virtual auto on_mouse(const EventMouse &) -> EventPropagation override;
    virtual auto on_focus_change(bool new_state) -> void override;
    virtual auto on_keyboard(const EventKeyboard &) -> EventPropagation override;

    auto show_popup() -> void;
};