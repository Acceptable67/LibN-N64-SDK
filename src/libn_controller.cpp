#include <libn_regs.h>
#include <libn_controller.h>
#include <array>
#include <cstdint>
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

std::array<uint64_t, 8> SI_CTRLR_STATUS =
{
	0xff010300ffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xfe00000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000001
};

std::array<u64, 8> SI_WRITE_MEMPK = 
{
	0xff230103ffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xffffffffffffffff,
	0xfe00000000000000,
	0x0000000000000000,
	0x0000000000000000,
	0x0000000000000001
};

std::array<char, sizeof(uint64_t)*sizeof(uint64_t)> si_data; 

CreateGlobalRegister(SI, SI_REG);

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

	void SI_WriteControllerStatus() 
	{
		SI_Write(std::addressof(SI_CTRLR_STATUS));
	}

	std::array<char, sizeof(u64)*sizeof(u64)>  SI_GetData() 
	{
		SI_ReadController();
		return si_data;
	}

	void SI_WriteController() 
	{
		SI_Write(std::addressof(SI_READ_MSG));
	}

	void SI_ReadController() 
	{
		SI_Read(std::addressof(si_data));
	}		
}