#include "scratchpost.h"

int loadFont(char* filename) {
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("Could not load font: \"%s\"!\n", filename);
		printf("Continuing with no font. Be advised, text mode will appear blank!\n");
		return 1;
	}

	//test font file size
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	rewind(fp);
	if (size != 4096) {
		printf("Non standard font size detected! Size: %d\n", size);
	}

	for (int i = 0; i < (int)((size > 4096) ? 4096: size); i++) {
		memorySpace[0x1100 + i] = fgetc(fp); 
	}
	fclose(fp);
	return 0;
}