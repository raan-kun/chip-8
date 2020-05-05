#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_events.h>
#include "chip8.h"

// display resolution
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCALE_FACTOR 2;

int display_width = SCREEN_WIDTH * SCALE_FACTOR
int display_height = SCREEN_HEIGHT * SCALE_FACTOR

void handle_keydown(chip8* chip, SDL_KeyboardEvent event) {
	switch(event.keysym.scancode) {
		case SDL_SCANCODE_1:
			chip->key[0x1] = 1;
			break;
		case SDL_SCANCODE_2:
			chip->key[0x2] = 1;
			break;
		case SDL_SCANCODE_3:
			chip->key[0x3] = 1;
			break;
		case SDL_SCANCODE_4:
			chip->key[0xC] = 1;
			break;
		case SDL_SCANCODE_Q:
			chip->key[0x4] = 1;
			break;
		case SDL_SCANCODE_W:
			chip->key[0x5] = 1;
			break;
		case SDL_SCANCODE_E:
			chip->key[0x6] = 1;
			break;
		case SDL_SCANCODE_R:
			chip->key[0xD] = 1;
			break;
		case SDL_SCANCODE_A:
			chip->key[0x7] = 1;
			break;
		case SDL_SCANCODE_S:
			chip->key[0x8] = 1;
			break;
		case SDL_SCANCODE_D:
			chip->key[0x9] = 1;
			break;
		case SDL_SCANCODE_F:
			chip->key[0xE] = 1;
			break;
		case SDL_SCANCODE_Z:
			chip->key[0xA] = 1;
			break;
		case SDL_SCANCODE_X:
			chip->key[0x0] = 1;
			break;
		case SDL_SCANCODE_C:
			chip->key[0xB] = 1;
			break;
		case SDL_SCANCODE_V:
			chip->key[0xF] = 1;
			break;
		default:
			break;
	};
}

void handle_keyup(chip8* chip, SDL_KeyboardEvent event) {
	switch(event.keysym.scancode) {
		case SDL_SCANCODE_1:
			chip->key[0x1] = 0;
			break;
		case SDL_SCANCODE_2:
			chip->key[0x2] = 0;
			break;
		case SDL_SCANCODE_3:
			chip->key[0x3] = 0;
			break;
		case SDL_SCANCODE_4:
			chip->key[0xC] = 0;
			break;
		case SDL_SCANCODE_Q:
			chip->key[0x4] = 0;
			break;
		case SDL_SCANCODE_W:
			chip->key[0x5] = 0;
			break;
		case SDL_SCANCODE_E:
			chip->key[0x6] = 0;
			break;
		case SDL_SCANCODE_R:
			chip->key[0xD] = 0;
			break;
		case SDL_SCANCODE_A:
			chip->key[0x7] = 0;
			break;
		case SDL_SCANCODE_S:
			chip->key[0x8] = 0;
			break;
		case SDL_SCANCODE_D:
			chip->key[0x9] = 0;
			break;
		case SDL_SCANCODE_F:
			chip->key[0xE] = 0;
			break;
		case SDL_SCANCODE_Z:
			chip->key[0xA] = 0;
			break;
		case SDL_SCANCODE_X:
			chip->key[0x0] = 0;
			break;
		case SDL_SCANCODE_C:
			chip->key[0xB] = 0;
			break;
		case SDL_SCANCODE_V:
			chip->key[0xF] = 0;
			break;
		default:
			break;
	};
}

int main(int argc, char** argv) {
	SDL_Window* window = NULL;
	SDL_Texture* texture = NULL;
	SDL_Renderer* renderer = NULL;

	SDL_Event event;
	SDL_KeyboardEvent kb_event;

	chip8 chip;

	// initialise chip8
	chip8_init(&chip);
	chip8_load_program(&chip, argv[1]);

	// initialise SDL window
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL failed initialising: %s\n", SDL_GetError());
		return 1;
	}
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
	Uint32* pixels = calloc(SCREEN_WIDTH*SCREEN_HEIGHT, sizeof(Uint32));

	// main loop
	bool quit = false;
	while(!quit) {
		SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));

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

		// simulate 1 cycle for chip8
		chip8_tick(&chip);

		if(chip.draw_flag) {
			// copy pixels from gfx[] into pixels array
			for(int y = 0; y < SCREEN_HEIGHT; y++) {
				for(int x = 0; x < SCREEN_WIDTH; x++) {
					pixels[SCREEN_WIDTH*y + x] = 0xffffffff * chip.gfx[SCREEN_WIDTH*y + x];
				}
			}
			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
			chip.draw_flag = false;
		}

		SDL_Delay(5);
	}

	free(pixels);
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}