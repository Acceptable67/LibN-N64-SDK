#include <string.h>
#include <array>

/*controller + SI handling*/
std::array<uint64_t, 8> SI_READ_MSG =
{
	0xff010401ffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xfe00000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000001
};

std::array<char, sizeof(uint64_t)*sizeof(uint64_t)> si_data; 

namespace LibN64::Controller 
{

	void _SI_Busy() { while(SI_REG->status & 3); }

	template<class VoidType>
	requires std::is_pointer<VoidType>::value
	void SI_Write(VoidType dram_address) 
	{
		_SI_Busy();

		SI_REG->dram_addr 	 = reinterpret_cast<std::any*>(dram_address);
		SI_REG->pif_addr_w64 = reinterpret_cast<std::any*>(PIF_RAM-0x4);
		
		_SI_Busy();
	}

	template<class VoidType>
	requires std::is_pointer<VoidType>::value
	void SI_Read(VoidType dram_address)
	{
		_SI_Busy();
		
		SI_REG->dram_addr 	 = reinterpret_cast<std::any*>(dram_address);
		SI_REG->pif_addr_r64 = reinterpret_cast<std::any*>(PIF_RAM-0x4);
		
		_SI_Busy();
	}

	void SI_WriteController() 
	{
		SI_Write(std::addressof(SI_READ_MSG));
	}

	void SI_ReadController() 
	{
		SI_Read(std::addressof(si_data));
	}		

	enum 
	{
		JOYUP 	= 0x00000072,
		JOYDOWN = 0x0000008E,
		JOYRIGHT= 0x00007200,
		JOYLEFT = 0x00008E00
	};

	struct Cpad
	{
		unsigned A : 1;
		unsigned B : 1;
		unsigned Z : 1;
		unsigned start : 1;
		unsigned up : 1;
		unsigned down : 1;
		unsigned left : 1;
		unsigned right : 1;
		unsigned : 2;
		unsigned L : 1;
		unsigned R : 1;
		unsigned C_up : 1;
		unsigned C_down : 1;
		unsigned C_left : 1;
		unsigned C_right : 1;
		signed x : 8;
		signed y : 8;
	};
}
auto *cpad_data = reinterpret_cast<LibN64::Controller::Cpad*>(PIF_RAM);
