#pragma once

#include <string>
#include <memory>

struct Event
{
};

struct EventKeyboard : public Event
{
    int modifiers;
    int key;
    bool keydown;
};

struct EventMouse : public Event
{
    bool pressed = false;
    int button = -1;
    int x = -1;
    int y = -1;
};

struct Size
{
    int width = 0;
    int height = 0;
};

struct PlatformWindow
{
    uint32_t *buf;
    std::string title;
    Size size;

    virtual auto on_keyboard(const EventKeyboard &) -> void
    {
        // by default, do nothing
    }

    virtual auto on_mouse(const EventMouse &) -> void
    {
        // by default, do nothing
    }

    virtual auto can_close() -> bool
    {
        return true;
    }

    auto inline put_pixel(int x, int y, uint32_t color) -> void
    {
        buf[(y * size.width) + x] = color;
    }

    auto inline get_pixel(int x, int y) -> uint32_t
    {
        return buf[(y * size.width) + x];
    }

    auto fill_rect(int x, int y, int w, int h, uint32_t c) -> void
    {
        for (int row = 0; row < h; row++)
        {
            for (int col = 0; col < w; col++)
            {
                put_pixel(x + col, y + row, c);
            }
        }
    }

    auto fill_circle(int x, int y, int r, uint32_t c) -> void
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

    auto line(struct fenster *f, int x0, int y0, int x1, int y1, uint32_t c) -> void
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

    auto fill(struct fenster *f, int x, int y, uint32_t old, uint32_t c) -> void
    {
        if (x < 0 || y < 0 || x >= size.width || y >= size.height)
        {
            return;
        }
        if (get_pixel(x, y) == old)
        {
            put_pixel(x, y, c);
            fill(f, x - 1, y, old, c);
            fill(f, x + 1, y, old, c);
            fill(f, x, y - 1, old, c);
            fill(f, x, y + 1, old, c);
        }
    }
};

class Platform
{
public:
    bool exit_loop = false;

    bool close_on_last_window = true;
    virtual auto init() -> void = 0;
    virtual auto done() -> void = 0;
    virtual auto open_window(int x, int y, int width, int height, const std::string title) -> std::shared_ptr<PlatformWindow> = 0;
    virtual auto main_loop() -> void = 0;
};
