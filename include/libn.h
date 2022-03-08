#ifndef LIBN_H
#define LIBN_H

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"

#define NUMBER_BUFFERS	       2

#define CachedAddr(X)	       ((void *)(((unsigned long)(X)) & ~0x20000000))
#define UncachedAddr(X)	       ((void *)(((unsigned long)(X)) | 0x20000000))

#define HALT()		       while (1)
#define MEMORY_BARRIER()   __asm__("" : : : "memory")

#define LibPrintf(format, ...) printf(format, __VA_ARGS__)
#define LibPrint(text)	       printf(text)

#include <libn/controller.h>
#include <libn/font.h>
#include <libn/mempak.h>
#include <libn/pi.h>
#include <libn/regs.h>
#include <libn/sprite.h>
#include <libn/stdlib.h>
#include <libn/timer.h>
#include <libn/types.h>
#include <libn/vector.h>
#include <libn/vi.h>
#include <libn/rdp.h>
#include <libn/interrupt.h>
#include <libn/cop0.h>

#endif