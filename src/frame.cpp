#include <libn/vi_display.hpp>
#include <libn/controller.hpp>
#include <libn/frame.hpp>
#include <libn/sprite.hpp>

using namespace LibN64::Display;

namespace LibN64 {
CreateControllerHandle(cpad_data);

constexpr Frame::Frame(const Display::Resolution res,
    const Display::Bitdepth bitdepth, const Display::AntiAliasing antialiasing)
    : r(res), bd(bitdepth), aa(antialiasing) {
}

void Frame::Begin() {
	bRunning		 = true;
	Display::TextColor local = {
	    Display::LibColor::YELLOW, Display::LibColor::BLACK | 0xFF};

	Display::Initialize(r, bd, aa, g);
	Display::FillScreen(Display::LibColor::GREY_SMOOTH);
	Display::SetColors(local.Foreground, local.Background);

	this->OnCreate();

	Display::SetVI_Intterupt(0x200);
	Controller::Write();

	while (bRunning) {
		this->FrameUpdate();

		/*clear screen on vertical retrace*/
		SetVI_IntCallback([&]() {
			if (bClearScreen) RDP::FillScreen(GREY_SMOOTH);
		});

		Controller::Read();
		if (cpad_data->A) { this->KeyAPressed(); }
		if (cpad_data->B) { this->KeyBPressed(); }
		if (cpad_data->Z) { this->KeyZPressed(); }
		if (cpad_data->start) { this->KeyStartPressed(); }
		if (cpad_data->up) { this->KeyDUpPressed(); }
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

void Frame::FrameUpdate() {
}
void Frame::OnCreate() {
}
void Frame::KeyAPressed() {
}
void Frame::KeyBPressed() {
}
void Frame::KeyZPressed() {
}
void Frame::KeyDUpPressed() {
}
void Frame::KeyDDownPressed() {
}
void Frame::KeyDLeftPressed() {
}
void Frame::KeyDRightPressed() {
}
void Frame::KeyStartPressed() {
}
void Frame::KeyJoyXPressed(int) {
}
void Frame::KeyJoyYPressed(int) {
}

u32 Frame::ScreenWidth() {
	return r.width;
}

u32 Frame::ScreenHeight() {
	return r.height;
}

} // namespace LibN64