/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include <cassert>
#include <cmath>
#include <string>
#include <vector>

#include <sizepoint.h>

struct HSL {
    double h = 0.0;
    double s = 0.0;
    double l = 0.0;
};

auto rgbToHSL(const uint32_t rgb) -> HSL;

auto hslToRGB(const HSL &hsl) -> int32_t;

auto constexpr inline MakeColor(uint8_t r, uint8_t g, uint8_t b) -> uint32_t {
    return r << 16 | g << 8 | b;
}

auto constexpr inline GetRed(uint32_t color) -> uint8_t { return color >> 16; }

auto constexpr inline GetGreen(uint32_t color) -> uint8_t { return (color >> 8) & 0x00ff; }

auto constexpr inline GetBlue(uint32_t color) -> uint8_t { return color & 0x00ff; }

auto constexpr inline get_bit(unsigned char number, char n) -> bool { return (number >> n) & 1; }

auto constexpr inline set_bit(unsigned char number, char n, bool on) -> int {
    if (on) {
        return number | 1 << n;
    }
    return number & ~(1 << n);
}

auto constexpr inline toggle_bit(unsigned char number, char n) -> int { return number ^ 1 << n; }

struct Gradient {
    double r_step;
    double g_step;
    double b_step;
    double r_current;
    double g_current;
    double b_current;

    Gradient(uint32_t color1, uint32_t color2, int steps) {
        auto r_start = GetRed(color1);
        auto g_start = GetGreen(color1);
        auto b_start = GetBlue(color1);
        auto r_end = GetRed(color2);
        auto g_end = GetGreen(color2);
        auto b_end = GetBlue(color2);

        r_step = (r_end - r_start) / steps;
        g_step = (g_end - g_start) / steps;
        b_step = (b_end - b_start) / steps;

        r_current = r_start;
        g_current = g_start;
        b_current = b_start;
    }

    auto next() -> void {
        r_current += r_step;
        g_current += g_step;
        b_current += b_step;
    }

    auto get_color() -> uint32_t {
        return MakeColor(std::round(r_current), std::round(g_current), std::round(b_current));
    }
};

auto Lighter(uint32_t color, double percentage = 0.1) -> uint32_t;
auto Darker(uint32_t color, double percentage = 0.1) -> uint32_t;

struct Bitmap {
    uint32_t background_color = 0;
    //    uint32_t *buf = 0;
    std::vector<uint32_t> buffer;
    Size size = {0, 0};

    virtual ~Bitmap() {}

    auto inline put_pixel(int x, int y, uint32_t color) -> void {
        if (x < 0 || x >= size.width)
            return;
        if (y < 0 || y >= size.height)
            return;
        buffer[(y * size.width) + x] = color;
    }

    auto inline get_pixel(int x, int y) const -> uint32_t {
        if (x >= size.width)
            return 0;
        if (y >= size.height)
            return 0;
        return buffer[(y * size.width) + x];
    }

    auto blend_pixel(int x, int y, uint32_t color, uint8_t alpha) -> void;

    auto resize(int width, int height) -> void;
    auto resize(Size size) -> void { resize(size.width, size.height); }

    auto fill(uint32_t color) -> void { std::fill(buffer.begin(), buffer.end(), color); }
    auto fill_rect(int x, int y, int w, int h, uint32_t color) -> void;
    auto fill_rect_gradient(int x, int y, int w, int h, uint32_t color1, uint32_t color2) -> void;
    auto fill_circle(int x, int y, int r, uint32_t colo) -> void;
    auto fill_elipse(int x, int y, int width, int height, uint32_t color) -> void;

    auto draw_circle(int x, int y, int r, uint32_t color) -> void;
    auto draw_elipse(int x0, int y0, int x1, int y1, uint32_t color) -> void;
    auto draw_bezier(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) -> void;

    auto line(int x0, int y0, int x1, int y1, uint32_t color) -> void;
    auto line_thikness(int x0, int y0, int x1, int y1, int thickness, uint32_t color) -> void;
    auto draw_rectangle(int x, int y, int width, int height, uint32_t color1, uint32_t color2)
        -> void;
    auto draw_rounded_rectangle(int x, int y, int width, int height, int radius, uint32_t color1,
                                uint32_t color2) -> void;

    auto fill(int x, int y, uint32_t old, uint32_t color) -> void;
    auto draw(Position position, const Bitmap &other) -> void;
};
