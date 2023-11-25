#pragma once
#include <SDL2/SDL.h>
#include <sys/types.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 320

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *screen;
    u_int8_t width;
    u_int8_t height;
    u_int8_t shouldReRender;
} Display;

Display* initDisplay(u_int8_t width, u_int8_t height);
void draw(Display* display, u_int8_t* screen);
void closeDisplay(Display* display);
void createOrSwapTexture(Display* display);
void setTexturesDimension(Display* display, u_int32_t width, u_int32_t height);
