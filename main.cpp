
#include <libn.hpp>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

using namespace LibN64;
using namespace LibN64::Display;

#include <libn/menu.hpp>

CreateControllerHandle(CPAD_DATA);

void TestCallback()
{
	printf("tester");
}

void Test2Callback()
{
	printf("tester2");
}

enum MenuId { Main_Menu, Second_Menu }; 
enum SubMenuId { SubM1, SubM2 };

class MainApp : public Frame {
public: 
	MainApp(Resolution res,
	Bitdepth bitdepth, 
	AntiAliasing antialiasing)
	: Frame(res, bitdepth, antialiasing){}  

	LibMenuManager mm;
	LibMenu *menu_first;

	void _SetupMenu()
	{
		menu_first  = mm.AddMenu(Main_Menu, "Main Menu", {40, 40}, BLACK, WHITE);
		menu_first->AddMenuItem(SubMenuId::SubM1, "Test ", TestCallback);
		menu_first->AddMenuItem(SubMenuId::SubM2, "Test 2", Test2Callback);
		menu_first->SetFocused(); 
	}

	void OnCreate() override 
	{
		_SetupMenu();
	}  
 
	void FrameUpdate() override 
	{
		constexpr auto buttonPress = [&](const s8* button) 
		{ 
			printf(fmt::format("{} has been pressed.", button));  
		}; 

		RDP::FillScreen(LibColor::GREY); 

		menu_first->Show();  

		Controller::Read();
		if (CPAD_DATA->A) {
			menu_first->WaitKeyPress();
			buttonPress("A");
		}
		if (CPAD_DATA->B) { buttonPress("B"); }
		if (CPAD_DATA->Z) { buttonPress("Z"); }
		if (CPAD_DATA->up) {
			buttonPress("D-Up");
			mm[0]->MoveSelectionUp();
		}

		if (CPAD_DATA->down) {
			buttonPress("D-Down");
			mm[0]->MoveSelectionDown();
		} 

		if (CPAD_DATA->left) { buttonPress("D-Left"); }

		if (CPAD_DATA->right) { buttonPress("D-Right"); }

		ResetConsole();
	}
};

EXTERN int main()
{
	MainApp instance({320,240}, Bitdepth::BD32BPP, AntiAliasing::AA_REPLICATE);
	instance.Begin();
	return 0;
}
