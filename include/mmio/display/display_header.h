#pragma once
#define MMIO_DISPLAY_H

int initDisplay();
int runDisplay();
int loadFont(char* filename);


extern SDL_Window* window;
extern SDL_Renderer* renderer;