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
        }

        u_int16_t opcode = fetch(cpu);
        DecodedOpcode* decodedOpcode = decode(cpu, opcode);
        // printf("--------------------------------------------------\n");
        // printf("OPCODE: %x\n", opcode);
        // printf("PC: %x\n", cpu->PC);
        // printf("I: %x\n", cpu->I);
        // printf("N: %x\n", decodedOpcode->n);
        // printf("NN: %x\n", decodedOpcode->nn);
        // printf("NNN: %x\n", decodedOpcode->nnn);
        // printf("X: %x\n", decodedOpcode->x);
        // printf("Y: %x\n", decodedOpcode->y);
        // printf("--------------------------------------------------\n");
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
        SDL_Delay(1000/60);
    }

    closeDisplay(display);
    closeCpu(cpu);
 
    return 0;
}