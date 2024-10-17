#include <functional>
#include <string>
#include <libn/timer.hpp>
#include <libn/types.hpp>

namespace LibN64 {
namespace Timer {

extern auto MillisecondsSinceStartup() {
    u64 ticks = 0;
    asm("mfc0 %0, $9\n" : "=r"(ticks));
    return ticks / ((93750000 / 2) / 1000); // take vr4300 clock speed and divide by two
}

extern float SecondsSinceStartup() {
    return MillisecondsSinceStartup() * 0.001f;
}

LibTimer::LibTimer() {
    this->localType = TimerType::TIMER;
    this->execute_at = 0.0f;
    this->lastFrameTime = MillisecondsSinceStartup(); // Initialize lastFrameTime
}

LibTimer::LibTimer(TimerType type, float tickExecution)
    : localType(type), execute_at(tickExecution) {
    this->lastFrameTime = MillisecondsSinceStartup(); // Initialize lastFrameTime
}

float LibTimer::GetSecondsPassed() {
    return ticks;
}

float LibTimer::GetMinutesPassed() {
    return ticks / 60;
}

float LibTimer::GetMsPassed() {
    return (ticks * 1000);
}

float LibTimer::GetDeltaTime() {
    // Get the current time in milliseconds
    u64 currentTime = MillisecondsSinceStartup();
    
    // Calculate the time difference since the last frame
    float deltaTime = (currentTime - lastFrameTime) * 0.001f; // Convert to seconds

    // Update the last frame time for the next call
    lastFrameTime = currentTime;

    return deltaTime; // Return the delta time in seconds
}

void LibTimer::Update(std::function<void()> callback) {
    auto TimerCheckAndInc = [&](bool bSetCall) {
        this->ticks += 0.01f;

        bCalled = bSetCall;

        if (localType == TimerType::ONE_SHOT && ticks >= execute_at) {
            callback();
            bStarted = false;
            this->ResetTicks();
            return;
        } else if (localType == TimerType::CONTINUOUS_CALL && ticks >= execute_at) {
            callback();
            this->ResetTicks();
        }
    };

    if (bStarted) {
        if (((Timer::MillisecondsSinceStartup() / 10) % 2) == 0) {
            if (!bCalled) { TimerCheckAndInc(true); }
        } else {
            if (bCalled) { TimerCheckAndInc(false); }
        }
    }
}

void LibTimer::Fetch() {
    if(this->bStarted) {
        u64 local_time = MillisecondsSinceStartup();
        u64 new_time = local_time - this->time_started;
        this->ticks = new_time;
    }
}

void LibTimer::ResetTicks() {
    ticks = 0;
}

void LibTimer::Start() {
    bStarted = true;
    this->time_started = MillisecondsSinceStartup() - 1.000f;
}

void LibTimer::Stop() {
    bStarted = false;
}

} // namespace Timer
} // namespace LibN64
