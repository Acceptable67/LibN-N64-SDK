#include <array>
#include <cstdint>
#include <libn/controller.hpp>
#include <libn/regs.hpp>
#include <libn/types.hpp>
#include <libn/si.hpp>
/*controller + SI handling*/

std::array<u64, 8> SI_READ_MSG	   = {0xff010401ffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
				      0xfe00000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001};

std::array<u64, 8> SI_CTRLR_STATUS = {0xff010300ffffffff, 0xffffffffffffffff, 0xffffffffffffffff, 0xffffffffffffffff,
				      0xfe00000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000001};

CreateGlobalRegister(SI, SI_REG);

namespace LibN64 {
	namespace Controller {
	void WriteStatus() {
		SI::SI_Write(std::addressof(SI_CTRLR_STATUS));
	}

	void Write() {
		SI::SI_Write(std::addressof(SI_READ_MSG));
	}

	void Read() {
		SI::SI_Read(reinterpret_cast<void*>(SI::_SI_Data()));
	}
	} 
} 