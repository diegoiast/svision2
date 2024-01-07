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
    std::list<std::shared_ptr<LayouttItem>> sub_items;
    LayoutParams padding = {};
    LayoutParams margin = {};
    double weight = 1;

    virtual auto relayout(Position posiition, const Size size) -> void = 0;
    virtual auto size_hint() const -> Size = 0;
    virtual auto ignore_layout() const -> bool { return false; }

    template <typename T> auto add(T layoutItem) -> T {
        sub_items.push_back(layoutItem);
        return layoutItem;
    };
};

struct HorizontalSpacer : LayouttItem {
    virtual auto relayout(Position position, const Size size) -> void override{};
    virtual auto size_hint() const -> Size override { return {0, 1}; }
};

struct VerticalSpacer : LayouttItem {
    virtual auto relayout(Position position, const Size size) -> void override{};
    virtual auto size_hint() const -> Size override { return {1, 0}; }
};

struct HorizontalLayout : LayouttItem {
    virtual auto relayout(Position position, const Size size) -> void override;
    virtual auto size_hint() const -> Size override;
};

struct VerticalLayout : LayouttItem {
    virtual auto relayout(Position position, const Size size) -> void override;
    virtual auto size_hint() const -> Size override;
};
