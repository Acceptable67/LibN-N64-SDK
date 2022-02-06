
#include <libn.hpp>


//#define DRAW_SPRITE
#undef DRAW_SPRITE
#ifdef DRAW_SPRITE
#	define SPRITE_LOCATION 0xB0101000
#endif

#define FMT_HEADER_ONLY
#include <fmt/format.h>

using namespace LibN64;
using namespace LibN64::Display;

#include <libn/menu.hpp>

CreateControllerHandle(CPAD_DATA);

enum MenuId
{
	Main_Menu
};

enum SubMenuId 
{
	SubM1,
	SubM2
};

static constexpr Resolution default_res = {320, 240};
static bool bRunning				 = false;

namespace LibN64 
{
	void InitDisplay() 
	{
		Initialize(default_res, Bitdepth::BD32BPP, AntiAliasing::AA_REPLICATE, Gamma::GAMMA_OFF);
		FillScreen(LibColor::GREY);
		SetColors(LibColor::GOLD, LibColor::NAVY_BLUE | 0xFF);

		bRunning = true;

		SetVI_Intterupt(0x200);
		Controller::Write();
	}

	EXTERN int main() 
	{ 
		InitDisplay();

		LibMenuManager mm;
		mm.AddMenu(Main_Menu, "Main Menu", {40,40}, BLACK, WHITE);
		mm[Main_Menu]->AddMenuItem(SubMenuId::SubM1, "Test ");
		mm[Main_Menu]->AddMenuItem(SubMenuId::SubM2, "Test 2");  
		mm[Main_Menu]->SetFocused();   

		constexpr auto buttonPress = [&](auto button) { fmt::print("{} has been pressed.", button); };
		while (bRunning)
		{
			/*On Vertical Interrupt the code below will be called, making
			* for little to no flicker.*/
			SetVI_IntCallback([&] 
			{
				RDP::FillScreen(LibColor::GREY);

				mm[Main_Menu]->Show();
				Controller::Read();
				if (CPAD_DATA->A) { mm[Main_Menu]->WaitKeyPress(); buttonPress('A'); }
				if (CPAD_DATA->B) { buttonPress('B'); } 
				if (CPAD_DATA->Z) { buttonPress('Z'); }
				if (CPAD_DATA->up) 
				{ 
					buttonPress("D-Up");
					mm[0]->MoveSelectionUp();
				}

				if (CPAD_DATA->down) 
				{
					buttonPress("D-Down");
					mm[0]->MoveSelectionDown();
				}

				if (CPAD_DATA->left) 
				{
					buttonPress("D-Left");
				}

				if (CPAD_DATA->right) 
				{
					buttonPress("D-Right");
				}

				if(mm[Main_Menu]->MenuItemIsSelected(SubM1)) 
				{
					printf("Menu option 1 selected");
				}

				if(mm[Main_Menu]->MenuItemIsSelected(SubM2)) 
				{
					printf("Menu option 2 selected");
				}
				});

				ResetConsole();
		}
		return 0;
	}
} // namespace LibN64
