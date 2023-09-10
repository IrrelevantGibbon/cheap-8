#include <stdlib.h>
#include "display.h"
#include "cpu.h"

Display* initDisplay()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_Window* win = SDL_CreateWindow("Cheap-8",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture *screen = SDL_CreateTexture(renderer,
                                            SDL_PIXELFORMAT_ARGB8888,
                                            SDL_TEXTUREACCESS_TARGET,
                                            64, 32);

    SDL_SetRenderTarget(renderer, screen);

    Display *display = (Display *)malloc(sizeof(Display));
    display->window = win;
    display->renderer = renderer;
    display->screen = screen;
    return display;
}

void draw(Display* display, Cpu* cpu)
{
    for (int x = 0; x < 64; x++)
    {
        for (int y = 0; y < 32; y++)
        {
            int i = y * 64 + x;
            if (cpu->SCREEN[i] == 1)
            {
                SDL_SetRenderDrawColor(display->renderer, 255, 255, 255, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
            }
            SDL_Rect rect = { x, y, 1, 1 };
            SDL_RenderFillRect(display->renderer, &rect);
        }
    }
}

void closeDisplay(Display* display)
{
    SDL_DestroyTexture(display->screen);
    SDL_DestroyRenderer(display->renderer);
    SDL_DestroyWindow(display->window);
    free(display);
}