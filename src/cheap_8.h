#pragma once

#include <SDL2/SDL.h>
#include "cpu.h"
#include "display.h"
#include "rom.h"
#include <time.h>

int init_cheap_8(const char* file_path);
void loop(Cpu* cpu, Display* display);
void emulate_cycle(Cpu* cpu);
void draw_screen(Cpu* cpu, Display* display);
int event_management(Cpu* cpu);
void get_keydown_events(Cpu* cpu, SDL_Event event);
void get_keyup_events(Cpu* cpu, SDL_Event event);
