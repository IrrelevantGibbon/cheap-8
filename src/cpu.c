#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "instructions.h"
#include <stdio.h>
#include <sys/file.h>


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
    cpu->shouldExit = 0;
    memset(cpu->keys, 0, sizeof(cpu->keys));
    memset(cpu->rpl_flag, 0, sizeof(cpu->rpl_flag));
    if (readRplFlags(cpu->rpl_flag) == 1)
    {
        writeRplFlags(cpu->rpl_flag);
    }
    return cpu;
}


u_int16_t fetch(Cpu* cpu)
{
    u_int16_t opcode = cpu->M[cpu->PC] << 8 | cpu->M[cpu->PC + 1];
    cpu->PC +=  2;
    return opcode;
}

u_int8_t readRplFlags(u_int8_t* rpl_flag)
{
    FILE* file = fopen("rpl.8", "rb");
    if (file == NULL)
    {
        perror("Error opening file for writing");
        return -1;
    }
    fread(rpl_flag, sizeof(u_int8_t), 8, file);
    fclose(file);
    return 0;
}

u_int8_t writeRplFlags(u_int8_t* rpl_flag)
{
    FILE* file = fopen("rpl.8", "wb");
    if (file == NULL)
    {
        perror("Error opening file for writing");
        return -1;
    }
    fwrite(rpl_flag, sizeof(u_int8_t), 8, file);
    fclose(file);
    return 0;
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
                    RET(cpu);
                    break;
                default:
                    break;
            }
            break;
        case 0X1000:
            JP(cpu, decodedOpcode->nnn);
            break;
        case 0X2000:
            CALL(cpu, decodedOpcode->nnn);
            break;
        case 0X3000:
            SE(cpu, decodedOpcode->x, decodedOpcode->nn);
            break;
        case 0X4000:
            SNE(cpu, decodedOpcode->x, decodedOpcode->nn);
            break;
        case 0X5000:
            SE_REG(cpu, decodedOpcode->x, decodedOpcode->y);
            break;
        case 0X6000:
            LD(cpu, decodedOpcode->x, decodedOpcode->nn);
            break;
        case 0x7000:
            ADD(cpu, decodedOpcode->x, decodedOpcode->nn);
            break;
        case 0x8000:
            switch (opcode & 0x000F)
            {
                case 0x0000:
                    LD_REG(cpu, decodedOpcode->x, decodedOpcode->y);
                    break;
                case 0x0001:
                    OR(cpu, decodedOpcode->x, decodedOpcode->y);
                    break;
                case 0x0002:
                    AND(cpu, decodedOpcode->x, decodedOpcode->y);
                    break;
                case 0x0003:
                    XOR(cpu, decodedOpcode->x, decodedOpcode->y);
                    break;
                case 0x0004:
                    ADD_REG(cpu, decodedOpcode->x, decodedOpcode->y);
                    break;
                case 0x0005:
                    SUB(cpu, decodedOpcode->x, decodedOpcode->y);
                    break;
                case 0x0006:
                    SHR(cpu, decodedOpcode->x);
                    break;
                case 0x0007:
                    SUBN(cpu, decodedOpcode->x, decodedOpcode->y);
                    break;
                case 0x000E:
                    SHL(cpu, decodedOpcode->x);
                    break;
                default:
                    break;
            }
            break;
        case 0x9000:
            SNE_REG(cpu, decodedOpcode->x, decodedOpcode->y);
            break;
        case 0xA000:
            LD_I(cpu, decodedOpcode->nnn);
            break;
        case 0xB000:
            JP_V0(cpu, decodedOpcode->nnn);
            break;
        case 0xC000:
            RND(cpu, decodedOpcode->x, decodedOpcode->nn);
            break;
        case 0xD000:
            DRW(cpu, decodedOpcode->x, decodedOpcode->y, decodedOpcode->n);
            break;
        case 0xE000:
            switch (opcode & 0x00FF)
            {
                case 0x009E:
                    SKP(cpu, decodedOpcode->x);
                    break;
                case 0x00A1:
                    SKNP(cpu, decodedOpcode->x);
                    break;
                default:
                    break;
            }
            break;
        case 0xF000:
            switch (opcode & 0x00FF)
            {
                case 0x0007:
                    LD_REG_DT(cpu, decodedOpcode->x);
                    break;
                case 0x000A:
                    LD_KEY(cpu, decodedOpcode->x);
                    break;
                case 0x0015:
                    LD_DT(cpu, decodedOpcode->x);
                    break;
                case 0x0018:
                    LD_ST(cpu, decodedOpcode->x);
                    break;
                case 0x001E:
                    ADD_I(cpu, decodedOpcode->x);
                    break;
                case 0x0029:
                    LD_F(cpu, decodedOpcode->x);
                    break;
                case 0x0033:
                    LD_B(cpu, decodedOpcode->x);
                    break;
                case 0x0055:
                    LD_MEM(cpu, decodedOpcode->x);
                    break;
                case 0x0065:
                    LD_REG_MEM(cpu, decodedOpcode->x);
                    break;
                default:
                    break;
            }
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
