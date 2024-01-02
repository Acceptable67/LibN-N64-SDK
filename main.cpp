

#include <libn.hpp>
#include <libn/controller.hpp>
#include <libn/interrupts.hpp>
#include <libn/vector.hpp>
#include "main.hpp"

using namespace LibN64;
using namespace LibN64::Display;

CreateControllerHandle(CData);

f32 x = 0, y = 0;
LibVector vec;  

#define INC_AMT 0.002;

void APressed()
{
	printf("A Pressed");
}

void BPressed()
{
	printf("B Pressed");
}

void CheckImmediateInput()
{
	if (CData->A) 
	{ 
		APressed(); 
	}

	if (CData->B) 
	{ 
		BPressed();
	 }

	if (CData->Z) 		{ }
	if (CData->start) 	{ }
	if (CData->x) 		{ }
	if (CData->y) 		{ }
}

void CheckCPADInput()
{
	if (CData->up) 		{ y-=INC_AMT; }
	if (CData->down) 	{ y+=INC_AMT; }
	if (CData->left) 	{ x-=INC_AMT; }
	if (CData->right) 	{ x+=INC_AMT; }
}

void FrameUpdate()
{
	RDP::FillScreen(GREY_SMOOTH);

	CheckImmediateInput();

	printf("vec = %u", vec.at(0));
	DrawRect({40 + x, 40 + y}, 40, 40, RED, true);
}


extern "C" int main()
{ 
	TextColor local = { LibColor::YELLOW, LibColor::BLACK | 0xFF};

	Initialize({320,240}, Bitdepth::BD32BPP, AA_REPLICATE, GAMMA_OFF);
	FillScreen(GREY);
	SetColors(local.Foreground, local.Background);

	Interrupts::Toggle(Interrupts::Type::VI, true);
	Interrupts::SetCallback(Interrupts::Type::VI, [&](){
		FrameUpdate();
	});

	Controller::Write();

	vec.push_back((void*)42);
	while(true)
	{
		Interrupts::Handle();
		Controller::Read();
		CheckCPADInput();
		ResetConsole();
		Display::SwapBuffers();
	}
	return 0;
}
