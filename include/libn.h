#ifndef LIBN_H
#define LIBN_H

#include <stdlib.h>
#include <math.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

#define EXTERN extern "C"
#define NUMBER_BUFFERS	2

#define CachedAddr(X) \
    ((void *)(((unsigned long)(X))&~0x20000000))
    
#define UncachedAddr(X) \
    ((void *)(((unsigned long)(X)) |0x20000000))

#define HALT() while(1)
#define MainLoop while(true)

#include <libn/types.h>
#include <libn/font.h>
#include <libn/regs.h>
#include <libn/controller.h>
#include <libn/mempak.h>
#include <libn/pi_dma.h>
#include <libn/frame.h>
#include <libn/vi_display.h>
#include <libn/sprite.h>
#include <libn/stdlib.h>
#include <libn/timer.h>

#endif