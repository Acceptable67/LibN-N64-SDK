#ifndef LIBN_DMA_PI_H
#define LIBN_DMA_PI_H

#include <any>
#include <libn/types.hpp>

namespace LibN64 {
namespace DMA {
void Wait();
void Read(AnyType RAM, AnyType PI, u32 Length);
void Read(AnyType RAM, u32 PI, u32 Length); //overloaded for use of int
void Write(AnyType RAM, AnyType PI, u32 Length);
} // namespace DMA
} // namespace LibN64
#endif
