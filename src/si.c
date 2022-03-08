
#include <libn/si.h>
#include <libn/regs.h>
#include <libn/types.h>

s8 si_data[64];
s8 si_status_data[64];

CreateGlobalRegister(SI, SI_REG);

s8* _SI_Data() {
	return &si_data;
}

void _SI_Busy() {
	while (SI_REG->status & 3);
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