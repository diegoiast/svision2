/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <layout.h>

auto HorizontalLayout ::relayout(Position position, const Size size) -> void {
    if (sub_items.empty()) {
        return;
    }
    auto recommended_size = Size{};
    auto total_padding = sub_items.size() * padding.get_horizontal();
    auto width = (size.width - total_padding - margin.get_horizontal()) / sub_items.size();
    recommended_size.width = width;
    recommended_size.height = size.height - margin.get_vertical() - padding.get_vertical();
    position.y += margin.top + padding.top;
    position.x += margin.start;

    for (auto item_iterator : sub_items) {
        position.x += padding.start;
        if (auto item = item_iterator.lock()) {
            item->relayout(position, recommended_size);
        }
        position.x += width;
        position.x += padding.end;
    }
}

auto VerticalLayout::relayout(Position position, const Size size) -> void {
    if (sub_items.empty()) {
        return;
    }
    auto recommended_size = Size{};
    auto total_padding = sub_items.size() * padding.get_vertical();
    auto height = (size.height - total_padding - margin.get_vertical()) / sub_items.size();
    recommended_size.height = height;
    recommended_size.width = size.width - margin.get_horizontal() - padding.get_horizontal();
    position.y += margin.top;
    position.x += margin.start + padding.start;

    for (auto item_iterator : sub_items) {
        position.y += padding.top;
        if (auto item = item_iterator.lock()) {
            item->relayout(position, recommended_size);
        }
        position.y += height;
        position.y += padding.bottom;
    }
}
