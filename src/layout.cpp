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

    // first pass - find all widgets with size hints, and subtract them
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

    if (width < 0) {
        width = 0;
    }
    if (recommended_size.height < 0) {
        recommended_size.height = 0;
    }
    if (recommended_size.width < 0) {
        recommended_size.width = 0;
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
            position.x += recommended_size.width;
            position.x += margin.end;
        }
    }
}

auto HorizontalLayout::size_hint() const -> Size {
    auto hint = Size{};
    auto found = 0;
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            if (item->ignore_layout()) {
                continue;
            }
            found++;
            auto item_hint = item->size_hint();
            hint.height = std::max(item_hint.height, hint.height);
            hint.width = std::max(item_hint.width, hint.width);
        }
    }

    if (found == 0) {
        return {};
    }
    hint.width += margin.get_horizontal();
    hint.height += margin.get_vertical();
    return hint;
}

auto VerticalLayout::relayout(Position position, const Size size) -> void {
    if (sub_items.empty()) {
        return;
    }
    auto recommended_size = Size{};
    auto total_padding = 0;

    // first pass - find all widgets with size hints, and subtract them
    // from the list
    auto widget_count = 0;
    auto height = size.height - margin.get_vertical();
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            if (item->ignore_layout()) {
                continue;
            }
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

    if (height < 0) {
        height = 0;
    }
    if (recommended_size.height < 0) {
        recommended_size.height = 0;
    }
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
            position.y += recommended_size.height;
            position.y += margin.bottom;
        }
    }
}

auto VerticalLayout::size_hint() const -> Size {
    auto hint = Size{};
    auto found = 0;
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            found++;
            auto item_hint = item->size_hint();
            hint.height = std::max(item_hint.height, hint.height);
            hint.width = std::max(item_hint.width, hint.width);
        }
    }

    if (found == 0) {
        return {};
    }
    hint.width += margin.get_horizontal();
    hint.height += margin.get_vertical();
    return hint;
}
