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

    // first pass - find all widgets with size hints, and subtract them
    // from the list
    auto widget_count = 0;
    auto width = size.width - margin.get_horizontal();
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            if (item->ignore_layout()) {
                continue;
            }
            auto hint = item->size_hint();
            if (hint.width <= 0) {
                widget_count++;
            } else {
                width -= hint.width;
            }
            width -= padding.get_horizontal();
        }
    }
    if (widget_count != 0) {
        width = width / widget_count;
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
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            if (item->ignore_layout()) {
                continue;
            }
            auto hint = item->size_hint();
            if (hint.width <= 0) {
                recommended_size.width = width;
            } else {
                recommended_size.width = hint.width;
            }

            if (hint.height > 0) {
                recommended_size.height = hint.height;
            }
            position.x += padding.start;
            item->relayout(position, recommended_size);
            position.x += recommended_size.width;
            position.x += padding.end;
        }
    }
}

auto HorizontalLayout::size_hint() const -> Size {
    auto hint = Size{};
    auto found = 0;
    auto has_auto_size = false;
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            if (item->ignore_layout()) {
                continue;
            }
            found++;
            auto item_hint = item->size_hint();
            if (item_hint.width != 0) {
                hint.width = std::max(item_hint.width, hint.width);
            }
            if (item_hint.height > 0) {
                hint.height = std::max(item_hint.height, hint.height);
            } else {
                has_auto_size = true;
            }
            hint.width += this->padding.get_horizontal();
        }
    }

    if (found != 0) {
        hint.width += margin.get_horizontal();
        hint.height += margin.get_vertical();
        if (has_auto_size) {
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
            } else {
                height -= hint.height;
            }
            height -= padding.get_vertical();
        }
    }
    if (widget_count != 0) {
        height = height / widget_count;
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
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            if (item->ignore_layout()) {
                continue;
            }
            auto hint = item->size_hint();
            if (hint.height <= 0) {
                recommended_size.height = height;
            } else {
                recommended_size.height = hint.height;
            }
            // TODO - this breaks the display. I think the type hints is borked.
            if (hint.width > 0) {
                //                recommended_size.width = hint.width;
            }
            position.y += padding.top;
            item->relayout(position, recommended_size);
            position.y += recommended_size.height;
            position.y += padding.bottom;
        }
    }
}

auto VerticalLayout::size_hint() const -> Size {
    auto hint = Size{};
    auto found = 0;
    auto has_auto_size = false;
    for (auto item_iterator : sub_items) {
        if (auto item = item_iterator.lock()) {
            if (item->ignore_layout()) {
                continue;
            }
            found++;
            auto item_hint = item->size_hint();
            if (item_hint.height != 0) {
                hint.height = std::max(item_hint.height, hint.height);
            }
            if (item_hint.width != 0) {
                hint.width = std::max(item_hint.width, hint.width);
            } else {
                has_auto_size = true;
            }
            hint.height += this->padding.get_vertical();
        }
    }

    if (found != 0) {
        hint.width += margin.get_horizontal();
        hint.height += margin.get_vertical();
        if (has_auto_size) {
            hint.width = 0;
        }
    }
    return hint;
}
