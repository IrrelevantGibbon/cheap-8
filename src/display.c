#include <stdlib.h>
#include "display.h"

Display* initDisplay(u_int8_t width, u_int8_t height)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    SDL_Window* win = SDL_CreateWindow("Cheap-8",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       SCREEN_WIDTH, SCREEN_HEIGHT, 0);

    Display *display = (Display *)malloc(sizeof(Display));
    display->window = win;
    display->renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    display->width = width;
    display->height = height;
    display->shouldReRender = 0;
    display->screen = NULL;
    createOrSwapTexture(display);

    return display;
}

void createOrSwapTexture(Display* display)
{
    SDL_Texture* texture = SDL_CreateTexture(display->renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_TARGET,
                                             display->width, display->height);
    if (display->screen != NULL) free(display->screen);
    display->screen = texture;
    SDL_SetRenderTarget(display->renderer, display->screen);
}

void setTexturesDimension(Display* display, u_int32_t width, u_int32_t height)
{
    display->width = width;
    display->height = height;
    display->shouldReRender = 1;
}


void draw(Display* display, u_int8_t* screen)
{
    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
    SDL_Rect rect = { 0, 0, display->width, display->height };
    SDL_RenderFillRect(display->renderer, &rect);

    for (int x = 0 ; x < display->width; x++)
    {
        for (int y = 0; y < display->height; y++)
        {
            int i = y * display->width + x;
            if (screen[i] == 1)
            {
                SDL_SetRenderDrawColor(display->renderer, 255, 255, 255, 255);
                SDL_Rect rect = { x, y, 1, 1 };
                SDL_RenderFillRect(display->renderer, &rect);
            }
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
