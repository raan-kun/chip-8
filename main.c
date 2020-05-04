#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_keyboard.h>
#include "chip8.h"

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

int main(int argc, char** argv) {
	chip8 chip;
	SDL_Event event;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	// initialise chip8
	chip8_init(&chip);
	chip8_load_program(&chip, "pong2.c8");

	// initialise SDL window
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL failed initialising: %s\n", SDL_GetError());
		return 1;
	}
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);

	// main loop
	while(true) {
		// simulate 1 cycle for chip8
		chip8_tick(&chip);

		// draw pixels from gfx[]
		if(chip.draw_flag) {
			// clear screen before draw
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
			SDL_RenderClear(renderer);

			// scan gfx[] for pixels that are turned on
			for(int y = 0; y < SCREEN_HEIGHT; y++) {
				for(int x = 0; x < SCREEN_WIDTH; x++) {
					if(chip.gfx[SCREEN_WIDTH*y + x] == 1) {
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
						SDL_RenderDrawPoint(renderer, x, y);
					}
				}
			}
		}
		// commit to draw
		SDL_RenderPresent(renderer);

		if(SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;

		SDL_Delay(32);
	}

	// for(int i = 0; i < 32; i++) {
	// 	printf("[");
	// 	for(int j = 0; j < 64; j++) {
	// 		printf("%d ", chip.gfx[SCREEN_WIDTH*i + j]);
	// 	}
	// 	printf("]\n");
	// }



	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}