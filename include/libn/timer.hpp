#ifndef LIBN_TIMER_H
#define LIBN_TIMER_H

#include <functional>
#include <libn/types.hpp>

namespace LibN64 {
namespace Timer {
auto MillisecondsSinceStartup();
float SecondsSinceStartup();

enum TimerType { ONE_SHOT, CONTINUOUS_CALL, TIMER };

/*Essentially a version of multithreading.
Specified code is called every X milliseconds the user requests it to.
Can stop, start, reset.*/
class LibTimer {
  private:
    TimerType localType;
    bool bCalled  = false;
    bool bStarted = false;
    f32 time_started;
    f32 ticks;
    f32 execute_at;
    f32 lastFrameTime;

    std::function<void()> localfunc;

  public:
    LibTimer();
    LibTimer(TimerType type, float tickExecution = 0.0f);
    float GetSecondsPassed();
    float GetMinutesPassed();
    float GetMsPassed();

    /*Optional function that would be called every second
    We could just probe the COP0 for the time and subtract from
    that to get our value but this will do.*/
    void Update(std::function<void()> callback = [](void){});
    void Fetch();
    void ResetTicks();
    void Start();
    void Stop();
};
} // namespace Timer
} // namespace LibN64

#endif
