/*
 * This file is part of SVision2
 * Copyright (c) Diego Iastrubni <diegoiast@gmail.com>
 *
 * SPDX-License-Identifier: MIT
 */

#include "bitmap.h"

// this file is autogenerated from old bitmap fonts
#include "fontdos.h"

#include <algorithm>
#include <cmath>
#include <spdlog/spdlog.h>

auto rgbToHSL(const uint32_t rgb) -> HSL {
    // Normalize RGB values
    double normR = GetRed(rgb) / 255.0;
    double normG = GetGreen(rgb) / 255.0;
    double normB = GetBlue(rgb) / 255.0;

    double cmax = fmax(fmax(normR, normG), normB);
    double cmin = fmin(fmin(normR, normG), normB);
    double delta = cmax - cmin;
    HSL hsl;

    if (delta == 0) {
        hsl.h = 0.0; // Undefined, but we set it to 0
    } else if (cmax == normR) {
        hsl.h = 60.0 * fmod(((normG - normB) / delta), 6);
    } else if (cmax == normG) {
        hsl.h = 60.0 * (((normB - normR) / delta) + 2);
    } else if (cmax == normB) {
        hsl.h = 60.0 * (((normR - normG) / delta) + 4);
    }

    hsl.l = 0.5 * (cmax + cmin);

    if (delta != 0) {
        hsl.s = delta / (1 - fabs(2 * hsl.l - 1));
    }

    return hsl;
}

