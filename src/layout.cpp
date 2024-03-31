/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include <layout.h>

auto HorizontalLayout::relayout(Position position, const Size size) -> void {
    if (sub_items.empty()) {
        return;
    }
    auto recommended_size = Size{};
    auto total_weight = 0.0;

    // first pass - find all widgets with size hints, and subtract them
    // from the list
    auto widget_count = 0;
    auto width = size.width - margin.get_horizontal();
    for (auto &item : sub_items) {
        if (item->ignore_layout()) {
            continue;
        }
        auto hint = item->size_hint();
        if (hint.width <= 0) {
            widget_count++;
            total_weight += item->weight;
        } else {
            width -= hint.width;
        }
        width -= padding.get_horizontal();
    }

    if (widget_count != 0) {
        width += padding.get_horizontal();
    }

    recommended_size.width = width;
    recommended_size.height = size.height - margin.get_vertical();
    position.y += margin.top;
    position.x += margin.start;

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
    for (auto &item : sub_items) {
        if (item->ignore_layout()) {
            continue;
        }
        auto hint = item->size_hint();
        if (hint.width <= 0) {
            recommended_size.width = std::lround((width * item->weight) / total_weight);

        } else {
            recommended_size.width = hint.width;
        }

        if (hint.height > 0) {
            recommended_size.height = hint.height;
        }

        if (item != *sub_items.begin()) {
            position.x += padding.start;
        }
        item->relayout(position, recommended_size);
        position.x += recommended_size.width;
        if (item != *std::prev(sub_items.end())) {
            position.x += padding.end;
        }
    }
}

auto HorizontalLayout::size_hint() const -> Size {
    auto hint = Size{};
    auto wideget_count = 0;
    auto has_auto_width = false;
    auto has_auto_height = false;

    for (auto &item : sub_items) {
        if (item->ignore_layout()) {
            continue;
        }
        wideget_count++;
        auto item_hint = item->size_hint();
        if (item_hint.width != 0) {
            hint.width = std::max(item_hint.width, hint.width);
        } else {
            has_auto_width = true;
        }
        if (item_hint.height > 0) {
            hint.height = std::max(item_hint.height, hint.height);
        } else {
            has_auto_height = true;
        }
        hint.width += padding.get_horizontal();
    }

    if (wideget_count != 0) {
        // Reduce start/end padding. We count inner padding only.
        hint.width += padding.get_horizontal();

        hint.width += margin.get_horizontal();
        hint.height += margin.get_vertical();
        if (has_auto_width) {
            hint.width = 0;
        }
        if (has_auto_height) {
            hint.height = 0;
        }
    }
    return hint;
}

auto VerticalLayout::relayout(Position position, const Size size) -> void {
    if (sub_items.empty()) {
        return;
    }
    auto recommended_size = Size{};
    auto total_weight = 0.0;

    // first pass - find all widgets with size hints, and subtract them
    // from the list
    auto widget_count = 0;
    auto height = size.height - margin.get_vertical();
    for (auto &item : sub_items) {
        if (item->ignore_layout()) {
            continue;
        }
        auto hint = item->size_hint();
        if (hint.height <= 0) {
            widget_count++;
            total_weight += item->weight;
        } else {
            height -= hint.height;
        }
        height -= padding.get_vertical();
    }

    if (widget_count != 0) {
        height += padding.get_vertical();
    }

    recommended_size.height = height;
    recommended_size.width = size.width - margin.get_horizontal();
    position.y += margin.top;
    position.x += margin.start;

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
    for (auto &item : sub_items) {
        if (item->ignore_layout()) {
            continue;
        }
        auto hint = item->size_hint();
        if (hint.height <= 0) {
            recommended_size.height = height / widget_count;
            recommended_size.height = std::lround((height * item->weight) / total_weight);
        } else {
            recommended_size.height = hint.height;
        }
        if (hint.width > 0) {
            recommended_size.width = hint.width;
        }

        if (item != *sub_items.begin()) {
            position.y += padding.top;
        }
        item->relayout(position, recommended_size);
        position.y += recommended_size.height;
        if (item != *std::prev(sub_items.end())) {
            position.y += padding.bottom;
        }
    }
}

auto VerticalLayout::size_hint() const -> Size {
    auto hint = Size{};
    auto widget_count = 0;
    auto has_auto_width = false;
    auto has_auto_height = false;

    for (auto &item : sub_items) {
        if (item->ignore_layout()) {
            continue;
        }
        widget_count++;
        auto item_hint = item->size_hint();
        if (item_hint.height != 0) {
            hint.height = std::max(item_hint.height, hint.height);
        } else {
            has_auto_height = true;
        }

        if (item_hint.width != 0) {
            hint.width = std::max(item_hint.width, hint.width);
        } else {
            has_auto_width = true;
        }
        hint.height += this->padding.get_vertical();
    }

    if (widget_count != 0) {
        // Reduce start/end padding. We count inner padding only.
        hint.width += padding.get_horizontal();

        hint.width += margin.get_horizontal();
        hint.height += margin.get_vertical();
        if (has_auto_width) {
            hint.width = 0;
        }
        if (has_auto_height) {
            hint.height = 0;
        }
    }
    return hint;
}
