#include "chip8.h"

// initialise registers and memory
void chip8_initialise(chip8 chip) {
    for(int i = 0; i < 4096; i++) {
        chip.memory[i] = 0;
    }

    for(int i = 0; i < 16; i++) {
        chip.V[i] = 0;
    }
}

// emulates one CPU cycle
void chip8_tick(chip8 chip) {
    // fetch opcode
    // opcodes are 2 bytes but memory locations are 1 byte, 
    // so we must access two memory locations and merge the two parts of the instruction together
    unsigned short opcode = chip.memory[chip.pc] << 8 | chip.memory[chip.pc + 1];

    // decode opcode
    // execute opecode
    // update timers
}