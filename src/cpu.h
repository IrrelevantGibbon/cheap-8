#pragma once
#include <stdint.h>
#include <sys/types.h>
#include <stdio.h>

#define dec_x(op) ((opcode & 0x0F00) >> 8)
#define dec_y(op) ((opcode & 0x00F0) >> 4)
#define dec_n(op) (opcode & 0xF)
#define dec_nn(op) (opcode & 0xFF)
#define dec_nnn(op) (opcode & 0xFFF)

typedef struct
{
    u_int8_t M[4096]; // Memory
    u_int8_t V[16]; // Registers
    u_int16_t I; // Index register
    u_int8_t DT; // Delay timer 
    u_int8_t ST; // Sound timer
    u_int16_t PC; // Program counter
    u_int8_t SP; // Stack pointer
    u_int16_t S[16]; // Stack
    uint8_t SCREEN[2048]; // Screen
    u_int8_t keys[16];
    u_int8_t shouldDraw;
    u_int8_t shouldExit;
    u_int8_t shouldExtend;
    u_int8_t rpl_flag[8];
} Cpu;

typedef struct
{
    u_int8_t x;
    u_int8_t y;
    u_int8_t n;
    u_int8_t nn;
    u_int16_t nnn;
} DecodedOpcode;

Cpu* initCpu();
u_int16_t fetch(Cpu* cpu);
DecodedOpcode* decode(Cpu* cpu, u_int16_t opcode);
void freeDecodedOpcode(DecodedOpcode* decodedOpcode);
void execute(Cpu* cpu, DecodedOpcode* decodedOpcode, u_int16_t opcode);
void decrementTimers(Cpu* cpu);
void closeCpu(Cpu* cpu);
u_int8_t readRplFlags(u_int8_t* rpl_flag);
u_int8_t writeRplFlags(u_int8_t* rpl_flag);

