#include "cpu.h"
#include <stdio.h>

void CLS(Cpu* cpu)
{
    for (int i = 0; i < 64 * 32; i++)
    {
        cpu->SCREEN[i] = 0;
    }
}

void JP(Cpu* cpu, u_int16_t nnn)
{
    cpu->PC = nnn;
}

void LD(Cpu* cpu, u_int8_t x, u_int8_t nn)
{
    cpu->V[x] = nn;
}

void ADD(Cpu* cpu, u_int8_t x, u_int8_t nn)
{
    cpu->V[x] = cpu->V[x] + nn;
}

void LD_I(Cpu* cpu, u_int16_t nnn)
{
    cpu->I = nnn;
}

void DRW(Cpu* cpu, u_int8_t x, u_int8_t y, u_int8_t n)
{
    cpu->V[0xF] = 0;
    u_int8_t vx = cpu->V[x];
    u_int8_t vy = cpu->V[y];
    u_int8_t heigh = n;


    for (u_int8_t i = 0; i < heigh; i++)
    {
        u_int8_t pxl = cpu->M[cpu->I + i];
        for (u_int8_t j = 0; j < 8; j++)
        {
            if ((pxl & (0x80 >> j)) != 0)
            {
                u_int8_t* screenPxl = &cpu->SCREEN[(vy + i) * 64 + vx + j];
                if ((*screenPxl) == 1)
                {
                    cpu->V[0xF] = 1;
                }
                (*screenPxl) ^= 1;
            }
        }
    }
    cpu->shouldDraw = 1;
}