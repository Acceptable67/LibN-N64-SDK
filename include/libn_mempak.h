#ifndef LIBN_MEMPAK_H
#define LIBN_MEMPAK_H 

namespace LibN64
{
    namespace Controller 
    {
        namespace MemPak
        {
            bool isInserted();

            void WriteEntry();
            void ReadEntry();

            PakBuffer ReadAddress(u32 const);
            void      WriteAddress(u32 const, void* data);
        }
    }
}

#endif