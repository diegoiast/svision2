/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "combobox.h"
#include "button.h"
#include "listview.h"
#include "theme.h"

Combobox::Combobox(const std::vector<std::string_view> &strings) : Combobox({}, 0, strings) {}

Combobox::Combobox(Position position, int width, const std::vector<std::string_view> &strings)
    : Widget(position, {0, 0}, 0) {
    this->strings = strings;
    this->can_focus = true;
    this->frame = {FrameStyles::Reversed, FrameSize::SingleFrame};

    // TODO - find a better size - according to the theme + font size
    this->content.resize({width, 22});
    auto s = Size{22, 22};
    auto p = Position{width - s.width, 0};
    this->popup_button = add_new<Button>(p, s, "*", false, [this](auto &) { show_popup(); });
}

auto Combobox::get_value() const -> std::string_view {
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

    auto text = get_value();
    auto my_theme = get_theme();
    auto text_padding = get_padding().get_horizontal();
    auto color = has_focus ? my_theme->colors.text_selection_color : my_theme->colors.text_color;
    auto text_size = my_theme->font.text_size(text);
    auto centered = content.size.centeredY(text_size, text_padding);
    my_theme->font.write(content, centered, text, color);
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
    auto result = EventPropagation::propagate;
    switch (event.key) {
    case KeyCodes::Enter:
        break;
    case KeyCodes::ArrowUp:
        selected_item--;
        if (selected_item < 0) {
            selected_item = 0;
        }
        result = EventPropagation::handled;
        invalidate();
        break;
    case KeyCodes::ArrowDown:
        selected_item++;
        if (selected_item >= strings.size()) {
            selected_item = strings.size() - 1;
        }
        result = EventPropagation::handled;
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

struct ComboboxList : ListView {
    std::function<void(ComboboxList &)> on_abort;

    ComboboxList(Position position, Size size) : ListView(position, size) {
        this->read_external_mouse_events = true;
    }

    virtual auto on_mouse_click(const EventMouse &event) -> EventPropagation override {
        if (event.is_local) {
            return ListView::on_mouse_click(event);
        }
        if (on_abort) {
            on_abort(*this);
        }
        return EventPropagation::handled;
    }

    virtual auto on_keyboard(const EventKeyboard &event) -> EventPropagation override {
        if (event.key == KeyCodes::Escape) {
            if (on_abort) {
                on_abort(*this);
            }
            return EventPropagation::handled;
        }
        return ListView::on_keyboard(event);
    }

    auto virtual ignore_layout() const -> bool override { return true; }
};

auto Combobox::size_hint() const -> Size {
    auto s = get_theme()->font.text_size("X");
    return {0, s.height + this->get_padding().get_vertical()};
}

auto Combobox::on_resize() -> void {
    auto s = get_theme()->font.text_size("X");
    s.height += get_padding().get_vertical();
    s.width = s.height;

    auto p = Position{content.size.width - s.height, 0};
    this->popup_button->position = p;
    this->popup_button->content.resize(s);
    this->popup_button->invalidate();
}

auto Combobox::show_popup() -> void {
    auto new_position = Position{this->position.x, this->position.y + this->content.size.height};
    if (!popup_list) {
        auto size = Size{this->content.size.width, 100};
        popup_list = window->add_new<ComboboxList>(new_position, size);
        popup_list->adapter = std::make_shared<ListItemAdapter>(strings);
        popup_list->on_item_selected = [this](auto &, auto index, auto reason) {
            this->selected_item = index;
            this->needs_redraw = true;
            if (reason == ListView::SelectionReason::Mouse ||
                reason == ListView::SelectionReason::Keyboard) {
                this->popup_list->hide();
                window->focus_widget(shared_from_this());
                if (this->on_item_selected) {
                    this->on_item_selected(*this, index);
                }
            }
        };
        popup_list->on_abort = [this](auto) {
            this->needs_redraw = true;
            this->popup_list->hide();
            window->focus_widget(shared_from_this());
        };
        popup_list->hide();
    }

    if (popup_list->is_visible()) {
        popup_list->hide();
        window->focus_widget(shared_from_this());
    } else {
        popup_list->position = new_position;
        popup_list->show();
        window->focus_widget(popup_list);
    }
}

auto Combobox::set_active_index(int index) -> void {
    if (index == this->selected_item) {
        return;
    }
    this->selected_item = index;
    this->invalidate();
}

auto Combobox::set_items(const std::vector<std::string_view> &new_strings) -> void {
    this->strings = new_strings;
    if (this->selected_item >= this->strings.size()) {
        this->selected_item = this->strings.size();
    }
    invalidate();
}
