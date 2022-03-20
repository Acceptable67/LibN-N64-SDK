#include <libn.hpp>

CreateGlobalRegister(SI, SI_REG);

namespace LibN64 {
    namespace SI {
        PakBuffer si_data;
        PakBuffer si_status_data;

        s8* _SI_Data() {
            return std::to_address((s8*)si_data.begin());
        }

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

    PakBuffer SI_GetData() {
        Controller::Read();
        return si_data;
    }
  }
}