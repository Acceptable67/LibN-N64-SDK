
#include <libn_types.h>
#include <libn_regs.h>

CreateGlobalRegister(PI, PI_REG);

namespace LibN64
{
	namespace DMA 
	{
		void Wait() 
		{ 
			while(PI_REG->status & 3); 
		} 

		//we have to void* this, std::any or templates wont work
		void Read(void* RAM, int CART, int length) 
		{
			Wait();
			PI_REG->dram = reinterpret_cast<u32>(RAM);
			PI_REG->cart = CART & 0x1FFFFFFF;
			PI_REG->wlength = length - 1;
			Wait();
		}

		void Write(void* RAM, std::any CART, int length) 
		{
			Wait();
			PI_REG->dram = reinterpret_cast<u32>(RAM);
			PI_REG->cart = std::any_cast<u32>(CART); //& 0x1FFFFFFF;
			PI_REG->rlength = length - 1;
			Wait();
		} 
	}

}


