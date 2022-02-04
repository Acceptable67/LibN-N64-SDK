#include <functional>
#include <string>
#include <libn/timer.hpp>
#include <libn/types.hpp>

namespace LibN64 {
namespace Timer {
extern auto MillisecondsSinceStartup() {
	u64 ticks = 0;
	asm("mfc0 %0, $9\n" : "=r"(ticks));
	return ticks / ((93750000 / 2) /
			   1000); // take vr4300 clock speed and divide by two
}

extern float SecondsSinceStartup() {
	return MillisecondsSinceStartup() * 0.001f;
}

/*Essentially a version of multithreading.
    Specified code is called every X milliseconds the user requests it to.
    Can stop, start, reset.*/
LibTimer::LibTimer() {
	this->localType	 = TimerType::TIMER;
	this->execute_at = 0.0f;
}

LibTimer::LibTimer(TimerType type, float tickExecution)
    : localType(type), execute_at(tickExecution) {
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

/*Optional function that would be called every second
    We could just probe the COP0 for the time and subtract from
    that to get our value but this will do.*/
void LibTimer::Update(std::function<void()> callback) {
	auto TimerCheckAndInc = [&](bool bSetCall) {
		this->ticks += 0.01f;

		bCalled = bSetCall;

		if (localType == TimerType::ONE_SHOT && ticks >= execute_at) {
			callback();
			bStarted = false;
			this->ResetTicks();
			return;
		} else if (localType == TimerType::CONTINUOUS_CALL &&
			   ticks >= execute_at) {
			callback();
			this->ResetTicks();
		}
	};

	if (bStarted) {
		/*there must be a better way to optimize this, but will figure
		 * out later*/
		if (((Timer::MillisecondsSinceStartup() / 10) % 2) == 0) {
			if (!bCalled) { TimerCheckAndInc(true); }
		} else {
			if (bCalled) { TimerCheckAndInc(false); }
		}
	}
}

void LibTimer::ResetTicks() {
	ticks = 0;
}

void LibTimer::Start() {
	bStarted = true;
}

void LibTimer::Stop() {
	bStarted = false;
}
} // namespace Timer
}; // namespace LibN64