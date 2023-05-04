/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cstdint>

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
};

struct Position {
    int x;
    int y;

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
