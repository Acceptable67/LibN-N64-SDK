

#include <libn.h>

/*I had my own method but LibDragon's method is the best way of dealing with this
  so I added my own implementation.*/

CreateGlobalRegister(MI, MI_REG);
CreateGlobalRegister(DP, DP_REG);

s32 cBuffer[RDP_BUF_SIZE];
u32 spot, start;

void RDP_AddCommand(u32 command) {
	if(((SPOT_SCALED - start)) >= RDP_BUF_SIZE) { 
		return; 
	}
	cBuffer[spot] = command;
	++spot;
}

void RDP_Send() {
	if((SPOT_SCALED - start) == 0) { return; }

	RDP_WAIT();
	DP_REG->status = 0b00010101; //0x15
	RDP_WAIT();
	 
	DP_REG->cmd_start = (u32)(UncachedAddr(cBuffer)) + start;
	DP_REG->cmd_end	  = (u32)(UncachedAddr(cBuffer)) + (SPOT_SCALED);
	
	if ((spot) >= RDP_BUF_SIZE) { 
		spot  = 0; 
		start = 0;
	} else {
		start = spot;    
	}
   
} 

void RDP_Debug() {
	LibPrintf("cBuffer %08X\ncBuffer+spot: %08X\nspot %d", 
	(u32)UncachedAddr((u32)(cBuffer)) + start, 
	(u32)UncachedAddr((u32)(cBuffer)) + (spot * sizeof(s32)), 
	spot);
}

void RDP_SendDisplayList() {
	RDP_Send();
}

void RDP_SetOtherModes() {
	RDP_AddCommand(0x2F102800);
	RDP_AddCommand(0x00000000);
}

void RDP_SetClipping(u32 tx, u32 ty, u32 bx, u32 by) {
	RDP_AddCommand((DL_SET_CLIP_AREA | (tx << 14) | (ty << 2)));
	RDP_AddCommand(((bx << 14) | (by << 2)));
}

void RDP_SetDefaultClipping(void) {
	RDP_SetClipping(0, 0, Display_FrameWidth(),Display_FrameHeight());
}

void RDP_EnablePrimitive(void) {
	RDP_AddCommand(DL_ENABLE_PRIM);
	RDP_AddCommand(DL_ENABLE_PRIM_2);
}

/*void RDP_LoadTexture(LibSprite *spr, u32 tslot) {
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

void RDP_Init() {
	MI_REG->mask = 0x0800;
}

void RDP_Close() {
	MI_REG->mask = 0x0400;
}

void RDP_EnableBlend() {
	RDP_AddCommand(DL_ENABLE_BLEND);
	RDP_AddCommand(DL_ENABLE_BLEND_2);
}

void RDP_SetPrimitiveColor(u32 color) {
	RDP_AddCommand(DL_SET_PRIM_COL);
	RDP_AddCommand(color);
}

void RDP_SetBlendColor(u32 color) {
	RDP_AddCommand(DL_SET_BLEND_COL);
	RDP_AddCommand(color);
}

void RDP_DrawRectangle(u32 tx, u32 ty, u32 bx, u32 by) {
	RDP_AddCommand((DL_DRAW_RECT | (bx << 14) | (by << 2)));
	RDP_AddCommand((tx << 14) | (ty << 2));
}

void RDP_Attach() {
	RDP_AddCommand((DL_ATTACH_FB | 0x00180000 | (Display_FrameWidth() - 1)));
	RDP_AddCommand((u32)(Display_GetActiveBuffer()));
}

void RDP_Sync() {
	RDP_AddCommand(DL_SYNC_PIPE); // PIPE
	RDP_AddCommand(DL_NULL_CMD);
}

void RDP_DrawRectangleSetup(u32 tx, u32 ty, u32 bx, u32 by, u32 color) {
	RDP_Init();
	RDP_Attach();
	RDP_SetDefaultClipping();
	RDP_Sync();
	RDP_EnablePrimitive();
	RDP_EnableBlend();
	RDP_Sync();
	RDP_SetPrimitiveColor(color);
	RDP_SetBlendColor(color);
	RDP_Sync();
	RDP_DrawRectangle(tx, ty, bx, by);
	RDP_Send();
	RDP_Close();
}

void RDP_FillScreen(u32 color) {
	RDP_DrawRectangleSetup(0, 0, Display_FrameWidth(), Display_FrameHeight(), color);
}
