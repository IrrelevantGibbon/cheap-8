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
} Display;

Display* initDisplay();
void draw(Display* display, Cpu* cpu);
void closeDisplay(Display* display);
