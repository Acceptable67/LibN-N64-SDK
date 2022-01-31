
#include <libn_types.hpp>
#include <libn_regs.hpp>

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
		template<class T>
		void Read(T RAM, int CART, int length) 
		{
			Wait();
			PI_REG->dram = reinterpret_cast<u32>(RAM);
			PI_REG->cart = CART & 0x1FFFFFFF;
			PI_REG->wlength = length - 1;
			Wait();
		}

		template<class T>
		void Write(T RAM, T CART, int length) 
		{
			Wait();
			PI_REG->dram = reinterpret_cast<u32>(RAM);
			PI_REG->cart = std::any_cast<u32>(CART); //& 0x1FFFFFFF;
			PI_REG->rlength = length - 1;
			Wait();
		} 
	}

}

template void LibN64::DMA::Read<u8*>(u8*, int, int);
template void LibN64::DMA::Read<u16*>(u16*, int, int);
template void LibN64::DMA::Read<u32*>(u32*, int, int);
template void LibN64::DMA::Read<u64*>(u64*, int, int);
