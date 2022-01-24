

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
#include <ranges>
#include <vector>
#include <map>
#include <utility>

#include "libn.h"

using namespace LibN64::Display;
using namespace LibN64::Timer;

namespace LibN64
{	
	class libntest  : public LibN64::Frame
	{
	public:
		libntest(Resolution res, Bitdepth bd, AntiAliasing aa) : Frame(res, bd, aa) {}
	
	protected:
		LibTimer localtimer = LibTimer(TimerType::TIMER);
		LibSprite *spr;

		auto OnCreate() -> void
		{
			Display::FillScreen(GREY_SMOOTH);
			Display::SetColors(LibColor::YELLOW, LibColor::RED);

			spr = new LibSprite(0xB0101000);
		}

		int x, y;
		auto FrameUpdate() -> void override
		{
			/*Optional RDP Screen refresh*/
			
			//RDP::DrawRectangleSetup(0,0,200,200,RED);
			std::bitset<sizeof(int)*8> controllerd(*reinterpret_cast<uint32_t*>(PIF_RAM));
			printf("%s", controllerd.to_string().c_str());
			printf("CPAD Data     %08lX\n"
				   "Local Timer   %0.2f\n"
				   "Since Startup %0.2f", controllerd.to_ulong(),
			localtimer.GetSecondsPassed(), Timer::SecondsSinceStartup());
			printf("\nPress A to start the timer");
			printf("Press B to stop the timer");
			localtimer.Update();

			spr->Draw(120,120);
	

		}

		auto KeyJoyXPressed(int data) -> void override
		{
			switch(data) 
			{
				case Controller::JOYLEFT:  DrawText(5,125,"Joy left "); x--; break;
				case Controller::JOYRIGHT: DrawText(5,125,"Joy right"); x++; break;
				default: break;
			}
		}

		auto KeyJoyYPressed(int data) -> void override
		{
			switch(data) 
			{
				case Controller::JOYUP:   DrawText(5,125,"Joy up   "); y--; break;
				case Controller::JOYDOWN: DrawText(5,125,"Joy down "); y++; break;
				default: break;
			}
		}

		auto KeyAPressed() -> void override {
			localtimer.Start();
			DrawText(40,100,"A has been pressed.");
		}

		auto KeyBPressed() -> void override {
			localtimer.Stop();
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
