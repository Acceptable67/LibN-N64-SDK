/*handle everything visual*/
#include <math.h>
#include <stdint.h>
#include <libn/font.h>
#include <libn/regs.h>
#include <libn/sprite.h>
#include <libn/vi_display.h>
#include <string.h>

CreateGlobalRegister(VI, VI_REG);
CreateGlobalRegister(MI, MI_REG);
CreateGlobalRegister(DP, DP_REG);

u32 buffer_list[2] = 
{ 
	FRAMEBUFFER_ADDR, 
	FRAMEBUFFER_ADDR + 0x01000000
};

Resolution global_res = {0, 0};
static TextColor localColor;
static s32 *active_buffer;
static s32 *active_draw_buffer;

s32* Display_GetBuffer(s32 id) {
	return (s32*)(buffer_list[id]);
} 

s32* Display_GetActiveBuffer() {
	return active_buffer;
}

void Display_SetActiveBuffer(s32 x) {
	active_buffer = (s32*)(buffer_list[x]);
}

void Display_SetDrawingBuffer(s32 x) {
	active_draw_buffer = (s32 *)(buffer_list[x]);
}

void Display_SetVI_IntCallback(void (*func)(void)) {
	if ((MI_REG->intr & MI_REG->mask) & 0x08) {
		func();
		VI_REG->currentvl = VI_REG->currentvl;
	}
}

void Display_SetVI_Intterupt(u32 line) {
	MI_REG->mask = 0x0080;
	VI_REG->vint = line;
}

void Display_SwapBuffers() {
	if (Display_GetActiveBuffer() == Display_GetBuffer(DISPLAY)) {
		Display_SetActiveBuffer(BACKUP);
		Display_SetDrawingBuffer(DISPLAY);
	} else {
		Display_SetActiveBuffer(DISPLAY);
		Display_SetDrawingBuffer(BACKUP);
	}
}

// clang-format off
void Display_Initialize(Resolution res, Bitdepth bd, AntiAliasing aa, Gamma gamma) {
	localColor.Foreground = WHITE;
	localColor.Background = BLACK;

	res.width	      = res.width;
	res.height	      = res.height;

	active_buffer	      = Display_GetBuffer(DISPLAY);
	active_draw_buffer    = Display_GetBuffer(BACKUP);

	VI_REG->origin	      = (u32)(active_buffer);

	VI_REG->status	      = (bd) | (aa) |(gamma);
	VI_REG->width	      = res.width;
	VI_REG->vint	      = 0x200;
	VI_REG->currentvl     = 0x0;
	VI_REG->vtiming	      = 0x3E52239;
	VI_REG->vsync	      = 0x20D;
	VI_REG->hsync	      = 0xC15;
	VI_REG->hsyncleap     = 0xC150C15;
	VI_REG->hvideo	      = 0x6C02EC;
	VI_REG->vvideo	      = 0x2501FF;
	VI_REG->vburst	      = 0xE0204;
	VI_REG->xscale	      = (0x100 * res.width) / 160;
	VI_REG->yscale	      = (0x100 * res.height) / 60;

	*(u32 *)(((PIF_RAM) - sizeof(s32)) + 0x3c) = 0x8;

	global_res						   = res;
}

void Display_DrawPixel(s32 x, s32 y, const u32 color) {
	*(Display_GetBuffer(DISPLAY) + (y * global_res.width + x)) = color;
}

void Display_DrawRect(LibPos pos, const u16 xd, const u16 yd, const u32 color, bool bFilled) {
	if(bFilled) 
	{
		for (u32 i = 0; i < xd; i++) {
			for (u32 d = 0; d < yd; d++) {
				Display_DrawPixel(pos.x + i, pos.y + d, color);
			}
		}
	} else {
		Display_DrawLine(pos.x, pos.y, pos.x + xd, pos.y, color); 
		Display_DrawLine(pos.x + xd, pos.y, pos.x+xd, pos.y + yd, color);
		Display_DrawLine(pos.x+xd, pos.y + yd, pos.x, pos.y + yd, color); 
		Display_DrawLine(pos.x, pos.y + yd, pos.x, pos.y, color);
	}
	
}

void Display_DrawCircle(LibPos pos, u32 scale, const u32 color, bool isFilled, float cStepSize) {
	if (isFilled) {
		for (float scaler = 0; scaler <= scale; scaler += 0.3) {
			for (float angles = 0; angles < 25 * scaler;
			     angles += cStepSize) {
				Display_DrawPixel((u32)(pos.x + cosf(angles) * 3.1415f * scaler),
				(u32)(pos.y + sinf(angles) * 3.1415f * scaler), color);
			}
		}
	} else {
		for (float angles = 0; angles < 25 * scale;
		     angles += cStepSize) {
			Display_DrawPixel((u32)(pos.x + cosf(angles) * 3.1415f * scale), (u32)(pos.y + sinf(angles) * 3.1415f * scale),color);
		}
	}
}
// clang-format on

