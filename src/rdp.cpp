
#include <libn.hpp>
#include <vector>
#include <libn/interrupts.hpp>

CreateGlobalRegister(MI, MI_REG);
CreateGlobalRegister(DP, DP_REG);

namespace RDP {

#define CBUF_SIZE 4096

s32 cBuffer[CBUF_SIZE * 2];
u32 cbuf_pos = 0;

bool spot_flag = false;

void AddCommand(u32 command) {
	if(cbuf_pos >= CBUF_SIZE / 2) { 
		return;  
	};

	cBuffer[cbuf_pos] = command;
	++cbuf_pos;
}

void Send() {

	while (DP_REG->status & 0x600) {};

	DP_REG->status = 0x15; // 0b00010101

	while (DP_REG->status & 0x600) {}

	DP_REG->cmd_start = reinterpret_cast<u32>(UncachedAddr(cBuffer));
	DP_REG->cmd_end	  = reinterpret_cast<u32>(UncachedAddr(cBuffer) + (sizeof(u32) * cbuf_pos));
	
}

void DebugAddr() {
	
}

void SendDisplayList() {
	Send();
}

void SetOtherModes() {
	AddCommand(0x2F102800);
	AddCommand(0x00000000);
}

void SetClipping(u32 tx, u32 ty, u32 bx, u32 by) {
	AddCommand((DL_SET_CLIP_AREA | (tx << 14) | (ty << 2)));
	AddCommand(((bx << 14) | (by << 2)));
}

void SetDefaultClipping(void) {
	SetClipping(0, 0, LibN64::Display::global_res.width,  LibN64::Display::global_res.height);
}

void EnablePrimitive(void) {
	AddCommand(DL_ENABLE_PRIM);
	AddCommand(DL_ENABLE_PRIM_2);
}

/*void LoadTexture(LibSprite *spr, u32 tslot) {
	auto RoundToPower = [](u32 number) {
		if (number <= 4) { return 4; }
		if (number <= 8) { return 8; }
		if (number <= 16) { return 16; }
		if (number <= 32) { return 32; }
		if (number <= 64) { return 64; }
		if (number <= 128) { return 128; }
		return 256;
	};

	auto Log2 = [](u32 number) {
		switch (number) {
			case 4: return 2;
			case 8: return 3;
			case 16: return 4;
			case 32: return 5;
			case 64: return 6;
			case 128: return 7;
			default: return 8;
		}
	};

	AddCommand(0xFD000000 | 0x180000 | (spr->Width() - 1));
	AddCommand((u32)(spr->Data()));

    auto tw = (spr->Width() - 1) - 0 + 1;
    auto th = (spr->Height() - 1) - 0 + 1;

	auto _rw   = RoundToPower(tw);
	auto _rh   = RoundToPower(th);
	auto wbits = Log2(_rw);
	auto hbits = Log2(_rh);

	AddCommand(0xF5000000 | 0x180000 |
		(((((_rw / 8) + ((_rw % 8) ? 1 : 0)) * 3) & 0x1FF) << 9) |
		((tslot / 8) & 0x1FF));
	AddCommand(((tslot & 0x7) << 24) | 0 | (hbits << 14) | (wbits << 4));
	Send();
}
*/

void Init() {
	Interrupts::Toggle(Interrupts::Type::DP, true);
}

void Close() {
	Interrupts::Toggle(Interrupts::Type::DP, false);
}

void EnableBlend() {
	AddCommand(DL_ENABLE_BLEND);
	AddCommand(DL_ENABLE_BLEND_2);
}

void SetPrimitiveColor(u32 color) {
	AddCommand(DL_SET_PRIM_COL);
	AddCommand(color);
}

void SetBlendColor(auto color) {
	AddCommand(DL_SET_BLEND_COL);
	AddCommand(color);
}

void DrawRectangle(u32 tx, u32 ty, u32 bx, u32 by) {
	AddCommand((DL_DRAW_RECT | (bx << 14) | (by << 2)));
	AddCommand((tx << 14) | (ty << 2));
}

void Attach() {
	AddCommand((DL_ATTACH_FB | 0x00180000 | ( LibN64::Display::global_res.width - 1)));
	AddCommand(reinterpret_cast<u32>(Display::GetActiveBuffer()));
}

void Sync() {
	AddCommand(DL_SYNC_PIPE); // PIPE
	AddCommand(DL_NULL_CMD);
}

void DrawRectangleSetup(u32 tx, u32 ty, u32 bx, u32 by, u32 color) {
	RDP::Attach();
	RDP::SetDefaultClipping();
	RDP::Sync();
	RDP::EnableBlend();
	RDP::Sync();
	RDP::SetBlendColor(color);
	RDP::Sync();
	RDP::DrawRectangle(tx, ty, bx, by);
	RDP::Sync();
	RDP::DrawRectangle(tx, ty, bx, by);
	RDP::Send();

}

void FillScreen(u32 color) {
	DrawRectangleSetup(0, 0,  LibN64::Display::global_res.width,  LibN64::Display::global_res.height, color);
}

}
