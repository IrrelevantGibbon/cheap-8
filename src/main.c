#include <SDL2/SDL.h>
#include "cpu.h"
#include "display.h"
#include "rom.h"

 int main(int argc, char *argv[])
{
 
    Display *display = initDisplay();
    Cpu *cpu = initCpu();

    if (argc < 2 || argc > 2) {
        printf("Usage: %s <rom>\n", argv[0]);
        return 1;
    }

    loadRom(argv[1], cpu->M);

    printf("FILE, LOADED IN RAM");

    while (1) {
        SDL_Event event;
        
        if (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                break;
            }

            if (event.type == SDL_KEYDOWN)
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

            if (event.type == SDL_KEYUP)
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
        }

        u_int16_t opcode = fetch(cpu);
        DecodedOpcode* decodedOpcode = decode(cpu, opcode);
        printf("--------------------------------------------------\n");
        printf("OPCODE: %x\n", opcode);
        printf("PC: %x\n", cpu->PC);
        printf("I: %x\n", cpu->I);
        printf("N: %x\n", decodedOpcode->n);
        printf("NN: %x\n", decodedOpcode->nn);
        printf("NNN: %x\n", decodedOpcode->nnn);
        printf("X: %x\n", decodedOpcode->x);
        printf("Y: %x\n", decodedOpcode->y);
        printf("--------------------------------------------------\n");
        execute(cpu, decodedOpcode, opcode);
        freeDecodedOpcode(decodedOpcode);
        if (cpu->shouldDraw == 1)
        {
            cpu->shouldDraw = 0;
            SDL_SetRenderTarget(display->renderer, display->screen);
            draw(display, cpu);
            SDL_SetRenderTarget(display->renderer, NULL);
            SDL_SetRenderDrawColor(display->renderer, 0, 0, 0, 255); // Clear the screen with black
            SDL_RenderClear(display->renderer);
            SDL_RenderCopy(display->renderer, display->screen, NULL, NULL);
            SDL_RenderPresent(display->renderer);
        }
        SDL_Delay(1000/300);
    }

    closeDisplay(display);
    closeCpu(cpu);
 
    return 0;
}