void Display_FillScreen(u32 color) {
	LibPos pos;
	pos.x = 0;
	pos.y = 0;
	Display_DrawRect(pos, global_res.width, global_res.height, color, true);
}

/*8x8 taken from LibDragon*/
void Display_DrawCharacter(const LibPos pos, const u8 ch) {

	u32 trans = ((localColor.Background & 0xff) == 0) ? 1 : 0;
	for (u32 row = 0; row < font_width; row++) {
		unsigned char c = __font_data[(ch * font_width) + row];
		for (u32 col = 0; col < font_height; col++) {
			if (trans) {
				if (c & 0x80) { 
					Display_DrawPixel(pos.x + col, pos.y + row , localColor.Foreground); 
				}
			} else {
				Display_DrawPixel(pos.x + col, pos.y + row ,
					  (c & 0x80) ? localColor.Foreground : localColor.Background);
			}

			c <<= 1;
		}
	}
}

void Display_DrawText(s32 x, s32 y, const char* text) {
	LibPos toffset = {0, 0};
	for (const char* c = text; *c != '\0'; c++) {
		if ((x + toffset.x) >= global_res.width || *c == '\n') {
			y += font_height;
			toffset.x = 0;
		} else if (*c == '\t' || *c == '\r') {
			x += font_width * 4;
		} else {
			LibPos tmp;
			tmp.x = x + toffset.x;
			tmp.y = y + toffset.y;
			Display_DrawCharacter(tmp, *c);
			toffset.x += font_width;
		}
	}
}

/*From libdragon*/
void Display_DrawLine(s32 x1, s32 y1, s32 x2, s32 y2, const u32 color) {
	s32 dy = y2 - y1;
	s32 dx = x2 - x1;
	s32 sx, sy;

	if (dy < 0) {
		dy = -dy;
		sy = -1;
	} else
		sy = 1;

	if (dx < 0) {
		dx = -dx;
		sx = -1;
	} else
		sx = 1;

	dy <<= 1;
	dx <<= 1;

	Display_DrawPixel(x1, y1, color);
	if (dx > dy) {
		s32 frac = dy - (dx >> 1);
		while (x1 != x2) {
			if (frac >= 0) {
				y1 += sy;
				frac -= dx;
			}
			x1 += sx;
			frac += dy;
			Display_DrawPixel(x1, y1, color);
		}
	} else {
		s32 frac = dx - (dy >> 1);
		while (y1 != y2) {
			if (frac >= 0) {
				x1 += sx;
				frac -= dy;
			}
			y1 += sy;
			frac += dx;
			Display_DrawPixel(x1, y1, color);
		}
	}
}

//void Display_DrawTri(LibPos pos1, LibPos pos2, LibPos pos3, const u32 color) {
//	DrawLine({pos1.x, pos1.y}, {pos2.x, pos2.y}, color);
//	DrawLine({pos2.x, pos2.y}, {pos3.x, pos3.y}, color);
//	DrawLine({pos3.x, pos3.y}, {pos1.x, pos1.y}, color);
//}

/*void Display_DrawSprite(LibPos pos, LibSprite& spr)
{
	s8 local = spr.Data();
}*/

void Display_SetColors(const u32 foreground, const u32 background) {
	localColor.Foreground = foreground;
	localColor.Background = background;
}

TextColor GetColors() {
	return localColor;
}

u32 cBuffer[48];

u32 spot;
void RDP_AddCommand(u32 command) {
	if (spot >= 48) { 
		spot = 0; 
	}
	cBuffer[spot] = command;

	spot++;
}

void RDP_Send() {

	while (DP_REG->status & 0x600) {};

	DP_REG->status = 0x15; // 0b00010101

	while (DP_REG->status & 0x600) {}

	DP_REG->cmd_start = ((u32)(cBuffer) | 0xA0000000);
	DP_REG->cmd_end	  = ((u32)(cBuffer + (spot * sizeof(s32))) | 0xA0000000);
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
	RDP_SetClipping(0, 0, global_res.width, global_res.height);
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
	RDP_AddCommand((DL_ATTACH_FB | 0x00180000 | (global_res.width - 1)));
	RDP_AddCommand((u32)(active_buffer));
}

void RDP_Sync() {
	RDP_AddCommand(DL_SYNC_PIPE); // PIPE
	RDP_AddCommand(DL_NULL_CMD);
}

void RDP_DrawRectangleSetup(u32 tx, u32 ty, u32 bx, u32 by, u32 color) {

	RDP_Attach();
	RDP_SetDefaultClipping();
	RDP_Sync();
	RDP_EnableBlend();
	RDP_Sync();
	RDP_SetBlendColor(color);
	RDP_Sync();
	RDP_DrawRectangle(tx, ty, bx, by);
	RDP_Send();
	RDP_Sync();
}

void RDP_FillScreen(u32 color) {
	RDP_DrawRectangleSetup(0, 0, global_res.width, global_res.height, color);
}
