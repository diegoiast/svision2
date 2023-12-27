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
    auto total_padding = 0;

    // first pass - find all widgets with size hints, and substract them
    // from the list
    auto widget_count = 0;
    auto width = size.width - margin.get_horizontal();
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            auto hint = item->size_hint();
            if (hint.width <= 0) {
                widget_count++;
                total_padding += padding.get_horizontal();
            } else {
                width -= hint.width + item->padding.get_horizontal();
            }
        }
    }
    if (widget_count != 0) {
        width = (width - total_padding - margin.get_horizontal()) / widget_count;
    }
    recommended_size.width = width;
    recommended_size.height = size.height - margin.get_vertical() - padding.get_vertical();
    position.y += margin.top + padding.top;
    position.x += margin.start + padding.start;

    if (recommended_size.height < 0) {
        recommended_size.height = 0;
    }

    // Second pass - resize items. Width is computed, unless the
    // widget has a size hint. In such case - we enforce it.
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            auto hint = item->size_hint();
            if (hint.width <= 0) {
                recommended_size.width = width;
            } else {
                recommended_size.width = hint.width;
            }
            position.x += padding.start;
            if (auto item = item_iterator.lock()) {
                item->relayout(position, recommended_size);
            }
            if (hint.width > 0) {
                position.x += padding.end;
            }
            position.x += recommended_size.width;
        }
    }
}

auto VerticalLayout::relayout(Position position, const Size size) -> void {
    if (sub_items.empty()) {
        return;
    }
    auto recommended_size = Size{};
    auto total_padding = 0;

    // first pass - find all widgets with size hints, and substract them
    // from the list
    auto widget_count = 0;
    auto height = size.height - margin.get_vertical();
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            auto hint = item->size_hint();
            if (hint.height <= 0) {
                widget_count++;
                total_padding += padding.get_vertical();
            } else {
                height -= hint.height + item->padding.get_vertical();
            }
        }
    }
    if (widget_count != 0) {
        height = (height - total_padding - margin.get_vertical()) / widget_count;
    }

    recommended_size.height = height;
    recommended_size.width = size.width - margin.get_horizontal() - padding.get_horizontal();
    position.y += margin.top + padding.top;
    position.x += margin.start + padding.start;

    if (recommended_size.width < 0) {
        recommended_size.width = 0;
    }

    // Second pass - resize items. Height is computed, unless the
    // widget has a size hint. In such case - we enforce it.
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            auto hint = item->size_hint();
            if (hint.height <= 0) {
                recommended_size.height = height;
            } else {
                recommended_size.height = hint.height;
            }
            position.y += padding.top;
            if (auto item = item_iterator.lock()) {
                item->relayout(position, recommended_size);
            }
            if (hint.height > 0) {
                position.y += padding.bottom;
            }
            position.y += recommended_size.height;
        }
    }
}
