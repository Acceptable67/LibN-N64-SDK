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

using namespace LibN64;
using namespace LibN64::Display;

bool running = true;


namespace LibN64 
{

	extern "C" int begin()
	{	
		Display::Initialize({320,240});
		Display::FillScreen(0x202020FF);
		Display::SetColors(0x20DF00FF, 0xFF0000FF);
		Controller::SI_WriteController();
		while(running) 
		{
			Controller::SI_ReadController();
			#define BUTTONA 31
			#define BUTTONB 30
			#define BUTTONZ 29
	
			std::bitset<32> controllerd(*(int*)(PIF_RAM));
			DrawTextFormat(20,20,"%s", controllerd.to_string().c_str());
			DrawTextFormat(20,30,"%08X", controllerd.to_ulong());

			if(controllerd.test(BUTTONA)) {
				
			}
			if(controllerd.test(BUTTONB)) {
				DrawText(40,50,"B has been pressed.");
			}
			if(controllerd.test(BUTTONZ)) {
				DrawText(40,60,"Z has been pressed.");
			}

			switch(controllerd.to_ulong()) 
			{
				case Controller::JOYUP:    DrawText(5,45,"Joy up   ");   break;
				case Controller::JOYDOWN:  DrawText(5,45,"Joy down "); break;
				case Controller::JOYLEFT:  DrawText(5,45,"Joy left "); break;
				case Controller::JOYRIGHT: DrawText(5,45,"Joy right");break;
				default: break;
			}
		}
	HALT();
	}
}
