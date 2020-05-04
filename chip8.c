#include "chip8.h"

unsigned char chip8_fontset[80] = {
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
  0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

// initialise registers and memory
void chip8_init(chip8* chip) {
	chip->pc = 0x200; // pc always starts at 0x200
	chip->opcode = 0;
	chip->I = 0;
	chip->sp = 0;

	// clear display
	for(int i = 0; i < 64*32; i++) {
		chip->gfx[i] = 0;
	}

	// clear stack and registers
	for(int i = 0; i < 16; i++) {
		chip->stack[i] = 0;
		chip->V[i] = 0;
	}

	// clear memory
	for(int i = 0; i < 4096; i++) {
		chip->memory[i] = 0;
	}

	// load fontset
	for(int i = 0; i < 80; i++) {
		chip->memory[i] = chip8_fontset[i];
	}

	// reset timers
	chip->delay_timer = 0;
	chip->sound_timer = 0;

	// draw flag
	chip->draw_flag = false;

	// seed RNG
	srand(time(NULL));
}

void chip8_load_program(chip8* chip, const char* filename) {
	// TODO: find out what this 'buffersize' and 'buffer' stuff is about
	FILE* fp = fopen(filename, "rb");

	// for(int i = 0; i < bufferSize; i++) {
	// 	chip.memory[i + 512] = buffer[i];
	// }

	fclose(fp);
}

// emulates one CPU cycle
void chip8_tick(chip8 chip) {
	// fetch opcode
	// opcodes are 2 bytes but memory locations are 1 byte, 
	// so we must access two memory locations and merge the two parts of the instruction together
	chip.opcode = chip.memory[chip.pc] << 8 | chip.memory[chip.pc + 1];
	unsigned short opcode = chip.opcode;

	// decode & execute opcode
	switch(opcode & 0xF000) {
		// temp storage variables -- have to declare them here
		unsigned char reg;
		unsigned char reg_x;
		unsigned char reg_y;
		unsigned short x;
		unsigned short y;
		unsigned char opcode_X;
		unsigned short height;
		unsigned short sprite_line;
		unsigned char offset;
		bool key_press;

		// multiple opcodes start with 0
		case 0x0000:
			switch(opcode & 0x000f) {
				// 00E0 -- clears the screen
				case 0x0000:
					// stub
				break;
				// 00EE -- return from subroutine
				case 0x00EE:
					chip.pc = chip.stack[chip.sp];
					chip.sp -= 1;
					chip.pc += 2;
				break;
				// 0NNN
				default:
				break;
					// stub
			};
		break;
		// 1NNN -- jump to NNN
		case 0x1000:
			chip.pc = opcode & 0x0FFF;
		break;
		// 2NNN -- call subroutine at NNN
		case 0x2000:
			chip.stack[chip.sp] = chip.pc;
			chip.sp += 1;
			chip.pc = opcode & 0x0fff;
		break;
		// 3XNN -- skip next instruction if V[X] == NN
		case 0x3000:
			reg = (opcode & 0x0f00) >> 8;
			if((opcode & 0x00FF) == chip.V[reg]) {
				chip.pc += 4;
			}
			else {
				chip.pc += 2;
			}

		break;
		// 4XNN -- skip next instruction if V[X] != NN
		case 0x4000:
			reg = (opcode & 0x0f00) >> 8;
			if((opcode & 0x00ff) != chip.V[reg]) {
				chip.pc += 4;
			}
			else {
				chip.pc +=2;
			}	
		break;
		// 5XY0 -- skip next instruction if V[X] == V[Y]
		case 0x5000:
			reg_x = (opcode & 0x0f00) >> 8;
			reg_y = (opcode & 0x00f0) >> 4;
			if(chip.V[reg_x] == chip.V[reg_y]) {
				chip.pc += 4;
			}
			else {
				chip.pc +=2;
			}
		break;
		// 6XNN -- sets V[X] to NN
		case 0x6000:
			chip.V[(opcode & 0x0f00) >> 8] = opcode & 0x00ff;
			chip.pc += 2;
		break;
		// 7XNN -- adds NN to V[X]
		case 0x7000:
			chip.V[(opcode & 0x0f00) >> 8] += opcode & 0x00ff;
			chip.pc += 2;
		break;
		// multiple opcodes start with 8
		case 0x8000:
			reg_x = (opcode & 0x0f00) >> 8;
			reg_y = (opcode & 0x00f0) >> 4;
			switch(opcode & 0x000f) {
				// 8XY0 -- set V[X] to val in V[Y]
				case 0x0000:
					chip.V[reg_x] = chip.V[reg_y];
				break;
				// 8XY1 -- V[X] = V[X] | V[Y]
				case 0x0001:
					chip.V[reg_x] = chip.V[reg_x] | chip.V[reg_y];
				break;
				// 8XY2 -- V[X] = V[X] & V[Y]
				case 0x0002:
					chip.V[reg_x] = chip.V[reg_x] & chip.V[reg_y];
				break;
				// 8XY3 -- V[X] = V[X] ^ V[Y]
				case 0x0003:
					chip.V[reg_x] = chip.V[reg_x] ^ chip.V[reg_y];
				break;
				// 8XY4 -- V[X] += V[Y]. V[F] = 1 if there's a carry
				case 0x004:
					if(chip.V[reg_y] > 0xff - chip.V[reg_x]) {
						chip.V[0xf] = 1; // carry
					}
					else {
						chip.V[0xf] = 0;
					}
					chip.V[reg_x] += chip.V[reg_y];
				break;
				// 8XY5 -- V[X] -= V[Y]. V[F] = 0 if there's a borrow
				case 0x005:
					if(chip.V[reg_y] > chip.V[reg_x]) {
						chip.V[0xf] = 0; // borrow
					}
					else {
						chip.V[0xf] = 1;
					}
					chip.V[reg_x] -= chip.V[reg_y];
				break;
				// 8XY6 -- store LSB of V[X] in V[F], then V[X] >> 1
				case 0x006:
					chip.V[0xf] = chip.V[reg_x] & 0x0001;
					chip.V[reg_x] >>= 1;
				break;
				// 8XY7 -- V[X] = V[Y] - V[X]. V[F] = 0 if there's a borrow
				case 0x007:
					if(chip.V[reg_x] > chip.V[reg_y]) {
						chip.V[0xf] = 0; // borrow
					}
					else {
						chip.V[0xf] = 1;
					}
					chip.V[reg_x] = chip.V[reg_y] - chip.V[reg_x];
				break;
				// 8XYE -- store LSB of V[X] in V[F], then V[X] << 1
				case 0x00E:
					chip.V[0xf] = chip.V[reg_x] & 0x0001;
					chip.V[reg_x] <<= 1;
				break;
				default:
					printf("ERROR -- Unknown opcode: 0x%X\n", opcode);
			};

			chip.pc += 2;
		break;
		// 9XY0 -- skip next instruction if V[X] != V[Y]
		case 0x9000:
			reg_x = (opcode & 0x0f00) >> 8;
			reg_y = (opcode & 0x00f0) >> 4;
			if(chip.V[reg_x] != chip.V[reg_y]) {
				chip.pc += 4;
			}
			else {
				chip.pc += 2;
			}

		break;
		// ANNN -- sets I to address NNN
		case 0xA000:
			chip.I = opcode & 0x0fff;
			chip.pc += 2;
		break;
		// BNNN -- jump to address (NNN + V[0])
		case 0xB000:
			chip.pc = (opcode & 0x0fff) + chip.V[0];
		break;
		// CXNN -- set V[X] to NN & rand(255)
		case 0xC000:
			chip.V[(opcode & 0x0f00) >> 8] = (opcode & 0x00ff) & (rand() % 256);
			chip.pc += 2;
		break;
		// DXYN -- draw a 8xN sprite at coord (V[X], V[Y])
		case 0xD000:
			x = chip.V[(opcode & 0x0f00) >> 8];
			y = chip.V[(opcode & 0x00f0) >> 4];
			height = opcode & 0x000f;

			chip.V[0xf] = 0;
			// for each line in the sprite
			for(int y_line = 0; y_line < height; y_line++) {
				// grab 1 line from the sprite
				sprite_line = chip.memory[chip.I + y_line];

				// for each bit (pixel) in this line,
				for(int x_offset = 0; x_offset < 8; x_offset++) {
					//  check if this pixel is 1
					if((sprite_line & (0x80 >> x_offset)) != 0) {
						// index of current pixel in chip.gfx[]
						unsigned short curr_pix = x + x_offset + ((y+y_line) * 64);

						// V[f] = 1 if collision occured
						if(chip.gfx[curr_pix] == 1) {
							chip.V[0xf] = 1;
						}

						// update this pixel
						chip.gfx[curr_pix] ^= 1;
					}
				}

			}

			chip.draw_flag = true;
			chip.pc += 2;
		break;
		// multiple opcodes begin with E
		case 0xE000:
			switch(opcode & 0x00ff) {
				// EX9E -- skip next instruction if key stored in V[X] is pressed
				case 0x009e:
					if(chip.key[chip.V[opcode & 0x0f00 >> 8]] != 0) {
						chip.pc += 4;
					}
					else {
						chip.pc += 2;
					}
				break;
				// EXA1 -- skip next instruction if key stored in V[X] isn't pressed
				case 0x00a1:
					if(chip.key[chip.V[opcode & 0x0f00 >> 8]] != 0) {
						chip.pc += 2;
					}
					else {
						chip.pc += 4;
					}
				break;
				default:
					printf("ERROR -- Unknown opcode: 0x%X\n", opcode);
			};
		// multiple opcodes begin with F
		case 0xF000:
			opcode_X = (opcode & 0x0f00) >> 8;

			switch(opcode & 0x00ff) {
				// FX07 -- set V[X] to val of delay timer
				case 0x0007:
					chip.V[opcode_X] = chip.delay_timer;
					chip.pc += 2;
				break;
				// FX0A -- wait for key press and store in V[X]
				case 0x000a:
					key_press = false;
					for(int i = 0; i < 16; i++) {
						if(chip.key[i] != 0) {
							chip.V[opcode_X] = i;
							key_press = true;
						}
					}

					// if no key pressed, skip this cycle
					if(!key_press) {
						return;
					}

					chip.pc += 2;
				break;
				// FX15 -- set delay timer to V[X]
				case 0x0015:
					chip.delay_timer = chip.V[opcode_X];
					chip.pc += 2;
				break;
				// FX18 -- set sound timer to V[X]
				case 0x0018:
					chip.sound_timer = chip.V[opcode_X];
					chip.pc += 2;
				break;
				// FX1E -- I += V[X]. V[F] = 1 if overflows
				case 0x001e:
					if(chip.V[opcode_X] > 0xff - chip.I) {
						chip.V[0xf] = 1; // overflow occured
					}
					else {
						chip.V[0xf] = 0;
					}
					chip.I += chip.V[opcode_X];
					chip.pc += 2;
				break;
				// FX29 -- 
				case 0x0029:
					// stub
				break;
				// FX33 -- stores binary-coded decimal representation of V[X]
				//         in mem[I], mem[I+1], mem[I+2], mem[I+3]
				case 0x0033:
					reg = opcode_X;
					chip.memory[chip.I] = (chip.V[reg] / 100);
					chip.memory[chip.I+1] = (chip.V[reg] / 10) % 10;
					chip.memory[chip.I+2] = (chip.V[reg] % 100) % 10;
					chip.pc += 2;
				break;
				// FX55 -- store V[0] to V[X] in mem, starting from I
				case 0x0055:
					offset = opcode_X;
					for(int i = 0; i < offset; i++) {
						chip.memory[chip.I + offset] = chip.V[offset];
					}
					chip.pc += 2;
				break;
				// FX65 -- fill V[0] to V[X] with vals from mem, starting at I
				case 0x0065:
					offset = opcode_X;
					for(int i = 0; i < offset; i++) {
						chip.V[offset] = chip.memory[chip.I + offset];
					}
					chip.pc += 2;
				break;
				default:
					printf("ERROR -- Unknown opcode: 0x%X\n", opcode);
			};
		default:
			printf("ERROR -- Unknown opcode: 0x%X\n", opcode);

	};

	// update timers
	if(chip.delay_timer > 0) {
		chip.delay_timer -= 1;
	}

	if(chip.sound_timer > 0) {
		if(chip.sound_timer == 1) {
			printf("BEEP\n");
		}
		chip.sound_timer -= 1;
	}
}
