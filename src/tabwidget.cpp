/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <button.h>
#include <tabwidget.h>

TabWidget::TabWidget() : Widget() {
    layout = std::make_shared<VerticalLayout>();
    top_layout = layout->add(std::make_shared<HorizontalLayout>());
    headers = add_new_to_layout<TabHeader>(top_layout);
    stacked_widgets = add_new_to_layout<Stackwidget>(layout);

    headers->on_item_selected = [this](auto &, auto new_index) {
        stacked_widgets->set_current_page(new_index);
        if (on_page_selected) {
            on_page_selected(*this, new_index);
        }
    };
}

auto TabWidget::set_active_page(size_t new_index) -> void {
    this->headers->set_active_tab(new_index);
    this->stacked_widgets->set_current_page(new_index);
}

auto TabWidget::remove_page(size_t position) -> void {
    this->headers->remove_tab(position);
    this->stacked_widgets->remove_page(position);
    this->needs_redraw = true;
}

auto TabWidget::set_buttons(std::shared_ptr<Button> start, std::shared_ptr<Button> end) -> void {
    top_layout->remove_all();

    if (start) {
        start->window = window;
        top_layout->add(start);
        Widget::widgets.add(start, window);
        start->parent = this;
    }
    top_layout->add(headers);

    if (end) {
        end->window = window;
        top_layout->add(end);
        Widget::widgets.add(end, window);
        end->parent = this;
    }
    invalidate();
}
