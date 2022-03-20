#ifndef LIB_INTERRUPT_H
#define LIB_INTERRUPT_H

#include <libn.hpp>

#define INT_COUNT 7

namespace Interrupts 
{
    enum Type 
    {
        VI,
        PI,
        DP,
        SI, 
        MI,
        AI,
        SP
    };

    void SetCallback(Type type, std::function<void()>);
    void SetCallback(Type type, void(*func_ptr));
    void Handle();
    void Clear();
    void Enable();
    void Disable();
    void Toggle(Type type, bool toggle);
}; 

#endif