auto hslToRGB(const HSL &hsl) -> int32_t {
    double c = (1 - fabs(2 * hsl.l - 1)) * hsl.s;
    double x = c * (1 - fabs(fmod(hsl.h / 60.0, 2) - 1));
    double m = hsl.l - 0.5 * c;

    double r, g, b;
    if (hsl.h < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (hsl.h < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (hsl.h < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (hsl.h < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (hsl.h < 300) {
        r = x;
        g = 0;
        b = c;
    } else {
        r = c;
        g = 0;
        b = x;
    }

    return MakeColor((r + m) * 255.0, (g + m) * 255.0, (b + m) * 255.0);
}

auto Darker(uint32_t color, double percentage) -> uint32_t {
    auto hsl = rgbToHSL(color);
    hsl.l -= percentage;
    hsl.l = fmax(0.0f, fmax(0.0f, hsl.l));
    return hslToRGB(hsl);
}

auto Lighter(uint32_t color, double percentage) -> uint32_t {
    auto hsl = rgbToHSL(color);
    hsl.l += percentage;
    hsl.l = fmin(1.0f, fmax(0.0f, hsl.l));
    return hslToRGB(hsl);
}

auto Bitmap::blend_pixel(int x, int y, uint32_t color, uint8_t alpha) -> void {
    if (x < 0)
        return;
    if (y < 0)
        return;
    if (x >= size.width)
        return;
    if (y >= size.height)
        return;

    auto color2 = get_pixel(x, y);
    auto red = ((255 - alpha) * GetRed(color2) + alpha * GetRed(color)) / 255;
    auto green = ((255 - alpha) * GetGreen(color2) + alpha * GetGreen(color)) / 255;
    auto blue = ((255 - alpha) * GetBlue(color2) + alpha * GetBlue(color)) / 255;
    put_pixel(x, y, MakeColor(red, green, blue));
}

auto Bitmap::resize(int width, int height) -> void {
    if (width == this->size.width && height == this->size.height) {
        return;
    }

    // no need to reallocate if we have a widget with the same buffer size
    // for example 100x200 -> 200x100, or 10x50 -> 5x100 etc.
    if (width * height != this->size.width * this->size.height) {
        // TODO - should we copy old buffer to new one?
        buffer.resize(width * height);
    }
    size.width = width;
    size.height = height;
}

auto Bitmap::fill_rect(int x, int y, int w, int h, uint32_t c) -> void {
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            put_pixel(x + col, y + row, c);
        }
    }
}

auto Bitmap::fill_rect_gradient(int x, int y, int w, int h, uint32_t color1, uint32_t color2)
    -> void {
    auto gradient = Gradient(color1, color2, h);
    for (int row = 0; row < h; row++) {
        for (int col = 0; col < w; col++) {
            put_pixel(x + col, y + row, gradient.get_color());
        }
        gradient.next();
    }
}

auto Bitmap::fill_circle(int x, int y, int r, uint32_t c) -> void {
    for (int dy = -r; dy < r; dy++) {
        for (int dx = -r; dx < r; dx++) {
            if (dx * dx + dy * dy <= r * r)
                put_pixel(x + dx, y + dy, c);
        }
    }
}

auto Bitmap::fill_elipse(int x, int y, int width, int height, uint32_t color) -> void {
    for (int yy = -height; yy <= height; yy++) {
        for (int xx = -width; xx <= width; xx++) {
            if (xx * xx * height * height + yy * yy * width * width <=
                height * height * width * width)
                put_pixel(xx + x, yy + y, color);
        }
    }
}

auto Bitmap::line(int x0, int y0, int x1, int y1, uint32_t color) -> void {
    int dx = abs(x1 - x0);
    int sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0);
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    for (;;) {  /* loop */
        int e2; /* error value e_xy */

        put_pixel(x0, y0, color);
        if (x0 == x1 && y0 == y1)
            break;
        e2 = 2 * err;

        /* e_xy+e_x > 0 */
        if (e2 >= dy) {
            err += dy;
            x0 += sx;
        }

        /* e_xy+e_y < 0 */
        if (e2 <= dx) {
            err += dx;
            y0 += sy;
        }
    }
}

auto Bitmap::line_thikness(int x0, int y0, int x1, int y1, int thickness, uint32_t color) -> void {
    float dx = x1 - x0;
    float dy = y1 - y0;
    float length = sqrtf(dx * dx + dy * dy);
    float nx = dx / length;
    float ny = dy / length;

    float half_thickness = thickness / 2.0f;
    float x_min = fminf(x0 - half_thickness, x1 - half_thickness);
    float y_min = fminf(y0 - half_thickness, y1 - half_thickness);
    float x_max = fmaxf(x0 + half_thickness, x1 + half_thickness);
    float y_max = fmaxf(y0 + half_thickness, y1 + half_thickness);

    for (int x = roundf(x_min); x <= roundf(x_max); x++) {
        for (int y = roundf(y_min); y <= roundf(y_max); y++) {
            float d = fabsf((x - x0) * ny - (y - y0) * nx);
            // TODO - alpha blend the colors, using `d`
            if (d <= half_thickness) {
                put_pixel(x, y, color);
            }
        }
    }
}

auto Bitmap::draw_rectangle(int x, int y, int width, int height, uint32_t color1, uint32_t color2)
    -> void {
    line(x + 0, y + 0, x + width - 2, y + 0, color1);
    line(x + 0, y + 0, x + 0, y + height - 2, color1);
    line(x + width - 1, y + 0, x + width - 1, y + height - 1, color2);
    line(x + 0, y + height - 1, x + width - 1, y + height - 1, color2);
}

auto Bitmap::draw_rounded_rectangle(int x, int y, int width, int height, int radius,
                                    uint32_t color1, uint32_t color2) -> void {
    // TODO - use rounded corners
    draw_rectangle(x, y, width, height, color1, color2);
}

auto Bitmap::draw_circle(int x, int y, int r, uint32_t color) -> void {
    auto xx = -r;
    auto yy = 0;
    auto err = 2 - 2 * r; /* II. Quadrant */

    do {
        put_pixel(x - xx, y + yy, color); /*   I. Quadrant */
        put_pixel(x - yy, y - xx, color); /*  II. Quadrant */
        put_pixel(x + xx, y - yy, color); /* III. Quadrant */
        put_pixel(x + yy, y + xx, color); /*  IV. Quadrant */
        r = err;
        if (r <= yy)
            err += ++yy * 2 + 1; /* e_xy+e_y < 0 */
        if (r > xx || err > yy)
            err += ++xx * 2 + 1; /* e_xy+e_x > 0 or no 2nd y-step */
    } while (xx < 0);
}

auto Bitmap::draw_elipse(int x0, int y0, int x1, int y1, uint32_t color) -> void {
    int a = abs(x1 - x0), b = abs(y1 - y0), b1 = b & 1;       /* values of diameter */
    long dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a; /* error increment */
    long err = dx + dy + b1 * a * a, e2;                      /* error of 1.step */

    /* if called with swapped points */
    if (x0 > x1) {
        x0 = x1;
        x1 += a;
    }
    /* .. exchange them */
    if (y0 > y1)
        y0 = y1;
    y0 += (b + 1) / 2;
    /* starting pixel */
    y1 = y0 - b1;
    a *= 8 * a;
    b1 = 8 * b * b;

    do {
        put_pixel(x1, y0, color); /*   I. Quadrant */
        put_pixel(x0, y0, color); /*  II. Quadrant */
        put_pixel(x0, y1, color); /* III. Quadrant */
        put_pixel(x1, y1, color); /*  IV. Quadrant */
        e2 = 2 * err;
        /* y step */
        if (e2 <= dy) {
            y0++;
            y1--;
            err += dy += a;
        }
        /* x step */
        if (e2 >= dx || 2 * err > dy) {
            x0++;
            x1--;
            err += dx += b1;
        }
    } while (x0 <= x1);

    /* too early stop of flat ellipses a=1 */
    while (y0 - y1 < b) {
        /* -> finish tip of ellipse */
        put_pixel(x0 - 1, y0, color);
        put_pixel(x1 + 1, y0++, color);
        put_pixel(x0 - 1, y1, color);
        put_pixel(x1 + 1, y1--, color);
    }
}

auto Bitmap::draw_bezier(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) -> void {
    int sx = x2 - x1, sy = y2 - y1;
    /* relative values for checks */
    long xx = x0 - x1, yy = y0 - y1, xy;
    /* curvature */
    double dx, dy, err, cur = xx * sy - yy * sx;
    /* sign of gradient must not change */
    assert(xx * sx <= 0 && yy * sy <= 0);

    /* begin with longer part */
    if (sx * (long)sx + sy * (long)sy > xx * xx + yy * yy) {
        /* swap P0 P2 */
        x2 = x0;
        x0 = sx + x1;
        y2 = y0;
        y0 = sy + y1;
        cur = -cur;
    }
    /* no straight line */
    if (cur != 0) {
        /* x step direction */
        xx += sx;
        xx *= sx = x0 < x2 ? 1 : -1;
        /* y step direction */
        yy += sy;
        yy *= sy = y0 < y2 ? 1 : -1;
        /* differences 2nd degree */
        xy = 2 * xx * yy;
        xx *= xx;
        yy *= yy;
        /* negated curvature? */
        if (cur * sx * sy < 0) {
            xx = -xx;
            yy = -yy;
            xy = -xy;
            cur = -cur;
        }
        /* differences 1st degree */
        dx = 4.0 * sy * cur * (x1 - x0) + xx - xy;
        dy = 4.0 * sx * cur * (y0 - y1) + yy - xy;
        /* error 1st step */
        xx += xx;
        yy += yy;
        err = dx + dy + xy;
        /* plot curve */
        do {
            put_pixel(x0, y0, color);
            /* last pixel -> curve finished */
            if (x0 == x2 && y0 == y2)
                return;
            /* save value for test of y step */
            y1 = 2 * err < dx;
            /* x step */
            if (2 * err > dy) {
                x0 += sx;
                dx -= xy;
                err += dy += yy;
            }
            /* y step */
            if (y1) {
                y0 += sy;
                dy -= xy;
                err += dx += xx;
            }
            /* gradient negates -> algorithm fails */
        } while (dy < dx);
    }
    /* plot remaining part to end */
    line(x0, y0, x2, y2, color);
}

auto Bitmap::fill(int x, int y, uint32_t old, uint32_t c) -> void {
    if (x < 0 || y < 0 || x >= size.width || y >= size.height) {
        return;
    }
    if (get_pixel(x, y) == old) {
        put_pixel(x, y, c);
        fill(x - 1, y, old, c);
        fill(x + 1, y, old, c);
        fill(x, y - 1, old, c);
        fill(x, y + 1, old, c);
    }
}

auto Bitmap::draw(Position position, const Bitmap &other) -> void {
    for (auto y = 0; y < other.size.height; y++) {
        auto yy = y + position.y;
        if (yy > size.height) {
            break;
        }
        for (auto x = 0; x < other.size.width; x++) {
            auto xx = x + position.x;
            if (xx > size.width) {
                break;
            }
            put_pixel(xx, yy, other.get_pixel(x, y));
        }
    }
}
