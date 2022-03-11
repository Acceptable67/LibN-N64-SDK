#ifndef LIB_INTERRUPT_H
#define LIB_INTERRUPT_H

#include <libn.h>

#define INT_COUNT 7

typedef enum {
    INT_VI,
    INT_PI,
    INT_DP,
    INT_SI, 
    INT_MI,
    INT_AI,
    INT_SP
} IntType;

void Interrupts_SetCallback(IntType type, void (*callbackFunction)());
void Interrupts_Handle();
void Interrupts_Clear();
void Interrupts_Enable();
void Interrupts_Disable();
void Interrupts_Toggle(IntType type, bool toggle);

#endif