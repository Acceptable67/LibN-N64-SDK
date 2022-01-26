#include <stdlib.h>

#include <string>
#include <bitset>
#include <math.h>
#include <functional>
#include <ranges>
#include <vector>
#include <cstring>
#include <map>

#include <utility>
#include <source_location>
#include <stack>
#include <memory>

#include <libn.h>

using namespace LibN64::Display;
using namespace LibN64::Timer;


namespace LibN64
{	
	class libntest  : public LibN64::Frame
	{
	public:
		libntest(Resolution res, Bitdepth bd, AntiAliasing aa, bool buf) : Frame(res, bd, aa, buf) {}

	private:
		LibTimer *exTimer;
		LibSprite spr = LibSprite(0xB0101000);
		auto OnCreate() -> void
		{
			SetColors(YELLOW, 0xFF);

			exTimer = new LibTimer(TimerType::TIMER);
			exTimer->Start();
		}
		
		int x = 0, y = 0;
		auto FrameUpdate() -> void override
		{
			DrawText(x,y,"This is a test. Picture of sky underneath");
			printf("%0.2f %0.2f",  exTimer->GetSecondsPassed(), Timer::SecondsSinceStartup());
			
			spr.Draw(20,20);

			exTimer->Update();
		}

	
		auto KeyJoyXPressed(int data) -> void override
		{
	
			switch(data) 
			{
				case Controller::JOYLEFT:  x-=1; 	break;
				case Controller::JOYRIGHT: x+=1;	break;
				default: break;
			}
		}

		auto KeyJoyYPressed(int data) -> void override
		{
			switch(data) 
			{
				case Controller::JOYUP:		y-=1;   break;
				case Controller::JOYDOWN: 	y+=1; 	break;
				default: break;
			}
		}

		auto KeyAPressed() -> void override {
			printf("A Pressed");
		}

		auto KeyBPressed() -> void override {
			printf("B Pressed");
		}

		auto KeyZPressed() -> void override {
			printf("Z Pressed");
		}
	};

	extern "C" int begin()
	{	
		libntest t({320,240}, BD32BPP, AA_REPLICATE, true);
		t.Begin();

		HALT();
	}
}
