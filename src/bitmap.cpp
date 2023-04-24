#include "bitmap.h"
#include <cstdlib>
#include <algorithm>

auto Bitmap::resize(int width, int height) -> void
{
    auto new_buffer = new uint32_t[width * height];
    auto xx = std::min(width, size.width);
    auto yy = std::min(height, size.height);

    // todo - instead of filling the whole buffer we could clean
    // the right, and bottom dirty parts.

    std::fill_n(new_buffer, width * height, background_color);
    if (yy > 0 && xx > 0)
    {
        for (auto y = 0; y < yy; y++)
        {
            for (auto x = 0; x < xx; x++)
            {
                new_buffer[(y * width) + x] = get_pixel(x, y);
            }
        }
    }

    delete[] buf;
    buf = new_buffer;
    size.width = width;
    size.height = height;
}

auto Bitmap::fill_rect(int x, int y, int w, int h, uint32_t c) -> void
{
    for (int row = 0; row < h; row++)
    {
        for (int col = 0; col < w; col++)
        {
            put_pixel(x + col, y + row, c);
        }
    }
}

auto Bitmap::fill_circle(int x, int y, int r, uint32_t c) -> void
{
    for (int dy = -r; dy < r; dy++)
    {
        for (int dx = -r; dx < r; dx++)
        {
            if (dx * dx + dy * dy <= r * r)
                put_pixel(x + dx, y + dy, c);
        }
    }
}

auto Bitmap::line(int x0, int y0, int x1, int y1, uint32_t c) -> void
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = (dx > dy ? dx : -dy) / 2, e2;
    for (;;)
    {
        put_pixel(x0, y0, c);
        if (x0 == x1 && y0 == y1)
        {
            break;
        }
        e2 = err;
        if (e2 > -dx)
        {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dy)
        {
            err += dx;
            y0 += sy;
        }
    }
}

auto Bitmap::fill(int x, int y, uint32_t old, uint32_t c) -> void
{
    if (x < 0 || y < 0 || x >= size.width || y >= size.height)
    {
        return;
    }
    if (get_pixel(x, y) == old)
    {
        put_pixel(x, y, c);
        fill(x - 1, y, old, c);
        fill(x + 1, y, old, c);
        fill(x, y - 1, old, c);
        fill(x, y + 1, old, c);
    }
}
