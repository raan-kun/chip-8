#ifndef CHIP8_H
#define CHIP8_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

typedef struct chip8 {
	unsigned short  opcode;
	unsigned char   memory[4096];
	unsigned char   V[16]; // 8-bit registers
	unsigned short  I; // index register
	unsigned short  pc; // program counter
	unsigned char   gfx[64 * 32];
	unsigned char   delay_timer;
	unsigned char   sound_timer;

	// call stack
	unsigned short  stack[16];
	unsigned short  sp;

	// key state
	unsigned char   key[16];

	// determines if we should redraw screen
	bool            draw_flag;
} chip8;

void chip8_init(chip8* chip);
void chip8_load_program(chip8* chip, const char* filename);
void chip8_emulate_cycle(chip8* chip);
void chip8_tick(chip8* chip);

#endif