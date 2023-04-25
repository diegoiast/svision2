#pragma once

#include <cstdint>
#include <cassert>

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
};
