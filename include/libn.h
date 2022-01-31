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

#define NUMBER_BUFFERS	2

#define assertf(__e, reason)  std::source_location loc = std::source_location::current(); \
    ((__e) ? (void)0 : __assert_func_cpp(loc.file_name(), loc.line(), loc.function_name(), #__e, reason)) \

#define CachedAddr  (_addr) ((void *)(((unsigned long)(_addr))&~0x20000000))
#define UncachedAddr(_addr) ((void *)(((unsigned long)(_addr)) |0x20000000))

#define HALT() while(1)

#include <libn_types.h>
#include <libn_font.h>
#include <libn_regs.h>
#include <libn_controller.h>
#include <libn_mempak.h>
#include <libn_dma_pi.h>
#include <libn_frame.h>
#include <libn_display.h>
#include <libn_sprite.h>
#include <libn_stdlib.h>
#include <libn_timer.h>

#endif