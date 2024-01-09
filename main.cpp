

#pragma GCC diagnostic ignored "-Wnarrowing"

#include <libn.hpp>
#include "inc/main.hpp"

#define INC_AMT 0.001f
#define INC_DEC(A, B) 		\
			A -= INC_AMT; 	\
			B += INC_AMT; 	\

using namespace LibN64;
using namespace LibN64::Display;

class N64Game : public Frame 
{
	public:
		N64Game(const Resolution res, const Bitdepth bitdepth, const AntiAliasing aa, const Gamma gamma, const Display::TextColor textcolor) : Frame(res, bitdepth, aa, gamma, textcolor) {}

		virtual void FrameUpdate();
		virtual void OnCreate();
		virtual void KeyAPressed();
		virtual void KeyDDownPressed();
		virtual void KeyDUpPressed();
		virtual void KeyDLeftPressed();
		virtual void KeyDRightPressed();
		virtual void KeyCDownPressed();
		virtual void KeyCUpPressed();
		virtual void KeyCLeftPressed();
		virtual void KeyCRightPressed();
		virtual void KeyJoyXPressed();
		virtual void KeyJoyYPressed();

	private:
		Timer::LibTimer timer 	= Timer::LibTimer(Timer::TimerType::CONTINUOUS_CALL, 0.115f);
		Lib2DVec<f32> tri_pos 	= {40.0f,40.0f};
		Lib2DVec<f32> rect_pos 	= {120.0f, 40.0f};
};

void N64Game::OnCreate()
{
	this->timer.Start();
}

void N64Game::FrameUpdate()
{
	RDP::FillScreen(LibColor::BLACK);
	printf("Tri:  X %.2f Y %.2f", this->tri_pos.x, this->tri_pos.y);
	printf("Rect: X %.2f Y %.2f", this->rect_pos.x, this->rect_pos.y);
	printf("Timer Secs Passed: %f", this->timer.GetSecondsPassed()*2);
	DrawTri({this->tri_pos.x, 		this->tri_pos.y}, 
			{this->tri_pos.x + 40, 	this->tri_pos.y}, 
			{this->tri_pos.x + 5, 	this->tri_pos.y + 40}, 
			LibColor::CYAN);
	DrawRect({static_cast<u32>(this->rect_pos.x), 
			  static_cast<u32>(this->rect_pos.y)},  
			  20, 60, LibColor::PURPLE, false);

	timer.Update([&]()
	{
		DrawRect({rand() % 320, rand() % 240}, rand() % 20 + 10, rand() % 20 + 10, RED, true);
	});

	this->timer.Fetch();
}

void N64Game::KeyAPressed()
{
	printf("%Controller Data X %i Y %i", this->JoyDataX(), (u32)this->JoyDataY());
}

void N64Game::KeyDUpPressed() {
	INC_DEC(tri_pos.y, rect_pos.x);
}

void N64Game::KeyDDownPressed() {
	INC_DEC(rect_pos.x, tri_pos.y);
}

void N64Game::KeyDLeftPressed() {
	INC_DEC(tri_pos.x, rect_pos.y);
}

void N64Game::KeyDRightPressed() {
	INC_DEC(rect_pos.y, tri_pos.x);
}

void N64Game::KeyCRightPressed() {
	printf("C-Right pressed");
}

void N64Game::KeyCUpPressed() {	
	printf("C-Up pressed");

}
void N64Game::KeyCDownPressed() {	
	printf("C-Down pressed");
}

void N64Game::KeyCLeftPressed() {	
	printf("C-Left pressed");
}

void N64Game::KeyJoyXPressed() {
	auto Left 	= [&]() 	{ printf("JoyX\tLeft"); };
	auto Right 	= [&]() 	{ printf("JoyX\tRight"); };
	switch(this->JoyDataX())
	{
		case Controller::JoyLeft: 	{ Left(); 	} break;
		case Controller::JoyRight: 	{ Right(); 	} break;
		default: break;
	}
}

void N64Game::KeyJoyYPressed() {
	auto Up 	= [&]() 	{ printf("JoyY\tUp"); };
	auto Down 	= [&]() 	{ printf("JoyY\tDown"); };
	switch(this->JoyDataY())
	{
		case Controller::JoyUp: 	{ Up(); 	} break;
		case Controller::JoyDown: 	{ Down(); 	} break;
		default: break;
	}
}

extern "C" int main()
{ 
	N64Game fr = N64Game(RESOLUTION_320x240, BD32BPP, DIVOT_ENABLE, GAMMA_ENABLE, {LibColor::WHITE, LibColor::BLACK});
	fr.Begin();
}
