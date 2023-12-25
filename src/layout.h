#pragma once

#include <list>
#include <memory>
#include <vector>

#include "sizepoint.h"

struct Widget;

struct LayouttItem {
    std::list<std::weak_ptr<LayouttItem>> sub_items;

    virtual auto relayout(Position posiition, const Size size) -> void = 0;

    template <typename T> auto add(T layoutItem) -> T {
        sub_items.push_back(layoutItem);
        return layoutItem;
    };
};

struct HorizontalLayout : LayouttItem {

    auto relayout(Position position, const Size size) -> void override {
        if (sub_items.empty()) {
            return;
        }
        auto recommended_size = size;
        auto width = size.width / sub_items.size();
        recommended_size.width = width;

        for (auto item_iterator : sub_items) {
            if (auto item = item_iterator.lock()) {
                item->relayout(position, recommended_size);
            }
            position.x += width;
        }
    }
};

struct VerticalLayout : LayouttItem {
    auto relayout(Position position, const Size size) -> void override {
        if (sub_items.empty()) {
            return;
        }
        auto recommended_size = size;
        auto height = size.height / sub_items.size();
        recommended_size.height = height;

        for (auto item_iterator : sub_items) {
            if (auto item = item_iterator.lock()) {
                item->relayout(position, recommended_size);
            }
            position.y += height;
        }
    }
};
