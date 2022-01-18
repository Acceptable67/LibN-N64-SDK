#include "regs.S"

	.section .boot
	.global _start
_start:
	lw t0, 0x80000318			/* memory size */
	li t1, 0x7FFFFFF0
	addu sp,t0,t1				/* init stack */
	la gp, _gp					/* init data pointer */
	li v0, 8
	sw v0,(0xbfc007fc)			/* magic N64 hardware init */
	
	jal main
	

.section .text
	main:
	.global main
  	j begin
	  
.section .code
