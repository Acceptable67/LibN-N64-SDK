#ifndef LIB_INTERRUPT_H
#define LIB_INTERRUPT_H

#include <libn.h>

void Interrupt_VI_SetCallback(void (*callbackFunction)());
void Interrupt_VI_Toggle(bool toggle, u32 vline);

#endif