#ifndef LIBN_DMA_PI_H
#define LIBN_DMA_PI_H

#include <any>
#include <libn_types.hpp>

namespace LibN64 {
namespace DMA {
void Wait();
void Read(AnyType, AnyType, u32);
void Read(AnyType, u32, u32);
void Write(AnyType, AnyType, u32);
} // namespace DMA
} // namespace LibN64
#endif
