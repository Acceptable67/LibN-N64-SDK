#include <string.h>
#include <libn/timer.h>
#include <libn/types.h>

extern u64 MillisecondsSinceStartup() {
	u64 ticks = 0;
	__asm__("mfc0 %0, $9\n" : "=r"(ticks));
	return ticks / ((93750000 / 2) /
			   1000); // take vr4300 clock speed and divide by two
}

extern u64 SecondsSinceStartup() {
	return MillisecondsSinceStartup() * 0.001f;
}

void LibTimer_Setup(LibTimer *timer, TimerType type, [[maybe_unused]] float TickExecution) 
{
	timer->localType = type;
	timer->execute_at = 0.0f;
}

float LibTimer_GetSecondsPassed(LibTimer *timer) {
	return timer->ticks;
}

float LibTimer_GetMinutesPassed(LibTimer *timer) {
	return timer->ticks / 60;
}

float LibTimer_GetMsPassed(LibTimer *timer) {
	return (timer->ticks * 1000);
}

/*Optional function that would be called every second
    We could just probe the COP0 for the time and subtract from
    that to get our value but this will do.*/
void TimerCheckAndInc(LibTimer *timer, bool bSetCall, void (*callback)(void)) 
{
		timer->ticks += 0.01f;

		timer->bCalled = bSetCall;

		if (timer->localType == ONE_SHOT 
		&& timer->ticks >= timer->execute_at) {
			callback();
			timer->bStarted = false;
			LibTimer_ResetTicks(timer);
			return;
		} else if (timer->localType == CONTINUOUS_CALL &&
			   timer->ticks >= timer->execute_at) {
			callback();
			LibTimer_ResetTicks(timer);
		}
}
void LibTimer_Update(LibTimer *timer, void (*callback)()) {


	if (timer->bStarted) {
		/*there must be a better way to optimize this, but will figure
		 * out later*/
		if (((MillisecondsSinceStartup() / 10) % 2) == 0) {
			if (!timer->bCalled) { TimerCheckAndInc(timer, true, callback); }
		} else {
			if (timer->bCalled) { TimerCheckAndInc(timer, false, callback); }
		}
	}
}

void LibTimer_ResetTicks(LibTimer *timer) {
	timer->ticks = 0;
}

void LibTimer_Start(LibTimer *timer) {
	timer->bStarted = true;
}

void LibTimer_Stop(LibTimer* timer) {
	timer->bStarted = false;
}
