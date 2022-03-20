
#include <libn.hpp>
#include <libn/controller.hpp>
#include <libn/interrupts.hpp>
#include <libn/vector.hpp>

#define FMT_HEADER_ONLY
#include <fmt/format.h>

using namespace LibN64;
using namespace LibN64::Display;

#include <libn/menu.hpp>

#define INC_AMT 0.002

void FrameUpdate();

CreateControllerHandle(cpad_data);

f32 x = 0, y = 0;

bool bRunning = false;
void InitDisplay()
{
		bRunning = true;
	TextColor local = { LibColor::YELLOW, LibColor::BLACK | 0xFF};

	Initialize({320,240}, Bitdepth::BD32BPP, AA_REPLICATE, GAMMA_OFF);
	FillScreen(GREY);
	SetColors(local.Foreground, local.Background);

	Interrupts::Toggle(Interrupts::Type::VI, true);
	Interrupts::SetCallback(Interrupts::Type::VI, [&](){
		FrameUpdate();
	});

	Controller::Write();
}

LibVector vec; 
void FrameUpdate()
{
	RDP::FillScreen(GREY_SMOOTH);

	printf("%u", vec.At(0));
	DrawRect({40 + x,40 + y}, 40, 40, RED, true);
}

EXTERN int main()
{
	InitDisplay();

	vec.Pushback((void*)42);
	while(bRunning)
	{
	Interrupts::Handle();

		Controller::Read();
		if (cpad_data->A) 	{ }
		if (cpad_data->B) 	{ }
		if (cpad_data->Z) 	{ }
		if (cpad_data->start) { }
		if (cpad_data->up) 	{ y-=INC_AMT; }
		if (cpad_data->down) { y+=INC_AMT; }
		if (cpad_data->left) { x-=INC_AMT; }
		if (cpad_data->right) { x+=INC_AMT; }
		if (cpad_data->x) {
			
		}
		if (cpad_data->y) {
			
		}

		ResetConsole();
		Display::SwapBuffers();
	}
	return 0;
}
