#ifndef LIBN_SI_H
#define LIBN_SI_H

#include <libn/types.hpp>

namespace LibN64 {
    namespace SI {
        void SI_Write(void* dram_address);
        void SI_Read(void* dram_address);
        PakBuffer SI_GetData();
        s8* _SI_Data();
}
}


#endif