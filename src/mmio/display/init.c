#include "scratchpost.h"

int initDisplay() {
	SDL_Init(SDL_INIT_EVERYTHING);
	window =  SDL_CreateWindow("Kibblebox display", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, 0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_RenderClear(renderer);
	loadFont("fonts/VGA8.F16");

	//add MMIO allocation here
	memorySpace[0x1100] = 1;
	return 0;
}

int runDisplay() {
	SDL_RenderClear(renderer);
	//test display mode first
	if (memorySpace[0x1100] == 0) {
		//draw characters into memory space
		//lets hope this works
		for (int i = 0; i < 37; i++) {
			for (int j = 0; j < 100; j++) {
				//get character from memoryspace
				int c = memorySpace[0x200 + i*100 + j];
				for (int h = 0; h < 16; h++) {
					for (int w = 0; w < 8; w++) {
						if ((memorySpace[0x1100 + c * 16 + h] & 1 << w) == 0) {
							SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
						} else {
							SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
						}
						SDL_RenderDrawPoint(renderer, 8 * j + 8-w, 16 * i + h);
					}
				}
			}
		}
	}
	if (memorySpace[0x1100] == 1) {
		for (int i = 0; i < 600; i++) {
			for (int j = 0; j < 800; j++) {
				SDL_SetRenderDrawColor(renderer, memorySpace[0x5100 + 800*i*3 + j*3], memorySpace[0x5100 + 800*i*3 + j*3 + 1], memorySpace[0x5100 + 800*i*3 + j*3 + 2], 255);
				SDL_RenderDrawPoint(renderer, j, i);
			}	
		}
	}

	SDL_RenderPresent(renderer);
	return 0;
}

/*display settings table
0x1100 - display mode
0 for text, 1 for color


*/