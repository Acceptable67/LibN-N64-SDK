#include <array>
#include <cstdint>
#include <libn/controller.hpp>
#include <libn/regs.hpp>
#include <libn/types.hpp>
/*controller + SI handling*/

std::array<u64, 8> SI_READ_MSG	   = {0xff010401ffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
				      0xfe00000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001};

std::array<u64, 8> SI_CTRLR_STATUS = {0xff010300ffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
				      0xfe00000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001};

PakBuffer si_data;
PakBuffer si_status_data;

CreateGlobalRegister(SI, SI_REG);

namespace LibN64 {
namespace Controller {
void _SI_Busy() {
	while (SI_REG->status & 3)
		;
}

void SI_Write(void *dram_address) {
	_SI_Busy();

	SI_REG->dram_addr    = reinterpret_cast<std::any *>(dram_address);
	SI_REG->pif_addr_w64 = reinterpret_cast<std::any *>(PIF_RAM - 0x4);

	_SI_Busy();
}

void SI_Read(void *dram_address) {
	_SI_Busy();

	SI_REG->dram_addr    = reinterpret_cast<std::any *>(dram_address);
	SI_REG->pif_addr_r64 = reinterpret_cast<std::any *>(PIF_RAM - 0x4);

	_SI_Busy();
}

void WriteStatus() {
	SI_Write(std::addressof(SI_CTRLR_STATUS));
}

PakBuffer SI_GetData() {
	Read();
	return si_data;
}

void Write() {
	SI_Write(std::addressof(SI_READ_MSG));
}

void Read() {
	SI_Read(std::addressof(si_data));
}
} // namespace Controller
} // namespace LibN64