#pragma once

#include <SDL2/SDL.h>
#include "cpu.h"
#include "display.h"
#include "rom.h"
#include <time.h>

#define CHEAP_8_SCREEN_WIDTH 64
#define CHEAP_8_SCREEN_HEIGHT 32

#define SCHIP_SCREEN_WIDTH 128
#define SCHIP_SCREEN_HEIGHT 64


typedef enum {
    CHIP_8 = 0,
    SCHIP = 1
} ChipType;

int init_cheap_8(const int type, const char* file_path);
void loop(Cpu* cpu, Display* display);
void emulate_cycle(Cpu* cpu, Display* display);
void draw_screen(Cpu* cpu, Display* display);
int event_management(Cpu* cpu);
void get_keydown_events(Cpu* cpu, const SDL_Event event);
void get_keyup_events(Cpu* cpu, const SDL_Event event);
