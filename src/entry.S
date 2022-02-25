#include "regs.S"

.section .boot
.global _start
_start:
	lui t3, 0x8000
	ori t3, t3, 0x0318
	lw t0, 0(t3)			
	li t1, 0x7FFFFFF0
	addu sp, t0, t1				
	la gp, _gp					
	li v0, 8
	lui t2, 0xBFC0
	sw v0, 0x7FC(t2)		
	
	la a0, __bss_start
	la a1, __bss_end

bss_clear:
	sd zero, (a0)
	addiu a0, 0x8
	bltu a0, a1, bss_clear
	nop

	mtc0 zero, v0
	mtc0 zero, v1
	mtc0 zero, a2 

	j main

loop:
	j loop
	nop
	  
.section .code

