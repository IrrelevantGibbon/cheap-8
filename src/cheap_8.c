#include "cheap_8.h"

int init_cheap_8(const char* filepath)
{
    Display *display = initDisplay();
    Cpu *cpu = initCpu();

    loadRom(filepath, cpu->M);

    loop(cpu, display);

    writeRplFlags(cpu->rpl_flag);
    closeDisplay(display);
    closeCpu(cpu);
}


void loop(Cpu* cpu, Display* display)
{
    while(1)
    {
        if (cpu->shouldExit == 1 || event_management(cpu) == 1) break;
        emulate_cycle(cpu);
        if (cpu->shouldDraw == 1) draw_screen(cpu, display);
        SDL_Delay(1000/500);
    }
}


void emulate_cycle(Cpu* cpu)
{
    u_int16_t opcode = fetch(cpu);
    DecodedOpcode* decodedOpcode = decode(cpu, opcode);
    execute(cpu, decodedOpcode, opcode);
    freeDecodedOpcode(decodedOpcode);
}

void draw_screen(Cpu* cpu, Display* display)
{
    cpu->shouldDraw = 0;
    SDL_SetRenderTarget(display->renderer, display->screen);
    draw(display, cpu);
    SDL_SetRenderTarget(display->renderer, NULL);
    SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255);
    SDL_RenderClear(display->renderer);
    SDL_RenderCopy(display->renderer, display->screen, NULL, NULL);
    SDL_RenderPresent(display->renderer);
}

int event_management(Cpu* cpu)
{
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return 1;
        }

        if (event.type == SDL_KEYDOWN)
        {
            get_keydown_events(cpu, event);
        }
        if (event.type == SDL_KEYUP)
        {
            get_keyup_events(cpu, event);
        }
    }

    return 0;
}

void get_keydown_events(Cpu* cpu, SDL_Event event)
{
    if (event.key.keysym.sym == SDLK_x) cpu->keys[0x0] = 1;
    if (event.key.keysym.sym == SDLK_a) cpu->keys[0x1] = 1;
    if (event.key.keysym.sym == SDLK_z) cpu->keys[0x2] = 1;
    if (event.key.keysym.sym == SDLK_e) cpu->keys[0x3] = 1;
    if (event.key.keysym.sym == SDLK_q) cpu->keys[0x4] = 1;
    if (event.key.keysym.sym == SDLK_w) cpu->keys[0x5] = 1;
    if (event.key.keysym.sym == SDLK_e) cpu->keys[0x6] = 1;
    if (event.key.keysym.sym == SDLK_u) cpu->keys[0x7] = 1;
    if (event.key.keysym.sym == SDLK_s) cpu->keys[0x8] = 1;
    if (event.key.keysym.sym == SDLK_d) cpu->keys[0x9] = 1;
    if (event.key.keysym.sym == SDLK_i) cpu->keys[0xA] = 1;
    if (event.key.keysym.sym == SDLK_c) cpu->keys[0xB] = 1;
    if (event.key.keysym.sym == SDLK_UP) cpu->keys[0xC] = 1;
    if (event.key.keysym.sym == SDLK_DOWN) cpu->keys[0xD] = 1;
    if (event.key.keysym.sym == SDLK_f) cpu->keys[0xE] = 1;
    if (event.key.keysym.sym == SDLK_v) cpu->keys[0xF] = 1;
}

void get_keyup_events(Cpu* cpu, SDL_Event event)
{
    if (event.key.keysym.sym == SDLK_x) cpu->keys[0x0] = 0;
    if (event.key.keysym.sym == SDLK_a) cpu->keys[0x1] = 0;
    if (event.key.keysym.sym == SDLK_z) cpu->keys[0x2] = 0;
    if (event.key.keysym.sym == SDLK_e) cpu->keys[0x3] = 0;
    if (event.key.keysym.sym == SDLK_q) cpu->keys[0x4] = 0;
    if (event.key.keysym.sym == SDLK_w) cpu->keys[0x5] = 0;
    if (event.key.keysym.sym == SDLK_e) cpu->keys[0x6] = 0;
    if (event.key.keysym.sym == SDLK_u) cpu->keys[0x7] = 0;
    if (event.key.keysym.sym == SDLK_s) cpu->keys[0x8] = 0;
    if (event.key.keysym.sym == SDLK_d) cpu->keys[0x9] = 0;
    if (event.key.keysym.sym == SDLK_i) cpu->keys[0xA] = 0;
    if (event.key.keysym.sym == SDLK_c) cpu->keys[0xB] = 0;
    if (event.key.keysym.sym == SDLK_UP) cpu->keys[0xC] = 0;
    if (event.key.keysym.sym == SDLK_DOWN) cpu->keys[0xD] = 0;
    if (event.key.keysym.sym == SDLK_f) cpu->keys[0xE] = 0;
    if (event.key.keysym.sym == SDLK_v) cpu->keys[0xF] = 0;
}
