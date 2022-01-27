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
#include <list>
#include <memory_resource>
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
		std::shared_ptr<LibTimer> exTimer = std::make_shared<LibTimer>(TimerType::TIMER);
		bool bTimerStatus;

		LibSprite spr = LibSprite(0xB0101000);
		auto OnCreate() -> void
		{
			SetColors(YELLOW, 0xFF);
			exTimer->Start();
		}
		
		int x = 0, y = 0;
		auto FrameUpdate() -> void override
		{
			
			printf("This is a test.\nPicture of sky underneath");
			printf("System Time %0.2f My Clock %0.2f", Timer::SecondsSinceStartup(), exTimer->GetSecondsPassed());
			
			spr.Draw(20 + x,20 + y);
			exTimer->Update();
			if(x >= 160 && y >= 120) exTimer->Stop();
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
			(bTimerStatus) ? exTimer->Stop() : exTimer->Start();
			bTimerStatus = !bTimerStatus;
			printf("B Pressed");
		}

		auto KeyZPressed() -> void override {
			printf("Z Pressed");
		}

		auto KeyStartPressed() -> void override
		{
			RDP::ClearScreen(GREY_SMOOTH);
		}
	};

	extern "C" int begin()
	{	
		libntest t({320,240}, BD32BPP, AA_REPLICATE, false);
		t.Begin();

		HALT();
	}
}
