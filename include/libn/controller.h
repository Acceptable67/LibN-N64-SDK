#ifndef LIBN_CONTROLLER_H
#define LIBN_CONTROLLER_H

#include <string.h>
#include <libn/regs.h>
#include <libn/types.h>

#define CreateControllerHandle(X)                                              \
    static const Cpad* X = (Cpad *)(PIF_RAM)

void _SI_Busy();

void SI_Write(AnyType dram_address);
void SI_Read(AnyType dram_address);

void Controller_Write();
void Controller_Read();
void Controller_WriteStatus();
s8* SI_GetData();

typedef enum {
    JoyUp    = 0x00000072,
    JoyDown  = 0x0000008E,
    JoyRight = 0x00007200,
    JoyLeft  = 0x00008E00
} JoyStick;

typedef struct {
    unsigned A : 1;
    unsigned B : 1;
    unsigned Z : 1;
    unsigned start : 1;
    unsigned up : 1;
    unsigned down : 1;
    unsigned left : 1;
    unsigned right : 1;
    unsigned : 2;
    unsigned L : 1;
    unsigned R : 1;
    unsigned C_up : 1;
    unsigned C_down : 1;
    unsigned C_left : 1;
    unsigned C_right : 1;
    signed x : 8;
    signed y : 8;
} Cpad;
#endif