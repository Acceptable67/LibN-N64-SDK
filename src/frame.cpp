#include <libn/vi.hpp>
#include <libn/controller.hpp>
#include <libn/frame.hpp>
#include <libn/sprite.hpp>
#include <libn/interrupts.hpp>

using namespace LibN64;
using namespace LibN64::Display;

CreateControllerHandle(cpad_data);

Frame::Frame(const Resolution res, const Bitdepth bitdepth, const AntiAliasing antialiasing, const Gamma gamma, Display::TextColor textcolor)
: r(res), bd(bitdepth), aa(antialiasing), g(gamma), tc(textcolor) {}

void Frame::Begin() 
{
	this->bRunning = true;

	Initialize(this->r, this->bd, this->aa, this->g);
	FillScreen(GREY);
	SetColors(this->tc.Foreground, this->tc.Background);

	Interrupts::Toggle(Interrupts::Type::VI, true);
	Interrupts::SetCallback(Interrupts::Type::VI, [&]()
	{
		this->FrameUpdate();
	});

	Controller::Write();

	this->OnCreate();

	while (bRunning) 
	{
		Controller::Read();
		if (cpad_data->A) 		{ this->KeyAPressed(); 		}
		if (cpad_data->B) 		{ this->KeyBPressed(); 		}
		if (cpad_data->Z) 		{ this->KeyZPressed(); 		}
		if (cpad_data->start) 	{ this->KeyStartPressed(); 	}
		if (cpad_data->up) 		{ this->KeyDUpPressed(); 	}
		if (cpad_data->down) 	{ this->KeyDDownPressed(); 	}
		if (cpad_data->left) 	{ this->KeyDLeftPressed(); 	}
		if (cpad_data->right) 	{ this->KeyDRightPressed(); }
		if (cpad_data->C_up) 	{ this->KeyCUpPressed(); 	}
		if (cpad_data->C_down) 	{ this->KeyCDownPressed(); 	}
		if (cpad_data->C_left) 	{ this->KeyCLeftPressed(); 	}
		if (cpad_data->C_right) { this->KeyCRightPressed(); }
		if (cpad_data->x) 		{ this->KeyJoyXPressed();	}
		if (cpad_data->y) 		{ this->KeyJoyYPressed();	}
 
		Interrupts::Handle();
		ResetConsole();
		Display::SwapBuffers();
	}
}

void Frame::SetScreenClear() 
{
	bClearScreen = true;
}

void Frame::ExitFrameLoop() 
{
	bRunning = false;
}

u32 Frame::ScreenWidth() 
{
	return r.width;
}

u32 Frame::ScreenHeight() 
{
	return r.height;
}

s32 Frame::JoyDataX() {
	return cpad_data->x;
}

s32 Frame::JoyDataY() {
	return cpad_data->y;
}

 void Frame::FrameUpdate(){}
 void Frame::OnCreate(){}
 void Frame::KeyAPressed(){}
 void Frame::KeyBPressed(){}
 void Frame::KeyZPressed(){}
 void Frame::KeyDUpPressed(){}
 void Frame::KeyDDownPressed(){}
 void Frame::KeyDLeftPressed(){}
 void Frame::KeyDRightPressed(){}
 void Frame::KeyCUpPressed(){}
 void Frame::KeyCDownPressed(){}
 void Frame::KeyCLeftPressed(){}
 void Frame::KeyCRightPressed(){}
 void Frame::KeyStartPressed(){}
 void Frame::KeyJoyXPressed(){}
 void Frame::KeyJoyYPressed(){}
