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

            //unfortunately templates or std::any do not work here so void it is
            bool ReadAddress(u16 const, void* output);
            void WriteAddress(u16 const, void* data);
        }
    }
}

#endif