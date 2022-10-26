#include "scratchpost.h"

char* mnemonicNames[16] = {
	"load"
	,"ldr"
	,"sta"
	,"str"
	,"add"
	,"sub"
	,"or"
	,"and"
	,"xor"
	,"not"
	,"lsl"
	,"lsr"
	,"jalcc"
};

char* jalccNames[10] = {
	"jaleq",
	"jalneq",
	"jalg",
	"jalleq",
	"jall",
	"jalgeq",
	"jalgs",
	"jalleqs",
	"jalls",
	"jalgeqs"
};

char* loadNames[8] = {
	"ldli",
	"ldui",
	"ldlz",
	"lduz",
	"ldls",
	"ldus"
};


uint8_t* memorySpace;

/*uint8_t testProgram[40] = {
	0x00, 0x10, 0x51, 0x00,
	0x00, 0x30, 0x00, 0x03,
	0x00, 0x40, 0x00, 0x18,
	0x00, 0x60, 0x00, 0x01,
	0x00, 0x20, 0xFF, 0xBC,
	0x00, 0x20, 0xF8, 0x00,
	0x00, 0x20, 0x80, 0x00,
	0x00, 0x11, 0x84, 0x00,
	0x00, 0x23, 0x08, 0x00,
	0x16, 0x02, 0x00, 0x00
}; pink */

uint8_t testProgram[32] = {
	0x00, 0x10, 0x00, 0x01,
	0x00, 0x20, 0x00, 0x01,
	0x00, 0x30, 0x00, 0x00,
	0x00, 0x90, 0x00, 0x10,
	0x06, 0x11, 0x0C, 0x00,
	0x06, 0x20, 0x04, 0x00,
	0x06, 0x30, 0x08, 0x00,
	0x16, 0x04, 0x80, 0x00
};

uint32_t registerBank[32];
uint32_t specialReg[1];

typedef struct {
	uint8_t opcode;
	uint8_t register1;
	uint8_t register2;
	uint8_t register3;
	uint8_t register4;
	uint8_t condition;
	uint16_t operand;
} instruction;

int init_core() {
	printf("hello from core!\n");
	int memorySpaceSize = 16777216;
	memorySpace = gmalloc(sizeof(*memorySpace) * memorySpaceSize);
	memset(memorySpace, 0, memorySpaceSize);

	for (int i = 0; i < 40; i++) {
		memorySpace[i] = testProgram[i];
	}
	for (int i = 0; i < 32; i++) {
		registerBank[i] = 0; 
	}
	specialReg[0] = 0;
	debug = 0;
	return 0;
}

uint32_t constructWord(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
	uint32_t word = 0;
	word += a << 24;
	word += b << 16;
	word += c << 8;
	word += d;
	//printf("0x%.8X\n", word);
	return word;
}

instruction decodeInstruction(uint32_t addr) {
	//make instruction 32 bits wide
	uint32_t word = constructWord(memorySpace[addr], memorySpace[addr + 1], memorySpace[addr + 2], memorySpace[addr + 3]);
	uint8_t opcode = word >> (32 - 7);
	//determine extraneous data
	uint8_t r1 = (0b11111 << (32 - (7 + 5)) & word) >> (32 - (7 + 5));
	uint8_t r2 = (0b11111 << (32 - (7 + 5 + 5)) & word) >> (32 - (7 + 5 + 5));
	uint8_t r3 = (0b11111 << (32 - (7 + 5 + 5 + 5)) & word) >> (32 - (7 + 5 + 5 + 5));
	uint8_t r4 = (0b11111 << (32 - (7 + 5 + 5 + 5 + 5)) & word) >> (32 - (7 + 5 + 5 + 5 + 5));
	uint8_t cond = (0b11111 << (32 - (7 + 5 + 5 + 5 + 5 + 5)) & word) >> (32 - (7 + 5 + 5 + 5 + 5 + 5));
	uint16_t operand = (uint16_t)word;
	if (debug) {
		char* opcodeName = mnemonicNames[opcode];
		if (opcode == 0xC) {
			opcodeName = jalccNames[cond >> 4 + (cond & 0b1111) * 2];
		}

		if (opcode == 0x0) {
			opcodeName = loadNames[r2 >> 1];
		}
		printf("op: %s, reg1: %d, reg2: %d, reg3: %d reg4: %d cond: %d\n", opcodeName, r1, r2, r3, r4, cond);
		printf("operand: 0x%.4X\n", operand);
	}
	return (instruction){.opcode = opcode,
						 .register1 = r1,
						 .register2 = r2,
						 .register3 = r3,
						 .register4 = r4,
						 .condition = cond,
						 .operand = operand};
}

