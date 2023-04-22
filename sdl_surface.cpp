#include <cstdint>
#include "SDL2/SDL.h"
#include "sdl_surface.h"

namespace svision2 {

uint32_t pixelFormat = SDL_PIXELFORMAT_RGBA8888;

sdl_surface::sdl_surface()
{
#if 1
    window   = SDL_CreateWindow("KangaWorld Debug Client", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, xRes, yRes, SDL_WINDOW_ALLOW_HIGHDPI);
    surface  = SDL_GetWindowSurface(window);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    //texture  = SDL_CreateTexture(renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING, xRes, yRes);
#else
    SDL_CreateWindowAndRenderer(xRes, yRes, 0, &window, &renderer); 
#endif
}

sdl_surface::~sdl_surface()
{
    // SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

size_t sdl_surface::width()
{
    return xRes;
}

size_t sdl_surface::height()
{
    return yRes;
}

void sdl_surface::setPixel(size_t x, size_t y, color color)
{
    auto r = getR(color);
    auto g = getG(color);
    auto b = getB(color);
    auto a = getA(color);
    SDL_SetRenderDrawColor(renderer, r, g, b, a );
    SDL_RenderDrawPoint(renderer, x, y);
}

color sdl_surface::getPixel(size_t x, size_t y)
{
    auto bpp = 4;
    auto *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
    return makeRGBA(p[0], p[1], p[2], p[3]);
}

void sdl_surface::sync()
{
    SDL_RenderPresent(renderer);
}

}
