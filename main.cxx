#include <cstdlib>
#include "surface.h"
#include "sdl_surface.h"

#include "SDL2/SDL.h"

int main2()
{
    auto pixelFormat = SDL_PIXELFORMAT_RGBA8888;
    auto xRes = 600;
    auto yRes = 400;

    SDL_Init(SDL_INIT_EVERYTHING);
#if 1
    auto window   = SDL_CreateWindow("TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, xRes, yRes, SDL_WINDOW_ALLOW_HIGHDPI);
    auto surface  = SDL_GetWindowSurface(window);
    auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    // auto texture  = SDL_CreateTexture(renderer, pixelFormat, SDL_TEXTUREACCESS_STREAMING, xRes, yRes);
#else
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(xRes, yRes, 0, &window, &renderer); 
#endif
    auto r = 0xff;
    auto g = 0;
    auto b = 0;
    auto a = 0xff;
    SDL_SetRenderDrawColor(renderer, r, g, b, a );
    for (int y =0; y < 100; y++) {
        for (int x =0; x < 100; x++) {
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
    SDL_RenderPresent(renderer);

    SDL_Event event;
    while (1)
        while (SDL_PollEvent(&event))
        {
            switch (event.type)

            {
                case SDL_QUIT:
                    exit(0);
                    break;

                default:
                    break;
            }
        }
    return EXIT_SUCCESS;
}

int main()
{

    SDL_Init(SDL_INIT_EVERYTHING);
    auto surface = svision2::sdl_surface();

    for (int y =0; y < 100; y++)
        for (int x =0; x < 100; x++)
            surface.setPixel(x, y, svision2::makeRGBA(0xff, 0, 0, 0xff));
    surface.sync();

    SDL_Event event;
    while (1)
    while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    SDL_Quit();
                    break;

                default:
                    break;
            }
        }
    return EXIT_SUCCESS;
}
