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

    virtual auto on_mouse_click(const EventMouse &event) -> EventPropagation {
        if (event.is_local) {
            return ListView::on_mouse_click(event);
        }
        if (on_abort) {
            on_abort(*this);
        }
        return EventPropagation::handled;
    }

    virtual auto on_keyboard(const EventKeyboard &event) -> EventPropagation {
        if (event.key == KeyCodes::Escape) {
            if (on_abort) {
                on_abort(*this);
            }
            return EventPropagation::handled;
        }
        return ListView::on_keyboard(event);
    }
};

auto Combobox::show_popup() -> void {
    if (!popup_list) {
        auto position = Position{this->position.x, this->position.y + this->content.size.height};
        auto size = Size{this->content.size.width, 100};
        popup_list = window->add_new<ComboboxList>(position, size);
        popup_list->adapter = std::make_shared<ListItemAdapter>(strings);
        popup_list->on_item_selected = [this](auto &listview, auto index, auto reason) {
            this->selected_item = index;
            this->needs_redraw = true;
            if (reason == ListView::SelectionReason::Mouse ||
                reason == ListView::SelectionReason::Keyboard) {
                this->popup_list->hide();
                window->focus_widget(shared_from_this());
            }
        };
        popup_list->on_abort = [this](auto &listview) {
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
        window->focus_widget(popup_list);
    }
}
