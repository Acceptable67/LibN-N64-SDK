#ifndef LIBN_CONTROLLER_H
#define LIBN_CONTROLLER_H

#include <string.h>
#include <array>
#include <libn/regs.hpp>
#include <libn/types.hpp>

#define CreateControllerHandle(X)                                              \
    static const auto X = reinterpret_cast<Controller::Cpad *>(PIF_RAM)

namespace LibN64 {
namespace Controller {
void Write();
void Read();
void WriteStatus();

enum JoyStick {
    JoyUp    = 0x00000072,
    JoyDown  = 0x0000008E,
    JoyRight = 0x00007200,
    JoyLeft  = 0x00008E00
};

struct Cpad {
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
};
} // namespace Controller
} // namespace LibN64

#endif