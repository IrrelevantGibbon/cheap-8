#pragma once
#include <SDL2/SDL.h>
#include "cpu.h"
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

Display* initDisplay();
void draw(Display* display, Cpu* cpu);
void closeDisplay(Display* display);
void createOrSwapTexture(Display* display);
void setTexturesDimension(Display* display, u_int32_t width, u_int32_t height);
