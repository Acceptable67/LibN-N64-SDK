
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

		void Read(std::any RAM, int CART, int length) 
		{
			Wait();
			PI_REG->dram = any_cast<int>(RAM);
			PI_REG->cart = CART & 0x1FFFFFFF;
			PI_REG->wlength = length - 1;
			Wait();
		}

		void Write(std::any RAM, std::any CART, int length) 
		{
			Wait();
			PI_REG->dram = any_cast<int>(RAM);
			PI_REG->cart = any_cast<int>(CART); //& 0x1FFFFFFF;
			PI_REG->rlength = length - 1;
			Wait();
		} 
	}

}


