#ifndef LIBN_H
#define LIBN_H

#include <c++/11.2.0/cstdio>
#include <c++/11.2.0/cstdint>
#include <c++/11.2.0/cstdbool>
#include <c++/11.2.0/cassert>
#include <stdlib.h>
#include <string>
#include <bitset>
#include <math.h>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>
#include <cstring>
#include <map>
#include <utility>
#include <source_location>
#include <stack>
#include <list>
#include <memory_resource>
#include <memory>
#include <any>

#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

#define EXTERN extern "C"
#define NUMBER_BUFFERS	2

#define assertf(__e, reason)  std::source_location loc = std::source_location::current(); \
    ((__e) ? (void)0 : __assert_func_cpp(loc.file_name(), loc.line(), loc.function_name(), #__e, reason)) \

#define CachedAddr(X) \
    ((void *)(((unsigned long)(X))&~0x20000000))
    
#define UncachedAddr(X) \
    ((void *)(((unsigned long)(X)) |0x20000000))

#define MEMORY_BARRIER()   __asm__("" : : : "memory")

#define LibPrintf(format, ...) printf(format, __VA_ARGS__)
#define LibPrint(text)	       printf(text, NULL)


#define HALT() while(1)
#define MainLoop while(true)

#include <libn/types.hpp>
#include <libn/font.hpp>
#include <libn/regs.hpp>
#include <libn/controller.hpp>
#include <libn/mempak.hpp>
#include <libn/interrupts.hpp>
#include <libn/pi.hpp>
#include <libn/frame.hpp>
#include <libn/vi.hpp>
#include <libn/sprite.hpp>
#include <libn/stdlib.hpp>
#include <libn/timer.hpp>
#include <libn/rdp.hpp>

EXTERN int begin();

#endif