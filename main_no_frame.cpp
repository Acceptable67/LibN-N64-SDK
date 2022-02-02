#include <stdlib.h>
#include <string>
#include <bitset>
#include <math.h>
#include <functional>
#include <iostream>
#include <ranges>
#include <vector>
#include <cstring>
#include <map>
#include <utility>
#include <source_location>
#include <stack>
#include <list>
#include <memory_resource>
#include <memory>

#include <libn.hpp>

using namespace LibN64::Display;
using namespace LibN64::Timer;

#define DRAW_SPRITE
#define SPRITE_LOCATION 0xB0101000

CreateControllerHandle(CPAD_DATA);

namespace LibN64 {

static bool bRunning;
static LibPos circlePos	      = {40, 40};
static Resolution default_res = {320, 240};
void InitDisplay() {
	Display::Initialize(default_res, BD32BPP, AA_RESAMP_ONLY, GAMMA_OFF);
	Display::FillScreen(GREY);
	Display::SetColors(LibColor::GOLD, LibColor::NAVY_BLUE | 0xFF);

	bRunning = true;

	Display::SetVI_Intterupt(0x200);
	Controller::WriteController();
}

constexpr u8 inc_amount = 2;
inline void CheckController() {
	Controller::ReadController();
	if (CPAD_DATA->A) { printf("Pressed A"); }
	if (CPAD_DATA->B) { printf("Pressed B"); }
	if (CPAD_DATA->Z) { printf("Pressed Z"); }
	if (CPAD_DATA->up) {
		printf("Pressed D-UP");
		circlePos.y -= inc_amount;
	}
	if (CPAD_DATA->down) {
		printf("Pressed D-DOWN");
		circlePos.y += inc_amount;
	}
	if (CPAD_DATA->left) {
		printf("Pressed D-LEFT");
		circlePos.x -= inc_amount;
	}
	if (CPAD_DATA->right) {
		printf("Pressed D-RIGHT");
		circlePos.x += inc_amount;
	}
}

EXTERN int begin() {
#ifdef DRAW_SPRITE
	auto spr = LibSprite(SPRITE_LOCATION);
#endif

	auto timer = LibTimer(TimerType::TIMER);

	InitDisplay();
	timer.Start();
	while (bRunning) {

		/*On Vertical Interrupt the code below will be called, making
		 * for little to no flicker.*/
		SetVI_IntCallback([&] {
			RDP::FillScreen(GREY);
			printf("Hello Github!");
			DrawTri({40, 40}, {70, 60},
				{90 + circlePos.x, 40 + circlePos.y}, PURPLE);
			printf("Cached %08X Uncached %08X",
			       reinterpret_cast<u32>(CachedAddr(0xA0001234)),
			       reinterpret_cast<u32>(UncachedAddr(0x80001234)));
			printf("Timer %0.2f System %0.2f",
			       timer.GetSecondsPassed(),
			       Timer::SecondsSinceStartup());

			CheckController();

#ifdef DRAW_SPRITE /*Lags the system a bit*/
			printf("\n\n\nThis is a sprite sky right below");
			spr.Draw({100, 100});
#endif

			DrawCircle({40 + circlePos.x, 40 + circlePos.y}, 2,
				   ORANGE, false, 0.4);
		});
		timer.Update();
		ResetConsole();
	}
	return 0;
}
} // namespace LibN64
