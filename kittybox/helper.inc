macro LDI? reg1, imm
	if imm > 0xFFFF
		LDLI reg1, (imm and 0xFFFF)
		LDUI reg1, (imm shr 16)
	else 
		LDLZ reg1, (imm and 0xFFFF)
	end if
end macro

macro MOV? reg1, reg2
	ADD reg1, r0, reg2
end macro

macro JMPR? reg1
	JALEQ r0, reg1, r0, r0
end macro

macro JMPA? addr
	LDI r31, addr
	JMPR r31
end macro
	