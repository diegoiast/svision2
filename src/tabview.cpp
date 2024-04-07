/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <tabview.h>

TabView::TabView() : Widget() {
    layout = std::make_shared<VerticalLayout>();
    //    top_layout = layout->add(std::make_shared<HorizontalLayout>());

    //    headers = top_layout->add_new<TabHeader>();
    //    headers->parent = this;
    headers = add_new<TabHeader>();
    widgets = add_new<Stackwidget>();

    headers->on_item_selected = [this](auto &, auto new_index) {
        widgets->set_current_page(new_index);
        if (on_page_selected) {
            on_page_selected(*this, new_index);
        }
    };
}

auto TabView::set_active_page(size_t new_index) -> void {
    this->headers->set_active_tab(new_index);
    this->widgets->set_current_page(new_index);
}
