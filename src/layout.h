#pragma once

#include <list>
#include <memory>
#include <vector>

#include "sizepoint.h"

struct Widget;

struct LayouttItem {
    virtual auto relayout(Position posiition, const Size size) -> void = 0;
};

struct HorizontalLayout : LayouttItem {
    std::list<std::weak_ptr<LayouttItem>> items;

    auto relayout(Position position, const Size size) -> void override {
        auto recommended_size = size;
        auto width = size.width / items.size();
        recommended_size.width = width;

        for (auto item_iterator : items) {
            if (auto item = item_iterator.lock()) {
                item->relayout(position, recommended_size);
            }
        }
    }
};

struct VerticalLayout : LayouttItem {
    std::list<std::weak_ptr<LayouttItem>> items;

    auto relayout(Position position, const Size size) -> void override {
        auto recommended_size = size;
        auto height = size.height / items.size();
        recommended_size.height = height;

        for (auto item_iterator : items) {
            if (auto item = item_iterator.lock()) {
                item->relayout(position, recommended_size);
            }
        }
    }
};
