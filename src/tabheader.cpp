/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "tabheader.h"
#include "theme.h"
#include <spdlog/spdlog.h>

TabHeader::TabHeader() : Widget() {
    //
}

auto TabHeader::add_tab(const std::string_view name) -> int {
    this->names.push_back(name);
    return this->names.size() - 1;
}

auto TabHeader::remove_tab(int index) -> void { this->names.erase(this->names.begin() + index); }

auto TabHeader::set_active_tab(int index) -> void {
    if (this->active_tab == index) {
        return;
    }
    this->active_tab = index;
    this->invalidate();
    if (this->on_item_selected) {
        on_item_selected(*this, index);
    }
}

auto TabHeader::get_tab_string(int index) const -> std::string_view {
    if (index < 0 || index >= names.size()) {
        return "";
    }
    return this->names[index];
}

auto default_padding_y = 10;
auto default_padding_x = 20;

auto TabHeader::draw() -> void {
    auto theme = get_theme();
    auto offset = 0;
    auto &font = theme->font;
    auto active_bg = theme->colors.window_background;
    auto non_active_bg = Darker(theme->colors.window_background);

    theme->draw_widget_background(content, has_focus);

    this->tab_offset.clear();
    this->tab_offset.resize(names.size());
    auto i = 0;
    for (auto s : names) {
        auto is_active_tab = i == this->active_tab;
        auto is_hover = i == this->hover_tab && mouse_over;
        auto padding_x = default_padding_x;
        auto padding_y = 5;

        auto tab_size = font.text_size(s);
        auto bg_color = i == active_tab ? active_bg : non_active_bg;
        auto position_y = is_active_tab ? 0 : padding_y;

        this->content.fill_rect(offset, position_y, tab_size.width + padding_x * 2,
                                tab_size.height + padding_y * 2, bg_color);
        if (is_active_tab) {
            theme->draw_frame(this->content, {offset, position_y},
                              {tab_size.width + padding_x * 2, tab_size.height + padding_y * 2 + 5},
                              FrameStyles::Hover, FrameSize::SingleFrame);
        }

        font.write(this->content, {offset + padding_x, padding_y}, s,
                   is_hover ? Lighter(0x010101, 0.3) : 0x00);
        this->tab_offset[i] = {offset, tab_size.width + padding_x + padding_x};
        offset += tab_size.width;
        offset += padding_x + padding_x;
        i++;
    }

    this->content.fill_rect(0, this->content.size.height - 1, this->content.size.width, 2,
                            active_bg);
}

auto TabHeader::on_mouse_click(const EventMouse &event) -> EventPropagation {
    if (event.pressed) {
        auto index = 0;
        for (const auto &offset : tab_offset) {
            if (event.x > offset.offset && event.x < offset.offset + offset.width) {
                this->needs_redraw = true;
                this->active_tab = index;
                if (this->on_item_selected) {
                    on_item_selected(*this, index);
                }
                return EventPropagation::handled;
            }
            index++;
        }
    }
    return Widget::on_mouse_click(event);
}

auto TabHeader::on_hover(const EventMouse &event) -> void {
    auto hover_tab = -1;
    auto index = 0;
    for (const auto &offset : tab_offset) {
        if (event.x > offset.offset && event.x < offset.offset + offset.width) {
            //            this->needs_redraw = true;
            hover_tab = index;
        }
        index++;
    }

    if (hover_tab >= 0 and hover_tab != this->hover_tab) {
        this->hover_tab = hover_tab;
        auto t = get_tab_string(hover_tab);
        spdlog::info("Mouse under {}", t);
        invalidate();
    }
}

auto TabHeader::size_hint() const -> Size {
    auto hint = this->get_theme()->font.text_size("X");
    hint.width = 0;
    hint.height += default_padding_y * 2;
    return hint;
}
