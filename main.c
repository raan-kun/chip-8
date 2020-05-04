#include <SDL2/SDL.h>
#include <stdio.h>
#include "chip8.h"

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

int main(int argc, char** argv) {
	chip8 chip;
	SDL_Event event;
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	// SDL_Surface* screen_surface = NULL;

	// initialise everything
	chip8_init(&chip);
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL failed initialising: %s\n", SDL_GetError());
		return 1;
	}
	SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN, &window, &renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);


	while(true) {
		// simulate 1 cycle for chip8
		chip8_tick(chip);

		// draw pixels from gfx[]
		SDL_RenderClear(renderer);
		for(int y = 0; y < SCREEN_HEIGHT; y++) {
			for(int x = 0; x < SCREEN_WIDTH; x++) {
				if(chip.gfx[SCREEN_HEIGHT*y + x] != 0) {
					SDL_RenderDrawPoint(renderer, x, y);
				}
			}
		}

		if(SDL_PollEvent(&event) && event.type == SDL_QUIT)
			break;
	}





	// SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
	// for(int i = 0; i < SCREEN_WIDTH; i++) {
	// 	SDL_RenderDrawPoint(renderer, i, 16);
	// 	SDL_RenderPresent(renderer);
	// }

	// SDL_Delay(2000);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}