uint32_t arithmeticShiftRight(uint32_t input, int count) {
	int64_t shift = input;
	shift >>= count;
	return (uint32_t)shift;

}

void handleJAL(instruction insr) {
	if (insr.condition & 0b10000) {
		switch (insr.condition & 0b1111) {
			case 0: //JALNEQ
				if (registerBank[insr.register3] != registerBank[insr.register4]) {
					registerBank[insr.register1] = specialReg[0] + 4;
					specialReg[0] = registerBank[insr.register2];
				}
				break;

			case 1: //JALLEQ
				if (registerBank[insr.register3] <= registerBank[insr.register4]) {
					registerBank[insr.register1] = specialReg[0] + 4;
					specialReg[0] = registerBank[insr.register2];
				}
				break;

			case 2: //JALGEQ
				if (registerBank[insr.register3] >= registerBank[insr.register4]) {
					registerBank[insr.register1] = specialReg[0] + 4;
					specialReg[0] = registerBank[insr.register2];
				}
				break;

			case 3: //JALLEQS
				if ((int32_t)registerBank[insr.register3] <= (int32_t)registerBank[insr.register4]) {
					registerBank[insr.register1] = specialReg[0] + 4;
					specialReg[0] = registerBank[insr.register2];
				}
				break;
			
			case 4: //JALGEQS
				if ((int32_t)registerBank[insr.register3] >= (int32_t)registerBank[insr.register4]) {
					registerBank[insr.register1] = specialReg[0] + 4;
					specialReg[0] = registerBank[insr.register2];
				}
				break;

			default:
				printf("UNIMPLEMENTED JALCC: %d\n", insr.condition);
		}
	} else {
		switch (insr.condition) {
			case 0: //JALEQ
				if (registerBank[insr.register3] == registerBank[insr.register4]) {
					registerBank[insr.register1] = specialReg[0] + 4;
					specialReg[0] = registerBank[insr.register2];
				}
				break;

			case 1: //JALG
				if (registerBank[insr.register3] > registerBank[insr.register4]) {
					registerBank[insr.register1] = specialReg[0] + 4;
					specialReg[0] = registerBank[insr.register2];
				}
				break;

			case 2: //JALL
				if (registerBank[insr.register3] < registerBank[insr.register4]) {
					registerBank[insr.register1] = specialReg[0] + 4;
					specialReg[0] = registerBank[insr.register2];
				}
				break;

			case 3: //JALGS
				if ((int32_t)registerBank[insr.register3] > (int32_t)registerBank[insr.register4]) {
					registerBank[insr.register1] = specialReg[0] + 4;
					specialReg[0] = registerBank[insr.register2];
				}
				break;

			case 4: //JALLS
				if ((int32_t)registerBank[insr.register3] < (int32_t)registerBank[insr.register4]) {
					registerBank[insr.register1] = specialReg[0] + 4;
					specialReg[0] = registerBank[insr.register2];
				}
				break;


			default:
				printf("UNIMPLEMENTED JALCC: %d\n", insr.condition);
				halted = 1;
		}
	}


}

