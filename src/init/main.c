#include "scratchpost.h"

int main(int argc, char** argv) {
	UNUSED(argc);
	UNUSED(argv);
	init_core();
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window =  SDL_CreateWindow("Kibblebox display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(renderer);
	while (1)	{
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				break;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
				break;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_SPACE) {
				run_emu();
			}
		} 

		for (int i = 0; i < 600; i++) {
			for (int j = 0; j < 800; j++) {
				SDL_SetRenderDrawColor(renderer, memorySpace[0x1000 + 800*i*3 + j*3], memorySpace[0x1000 + 800*i*3 + j*3 + 1], memorySpace[0x1000 + 800*i*3 + j*3 + 2], 255);
				SDL_RenderDrawPoint(renderer, j, i);

			}

		}
		SDL_RenderPresent(renderer);
	}
	return 0;
}
