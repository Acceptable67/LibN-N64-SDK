#ifndef LIBN_DMA_PI_H
#define LIBN_DMA_PI_H

#include <any>

namespace LibN64 {
namespace DMA {
void Wait();
void Read(void *, int, int);
void Write(void *, void *, int length);
} // namespace DMA
} // namespace LibN64
#endif