void handleLOAD(instruction insr) {
	int mcd = insr.register2 >> 1;
	if ((mcd & 0b1000) != 0) {
		int addr = (mcd & 0b111) << 16 + insr.operand;
		registerBank[insr.register1] = constructWord(memorySpace[addr], memorySpace[addr + 1], memorySpace[addr + 2], memorySpace[addr + 3]);
		return;
	}

	switch(mcd) {
		case 0: //LDLI
			registerBank[insr.register1] = (registerBank[insr.register1] & 0xFFFF0000) + insr.operand;
			break;

		case 1: //LDUI
			registerBank[insr.register1] = (registerBank[insr.register1] & 0x0000FFFF) + (insr.operand << 16);
			break;
			
		case 2: //LDLZ
			registerBank[insr.register1] = insr.operand;
			break;
			
		case 3: //LDUZ
			registerBank[insr.register1] = insr.operand << 16;
			break;
			
		case 4: //LDLS
			registerBank[insr.register1] = insr.operand | 0xFFFF0000;
			break;
			
		case 5: //LDUS
			registerBank[insr.register1] = insr.operand << 16 | 0x0000FFFF;
			break;
			
		default:
			printf("UNIMPLEMENTED LOAD: %d\n", mcd);
			halted = 1;
	}
}

int run_emu() {
	instruction insr = decodeInstruction(specialReg[0]);
	if (debug) {
		printf("      r1: 0x%.8X, r2: 0x%.8X\n", registerBank[1], registerBank[2]);
		printf("      r3: 0x%.8X, r4: 0x%.8X\n", registerBank[3], registerBank[4]);
		printf("      IP: 0x%.8X\n", specialReg[0]);
        int offset = 0x200000;		
		for (int i = 0; i < 512/32; i++) {
			printf(YEL"0x%.8X"RESET" "BLU"|"RESET" ", offset + i * 32);
			for (int j = 0; j < 32; j++) {
				if (offset + j + i * 32 < (int)specialReg[0] + 4 && j + i * 32 >= (int)specialReg[0]) {
					printf(RED);
				}
				printf("%.2X "RESET, memorySpace[offset + j + i * 32]);
			}
			printf("\n");
		}
		printf("\n");
	}

	registerBank[0] = 0;
	specialReg[0]+=4;
	//opcode decoding
	//calculate LDR offset
	uint32_t ldroffset = registerBank[insr.register2] + arithmeticShiftRight(insr.operand, 17);

	switch (insr.opcode) {
		case 0x0: //LDI
			handleLOAD(insr);
			break;

		case 0x1: //LDR
			registerBank[insr.register1] = constructWord(memorySpace[ldroffset], memorySpace[ldroffset + 1], memorySpace[ldroffset + 2], memorySpace[ldroffset + 3]);
			break;

		case 0x2: //STA
			memorySpace[insr.operand] = registerBank[insr.register1] >> 24;
			memorySpace[insr.operand + 1] = (registerBank[insr.register1] >> 16) & 0xFF;
			memorySpace[insr.operand + 2] = (registerBank[insr.register1] >> 8) & 0xFF;
			memorySpace[insr.operand + 3] = (registerBank[insr.register1]) & 0xFF;
			break;

		case 0x3: //STR
			memorySpace[registerBank[insr.register2]    ] =  registerBank[insr.register1] >> 24;
			memorySpace[registerBank[insr.register2] + 1] = (registerBank[insr.register1] >> 16) & 0xFF;
			memorySpace[registerBank[insr.register2] + 2] = (registerBank[insr.register1] >> 8 ) & 0xFF;
			memorySpace[registerBank[insr.register2] + 3] = (registerBank[insr.register1]      ) & 0xFF;
			break;

		case 0x4: //ADD
			registerBank[insr.register3] = registerBank[insr.register1] + registerBank[insr.register2];
			break;

		case 0x5: //SUB
			registerBank[insr.register3] = registerBank[insr.register1] - registerBank[insr.register2];
			break;

		case 0x6: //OR
			registerBank[insr.register3] = registerBank[insr.register1] | registerBank[insr.register2];
			break;

		case 0x7: //AND
			registerBank[insr.register3] = registerBank[insr.register1] & registerBank[insr.register2];
			break;

		case 0x8: //XOR
			registerBank[insr.register3] = registerBank[insr.register1] ^ registerBank[insr.register2];
			break;

		case 0x9: //NOT
			registerBank[insr.register2] = ~registerBank[insr.register1];
			break;

		case 0xA: //LSL
			registerBank[insr.register2] = registerBank[insr.register1] << insr.register3;
			break;

		case 0xB: //LSR
			registerBank[insr.register2] = registerBank[insr.register1] >> insr.register3;
			break;

		case 0xC: //JALCC
			handleJAL(insr);
			break;

		default:
			printf("UNIMPLEMENTED!: %d\n", insr.opcode);
			halted = 1;
			break;
	}
	return 0;
}

