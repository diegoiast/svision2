/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "stackwidget.h"
#include "layout.h"

struct SingleWidgetVisible : LayoutItem {
    virtual auto relayout(Position position, const Size size) -> void {
        for (auto &item : sub_items) {
            item->relayout(position, size);
        }
    };
    virtual auto size_hint() const -> Size { return {0, 0}; }
    virtual auto ignore_layout() const -> bool { return false; }

    auto set_current(size_t c) {
        if (c >= sub_items.size()) {
            return;
        }

        auto i = size_t(0);
        for (auto &item : sub_items) {
            auto w = std::dynamic_pointer_cast<Widget>(item);
            if (w == nullptr) {
                continue;
            }

            if (i == c) {
                w->show();
            } else {
                w->hide();
            }
            i++;
        }
    };

    auto get_current() const -> int {
        auto i = 0;
        for (const auto &item : sub_items) {
            i++;
            auto w = std::dynamic_pointer_cast<Widget>(item);
            if (w == nullptr) {
                continue;
            }
            if (w->is_visible()) {
                return i;
            }
        }
        return -1;
    }
};

Stackwidget::Stackwidget(Position p, Size s) : Widget(p, s, 0) {
    this->layout = std::make_shared<SingleWidgetVisible>();
    this->layout->on_item_added = [](LayoutItem &layout, auto index) {
        auto item = layout.sub_items.back();
        auto w = std::dynamic_pointer_cast<Widget>(item);
        if (w) {
            if (index == 0) {
                w->show();
            } else {
                w->hide();
            }
        }
    };
}

auto Stackwidget::get_current_page() const -> int {
    auto w = std::dynamic_pointer_cast<SingleWidgetVisible>(layout);
    return w->get_current();
}

auto Stackwidget::set_current_page(int i) -> void {
    auto w = std::dynamic_pointer_cast<SingleWidgetVisible>(layout);
    w->set_current(i);
}

auto Stackwidget::get_page(size_t i) -> std::shared_ptr<Widget> {
    return this->widgets.widgets.at(i);
}

auto Stackwidget::remove_page(int i) -> void {
    this->widgets.widgets.erase(this->widgets.widgets.begin() + i);
}

auto Stackwidget::get_page_count() const -> size_t {
    // TODO - this is wrong, as sub items can be layouts not only widgets
    return layout->sub_items.size();
}
