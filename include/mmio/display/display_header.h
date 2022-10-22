#pragma once
#define MMIO_DISPLAY_H

int initDisplay();
int runDisplay();
int loadFont(char* filename);


SDL_Window* window;
SDL_Renderer* renderer;