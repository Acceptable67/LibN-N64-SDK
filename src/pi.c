
#include <libn/types.h>
#include <libn/regs.h>

CreateGlobalRegister(PI, PI_REG);

void DMA_Wait() {
	while (PI_REG->status & 3)
		;
}

void DMA_Read(AnyType RAM, AnyType CART, u32 length) {
	DMA_Wait();
	PI_REG->dram	= (u32)(RAM);
	PI_REG->cart	= (u32)(CART) & 0x1FFFFFFF;
	PI_REG->wlength = length - 1;
	DMA_Wait();
}

//void Read(AnyType RAM, u32 CART, u32 length) {
//	Wait();
//	PI_REG->dram	= (u32)(RAM);
//	PI_REG->cart	= CART & 0x1FFFFFFF;
//	PI_REG->wlength = length - 1;
//	Wait();
//}

void DMA_Write(AnyType RAM, AnyType CART, u32 length) {
	DMA_Wait();
	PI_REG->dram	= (u32)(RAM);
	PI_REG->cart	= (u32)(CART); //& 0x1FFFFFFF;
	PI_REG->rlength = length - 1;
	DMA_Wait();
}
