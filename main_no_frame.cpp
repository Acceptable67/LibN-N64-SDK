
#include <libn.hpp>

//#define DRAW_SPRITE
#undef DRAW_SPRITE
#ifdef DRAW_SPRITE
#	define SPRITE_LOCATION 0xB0101000
#endif

#define FMT_HEADER_ONLY
#include <fmt/format.h>

using namespace LibN64::Display;

CreateControllerHandle(CPAD_DATA);

namespace LibN64 {

static Display::LibPos circlePos		 = {40, 40};
static constexpr Display::Resolution default_res = {320, 240};
static bool pakRead				 = false;
static bool bRunning				 = false;
constexpr u8 inc_amount				 = 2;

struct cpak_data {
	u32 data[8];
};

void InitDisplay() {
	Display::Initialize(default_res, Bitdepth::BD32BPP, AntiAliasing::AA_REPLICATE, Gamma::GAMMA_OFF);
	Display::FillScreen(Display::LibColor::GREY);
	Display::SetColors(Display::LibColor::GOLD, Display::LibColor::NAVY_BLUE | 0xFF);

	bRunning = true;

	Display::SetVI_Intterupt(0x200);
	Controller::Write();
}

inline void CheckController() {
	constexpr auto buttonPress = [&](auto button) { fmt::print("{} has been pressed.", button); };
	Controller::Read();
	if (CPAD_DATA->A) { buttonPress('A'); }
	if (CPAD_DATA->B) { buttonPress('B'); }
	if (CPAD_DATA->Z) { buttonPress('Z'); }
	if (CPAD_DATA->up) {
		buttonPress("D-Up");
		circlePos.y -= inc_amount;
	}
	if (CPAD_DATA->down) {
		buttonPress("D-Down");
		circlePos.y += inc_amount;
	}
	if (CPAD_DATA->left) {
		buttonPress("D-Left");
		circlePos.x -= inc_amount;
	}
	if (CPAD_DATA->right) {
		buttonPress("D-Right");
		circlePos.x += inc_amount;
	}
}

EXTERN int begin() {
#ifdef DRAW_SPRITE
	auto spr = LibSprite(SPRITE_LOCATION);
#endif
	InitDisplay();

	auto timer = Timer::LibTimer(Timer::TimerType::CONTINUOUS_CALL, 0.0);
	timer.Start();

	auto pak_buf = static_cast<cpak_data *>(std::malloc(sizeof(cpak_data)));
	while (bRunning) {
		/*On Vertical Interrupt the code below will be called, making
		 * for little to no flicker.*/
		Display::SetVI_IntCallback([&] {
			Display::RDP::FillScreen(Display::GREY);
			Display::DrawTri({40, 40}, {70, 60}, {90 + circlePos.x, 40 + circlePos.y}, Display::PURPLE);
			fmt::print("Cached {:8X} Uncached {:8X}", reinterpret_cast<u32>(CachedAddr(0xA0001234)),
				   reinterpret_cast<u32>(UncachedAddr(0x80001234)));
			fmt::print("Timer {:.2f} System {:.2f}", timer.GetSecondsPassed(), Timer::SecondsSinceStartup());

			/*controller pak testing*/
			if (!pakRead) {
				if (Controller::MemPak::isInserted()) {
					if (Controller::MemPak::ReadAddress(0x40, pak_buf)) {
						pakRead = true;
					} else {
						fmt::print("Reading controller pak...");
					}
				}
			} else {
				/*Data read, display data*/
				fmt::print("0x40: {:8X} 0x44: {:8X}", pak_buf->data[0], pak_buf->data[1]);
			}
			CheckController();

#ifdef DRAW_SPRITE /*Lags the system a bit*/
			printf("\n\n\nThis is a sprite sky right below");
			spr.Draw(100, 100);
#endif
			Display::DrawText({circlePos.x, circlePos.y + 20}, "Player 1");
			Display::DrawRect(circlePos, 15, 15, Display::PURPLE, false);
			Display::DrawCircle({40 + circlePos.x, 40 + circlePos.y}, 2, Display::ORANGE, true, 0.5);
		});

		/*Draw a random red rectangle on the screen every 0.00 calls to the timer*/
		timer.Update([&](){
			DrawRect({rand() % 320, rand() % 240}, rand() % 20 + 10, rand() % 20 + 10, RED, true);
		});
		ResetConsole();
	}
	return 0;
}
} // namespace LibN64
