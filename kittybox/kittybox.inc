r0  = 0
r1  = 1
r2  = 2
r3  = 3
r4  = 4
r5  = 5
r6  = 6
r7  = 7
r8  = 8
r9  = 9
r10 = 10
r11 = 11
r12 = 12
r13 = 13
r14 = 14
r15 = 15
r16 = 16
r17 = 17
r18 = 18
r19 = 19
r20 = 20
r21 = 21
r22 = 22
r23 = 23
r24 = 24
r25 = 25
r26 = 26
r27 = 27
r28 = 28
r29 = 29
r30 = 30
r31 = 31

;load imm family

macro LDLI? reg, imm
	if imm > 0xFFFF
		err 'imm in LDLI too big'
	end if

	db (0000000b shl 1) + (reg shr 4)
	db (reg and 1111b) shl 4 + 0000b
	db imm shr 8
	db imm and 0xFF
end macro 

macro LDUI? reg, imm
	if imm > 0xFFFF
		err 'imm in LDUI too big'
	end if

	db (0000000b shl 1) + (reg shr 4)
	db (reg and 1111b) shl 4 + 0001b
	db imm shr 8
	db imm and 0xFF
end macro

macro LDLZ? reg, imm
	if imm > 0xFFFF
		err 'imm in LDLZ too big'
	end if

	db (0000000b shl 1) + (reg shr 4)
	db (reg and 1111b) shl 4 + 0010b
	db imm shr 8
	db imm and 0xFF
end macro

macro LDUZ? reg, imm
	if imm > 0xFFFF
		err 'imm in LDZU too big'
	end if

	db (0000000b shl 1) + (reg shr 4)
	db (reg and 1111b) shl 4 + 0011b
	db imm shr 8
	db imm and 0xFF
end macro

macro LDLS? reg, imm
	if imm > 0xFFFF
		err 'imm in LDLI too big'
	end if

	db (0000000b shl 1) + (reg shr 4)
	db (reg and 1111b) shl 4 + 0100b
	db imm shr 8
	db imm and 0xFF
end macro

macro LDUS? reg, imm
	if imm > 0xFFFF
		err 'imm in LDLU too big'
	end if

	db (0000000b shl 1) + (reg shr 4)
	db (reg and 1111b) shl 4 + 0101b
	db imm shr 8
	db imm and 0xFF
end macro

macro LDR? reg1, reg2, imm
	if imm > 0x7FFF
		err 'imm in LDR too big'
	end if

	db (0000001b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (imm and 0x7F00) shr 8
	db imm and 0xFF
end macro

;store instructions

macro STA? reg, addr
	if addr > 0xFFFF
		err 'addr in STA too big'
	endif

	db (0000010b shl 1) + (reg shr 4)
	db (reg and 1111b) shl 4 
	db addr shr 8
	db addr and 0xFF
end macro

macro STR? reg1, reg2
	db (0000011b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7
	db 0
end macro

;arithmetic instructions

macro ADD? reg1, reg2, reg3
	db (0000100b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2)
	db 0
end macro

macro SUB? reg1, reg2, reg3
	db (0000101b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2)
	db 0
end macro

macro OR? reg1, reg2, reg3
	db (0000110b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2)
	db 0
end macro

macro AND? reg1, reg2, reg3
	db (0000111b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2)
	db 0
end macro

macro XOR? reg1, reg2, reg3
	db (0001000b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2)
	db 0
end macro

macro NOT? reg1, reg2
	db (0001001b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7
	db 0
end macro

macro LSL? reg1, reg2, n
	db (0001010b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (n shl 2)
	db 0
end macro

macro LSR? reg1, reg2, n
	db (0001011b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (n shl 2)
	db 0
end macro

;conditional instructions

macro JALEQ? reg1, reg2, reg3, reg4
	db (0001100b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2) + (reg4 and 11000b) shr 3
	db (reg4 and 111b) shl 5 + 00000b 
end macro

macro JALG? reg1, reg2, reg3, reg4
	db (0001100b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2) + (reg4 and 11000b) shr 3
	db (reg4 and 111b) shl 5 + 00001b 
end macro

macro JALL? reg1, reg2, reg3, reg4
	db (0001100b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2) + (reg4 and 11000b) shr 3
	db (reg4 and 111b) shl 5 + 00010b 
end macro

macro JALGS? reg1, reg2, reg3, reg4
	db (0001100b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2) + (reg4 and 11000b) shr 3
	db (reg4 and 111b) shl 5 + 00011b 
end macro

macro JALLS? reg1, reg2, reg3, reg4
	db (0001100b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2) + (reg4 and 11000b) shr 3
	db (reg4 and 111b) shl 5 + 00100b 
end macro

; conditional negation instructions

macro JALNEQ? reg1, reg2, reg3, reg4
	db (0001100b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2) + (reg4 and 11000b) shr 3
	db (reg4 and 111b) shl 5 + 10000b 
end macro

macro JALLEQ? reg1, reg2, reg3, reg4
	db (0001100b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2) + (reg4 and 11000b) shr 3
	db (reg4 and 111b) shl 5 + 10001b 
end macro

macro JALGEQ? reg1, reg2, reg3, reg4
	db (0001100b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2) + (reg4 and 11000b) shr 3
	db (reg4 and 111b) shl 5 + 10010b 
end macro

macro JALLEQS? reg1, reg2, reg3, reg4
	db (0001100b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2) + (reg4 and 11000b) shr 3
	db (reg4 and 111b) shl 5 + 10011b 
end macro

macro JALGEQS? reg1, reg2, reg3, reg4
	db (0001100b shl 1) + (reg1 shr 4)
	db (reg1 and 1111b) shl 4 + (reg2 shr 1)
	db (reg2 and 1b) shl 7 + (reg3 shl 2) + (reg4 and 11000b) shr 3
	db (reg4 and 111b) shl 5 + 10100b 
end macro

