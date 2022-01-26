#include "regs.S"

	.section .boot
	.global _start
_start:
	lw t0, 0x80000318			
	li t1, 0x7FFFFFF0
	addu sp,t0,t1				
	la gp, _gp					
	li v0, 8
	sw v0,(0xbfc007fc)			
	
	jal main
	

.section .text
	main:
	.global main
  	j begin
	  
.section .code

