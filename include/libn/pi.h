#ifndef LIBN_DMA_PI_H
#define LIBN_DMA_PI_H

#include <libn/types.h>

void DMA_Wait();
void DMA_Read(AnyType RAM, AnyType PI, u32 Length);
//void DMA_Read_Int(AnyType RAM, u32 PI, u32 Length); //overloaded for use of int
void DMA_Write(AnyType RAM, AnyType PI, u32 Length);

#endif
