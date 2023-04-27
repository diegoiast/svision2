#pragma once

#include <cstdint>
#include <cassert>
#include <string>

struct Size
{
    int width = 0;
    int height = 0;

    auto inline operator==(const Size &other) const -> bool
    {
        return width == other.width && height == other.height;
    }
    auto inline operator!=(const Size &other) const -> bool
    {
        return width != other.width || height != other.height;
    }
};

struct Position
{
    int x;
    int y;
};

auto inline MakeColor(char r, char g, char b) -> uint32_t
{
    auto color = r << 16 | g << 8 | b;
    return color;
}

auto inline GetRed(uint32_t color) -> char
{
    return color >> 16;
}

auto inline GetGreen(uint32_t color) -> char
{
    return (color >> 8) & 0x00ff;
}

auto inline GetBlue(uint32_t color) -> char
{
    return color & 0x00ff;
}

auto inline get_bit(unsigned char number, char n) -> bool
{
    return (number >> n) & 1;
}

auto inline set_bit(unsigned char number, char n, bool on) -> int
{
    if (on)
    {
        return number |= 1 << n;
    }

    return number &= ~(1 << n);
}

auto inline toggle_bit(unsigned char number, char n) -> int
{
    return number ^= 1 << n;
}

auto Lighter(uint32_t color, double percentage = 0.1) -> uint32_t;
auto Darker(uint32_t color, double percentage = 0.1) -> uint32_t;

struct Bitmap
{
    uint32_t background_color = 0;
    uint32_t *buf = 0;
    Size size = {0, 0};

    virtual ~Bitmap()
    {
        delete[] buf;
    }

    auto inline put_pixel(int x, int y, uint32_t color) -> void
    {
        buf[(y * size.width) + x] = color;
    }

    auto inline get_pixel(int x, int y) const -> uint32_t
    {
        return buf[(y * size.width) + x];
    }

    auto resize(int width, int height) -> void;
    auto resize(Size size) -> void
    {
        resize(size.width, size.height);
    }

    auto fill_rect(int x, int y, int w, int h, uint32_t c) -> void;
    auto fill_circle(int x, int y, int r, uint32_t c) -> void;
    auto line(int x0, int y0, int x1, int y1, uint32_t c) -> void;
    auto fill(int x, int y, uint32_t old, uint32_t c) -> void;
    auto draw(Position position, const Bitmap &other) -> void;
    auto write_fixed(Position, const std::string &str, const uint32_t color) -> void;
    auto write_fixed_char(Position, char c, const uint32_t color) -> void;
};
