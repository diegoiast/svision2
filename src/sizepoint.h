/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>

struct LayoutParams {
    int top = 0;
    int bottom = 0;
    int start = 0;
    int end = 0;

    auto set_horizontal(int value) {
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
    auto is_valid() const -> bool { return top != 0 || bottom != 0 || start != 0 || end != 0; }
};

struct Position {
    int x = 0;
    int y = 0;

    auto inline operator+(const int delta) const -> Position {
        auto p = *this;
        p.x += delta;
        p.y += delta;
        return p;
    }
    auto inline operator-(const int delta) const -> Position {
        auto p = *this;
        p.x -= delta;
        p.y -= delta;
        return p;
    }
};

struct Size {
    int width = 0;
    int height = 0;

    auto inline operator==(const Size &other) const -> bool {
        return width == other.width && height == other.height;
    }

    auto inline operator!=(const Size &other) const -> bool {
        return width != other.width || height != other.height;
    }

    auto inline operator-(const Size &other) const -> Size {
        auto s = *this;
        s.width -= other.width;
        s.height -= other.height;
        return s;
    }

    auto inline operator+(const Size &other) const -> Size {
        auto s = *this;
        s.width -= other.width;
        s.height -= other.height;
        return s;
    }

    auto inline operator+(const int delta) const -> Size {
        auto s = *this;
        s.width += delta;
        s.height += delta;
        return s;
    }

    auto inline operator-(const int delta) const -> Size {
        auto s = *this;
        s.width -= delta;
        s.height -= delta;
        return s;
    }

    auto inline operator/(const int ratio) const -> Size {
        auto s = *this;
        s.width /= ratio;
        s.height /= ratio;
        return s;
    }

    auto inline operator*(const int ratio) const -> Size {
        auto s = *this;
        s.width *= ratio;
        s.height *= ratio;
        return s;
    }

    auto inline centered(const Size &other, int padding = 0) const -> Position {
        auto s = *this - other;
        s.width -= padding * 2;
        s.height -= padding * 2;
        return {s.width / 2 + padding, s.height / 2 + padding};
    }

    auto inline centeredX(const Size &other, int padding = 0) const -> Position {
        auto s = *this - other;
        return {s.width / 2, padding};
    }

    auto inline centeredY(const Size &other, int padding = 0) const -> Position {
        auto s = *this - other;
        return {padding, s.height / 2};
    }

    auto inline centeredY(LayoutParams padding = {}) const -> Position {
        return {padding.start, (height - padding.get_vertical()) / 2};
    }

    auto inline centeredX(LayoutParams padding = {}) const -> Position {
        return {(width - padding.get_horizontal()) / 2, padding.top};
    }
};
