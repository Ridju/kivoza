#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stdio.h>

uint16_t sign_extend(uint16_t x, int bit_count);
uint16_t swap16(uint16_t x);
void update_flags(uint16_t r);
void read_image_file(FILE *file);
int read_image(const char *image_path);
uint16_t check_key();
void mem_write(uint16_t address, uint16_t val);
uint16_t mem_read(uint16_t address);
void disable_input_buffering();
void restore_input_buffering();
void handle_interrupt(int signal);

enum { MR_KBSR = 0xFE00, MR_KBDR = 0xFE02 };

#endif