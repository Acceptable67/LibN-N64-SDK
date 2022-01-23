

#include <stdlib.h>
#include <c++/11.2.0/cstdlib>
#include <c++/11.2.0/cstdio>
#include <c++/11.2.0/cstdint>
#include <c++/11.2.0/cstdbool>
#include <c++/11.2.0/cassert>
#include <string>
#include <bitset>
#include <math.h>
#include <functional>
#include <vector>
#include <thread>
#include <future>
#include <map>
#include <utility>

#include "libn_font.h"

#include "libn_timer.h"
#include "libn_regs.h"
#include "libn_display.h"
#include "libn_controller.h"
#include "libn_dma_pi.h"
#include "libn_frame.h"
#include "libn_stdlib.h"

#define BUTTONA 31
#define BUTTONB 30
#define BUTTONZ 29

using namespace LibN64::Display;

namespace LibN64
{	
	class libntest  : public LibN64::Frame
	{
	public:
		libntest(Resolution res, Bitdepth bd, AntiAliasing aa) : Frame(res, bd, aa) {}
	
	protected:
		LibN64::Timer::LibTimer localtimer
		 = Timer::LibTimer(Timer::TimerType::TIMER);
		auto OnCreate() -> void
		{
			Display::FillScreen(BLACK);
			Display::SetColors(LibColor::YELLOW, LibColor::RED);
		}

		int x, y;
		auto FrameUpdate() -> void override
		{
			/*Optional RDP Screen refresh*/
			//RDP::ClearScreen(GREY_SMOOTH);
			//FillScreen(GREY_SMOOTH);
			std::bitset<sizeof(int)*8> controllerd(*reinterpret_cast<uint32_t*>(PIF_RAM));
			DrawTextFormat(x,y,"%s", controllerd.to_string().c_str());
			DrawTextFormat(20,50,"CPAD Data %08X\nLocal Timer %0.2f\nSince Startup %0.2f", controllerd.to_ulong(),
			localtimer.GetSecondsPassed(), Timer::SecondsSinceStartup());
			DrawText(20,80,"Press A to start the timer");

			localtimer.Update();
		}

		auto KeyJoyXPressed(int data) -> void override
		{
			switch(data) 
			{
				case Controller::JOYLEFT:  DrawText(5,45,"Joy left "); x--; break;
				case Controller::JOYRIGHT: DrawText(5,45,"Joy right"); x++; break;
				default: break;
			}
		}

		auto KeyJoyYPressed(int data) -> void override
		{
			switch(data) 
			{
				case Controller::JOYUP:   DrawText(5,45,"Joy up   "); y--; break;
				case Controller::JOYDOWN: DrawText(5,45,"Joy down "); y++; break;
				default: break;
			}
		}

		auto KeyAPressed() -> void override {
			localtimer.Start();
			DrawText(40,100,"A has been pressed.");
		}

		auto KeyBPressed() -> void override {
			DrawText(40,100,"B has been pressed.");
		}

		auto KeyZPressed() -> void override {
			DrawText(40,100,"Z has been pressed.");
		}
	};

	extern "C" int begin()
	{	
		libntest t({320,240}, BD32BPP, AA_REPLICATE);
		t.Begin();

		HALT();
	}
}
