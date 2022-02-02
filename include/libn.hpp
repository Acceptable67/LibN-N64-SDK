#ifndef LIBN_H
#define LIBN_H

#include <c++/11.2.0/cstdlib>
#include <c++/11.2.0/cstdio>
#include <c++/11.2.0/cstdint>
#include <c++/11.2.0/cstdbool>
#include <c++/11.2.0/cassert>
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

#define HALT() while(1)

#include <libn_types.hpp>
#include <libn_font.hpp>
#include <libn_regs.hpp>
#include <libn_controller.hpp>
#include <libn_mempak.hpp>
#include <libn_dma_pi.hpp>
#include <libn_frame.hpp>
#include <libn_display.hpp>
#include <libn_sprite.hpp>
#include <libn_stdlib.hpp>
#include <libn_timer.hpp>

EXTERN int begin();

#endif