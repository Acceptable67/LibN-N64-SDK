#ifndef LIBN_RDP_H
#define LIBN_RDP_H

#define SPOT_SCALED  spot * sizeof(s32)
#define RDP_BUF_SIZE 480
#define RDP_WAIT() while (DP_REG->status & 0x600) {}

enum RDP_Command {
	DL_ENABLE_PRIM	  = 0xEFB000FF,
	DL_ENABLE_PRIM_2  = 0x00004000,
	DL_ENABLE_BLEND_2 = 0x80000000,
	DL_ENABLE_BLEND	  = 0xEF0000FF,
	DL_SET_PRIM_COL	  = 0xF7000000,
	DL_SET_BLEND_COL  = 0xF9000000,
	DL_DRAW_RECT	  = 0xF6000000,
	DL_ATTACH_FB	  = 0x3F000000,
	DL_SET_CLIP_AREA  = 0xED000000,
	DL_SYNC_PIPE	  = 0xE7000000,
	DL_NULL_CMD	  = 0x00000000
};

void RDP_Debug();
void RDP_SendDisplayList();
void RDP_AddCommand(u32);
void RDP_Send(); 
void RDP_Sync();
void RDP_Attach();
void RDP_EnableBlend();
void RDP_EnablePrimitive();
void RDP_SetDefaultClipping();
void RDP_SetClipping(u32, u32, u32, u32);
void RDP_SetBlendColor(const u32);
void RDP_SetPrimitiveColor(const u32);
void RDP_DrawRectangle(u32 point1, u32 point2, u32 width, u32 heiht);
void RDP_DrawRectangleSetup(u32 tx, u32 ty, u32 bx, u32 by, u32 color);
void RDP_Close();
void RDP_FillScreen(u32 color);
void RDP_Init();

#endif