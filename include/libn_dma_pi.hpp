#ifndef LIBN_DMA_PI_H
#define LIBN_DMA_PI_H

#include <any>

namespace LibN64 {
namespace DMA {
void Wait();

template <class T> void Read(T, int, int);
template <class T> void Write(T, T, int length);
} // namespace DMA
} // namespace LibN64
#endif
