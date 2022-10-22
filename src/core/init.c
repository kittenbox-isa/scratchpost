#include "scratchpost.h"

uint8_t* memorySpace;

int debug;

char* mnemonicNames[16] = {
	"ldl"
	,"ldh"
	,"load"
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

uint8_t* memorySpace;

uint8_t testProgram[40] = {
	0x00, 0x10, 0x51, 0x00,
	0x00, 0x30, 0x00, 0x03,
	0x00, 0x40, 0x00, 0x18,
	0x00, 0x60, 0x00, 0x01,
	0x02, 0x20, 0xFF, 0xBC,
	0x00, 0x20, 0xF8, 0x00,
	0x08, 0x20, 0x80, 0x00,
	0x0A, 0x11, 0x84, 0x00,
	0x0C, 0x23, 0x08, 0x00,
	0x1A, 0x02, 0x00, 0x00
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
	memorySpace = gmalloc(sizeof(*memorySpace) * 16777216);
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
		printf("op: %s, reg1: %d, reg2: %d, reg3: %d\n", mnemonicNames[opcode], r1, r2, r3);
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
		switch (insr.condition) {
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
		}
	}


}

int run_emu() {
	instruction insr = decodeInstruction(specialReg[0]);
	if (debug) {
		printf("      r1: 0x%.8X, r2: 0x%.8X\n", registerBank[1], registerBank[2]);
		printf("      r3: 0x%.8X, r4: 0x%.8X\n", registerBank[3], registerBank[4]);
		printf("      IP: 0x%.8X\n", specialReg[0]);		
		for (int i = 0; i < 512/32; i++) {
			printf(YEL"0x%.4X"RESET" "BLU"|"RESET" ", i * 32);
			for (int j = 0; j < 32; j++) {
				if (j + i * 32 < (int)specialReg[0] + 4 && j + i * 32 >= (int)specialReg[0]) {
					printf(RED);
				}
				printf("%.2X "RESET, memorySpace[j + i * 32]);
			}
			printf("\n");
		}
		printf("\n");
	}

	registerBank[0] = 0;
	specialReg[0]+=4;
	//opcode decoding
	//determine LOAD offset
	uint32_t loadoffset = registerBank[insr.register2] + arithmeticShiftRight(insr.operand, 17);
	if (loadoffset > 512) 
		loadoffset = 0;

	switch (insr.opcode) {
		case 0x0: //LDL				
			registerBank[insr.register1] += insr.operand;
			break;

		case 0x1: //LDH
			registerBank[insr.register1] = insr.operand << 16;
			break;

		case 0x2: //LOAD
			registerBank[insr.register1] = constructWord(memorySpace[loadoffset], memorySpace[loadoffset + 1], memorySpace[loadoffset + 2], memorySpace[loadoffset + 3]);
			break;

		case 0x3: //STA
			memorySpace[insr.operand] = registerBank[insr.register1] >> 24;
			memorySpace[insr.operand + 1] = (registerBank[insr.register1] >> 16) & 0xFF;
			memorySpace[insr.operand + 2] = (registerBank[insr.register1] >> 8) & 0xFF;
			memorySpace[insr.operand + 3] = (registerBank[insr.register1]) & 0xFF;
			break;

		case 0x4: //STR
			memorySpace[registerBank[insr.register2]] = 	 registerBank[insr.register1] >> 24;
			memorySpace[registerBank[insr.register2] + 1] = (registerBank[insr.register1] >> 16) & 0xFF;
			memorySpace[registerBank[insr.register2] + 2] = (registerBank[insr.register1] >> 8) & 0xFF;
			memorySpace[registerBank[insr.register2] + 3] = (registerBank[insr.register1]) & 0xFF;
			break;

		case 0x5: //ADD
			registerBank[insr.register3] = registerBank[insr.register1] + registerBank[insr.register2];
			break;

		case 0x6: //SUB
			registerBank[insr.register3] = registerBank[insr.register1] - registerBank[insr.register2];
			break;

		case 0x7: //OR
			registerBank[insr.register3] = registerBank[insr.register1] | registerBank[insr.register2];
			break;

		case 0x8: //AND
			registerBank[insr.register3] = registerBank[insr.register1] & registerBank[insr.register2];
			break;

		case 0x9: //XOR
			registerBank[insr.register3] = registerBank[insr.register1] ^ registerBank[insr.register2];
			break;

		case 0xA: //NOT
			registerBank[insr.register2] = ~registerBank[insr.register1];
			break;

		case 0xB: //LSL
			registerBank[insr.register2] = registerBank[insr.register1] << insr.register3;
			break;

		case 0xC: //LSR
			registerBank[insr.register2] = registerBank[insr.register1] >> insr.register3;
			break;

		case 0xD: //JALCC
			handleJAL(insr);
			break;

		default:
			printf("UNIMPLEMENTED!: %d\n", insr.opcode);
			exit(0);
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
0001101 00000 00100 00000 00000 00000

00000000 00010000 01010001 00000000
00000000 00110000 00000000 00000011
00000000 01000000 00000000 00010100
00000010 00100000 11111111 10111100
00000000 00100000 11111000 00000000
00001000 00100000 10000000 00000000
00001010 00010001 10000100 00000000
00001100 00100011 00001000 00000000
00011010 00000010 00000000 00000000

00 10 51 00
00 30 00 03
00 40 00 14
02 20 FF BC
00 20 F8 00
08 20 80 00
0A 11 84 00
0C 21 88 00
1A 02 00 00
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
0001111 00000 00000 00100 0000000000


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