#include "scratchpost.h"

int main(int argc, char** argv) {
	UNUSED(argc);
	UNUSED(argv);
	init_core();
	initDisplay();

	int refresh_cycle = 10000; //cycles to refresh the screen
	int count = 0;

	//load a file from argv
	if (argc > 1) {
		//assume argv[1] is the binary
		FILE* fp = fopen(argv[1], "r");
		if (fp == NULL) {
			printf("Failed to load raw binary \"%s\", exiting!\n", argv[1]);
			return -1;
		}

		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		rewind(fp);

		printf("Size of raw binary: %d\n", size);
		for (int i = 0; i < size; i++) {
			memorySpace[0x200000 + i] = fgetc(fp);
		}
		specialReg[0] = 0x200000;
	}

	halted = 0;
	while (1)	{
		SDL_Event event;
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				break;
			} else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE) {
				break;
			}
		} 

		if (halted == 0) {
			run_emu();
		}

		if (count > refresh_cycle) { 
			runDisplay();
			count = 0;
		}
		count++;
	}
	return 0;
}
