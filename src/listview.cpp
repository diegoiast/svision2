/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "listview.h"
#include "scrollbar.h"
#include "theme.h"

auto ListItemAdapter::get_widget(size_t /*position*/, std::shared_ptr<Theme> theme) -> PWidget {
    auto font_size =
        theme->font.text_size("X") + theme->get_padding(PaddingStyle::Label).get_vertical();
    auto position = Position{0, 0};
    auto size = font_size;
    auto p = std::make_shared<ListItemWidget>(position, size, "");
    p->can_focus = true;
    p->draw_background = false;
    p->theme = theme;
    return p;
}

auto ListItemAdapter::set_content(PWidget widget, size_t position, ItemStatus status) -> void {
    auto item = std::dynamic_pointer_cast<ListItemWidget>(widget);
    item->text = strings.at(position);
    item->status = status;
}

ListView::ListView(Position position, Size size) : Widget(position, size, 0) {
    position.x = size.width - 24;
    position.y = 0;
    this->scrollbar = add_new<ScrollBar>(position, size.height, false);
    this->scrollbar->did_change = [this](auto *s, int value) { this->invalidate(); };

    // We will complete redraw the background, the theme should only draw the frame
    // and sub children.
    this->draw_background = false;
    this->can_focus = true;
    this->frame = {FrameStyles::Reversed, FrameSize::SingleFrame};
}

auto ListView::draw() -> void {
    auto t = get_theme();
    t->draw_listview_background(content, has_focus, true);
    if (reserved_widgets.empty()) {
        did_adapter_update();
    }

    auto first_widget = adapter->get_widget(0, t);
    auto item_height = (first_widget->content.size.height);
    auto widget_count = this->content.size.height / item_height + 1;
    auto first_item = scrollbar->value / item_height;
    auto offset = -(scrollbar->value % item_height);

    for (auto i = 0; i < widget_count; i++) {
        auto position = Position{0, offset};
        auto size = Size{this->content.size.width - this->scrollbar->content.size.width,
                         first_widget->content.size.height};
        auto status = ItemStatus{false, false};
        auto w = reserved_widgets[i];
        if (!w) {
            continue;
        }

        if (first_item >= adapter->get_count()) {
            w->hide();
            continue;
        }
        status.is_active = this->current_item == first_item;
        w->position = position;
        if (size.height < 0)
            size.height = 0;
        if (size.width < 0)
            size.width = 0;
        w->content.resize(size);
        adapter->set_content(w, first_item, status);
        if (!w->is_visible()) {
            w->show();
        } else {
            w->invalidate();
        }
        offset += item_height;
        first_item++;
    }
    get_theme()->draw_listview_background(content, has_focus, false);
    Widget::draw();

    auto frame_proxy = this->frame;
    if (can_focus && t->modify_frame_on_hover()) {
        // Setting hover frame works only on selectable widgets
        if (frame_proxy.style == FrameStyles::Normal ||
            frame_proxy.style == FrameStyles::Reversed) {
            if (this->has_focus) {
                // TODO - are we missing another frame style?
                frame_proxy.style = FrameStyles::Hover;
            } else if (this->mouse_over) {
                frame_proxy.style = FrameStyles::Hover;
            }
        }
    }
    if (frame_proxy.style != FrameStyles::NoFrame) {
        t->draw_frame(content, {0, 0}, content.size, frame_proxy.style, frame_proxy.size);
    }
}

EventPropagation ListView::on_mouse_click(const EventMouse &event) {
    if (!event.pressed) {
        return Widget::on_mouse(event);
    }

    auto p = Widget::on_mouse_click(event);
    if (p == EventPropagation::handled) {
        return p;
    }

    auto first_widget = adapter->get_widget(0, get_theme());
    auto item_height = (first_widget->content.size.height);
    auto first_item = scrollbar->value / item_height;
    auto offset = -(scrollbar->value % item_height);

    auto clicked_item_offset = (event.y - offset) / item_height;
    this->current_item = clicked_item_offset + first_item;
    invalidate();
    if (this->on_item_selected) {
        this->on_item_selected(*this, current_item, SelectionReason::Mouse);
    }
    return EventPropagation::handled;
}

