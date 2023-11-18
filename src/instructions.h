#pragma once
#include "cpu.h"
#include <sys/types.h>
void CLS(Cpu* cpu);
void RET(Cpu* cpu);
void JP(Cpu* cpu, u_int16_t nnn);
void CALL(Cpu* cpu, u_int16_t nnn);
void SE(Cpu* cpu, u_int8_t x, u_int8_t nn);
void SNE(Cpu* cpu, u_int8_t x, u_int8_t nn);
void SE_REG(Cpu* cpu, u_int8_t x, u_int8_t y);
void LD(Cpu* cpu, u_int8_t x, u_int8_t nn);
void ADD(Cpu* cpu, u_int8_t x, u_int8_t nn);
void LD_REG(Cpu* cpu, u_int8_t x, u_int8_t y);
void OR(Cpu* cpu, u_int8_t x, u_int8_t y);
void AND(Cpu* cpu, u_int8_t x, u_int8_t y);
void XOR(Cpu* cpu, u_int8_t x, u_int8_t y);
void ADD_REG(Cpu* cpu, u_int8_t x, u_int8_t y);
void SUB(Cpu* cpu, u_int8_t x, u_int8_t y);
void SHR(Cpu* cpu, u_int8_t x);
void SUBN(Cpu* cpu, u_int8_t x, u_int8_t y);
void SHL(Cpu* cpu, u_int8_t x);
void SNE_REG(Cpu* cpu, u_int8_t x, u_int8_t y);
void LD_I(Cpu* cpu, u_int16_t nnn);
void JP_V0(Cpu* cpu, u_int16_t nnn);
void RND(Cpu* cpu, u_int8_t x, u_int8_t nn);
void DRW(Cpu* cpu, u_int8_t x, u_int8_t y, u_int8_t n);
void SKP(Cpu* cpu, u_int8_t x);
void SKNP(Cpu* cpu, u_int8_t x);
void LD_REG_DT(Cpu* cpu, u_int8_t x);
void LD_KEY(Cpu* cpu, u_int8_t x);
void LD_DT(Cpu* cpu, u_int8_t x);
void LD_ST(Cpu* cpu, u_int8_t x);
void ADD_I(Cpu* cpu, u_int8_t x);
void LD_F(Cpu* cpu, u_int8_t x);
void LD_B(Cpu* cpu, u_int8_t x);
void LD_MEM(Cpu* cpu, u_int8_t x);
void LD_REG_MEM(Cpu* cpu, u_int8_t x);

// SuperChip

void SCR_D(Cpu* cpu, u_int8_t n);
void SCR_DR(Cpu* cpu);
void SCR_DL(Cpu* cpu);
void EXIT(Cpu* cpu);
void DESM(Cpu* cpu);
void EESM(Cpu* cpu);
void LD_FE(Cpu* cpu, u_int8_t x);
void STR_RPL(Cpu* cpu, u_int8_t x);
void LD_RPL(Cpu* cpu, u_int8_t x);
