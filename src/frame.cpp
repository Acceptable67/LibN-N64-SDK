#include <libn/vi_display.hpp>
#include <libn/controller.hpp>
#include <libn/frame.hpp>
#include <libn/sprite.hpp>

using namespace LibN64;
using namespace LibN64::Display;

CreateControllerHandle(cpad_data);

Frame::Frame(const Resolution res, const Bitdepth bitdepth, 
	const AntiAliasing antialiasing)
	: r(res), bd(bitdepth), aa(antialiasing) 
{

}

void Frame::Begin() 
{
	this->bRunning = true;
	TextColor local = { LibColor::YELLOW, LibColor::BLACK | 0xFF};

	Initialize(this->r, this->bd, this->aa, this->g);
	SetColors(local.Foreground, local.Background);

	this->OnCreate();

	SetVI_Intterupt(0x200);
	Controller::Write();

	while (bRunning) {
		/*clear screen on vertical retrace*/
		SetVI_IntCallback([&]() {
			this->FrameUpdate();
		});

		Controller::Read();
		if (cpad_data->A) 	{ this->KeyAPressed(); }
		if (cpad_data->B) 	{ this->KeyBPressed(); }
		if (cpad_data->Z) 	{ this->KeyZPressed(); }
		if (cpad_data->start) { this->KeyStartPressed(); }
		if (cpad_data->up) 	{ this->KeyDUpPressed(); }
		if (cpad_data->down) { this->KeyDDownPressed(); }
		if (cpad_data->left) { this->KeyDLeftPressed(); }
		if (cpad_data->right) { this->KeyDRightPressed(); }
		if (cpad_data->x) {
			this->KeyJoyXPressed(
			    *reinterpret_cast<u32 *>(cpad_data) & 0x0000FF00);
		}
		if (cpad_data->y) {
			this->KeyJoyYPressed(
			    *reinterpret_cast<u32 *>(cpad_data) & 0x000000FF);
		}
	}
}

void Frame::SetScreenClear() {
	bClearScreen = true;
}

void Frame::ExitFrameLoop() {
	bRunning = false;
}

u32 Frame::ScreenWidth() {
	return r.width;
}

u32 Frame::ScreenHeight() {
	return r.height;
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
 void Frame::KeyStartPressed(){}
 void Frame::KeyJoyXPressed(int){}
 void Frame::KeyJoyYPressed(int){}