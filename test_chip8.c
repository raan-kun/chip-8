#include "chip8.h"

void test_load_program() {
	chip8 chip;
	chip8_init(&chip);
	chip8_load_program(&chip, "pong2.c8");

	int i = 0;
	while(i < 1024) {
		for(int j = 0; j < 16; j++) {
			printf("%02X ", chip.memory[i+j]);
		}
		printf("\n");
		i+= 16;
	}
}

int main(int argc, char** argv) {
	test_load_program();
}