include 'kittybox/kittybox.inc'
include 'kittybox/helper.inc'

org 0x200000

JMPA _start

mul: ; r9 = n, r10 = m, r11 = n * m, r12.. scratch
	ldi r12, 0x1
	ldi r13, 0x0
	ldi r11, 0x0
	ldi r15, mul_loop

	mul_loop:
		add r11, r10, r11
		add r12, r13, r13
		jall r0, r15, r13, r9

	jmpr r31


_start:
	ldi r9, 0xF
	ldi r10, 0xF
	ldi r30, mul
	jaleq r31, r30, r0, r0
	mov r11, r1
	db 0xFF