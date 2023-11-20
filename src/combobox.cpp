/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "combobox.h"
#include "button.h"
#include "listview.h"
#include "spdlog/spdlog.h"
#include "theme.h"

Combobox::Combobox(Position position, int width, const std::vector<std::string> strings)
    : Widget(position, {0, 0}, 0) {
    this->strings = strings;
    this->can_focus = true;
    this->frame = {FrameStyles::Reversed, FrameSize::SingleFrame};

    // TODO - find a better size - according to the theme + font size
    this->content.resize({width, 20});
    auto s = Size{20, 20};
    auto p = Position{width - s.width, 0};
    add_new<Button>(p, s, "*", false, [this]() { show_popup(); });
}

auto Combobox::get_value() const -> std::string {
    if (this->selected_item < 0) {
        return {};
    }
    if (this->selected_item >= this->strings.size()) {
        return {};
    }
    return this->strings.at(this->selected_item);
}

auto Combobox::draw() -> void {
    Widget::draw();

    auto theme = get_theme();
    auto padding = 5;
    auto color = has_focus ? theme->colors.text_selection_color : theme->colors.text_color;
    content.write_fixed({padding, padding}, get_value(), color);
}

auto Combobox::on_mouse(const EventMouse &event) -> EventPropagation {
    // TODO - do we need nything?
    return Widget::on_mouse(event);
}

auto Combobox::on_focus_change(bool new_state) -> void {
    auto theme = get_theme();
    if (new_state) {
        this->content.background_color = theme->colors.text_selection_background;
    } else {
        this->content.background_color = theme->colors.window_background;
    }
}

auto Combobox::on_keyboard(const EventKeyboard &event) -> EventPropagation {
    // TODO - down,up,home,end, pgup, pgdown
    auto result = EventPropagation::propagate;
    switch (event.key) {
    case KeyCodes::Enter:
        spdlog::info("should open the list");
        break;
    case KeyCodes::ArrowUp:
        selected_item--;
        if (selected_item < 0) {
            selected_item = 0;
        }
        spdlog::info("up - new value = {}", selected_item);
        result = EventPropagation::handled;
        invalidate();
        break;
    case KeyCodes::ArrowDown:
        selected_item++;
        if (selected_item >= strings.size()) {
            selected_item = strings.size() - 1;
        }
        result = EventPropagation::handled;
        spdlog::info("Down - new value = {}", selected_item);
        invalidate();
        break;
    case KeyCodes::Home:
        selected_item = 0;
        result = EventPropagation::handled;
        invalidate();
        break;
    case KeyCodes::End:
        selected_item = strings.size() - 1;
        invalidate();
        result = EventPropagation::handled;
        break;
    default:
        result = Widget::on_keyboard(event);
    }

    return result;
}
auto Combobox::show_popup() -> void {
    spdlog::info("Will show popup");
    if (!popup_list) {
        auto position = Position{this->position.x, this->position.y + this->content.size.height};
        auto size = Size{this->content.size.width, 100};
        popup_list = window->add_new<ListView>(position, size);
        popup_list->adapter = std::make_shared<ListItemAdapter>(strings);
        popup_list->on_item_selected = [this](auto listview, auto index) {
            this->selected_item = index;
            this->popup_list->hide();
            this->invalidate();
            //            this->window->invalidate();
        };
    } else {
        if (popup_list->is_visible()) {
            popup_list->hide();
        } else {
            popup_list->show();
        }
    }
}
