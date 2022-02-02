
#include <libn_types.hpp>
#include <libn_regs.hpp>

CreateGlobalRegister(PI, PI_REG);

namespace LibN64 {
namespace DMA {
void Wait() {
	while (PI_REG->status & 3)
		;
}

// we have to void* this, std::any or templates wont work
void Read(AnyType RAM, AnyType CART, u32 length) {
	Wait();
	PI_REG->dram	= reinterpret_cast<u32>(RAM);
	PI_REG->cart	= reinterpret_cast<u32>(CART) & 0x1FFFFFFF;
	PI_REG->wlength = length - 1;
	Wait();
}

void Read(AnyType RAM, u32 CART, u32 length) {
	Wait();
	PI_REG->dram	= reinterpret_cast<u32>(RAM);
	PI_REG->cart	= CART & 0x1FFFFFFF;
	PI_REG->wlength = length - 1;
	Wait();
}

void Write(AnyType RAM, AnyType CART, u32 length) {
	Wait();
	PI_REG->dram	= reinterpret_cast<u32>(RAM);
	PI_REG->cart	= reinterpret_cast<u32>(CART); //& 0x1FFFFFFF;
	PI_REG->rlength = length - 1;
	Wait();
}
} // namespace DMA

} // namespace LibN64
