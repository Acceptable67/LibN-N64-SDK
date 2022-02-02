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

#define FMT_HEADER_ONLY

using namespace LibN64::Display;
using namespace LibN64::Timer;

namespace LibN64 {
static bool bRunning;
static LibPos circlePos = {40, 40};

CreateControllerHandle(CPAD_DATA);

void InitDisplay() {
	// ReadROMTitle();
	Display::Initialize({320, 240}, BD32BPP, AA_RESAMP_ONLY, GAMMA_OFF);
	Display::FillScreen(GREY);
	Display::SetColors(LibColor::GOLD, LibColor::NAVY_BLUE | 0xFF);

	bRunning = true;

	Display::SetVI_Intterupt(0x200);
	Controller::WriteController();
}

inline void CheckController() {
	Controller::ReadController();
	if (CPAD_DATA->A) {
		printf("Pressed A");
		circlePos.x++;
	}
	if (CPAD_DATA->B) {
		printf("Pressed B");
		circlePos.x--;
	}
	if (CPAD_DATA->Z) {
		printf("Pressed Z");
		circlePos.y++;
	}
	if (CPAD_DATA->up) {
		printf("Pressed D-UP");
		circlePos.y--;
	}
}

constexpr u32 model_addr = 0x80001234;
extern "C" int begin() {
	InitDisplay();

	while (bRunning) {
		SetVI_IntCallback([&] {
			RDP::FillScreen(GREY);
			printf("Cached %08X Uncached %08X",
			    reinterpret_cast<u32>(CachedAddr(model_addr)),
			    reinterpret_cast<u32>(UncachedAddr(model_addr)));

			CheckController();
			DrawCircle({40 + circlePos.x, 40 + circlePos.y}, 2,
			    BLACK, false, 0.4);
		});
		ResetConsole();
	}
	return 0;
}
} // namespace LibN64
