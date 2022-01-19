#define FRAMEBUFFER_ADDR 0xA0100000
#define PIF_BASE		 0xBFC00000
#define PIF_RAM  		 0xBFC007C4
#define VI_ADDRESS		 0xA4400000
#define SI_ADDRESS 		 0xA4800000 
#define PI_ADDRESS 		 0xA4600000
#define HALT() while(1)

#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <stdint.h>
#include <stdbool.h>
#include <string>
#include <bitset>

#include "font.h"
#include "libn_stdlib.h"
#include "libn_regs.h"
#include "libn_controller.h"
#include "libn_display.h"
#include "libn_dma_pi.h"
#include "libn_frame.h"

#define BUTTONA 31
#define BUTTONB 30
#define BUTTONZ 29

using namespace LibN64;
using namespace LibN64::Display;

namespace LibN64
{	
	class libntest  : public LibN64::Frame
	{
	public:
		libntest(resolution_t res) : Frame(res) {}

	protected:
		void FrameUpdate() override 
		{
		
			std::bitset<32> controllerd(*(int*)(PIF_RAM));
			DrawTextFormat(20,20,"%s", controllerd.to_string().c_str());
			DrawTextFormat(20,30,"%08X", controllerd.to_ulong());
		}

		void KeyJoyXPressed(int data)
		{
			switch(data) 
			{
				case Controller::JOYLEFT: DrawText(5,45,"Joy left ");break;
				case Controller::JOYRIGHT: DrawText(5,45,"Joy right");break;
				default: break;
			}
		}

		void KeyJoyYPressed(int data)
		{
			switch(data) 
			{
				case Controller::JOYUP:  DrawText(5,45,"Joy up   ");break;
				case Controller::JOYDOWN: DrawText(5,45,"Joy down ");break;
				default: break;
			}
		}

		void KeyAPressed() override {
			DrawText(40,100,"A has been pressed.");
		}

		void KeyBPressed() override {
			DrawText(40,100,"B has been pressed.");
		}

		void KeyZPressed() override {
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
