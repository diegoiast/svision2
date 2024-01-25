/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "tabheader.h"
#include "theme.h"

TabHeader::TabHeader() : Widget() {
    //
}

auto TabHeader::add_tab(const std::string_view name) -> int {
    this->names.push_back(name);
    return this->names.size() - 1;
}

auto TabHeader::remove_tab(int index) -> void { this->names.erase(this->names.begin() + index); }

auto TabHeader::set_active_tab(int index) -> void {
    this->active_tab = index;
    this->invalidate();
}

auto TabHeader::get_tab_string(int index) const -> std::string_view {
    if (index < 0 || index >= names.size()) {
        return "";
    }
    return this->names[index];
}

auto default_padding_y = 5;
auto default_padding_x = 20;

auto TabHeader::draw() -> void {
    auto theme = get_theme();
    auto offset = 0;
    auto &font = theme->font;

    auto active_bg = theme->colors.button_background_2;
    auto non_active_bg = Darker(theme->colors.button_background_1);

    theme->draw_widget_background(content, has_focus);

    this->content.fill(0);
    auto i = 0;
    for (auto s : names) {
        auto is_active_tab = i == this->active_tab;
        auto padding_x = default_padding_x;
        auto padding_y = 5;

        auto tab_size = font.text_size(s);
        auto bg_color = i == active_tab ? active_bg : non_active_bg;
        auto position_y = is_active_tab ? 0 : padding_y;

        this->content.fill_rect(offset, position_y, tab_size.width + padding_x * 2,
                                tab_size.height + padding_y * 2, bg_color);

        font.write(this->content, {offset + padding_x, padding_y}, s, 0x00);
        offset += tab_size.width;
        offset += padding_x + padding_x;
        i++;
    }
}

auto TabHeader::size_hint() const -> Size {
    auto hint = this->get_theme()->font.text_size("X");
    hint.width = 0;
    hint.height += default_padding_y * 2;
    return hint;
}
