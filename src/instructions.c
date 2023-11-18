#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
void CLS(Cpu* cpu)
{
    for (int i = 0; i < 64 * 32; i++)
    {
        cpu->SCREEN[i] = 0;
        cpu->shouldDraw = 1;
    }
}

void JP(Cpu* cpu, u_int16_t nnn)
{
    cpu->PC = nnn;
}

void RET(Cpu* cpu)
{
    cpu->SP--;
    cpu->PC = cpu->S[cpu->SP];
}

void CALL(Cpu* cpu, u_int16_t nnn)
{
    cpu->S[cpu->SP] = cpu->PC;
    cpu->SP++;
    cpu->PC = nnn;
}

void SE(Cpu* cpu, u_int8_t x, u_int8_t nn)
{
    if (cpu->V[x] == nn) 
    {
        cpu->PC += 2;
    }
}

void SNE(Cpu* cpu, u_int8_t x, u_int8_t nn)
{
    if (cpu->V[x] != nn) 
    {
        cpu->PC += 2;
    }
}

void SE_REG(Cpu* cpu, u_int8_t x, u_int8_t y)
{
    if (cpu->V[x] == cpu->V[y])
    {
        cpu->PC += 2;
    }
}

void LD(Cpu* cpu, u_int8_t x, u_int8_t nn)
{
    cpu->V[x] = nn;
}

void ADD(Cpu* cpu, u_int8_t x, u_int8_t nn)
{
    cpu->V[x] += nn;
}

void LD_REG(Cpu* cpu, u_int8_t x, u_int8_t y)
{
    cpu->V[x] = cpu->V[y];
}

void OR(Cpu* cpu, u_int8_t x, u_int8_t y)
{
    cpu->V[x] |= cpu->V[y]; 
}

void AND(Cpu* cpu, u_int8_t x, u_int8_t y)
{
    cpu->V[x] &= cpu->V[y];
}

void XOR(Cpu* cpu, u_int8_t x, u_int8_t y)
{
    cpu->V[x] ^= cpu->V[y];
}

void ADD_REG(Cpu* cpu, u_int8_t x, u_int8_t y)
{
    cpu->V[x] += cpu->V[y];
    cpu->V[0xF] = 0;
    if (cpu->V[y] > 0xFF - cpu->V[x])
    {
        cpu->V[0xF] = 1;
    }
}

void SUB(Cpu* cpu, u_int8_t x, u_int8_t y)
{
    cpu->V[0xF] = 0;
    if (cpu->V[x] > cpu->V[y])
    {
        cpu->V[0xF] = 1;
    }
    cpu->V[x] -= cpu->V[y];
}

void SHR(Cpu* cpu, u_int8_t x)
{
    cpu->V[0xF] = 0;
    if ((cpu->V[x] & 0x01) == 1) 
    {
        cpu->V[0xF] = 1;
    }
    cpu->V[x] /= 2;
}

void SUBN(Cpu* cpu, u_int8_t x, u_int8_t y)
{
    cpu->V[0xF] = 0;
    if (cpu->V[x] < cpu->V[y])
    {
        cpu->V[0xF] = 1;
    }
    cpu->V[x] =  cpu->V[y] - cpu->V[x];
}

void SHL(Cpu* cpu, u_int8_t x)
{
    cpu->V[0xF] = 0;
    if ( ((cpu->V[x] >> 7) & 0x01) == 1) 
    {
        cpu->V[0xF] = 1;
    }
    cpu->V[x] *= 2;
}

void SNE_REG(Cpu* cpu, u_int8_t x, u_int8_t y)
{
    if (cpu->V[x] != cpu->V[y])
    {
        cpu->PC += 2;
    }
}

void LD_I(Cpu* cpu, u_int16_t nnn)
{
    cpu->I = nnn;
}

void JP_V0(Cpu* cpu, u_int16_t nnn)
{
    cpu->PC = cpu->V[0] + nnn; 
}

void RND(Cpu* cpu, u_int8_t x, u_int8_t nn)
{
    cpu->V[x] = (u_int16_t)((rand() % 256) & nn);
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
        for (u_int8_t j = 0; j < (cpu->extended ? 16 : 8); j++)
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

void SKP(Cpu* cpu, u_int8_t x)
{
    if (cpu->keys[cpu->V[x]] == 1)
    {
        cpu->PC +=2;
    }
}

void SKNP(Cpu* cpu, u_int8_t x)
{
    if (cpu->keys[cpu->V[x]] == 0)
    {
        cpu->PC +=2;
    }
}

void LD_REG_DT(Cpu* cpu, u_int8_t x)
{
    cpu->V[x] = cpu->DT;
}

void LD_KEY(Cpu* cpu, u_int8_t x)
{
    for (u_int8_t i = 0; i <= 16; i++) 
    {
        if (cpu->keys[i] == 1)
        {
            cpu->V[x] = i; 
            return;
        }
    }
    cpu->PC -= 2;
}

void LD_DT(Cpu* cpu, u_int8_t x)
{
    cpu->DT = cpu->V[x];
}

void LD_ST(Cpu* cpu, u_int8_t x)
{
    cpu->ST = cpu->V[x];
}

void ADD_I(Cpu* cpu, u_int8_t x)
{
    cpu->V[0xF] = 0;
    if(cpu->I + cpu->V[x] > 0xFFF)
    {
        cpu->V[0xF] = 1;
    }
    cpu->I += cpu->V[x];
}

void LD_F(Cpu* cpu, u_int8_t x)
{
    cpu->I = cpu->V[x] * 0x5;
}

void LD_B(Cpu* cpu, u_int8_t x)
{
    u_int16_t vx = cpu->V[x];
    cpu->M[cpu->I] = vx / 100;
    cpu->M[cpu->I + 1] = (vx / 10) % 10;
    cpu->M[cpu->I + 2] = vx % 10;
}

void LD_MEM(Cpu* cpu, u_int8_t x)
{
    for (u_int8_t i = 0; i <= x; i++)
    {
        cpu->M[cpu->I + i] = cpu->V[i];
    }
}

void LD_REG_MEM(Cpu* cpu, u_int8_t x)
{
    for (u_int8_t i = 0; i <= x; i++)
    {
        cpu->V[i] = cpu->M[cpu->I + i];
    }
}

// SuperChip

void EXIT(Cpu* cpu)
{
    cpu->shouldExit = 1;
}

void LD_FE(Cpu* cpu, u_int8_t x)
{
    cpu->I = cpu->V[x] * 0xA;
}

void STR_RPL(Cpu* cpu, u_int8_t x)
{
    if (x < 8)
    {
        for (u_int8_t i = 0; i < x; x++)
        {
            cpu->rpl_flag[x] = cpu->V[x];
        }
    }
}

void LD_RPL(Cpu* cpu, u_int8_t x)
{
    if (x < 8)
    {
        for (u_int8_t i = 0; i < x; x++)
        {
            cpu->V[x] = cpu->rpl_flag[x];
        }
    }
}

void DESM(Cpu* cpu)
{
    cpu->shouldExtend = 1;
    cpu->extended = 0;
}

void EESM(Cpu* cpu)
{
    cpu->shouldExtend = 2;
    cpu->extended = 1;
}

void SCR_D(Cpu* cpu, u_int8_t n)
{
    cpu->shift_y += n;
}

void SCR_DR(Cpu* cpu)
{
    cpu->shift_x += 4;
}

void SCR_DL(Cpu* cpu)
{
    cpu->shift_x -= 4;
}
