#ifndef LIBN_TIMER_H
#define LIBN_TIMER_H

#include <libn/types.h>
#include <stdbool.h>

u64 MillisecondsSinceStartup();
u64 SecondsSinceStartup();

typedef enum { ONE_SHOT, CONTINUOUS_CALL, TIMER } TimerType;

/*Essentially a version of multithreading.
Specified code is called every X milliseconds the user requests it to.
Can stop, start, reset.*/
typedef struct {
    TimerType localType;
    bool bCalled;
    bool bStarted;
    f32 ticks;
    f32 execute_at;
} LibTimer;

void LibTimer_ResetTicks(LibTimer *timer);
void LibTimer_Start(LibTimer *timer);
void LibTimer_Stop(LibTimer* timer);
void LibTimer_Update(LibTimer *timer, void (*callback)());
void LibTimer_Setup(LibTimer *timer, TimerType type, float TickExecution);
float LibTimer_GetSecondsPassed(LibTimer *timer);
float LibTimer_GetMinutesPassed(LibTimer *timer);
float LibTimer_GetMsPassed(LibTimer *timer);

#endif