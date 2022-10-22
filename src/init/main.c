#include "scratchpost.h"

int main(int argc, char** argv) {
	UNUSED(argc);
	UNUSED(argv);
	init_core();
	initDisplay();

	int refresh_cycle = 10000; //cycles to refresh the screen
	int count = 0;
	
	while (1)	{
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				break;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
				break;
			}
		} 
		run_emu();
		if (count > refresh_cycle) { 
			runDisplay();
			count = 0;
		}
		count++;
	}
	return 0;
}
