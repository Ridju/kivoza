#include "./vm/vm.h"
#include "./utils/utils.h"

#include <stdlib.h>
#include <signal.h>

int main(int argc, const char *argv[]) {
  if (argc < 2) {
    printf("lc3 [image-file1] ...\n");
    exit(2);
  }

  for (int i = 1; i < argc; ++i) {
    if (!read_image(argv[i])) {
      printf("failed to load the image: %s\n", argv[i]);
      exit(1);
    }
  }
  enum { PC_START = 0x3000 };

  signal(SIGINT, handle_interrupt);
  disable_input_buffering();

  reg[R_COND] = FL_ZRO;
  reg[R_PC] = PC_START;

  int running = 1;
  while (running) {
    uint16_t instr = mem_read(reg[R_PC]++);
    uint16_t op = instr >> 12;

    switch (op) {
    case OP_ADD:
      op_add(instr);
      break;
    case OP_AND:
      op_and(instr);
      break;
    case OP_NOT:
      op_not(instr);
      break;
    case OP_BR:
      op_br(instr);
      break;
    case OP_JMP:
      op_jmpret(instr);
      break;
    case OP_JSR:
      op_jsr(instr);
      break;
    case OP_LD:
      op_ld(instr);
      break;
    case OP_LDI:
      op_ldi(instr);
      break;
    case OP_LDR:
      op_ldr(instr);
      break;
    case OP_LEA:
      op_lea(instr);
      break;
    case OP_ST:
      op_st(instr);
      break;
    case OP_STI:
      op_sti(instr);
      break;
    case OP_STR:
      op_str(instr);
      break;
    case OP_TRAP:
      switch_trap(instr);
      break;
    case OP_RES:
      op_res(instr);
      break;
    case OP_RTI:
      op_rti(instr);
      break;
    default:
      puts("ABORTING...");
      abort();
      break;
    }
  }
  restore_input_buffering();
  return 0;
}
