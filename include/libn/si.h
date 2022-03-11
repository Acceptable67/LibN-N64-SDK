#ifndef LIBN_SI_H
#define LIBN_SI_H

#include <libn/types.h>

void SI_Write(AnyType dram_address);
void SI_Read(AnyType dram_address);
s8* _SI_Data();

#endif