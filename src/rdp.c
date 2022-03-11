

#include <libn.h>

CreateGlobalRegister(DP, DP_REG);

s32 cBuffer[RDP_BUF_SIZE*2];
u32 spot = 0;

void RDP_AddCommand(u32 command) {
	cBuffer[spot] = command;
	++spot;

	if(spot >= RDP_BUF_SIZE) { 
		memset(cBuffer, 0, sizeof(cBuffer));
		spot = 0;
		return; 
	};
}

void RDP_Send() {
	RDP_WAIT();
	DP_REG->status = 0b00010101; //0x15
	RDP_WAIT();
	 
	DP_REG->cmd_start = (u32)(UncachedAddr(cBuffer));
	DP_REG->cmd_end	  = (u32)(UncachedAddr(cBuffer)) + SPOT_SCALED;
   
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

void RDP_Debug()
{
	LibPrintf("Spot %u SCALED %08X", spot, SPOT_SCALED);
}

void RDP_Init() {
	Interrupts_Toggle(INT_DP, true);
}

void RDP_Close() {
	Interrupts_Toggle(INT_DP, false);
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
	RDP_Sync();
	RDP_DrawRectangle(tx, ty, bx, by);
	RDP_Send();
	RDP_Close();
}

void RDP_FillScreen(u32 color) {
	RDP_DrawRectangleSetup(0, 0, Display_FrameWidth(), Display_FrameHeight(), color);
}
