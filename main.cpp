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
		Display::Initialize();
		Display::FillScreen(0x202020FF);
		Display::SetColors(0x20DF00FF, 0xFF0000FF);
		Controller::SI_WriteController();
		while(running) 
		{
			#define BUTTONA 31
			#define BUTTONB 30
			#define BUTTONZ 29
			LibN64::Controller::SI_ReadController();

			std::bitset<32> controllerd(reinterpret_cast<int*>(PIF_RAM));
			DrawTextFormat(20,20,"%s", controllerd.to_string().c_str());

			if(controllerd.test(BUTTONA)) {
				DrawText(40,40,"A has been pressed.");
			}
			if(controllerd.test(BUTTONB)) {
				DrawText(40,50,"B has been pressed.");
			}
			if(controllerd.test(BUTTONZ)) {
				DrawText(40,60,"Z has been pressed.");
			}
			switch(*libn_controller_data) 
			{
				case Controller::JOYUP:    Display::FillScreen(0x202020FF); DrawText(5,25,"joy up");       break;
				case Controller::JOYDOWN:  Display::FillScreen(0x20FF20FF); DrawText(5,35,"joy down");   break;
				case Controller::JOYLEFT:  Display::FillScreen(0x2020FFFF); DrawText(5,45,"joy left"); break;
				case Controller::JOYRIGHT: Display::FillScreen(0xFF2020FF); DrawText(5,55,"joy right"); break;
				default: break;
			}
		}
	HALT();
	}
}
