#ifndef VM_H
#define VM_H

#include <stdint.h>

uint16_t memory[UINT16_MAX];

enum {
  R_R0 = 0,
  R_R1,
  R_R2,
  R_R3,
  R_R4,
  R_R5,
  R_R6,
  R_R7,
  R_PC,
  R_COND,
  R_COUNT,
};

uint16_t reg[R_COUNT];

enum {
  OP_BR = 0,
  OP_ADD,
  OP_LD,
  OP_ST,
  OP_JSR,
  OP_AND,
  OP_LDR,
  OP_STR,
  OP_RTI,
  OP_NOT,
  OP_LDI,
  OP_STI,
  OP_JMP,
  OP_RES,
  OP_LEA,
  OP_TRAP
};

enum {
  FL_POS = 1 << 0,
  FL_ZRO = 1 << 1,
  FL_NEG = 1 << 2,
};

enum {
  TRAP_GETC = 0x20,
  TRAP_OUT = 0x21,
  TRAP_PUTS = 0x22,
  TRAP_IN = 0x23,
  TRAP_PUTSP = 0x24,
  TRAP_HALT = 0x25,
};

void op_add(uint16_t instr);
void op_and(uint16_t instr);
void op_br(uint16_t instr);
void op_jmpret(uint16_t instr);
void op_jsr(uint16_t instr);
void op_ld(uint16_t instr); 
void op_ldi(uint16_t instr);
void op_ldr(uint16_t instr);
void op_lea(uint16_t instr);
void op_not(uint16_t instr);
void op_ret(uint16_t instr);
void op_rti(uint16_t instr);
void op_res(uint16_t instr);
void op_st(uint16_t instr);
void op_sti(uint16_t instr); 
void op_str(uint16_t instr); 
void trap_c();
void trap_out();
void trap_puts(); 
void trap_in(); 
void trap_put_sp();
void trap_halt();
void switch_trap(uint16_t instr);

#endif