/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "radiobuttongroup.h"
#include "checkbox.h"
#include "layout.h"
#include <spdlog/spdlog.h>

auto on_radio_button_selected(RadioButtonGroup &group, Checkbox &checkbox) -> void {
    auto index = 0;
    for (auto &cb : group.radio_buttons) {
        if (cb.get() == &checkbox) {
            cb->set_checked(EventPropagation::handled);
            if (group.on_selected) {
                group.on_selected(index, checkbox);
            }
        } else {
            cb->set_unchecked(EventPropagation::handled);
        }
        index++;
    }
}

RadioButtonGroup::RadioButtonGroup(const std::vector<std::string> &items)
    : RadioButtonGroup({}, {}, items) {}

RadioButtonGroup::RadioButtonGroup(Position position, int width,
                                   const std::vector<std::string> &items)
    : Widget(position, {}, 0) {
    auto p = Position{0, 0};
    auto height = 0;
    this->layout = std::make_shared<VerticalLayout>();
    mouse_cursor = MouseCursor::Link;

    for (auto &item : items) {
        auto cb = add_new<Checkbox>(p, width, item);
        cb->shape = CheckboxShape::RadioButton;
        cb->on_checkbox_change = [this](Checkbox &cb) { on_radio_button_selected(*this, cb); };
        radio_buttons.push_back(cb);
        p.y += cb->content.size.height + get_padding().get_vertical();
        height += cb->content.size.height + get_padding().get_vertical();
    }
    content.resize(width, height);
}
