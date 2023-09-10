#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "instructions.h"


Cpu* initCpu()
{
    u_int8_t FONT[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

    Cpu *cpu = (Cpu *)malloc(sizeof(Cpu));
    cpu->PC = 0x200;
    cpu->I = 0;
    cpu->SP = 0;
    cpu->DT = 0;
    cpu->ST = 0;
    memset(cpu->V, 0, sizeof(cpu->V));
    memset(cpu->S, 0, sizeof(cpu->S));
    memset(cpu->M, 0, sizeof(cpu->M));
    memset(cpu->SCREEN, 0, sizeof(cpu->SCREEN));
    memccpy(cpu->M, FONT, 0, sizeof(FONT));
    cpu->shouldDraw = 0;
    memset(cpu->keys, 0, sizeof(cpu->keys));
    return cpu;
}


u_int16_t fetch(Cpu* cpu)
{
    u_int16_t opcode = cpu->M[cpu->PC] << 8 | cpu->M[cpu->PC + 1];
    cpu->PC +=  2;
    return opcode;
}

DecodedOpcode* decode(Cpu* cpu, u_int16_t opcode)
{
    DecodedOpcode *decodedOpcode = (DecodedOpcode *)malloc(sizeof(DecodedOpcode));
    decodedOpcode->x = dec_x(opcode);
    decodedOpcode->y = dec_y(opcode);
    decodedOpcode->n = dec_n(opcode);
    decodedOpcode->nn = dec_nn(opcode);
    decodedOpcode->nnn = dec_nnn(opcode);
    return decodedOpcode;
}

void freeDecodedOpcode(DecodedOpcode* decodedOpcode)
{
    free(decodedOpcode);
}

void execute(Cpu* cpu, DecodedOpcode* decodedOpcode, u_int16_t opcode)
{
    switch (opcode & 0xF000)
    {
        case 0x0000:
            switch (opcode & 0x000F)
            {
                case 0x0000:
                    CLS(cpu);
                    break;
                case 0x000E:
                    break;
                default:
                    break;
            }
            break;
        case 0X1000:
            JP(cpu, decodedOpcode->nnn);
            break;
        case 0X2000:
            break;
        case 0x6000:
            LD(cpu, decodedOpcode->x, decodedOpcode->nn);
            break;
        case 0x7000:
            ADD(cpu, decodedOpcode->x, decodedOpcode->nn);
            break;
        case 0xA000:
            LD_I(cpu, decodedOpcode->nnn);
            break;
        case 0xD000:
            DRW(cpu, decodedOpcode->x, decodedOpcode->y, decodedOpcode->n);
            break;
        default:
            break;
    }
    decrementTimers(cpu);
}

void decrementTimers(Cpu* cpu)
{
    if (cpu->DT > 0)
    {
        cpu->DT -= 1;
    }

    if (cpu->ST > 0)
    {
        cpu->ST -=1;
    }
}

void closeCpu(Cpu* cpu)
{
    free(cpu);
}
