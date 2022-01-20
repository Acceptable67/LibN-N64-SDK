#define FRAMEBUFFER_ADDR 0xA0100000
#define PIF_BASE		 0xBFC00000
#define PIF_RAM  		 0xBFC007C4
#define VI_ADDRESS		 0xA4400000
#define SI_ADDRESS 		 0xA4800000 
#define PI_ADDRESS 		 0xA4600000
#define HALT() while(1)

#include <stdlib.h>
#include <c++/11.2.0/cstdlib>
#include <c++/11.2.0/cstdio>
#include <c++/11.2.0/cstdint>
#include <c++/11.2.0/cstdbool>
#include <c++/11.2.0/cassert>
#include <string>
#include <bitset>
#include <vector>

#include "font.h"

#include "libn_regs.h"
#include "libn_controller.h"
#include "libn_display.h"
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
		libntest(Resolution res) : Frame(res) {}
		
	protected:
		auto OnCreate() -> void
		{
			Display::SetColors(0xFFFFFFFF, 0x000000FF);
		}

		int x, y;
		auto FrameUpdate() -> void override
		{
			/*Optional RDP Screen refresh*/
			//RDP::DrawRectangleSetup(0,0,ScreenWidth(),ScreenHeight(),0x202020FF);
                
			std::bitset<sizeof(int)*8> controllerd(*reinterpret_cast<uint32_t*>(PIF_RAM));
			DrawTextFormat(20,20,"%s", controllerd.to_string().c_str());
			DrawTextFormat(20,30,"%08X", controllerd.to_ulong());
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
		libntest t({320,240});
		t.Begin();

		HALT();
	}
}
