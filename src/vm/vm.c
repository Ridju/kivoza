#include "vm.h" 
#include "../utils/utils.h"
#include <stdlib.h>

void op_add(uint16_t instr) {
   /* destination register (DR) */
  uint16_t r0 = (instr >> 9) & 0x7; // boolean mask of size 3 (0b0111)
  /* first operand (SR1) */
  uint16_t r1 = (instr >> 6) & 0x7;
  /* whether we are in immediate mode */
  uint16_t imm_flag = (instr >> 5) & 0x1; // boolean mask of size 1 (0b0001)
  if (imm_flag) {
    uint16_t imm5 =
        sign_extend(instr & 0x1F, 5); // boolean mask of size 5 (0b0001 1111)
    reg[r0] = reg[r1] + imm5;
  } else {
    uint16_t r2 = instr & 0x7;
    reg[r0] = reg[r1] + reg[r2];
  }
  update_flags(r0);
}

void op_and(uint16_t instr) {
  /*destination register (DR)*/
  uint16_t r0 = (instr >> 9) & 0x7;
  /*first operand (SR1)*/
  uint16_t r1 = (instr >> 6) & 0x7;
  /*wheter we are in immediate mode*/
  uint16_t imm_flag = (instr >> 5) & 0x1;

  if (imm_flag) {
    uint16_t imm5 = sign_extend(instr & 0x1F, 5);
    reg[r0] = reg[r1] & imm5;
  } else {
    uint16_t r2 = instr & 0x7;
    reg[r0] = reg[r1] & reg[r2];
  }
  update_flags(r0);
}

void op_br(uint16_t instr) {
  uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);
  uint16_t cond_flag = (instr >> 9) & 0x7;
  if (cond_flag & reg[R_COND]) {
    reg[R_PC] += pc_offset;
  }
}

void op_jmpret(uint16_t instr) {
  /*handles jump and return*/
  uint16_t r1 = (instr >> 6) & 0x7;
  reg[R_PC] = reg[r1];
}

void op_jsr(uint16_t instr) {
  reg[R_R7] = reg[R_PC];
  uint16_t cond = (instr >> 11) & 0x1;
  if (cond) {
    uint16_t offset = sign_extend(instr & 0x7FF, 11);
    reg[R_PC] += offset;
  } else {
    uint16_t baseR = (instr >> 6) & 0x7;
    reg[R_PC] = reg[baseR];
  }
}

void op_ld(uint16_t instr) {
  /*DR*/
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t offset = sign_extend(instr & 0x1FF, 9);
  reg[r0] = mem_read(reg[R_PC] + offset);
  update_flags(r0);
}

void op_ldi(uint16_t instr) {
  /*destination register (DR)*/
  uint16_t r0 = (instr >> 9) & 0x7;
  /*PCoffset 9*/
  uint16_t pc_offset = sign_extend(instr & 0x1FF, 9);

  reg[r0] = mem_read(mem_read(reg[R_PC] + pc_offset));
  update_flags(r0);
}

void op_ldr(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t baseR = (instr >> 6) & 0x7;
  uint16_t offset = sign_extend(instr & 0x3F, 6);

  reg[r0] = mem_read(reg[baseR] + offset);
  update_flags(r0);
}

void op_lea(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t offset = sign_extend(instr & 0x1FF, 9);
  reg[r0] = reg[R_PC] + offset;
  update_flags(r0);
}

void op_not(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  reg[r0] = ~reg[r1];
  update_flags(r0);
}

void op_ret(uint16_t instr) { reg[R_PC] = reg[R_R7]; }

void op_rti(uint16_t instr) { abort(); }

void op_res(uint16_t instr) { abort(); }

void op_st(uint16_t instr) {
  uint16_t offset = sign_extend(instr & 0x1FF, 9);
  uint16_t r0 = (instr >> 9) & 0x7;

  mem_write(reg[R_PC] + offset, reg[r0]);
}

void op_sti(uint16_t instr) {
  uint16_t offset = sign_extend(instr & 0x1FF, 9);
  uint16_t r0 = (instr >> 9) & 0x7;
  mem_write(mem_read(reg[R_PC] + offset), reg[r0]);
}

void op_str(uint16_t instr) {
  uint16_t r0 = (instr >> 9) & 0x7;
  uint16_t r1 = (instr >> 6) & 0x7;
  uint16_t offset = sign_extend(instr & 0x3F, 6);
  mem_write(reg[r1] + offset, reg[r0]);
}

void trap_c() { 
  reg[R_R0] = (uint16_t)getchar();
  update_flags(R_R0);      
}

void trap_out() { 
  putc((char)reg[R_R0], stdout);
  fflush(stdout);
}

void trap_puts() { 
  uint16_t *c = memory + reg[R_R0];
  while (*c) {
    putc((char)*c, stdout);
    ++c;
  }
  fflush(stdout);
}


void trap_in() { 
  printf("Enter a character: ");
  char c = getchar();
  putc(c, stdout);
  fflush(stdout);
  reg[R_R0] = (uint16_t)c;
  update_flags(R_R0);
}

void trapp_put_sp() { 
  uint16_t *c = memory + reg[R_R0];
  while (*c) {
    char char1 = (*c) & 0xFF;
    putc(char1, stdout);
    char char2 = (*c) >> 8;
    if (char2)
      putc(char2, stdout);
    ++c;
  }
  fflush(stdout);
}

void trap_halt(){ 
  puts("HALT");
  fflush(stdout);
  exit(0);
}

void switch_trap(uint16_t instr) {
  switch (instr & 0xFF) { 
    case TRAP_GETC: 
      trap_c();
      break;
    case TRAP_OUT: 
      trap_out();
      break;
    case TRAP_PUTS: 
      trap_puts();
      break;
    case TRAP_IN: 
      trap_in();
      break;
    case TRAP_PUTSP:
      break;
    case TRAP_HALT: 
      trap_halt();
      break;
  }
}