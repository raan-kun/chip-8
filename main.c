#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_events.h>
#include "chip8.h"

// cpu clock speed
#define CLOCK_HZ 60.0
#define CLOCK_RATE_MS ((int) ((1000.0 / CLOCK_HZ) + 0.5)) // rounded up

// display resolution
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCALE_FACTOR 10.0

int display_width = SCREEN_WIDTH * SCALE_FACTOR;
int display_height = SCREEN_HEIGHT * SCALE_FACTOR;

int keymap(SDL_Scancode scancode) {
	switch(scancode) {
		case SDL_SCANCODE_1: return 0x1;
		case SDL_SCANCODE_2: return 0x2;
		case SDL_SCANCODE_3: return 0x3;
		case SDL_SCANCODE_4: return 0xC;
		case SDL_SCANCODE_Q: return 0x4;
		case SDL_SCANCODE_W: return 0x5;
		case SDL_SCANCODE_E: return 0x6;
		case SDL_SCANCODE_R: return 0xD;
		case SDL_SCANCODE_A: return 0x7;
		case SDL_SCANCODE_S: return 0x8;
		case SDL_SCANCODE_D: return 0x9;
		case SDL_SCANCODE_F: return 0xE;
		case SDL_SCANCODE_Z: return 0xA;
		case SDL_SCANCODE_X: return 0x0;
		case SDL_SCANCODE_C: return 0xB;
		case SDL_SCANCODE_V: return 0xF;
		default: return -1;
	};
}

void handle_keydown(chip8* chip, SDL_KeyboardEvent event) {
	chip->key[keymap(event.keysym.scancode)] = 1;
}

void handle_keyup(chip8* chip, SDL_KeyboardEvent event) {
	chip->key[keymap(event.keysym.scancode)] = 0;
}

void output_chip_state(chip8* chip) {
	system("clear");
	printf("opcode: %04X\n", chip->opcode);
	for(int i = 0; i < 8; i++) {
		printf("v%X=%02X  ", i, chip->V[i]);
	}
	printf("\n");
	for(int i = 8; i < 16; i++) {
		printf("v%X=%02X  ", i, chip->V[i]);
	}
	printf("I=%d\n", chip->I);
	printf("Key: [ ");
	for(int i = 0; i < 16; i++) {
		printf("%d ", chip->key[i]);
	}
	printf("]\n");
	printf("pc: %d\n", chip->pc);
	printf("Stack: [ ");
	for(int i = 0; i < 16; i++) {
		printf("%d ", chip->stack[i]);
	}
	printf("]\n");
	printf("sp: %d\n", chip->sp);
	printf("delay_timer: %d    sound_timer: %d\n", chip->delay_timer, chip->sound_timer);
}

int main(int argc, char** argv) {
	SDL_Window* window = NULL;
	SDL_Texture* texture = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event event;
	SDL_KeyboardEvent kb_event;

	Uint32 time_now;
	Uint32 time_prev;

	// initialise chip8
	chip8 chip;
	chip8_init(&chip);
	chip8_load_program(&chip, argv[1]);

	// initialise SDL window
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL failed initialising: %s\n", SDL_GetError());
		return 1;
	}
	window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, display_width, display_height, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE | SDL_RENDERER_TARGETTEXTURE);
	SDL_RenderSetScale(renderer, SCALE_FACTOR, SCALE_FACTOR);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
	Uint32* pixels = calloc(SCREEN_WIDTH*SCREEN_HEIGHT, sizeof(Uint32));

	// this ensures timers run from the first iteration if required
	time_prev = SDL_GetTicks();
	SDL_Delay(17);


	// main loop
	bool quit = false;
	while(!quit) {
		// event loop
		while(SDL_PollEvent(&event) != 0) {
			switch(event.type) {
				case SDL_QUIT:
					quit = true;
					break;
				case SDL_KEYDOWN:
					kb_event = event.key;
					handle_keydown(&chip, kb_event);
					break;
				case SDL_KEYUP:
					kb_event = event.key;
					handle_keyup(&chip, kb_event);
					break;
				default:
					// unhandled event
					break;
			};
		}

		// emulate 1 cpu cycle
		chip8_emulate_cycle(&chip);

		// draw screen if we need to
		if(chip.draw_flag) {
			// copy pixels from gfx[] into pixels array
			for(int y = 0; y < SCREEN_HEIGHT; y++) {
				for(int x = 0; x < SCREEN_WIDTH; x++) {
					pixels[SCREEN_WIDTH*y + x] = 0xffffffff * chip.gfx[SCREEN_WIDTH*y + x];
				}
			}
			SDL_RenderClear(renderer);
			SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
			chip.draw_flag = false;
		}

		// limit timer updates to 60Hz
		time_now = SDL_GetTicks();
		if((int)(time_now - time_prev + 0.5) >= CLOCK_RATE_MS) {
			chip8_tick(&chip);
			time_prev = time_now;
		}

		// print state
		output_chip_state(&chip);
	}

	system("clear");
	free(pixels);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}