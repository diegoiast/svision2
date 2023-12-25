/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <list>
#include <memory>
#include <vector>

#include "sizepoint.h"

struct Widget;

struct LayoutParams {
    int top = 0;
    int bottom = 0;
    int start = 0;
    int end = 0;

    auto set_horitzonal(int value) {
        start = value;
        end = value;
    }

    auto set_vertical(int value) {
        top = value;
        bottom = value;
    }

    auto set(int value) {
        top = value;
        bottom = value;
        start = value;
        end = value;
    }

    auto get_horizontal() const -> auto { return start + end; }
    auto get_vertical() const -> auto { return top + bottom; }
};

struct LayouttItem {
    std::list<std::weak_ptr<LayouttItem>> sub_items;
    LayoutParams padding = {};
    LayoutParams margin = {};

    virtual auto relayout(Position posiition, const Size size) -> void = 0;

    template <typename T> auto add(T layoutItem) -> T {
        sub_items.push_back(layoutItem);
        return layoutItem;
    };
};

struct HorizontalLayout : LayouttItem {
    auto relayout(Position position, const Size size) -> void override;
};

struct VerticalLayout : LayouttItem {
    auto relayout(Position position, const Size size) -> void override;
};
