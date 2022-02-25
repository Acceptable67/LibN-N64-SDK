
#include <stdint.h>
#include <libn/controller.h>
#include <libn/regs.h>
#include <libn/types.h>
/*controller + SI handling*/

u64 SI_READ_MSG[8] = {0xff010401ffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
				      0xfe00000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001};

u64 SI_CTRLR_STATUS[8] ={0xff010300ffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
				      0xfe00000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001};

s8 si_data[64];
s8 si_status_data[64];

CreateGlobalRegister(SI, SI_REG);

void _SI_Busy() {
	while (SI_REG->status & 3)
		;
}

void SI_Write(void *dram_address) {
	_SI_Busy();

	SI_REG->dram_addr    = (void*)(dram_address);
	SI_REG->pif_addr_w64 = (void*)(PIF_RAM - 0x4);

	_SI_Busy();
}

void SI_Read(void *dram_address) {
	_SI_Busy();

	SI_REG->dram_addr    = (void*)(dram_address);
	SI_REG->pif_addr_r64 = (void*)(PIF_RAM - 0x4);

	_SI_Busy();
}

void Controller_WriteStatus() {
	SI_Write(&SI_CTRLR_STATUS);
}

s8* SI_GetData() {
	Controller_Read();
	return si_data;
}

void Controller_Write() {
	SI_Write(&SI_READ_MSG);
}

void Controller_Read() {
	SI_Read(&si_data);
}