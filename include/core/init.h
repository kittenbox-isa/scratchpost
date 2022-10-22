#pragma once
#define CORE_INIT_H

int init_core();
int run_emu();

uint8_t* memorySpace;

int debug;

uint32_t registerBank[32];
uint32_t specialReg[1];

int halted;