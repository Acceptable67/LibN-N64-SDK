#ifndef LIBN_DMA_PI_H
#define LIBN_DMA_PI_H

#include <any>

namespace LibN64
{
	namespace DMA 
	{
		void Wait();
		void Read(std::any, int, int);
		void Write(std::any, std::any, int length);
	}
}
#endif

