#pragma once

#include "surface.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Surface;
struct SDL_Texture;

namespace svision2 {

// int color = 0xRRGGBBAA;
auto inline getR(svision2::color color) {
    uint8_t red = (color & 0xFF000000) >> 24;
    return red;
}

auto inline getG(svision2::color color) {
    uint8_t green = (color & 0x00FF0000) >> 16;
    return green;
}

auto inline getB(svision2::color color) {
    uint8_t blue = (color & 0x0000FF00) >> 8;
    return blue;
}

auto inline getA(svision2::color color) {
    uint8_t alpha = (color & 0x000000FF);
    return alpha;
}


auto inline makeRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    svision2::color c = 0;
    c |= a;
    c |= b << 8;
    c |= g << 16;
    c |= r << 24;
    return c;
}

class sdl_surface : public surface {
private:
    SDL_Window   *window  = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Surface  *surface  = NULL;
    SDL_Texture  *texture = NULL;

    size_t xRes = 600;
    size_t yRes = 400;

public:
    sdl_surface();
    virtual ~sdl_surface();

    size_t width();
    size_t height();
    void setPixel(size_t x, size_t y, color color);
    color getPixel(size_t x, size_t y);
    void sync();
};

}