//test program below

/*
ldli $1, 0x5100
ldli $3, 0x3
ldli $4, 0x14 
ldhi $2, 0xffbc
ldli $2, 0xf800
str $2, $1
add $1, $3, $1
sub $2, $6, $2
jaleq $0, $4, $0, $0

0000000 00001 0000 01010001 00000000
0000000 00011 0000 00000000 00000011
0000000 00100 0000 00000000 00010100
0000001 00010 0000 11111111 10111100
0000000 00010 0000 11111000 00000000
0000100 00010 00001 0000000 00000000
0000101 00001 00011 00001 0000000000
0000110 00010 00110 00010 0000000000
0001011 00000 00100 00000 00000 00000

00000000 00010000 01010001 00000000
00000000 00110000 00000000 00000011
00000000 01000000 00000000 00010100
00000010 00100000 11111111 10111100
00000000 00100000 11111000 00000000
00001000 00100000 10000000 00000000
00001010 00010001 10000100 00000000
00001100 00100011 00001000 00000000
00010110 00000010 00000000 00000000

00 10 51 00
00 30 00 03
00 40 00 14
02 20 FF BC
00 20 F8 00
08 20 80 00
0A 11 84 00
0C 21 88 00
16 02 00 00
*/



/*
addr | instruction
-----+-------------
0x00 | ldli $1, 0x1
0x04 | ldli $2, 0x1
0x08 | ldli $3, 0x0
0x0C | add  $1, $2, $3
0x10 | sta $2, [0x0]
0x14 | sta $3, [0x4]
0x18 | load $1, $0, 0x0
0x1C | load $2, $0, 0x4
0x20 | ldli $4, 0x0C
0x24 | jal  0, $0, $4

0000000 00001 0000 00000000 00000001
0000000 00010 0000 00000000 00000001
0000000 00011 0000 00000000 00000000
0000111 00001 00010 00011 0000000000
0000100 00010 0000 00000000 00000000
0000100 00011 0000 00000000 00000100
0000010 00001 00000 0000000 00000000
0000010 00010 00000 0000000 00000100
0000000 00100 0000 00000000 00001100
0001011 00000 00000 00100 0000000000


00000000 00010000 00000000 00000001
00000000 00100000 00000000 00000001
00000000 00110000 00000000 00000000
00001110 00010001 00001100 00000000
00001000 00100000 00000000 00000000
00001000 00110000 00000000 00000100
00000100 00010000 00000000 00000000
00000100 00100000 00000000 00000100
00000000 01000000 00000000 00001100
00011110 00000000 00010000 00000000

00 10 00 01
00 20 00 01
00 30 00 00
0E 11 0C 00
08 20 00 00
08 30 00 04
04 10 00 00
04 20 00 04
00 40 00 0C
1E 00 10 00

0010000100200001003000000E110C00082000000830000404100000042000040040000C1E001000

*/
