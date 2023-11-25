#include <stdlib.h>
#include <string.h>
#include "cpu.h"
#include "instructions.h"
#include <stdio.h>
#include <sys/file.h>


Cpu* initCpu()
{
    u_int8_t FONT[240] = {
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
    0xF0, 0x80, 0xF0, 0x80, 0x80,  // F
    0xFF, 0xFF, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, // 0
    0x18, 0x78, 0x78, 0x18, 0x18, 0x18, 0x18, 0x18, 0xFF, 0xFF, // 1
    0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, // 2
    0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF, // 3
    0xC3, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0x03, 0x03, 0x03, 0x03, // 4
    0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF, // 5
    0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF, // 6
    0xFF, 0xFF, 0x03, 0x03, 0x06, 0x0C, 0x18, 0x18, 0x18, 0x18, // 7
    0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF, // 8
    0xFF, 0xFF, 0xC3, 0xC3, 0xFF, 0xFF, 0x03, 0x03, 0xFF, 0xFF, // 9
    0x7E, 0xFF, 0xC3, 0xC3, 0xC3, 0xFF, 0xFF, 0xC3, 0xC3, 0xC3, // A
    0xFC, 0xFC, 0xC3, 0xC3, 0xFC, 0xFC, 0xC3, 0xC3, 0xFC, 0xFC, // B
    0x3C, 0xFF, 0xC3, 0xC0, 0xC0, 0xC0, 0xC0, 0xC3, 0xFF, 0x3C, // C
    0xFC, 0xFE, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xC3, 0xFE, 0xFC, // D
    0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, // E
    0xFF, 0xFF, 0xC0, 0xC0, 0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xC0  // F
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
    memset(cpu->BUFFER, 0, sizeof(cpu->BUFFER));
    memccpy(cpu->M, FONT, 0, sizeof(FONT));
    cpu->shouldExit = 0;
    memset(cpu->keys, 0, sizeof(cpu->keys));
    memset(cpu->rpl_flag, 0, sizeof(cpu->rpl_flag));
    if (readRplFlags(cpu->rpl_flag) == 1)
    {
        writeRplFlags(cpu->rpl_flag);
    }
    cpu->extended = 1;
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

DecodedOpcode decode(u_int16_t opcode)
{
    return (DecodedOpcode){ opcode, dec_x(opcode), dec_y(opcode), dec_n(opcode), dec_nn(opcode), dec_nnn(opcode)};
}

void execute(Cpu* cpu, Display* display, DecodedOpcode decodedOpcode)
{
    printf("Executing opcode: 0x%X\n", decodedOpcode.opcode);

    u_int16_t op = decodedOpcode.opcode;

    switch (op & 0xF000)
    {
        case 0x0000:
            switch (op & 0x000F)
            {
                case 0x0000:
                    CLS(cpu);
                    printf("CLS() called\n");
                    break;
                case 0x000E:
                    RET(cpu);
                    printf("RET() called\n");
                    break;
                case 0x00FB:
                    SCR_DR(cpu);
                    printf("SCR_DR() called\n");
                    break;
                case 0x00FC:
                    SCR_DR(cpu);
                    printf("SCR_DR() called\n");
                    break;
                case 0x00FE:
                    DESM(cpu, display);
                    printf("DESM() called\n");
                    break;
                case 0x00FF:
                    EESM(cpu, display);
                    printf("EESM() called\n");
                    break;
                default:
                    SCR_D(cpu, decodedOpcode.n);
                    printf("Unknown opcode in 0x0000 switch: 0x%X\n", op);
                    break;
            }
            break;
        case 0X1000:
            JP(cpu, decodedOpcode.nnn);
            printf("JP(%u) called\n", decodedOpcode.nnn);
            break;
        case 0X2000:
            CALL(cpu, decodedOpcode.nnn);
            printf("CALL(%u) called\n", decodedOpcode.nnn);
            break;
        case 0X3000:
            SE(cpu, decodedOpcode.x, decodedOpcode.nn);
            printf("SE(%u, %u) called\n", decodedOpcode.x, decodedOpcode.nn);
            break;
        case 0X4000:
            SNE(cpu, decodedOpcode.x, decodedOpcode.nn);
            printf("SNE(%u, %u) called\n", decodedOpcode.x, decodedOpcode.nn);
            break;
        case 0X5000:
            SE_REG(cpu, decodedOpcode.x, decodedOpcode.y);
            printf("SE_REG(%u, %u) called\n", decodedOpcode.x, decodedOpcode.y);
            break;
        case 0X6000:
            LD(cpu, decodedOpcode.x, decodedOpcode.nn);
            printf("LD(%u, %u) called\n", decodedOpcode.x, decodedOpcode.nn);
            break;
        case 0x7000:
            ADD(cpu, decodedOpcode.x, decodedOpcode.nn);
            printf("ADD(%u, %u) called\n", decodedOpcode.x, decodedOpcode.nn);
            break;
        case 0x8000:
            switch (op & 0x000F)
            {
                case 0x0000:
                    LD_REG(cpu, decodedOpcode.x, decodedOpcode.y);
                    printf("LD_REG(%u, %u) called\n", decodedOpcode.x, decodedOpcode.y);
                    break;
                case 0x0001:
                    OR(cpu, decodedOpcode.x, decodedOpcode.y);
                    printf("OR(%u, %u) called\n", decodedOpcode.x, decodedOpcode.y);
                    break;
                case 0x0002:
                    AND(cpu, decodedOpcode.x, decodedOpcode.y);
                    printf("AND(%u, %u) called\n", decodedOpcode.x, decodedOpcode.y);
                    break;
                case 0x0003:
                    XOR(cpu, decodedOpcode.x, decodedOpcode.y);
                    printf("XOR(%u, %u) called\n", decodedOpcode.x, decodedOpcode.y);
                    break;
                case 0x0004:
                    ADD_REG(cpu, decodedOpcode.x, decodedOpcode.y);
                    printf("ADD_REG(%u, %u) called\n", decodedOpcode.x, decodedOpcode.y);
                    break;
                case 0x0005:
                    SUB(cpu, decodedOpcode.x, decodedOpcode.y);
                    printf("SUB(%u, %u) called\n", decodedOpcode.x, decodedOpcode.y);
                    break;
                case 0x0006:
                    SHR(cpu, decodedOpcode.x);
                    printf("SHR(%u) called\n", decodedOpcode.x);
                    break;
                case 0x0007:
                    SUBN(cpu, decodedOpcode.x, decodedOpcode.y);
                    printf("SUBN(%u, %u) called\n", decodedOpcode.x, decodedOpcode.y);
                    break;
                case 0x000E:
                    SHL(cpu, decodedOpcode.x);
                    printf("SHL(%u) called\n", decodedOpcode.x);
                    break;
                default:
                    printf("Unknown opcode in 0x8000 switch: 0x%X\n", op);
                    break;
            }
            break;
        case 0x9000:
            SNE_REG(cpu, decodedOpcode.x, decodedOpcode.y);
            printf("SNE_REG(%u, %u) called\n", decodedOpcode.x, decodedOpcode.y);
            break;
        case 0xA000:
            LD_I(cpu, decodedOpcode.nnn);
            printf("LD_I(%u) called\n", decodedOpcode.nnn);
            break;
        case 0xB000:
            JP_V0(cpu, decodedOpcode.nnn);
            printf("JP_V0(%u) called\n", decodedOpcode.nnn);
            break;
        case 0xC000:
            RND(cpu, decodedOpcode.x, decodedOpcode.nn);
            printf("RND(%u, %u) called\n", decodedOpcode.x, decodedOpcode.nn);
            break;
        case 0xD000:
            DRW(cpu, display->width, decodedOpcode.x, decodedOpcode.y, decodedOpcode.n);
            printf("DRW(%u, %u, %u) called\n", decodedOpcode.x, decodedOpcode.y, decodedOpcode.n);
            break;
        case 0xE000:
            switch (op & 0x00FF)
            {
                case 0x009E:
                    SKP(cpu, decodedOpcode.x);
                    printf("SKP(%u) called\n", decodedOpcode.x);
                    break;
                case 0x00A1:
                    SKNP(cpu, decodedOpcode.x);
                    printf("SKNP(%u) called\n", decodedOpcode.x);
                    break;
                default:
                    printf("Unknown opcode in 0xE000 switch: 0x%X\n", op);
                    break;
            }
            break;
        case 0xF000:
            switch (op & 0x00FF)
            {
                case 0x0007:
                    LD_REG_DT(cpu, decodedOpcode.x);
                    printf("LD_REG_DT(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                case 0x000A:
                    LD_KEY(cpu, decodedOpcode.x);
                    printf("LD_KEY(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                case 0x0015:
                    LD_DT(cpu, decodedOpcode.x);
                    printf("LD_DT(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                case 0x0018:
                    LD_ST(cpu, decodedOpcode.x);
                    printf("LD_ST(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                case 0x001E:
                    ADD_I(cpu, decodedOpcode.x);
                    printf("ADD_I(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                case 0x0029:
                    LD_F(cpu, decodedOpcode.x);
                    printf("LD_F(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                case 0x0030:
                    LD_FE(cpu, decodedOpcode.x);
                    printf("LD_FE(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                case 0x0033:
                    LD_B(cpu, decodedOpcode.x);
                    printf("LD_B(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                case 0x0055:
                    LD_MEM(cpu, decodedOpcode.x);
                    printf("LD_MEM(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                case 0x0065:
                    LD_REG_MEM(cpu, decodedOpcode.x);
                    printf("LD_REG_MEM(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                case 0x0075:
                    STR_RPL(cpu, decodedOpcode.x);
                    printf("STR_RPL(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                case 0x0085:
                    LD_RPL(cpu, decodedOpcode.x);
                    printf("LD_RPL(%u) called, Opcode: 0x%X\n", decodedOpcode.x, op);
                    break;
                default:
                    printf("Unknown opcode in 0xF000 switch: 0x%X\n", op);
                    break;
            }
            break;
        default:
            printf("Unknown main switch opcode: 0x%X\n", op);
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