auto static ensure_item_in_viewport(ListView &l, std::shared_ptr<Theme> theme) {
    auto first_widget = l.adapter->get_widget(0, theme);
    auto item_height = (first_widget->content.size.height);
    auto widget_count = l.content.size.height / item_height;
    auto first_visible_item = l.scrollbar->value / item_height;

    if (l.current_item >= first_visible_item &&
        l.current_item < first_visible_item + widget_count) {
        return;
    }

    if (l.current_item < first_visible_item) {
        l.scrollbar->set_value(item_height * l.current_item);
        return;
    }

    auto lll = (l.current_item - widget_count + 1);
    l.scrollbar->set_value(item_height * lll);
}

auto ListView::on_keyboard(const EventKeyboard &event) -> EventPropagation {
    auto result = EventPropagation::propagate;
    auto old_item = current_item;
    switch (event.key) {
    case KeyCodes::ArrowDown:
        result = EventPropagation::handled;
        if (this->current_item < adapter->get_count() - 1)
            this->current_item++;
        break;
    case KeyCodes::ArrowUp:
        result = EventPropagation::handled;
        if (this->current_item > 0)
            this->current_item--;
        break;
    case KeyCodes::Home:
        result = EventPropagation::handled;
        this->current_item = 0;
        break;
    case KeyCodes::End:
        result = EventPropagation::handled;
        this->current_item = adapter->get_count() - 1;
        break;
    case KeyCodes::PageDown:
        result = EventPropagation::handled;
        this->current_item += adapter->get_count() / 5;
        if (this->current_item >= adapter->get_count())
            this->current_item = adapter->get_count() - 1;
        break;
    case KeyCodes::PageUp:
        result = EventPropagation::handled;
        this->current_item -= adapter->get_count() / 5;
        if (this->current_item < 0)
            this->current_item = 0;
        break;
    default:
        break;
    }

    if (old_item != this->current_item) {
        ensure_item_in_viewport(*this, get_theme());
        invalidate();
        if (this->on_item_selected) {
            this->on_item_selected(*this, current_item, SelectionReason::KeyboardMove);
        }
    } else if (event.key == KeyCodes::Enter) {
        if (this->on_item_selected) {
            this->on_item_selected(*this, current_item, SelectionReason::Keyboard);
        }
    }
    return result;
}

auto ListView::on_resize() -> void {
    auto default_buttons_size = this->scrollbar->get_padding().get_horizontal();
    auto position = Position{content.size.width - default_buttons_size, 0};

    for (auto &w : reserved_widgets) {
        w->hide();
    }
    this->reserved_widgets.clear();
    this->scrollbar->position = position;
    this->scrollbar->content.resize(default_buttons_size, content.size.height);
    this->scrollbar->on_resize();
}

auto ListView::did_adapter_update() -> void {
    reserved_widgets.clear();
    auto t = get_theme();
    auto first_widget = adapter->get_widget(0, t);
    auto item_height = first_widget->content.size.height;
    auto widget_count = (this->content.size.height - 2) / item_height + 1;

    auto k = adapter->get_count() - widget_count;
    if (k < 0)
        k = 0;
    if (adapter->get_count() < widget_count) {
        k = 0;
    }

    // the speed is just to make weird funkey updates
    this->scrollbar->set_values(0, k * item_height, 0, (item_height * 3) / 11);

    widget_count++;
    while (widget_count != 0) {
        // while this would work, this is unacceptable code. It does 3
        // different actions in a single line, and will become undebuggable
        // reserved_widgets.push_back(add(adapter->get_widget(widget_count)));

        auto b = adapter->get_widget(widget_count, t);
        b->hide();
        this->add(b);
        this->reserved_widgets.push_back(b);
        widget_count--;
    }
    this->invalidate();
}

auto ListItemWidget::draw() -> void {
    auto theme = get_theme();
    theme->draw_listview_item(content, text, status, mouse_over);
}
