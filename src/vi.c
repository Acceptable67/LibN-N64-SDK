/*handle everything visual*/
#include <math.h>
#include <stdint.h>
#include <libn.h>
#include <string.h>

CreateGlobalRegister(VI, VI_REG);

u32 buffer_list[2] = 
{ 
	FRAMEBUFFER_ADDR, 
	FRAMEBUFFER_ADDR + 0x01000000
};

static TextColor localColor;
Resolution global_res = {0, 0};
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


void Display_SwapBuffers() {
	bool buffer_condition = Display_GetActiveBuffer() == Display_GetBuffer(DISPLAY);
	Display_SetActiveBuffer((buffer_condition) ? BACKUP : DISPLAY);
	Display_SetDrawingBuffer((buffer_condition) ? DISPLAY : BACKUP);
}

s32 Display_FrameWidth() {
	return global_res.width;
}

s32 Display_FrameHeight() {
	return global_res.height;
}

// clang-format off
void Display_Initialize(Resolution res, Bitdepth bd, AntiAliasing aa, Gamma gamma) {
	localColor.Foreground = WHITE;
	localColor.Background = BLACK;

	global_res.width	      = res.width;
	global_res.height	      = res.height;

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
}

void Display_DrawPixel(s32 x, s32 y, const u32 color) {
	*(Display_GetBuffer(DISPLAY) + (y * global_res.width + x)) = color;
}

void Display_DrawRect(const u16 bx, const u16 by, const u16 xd, const u16 yd, const u32 color, bool bFilled) {
	if(bFilled) 
	{
		for (u32 i = 0; i < xd; i++) {
			for (u32 d = 0; d < yd; d++) {
				Display_DrawPixel(bx + i, by + d, color);
			}
		}
	} else {
		Display_DrawLine(bx, by, bx + xd, by, color); 
		Display_DrawLine(bx + xd, by, bx + xd, by + yd, color);
		Display_DrawLine(bx + xd, by + yd, bx, by + yd, color); 
		Display_DrawLine(bx, by + yd, bx, by, color);
	}
	
}

void Display_DrawCircle(const u16 cx, const u16 cy, u32 scale, const u32 color, bool isFilled, float cStepSize) {
	float PI = 3.1415f;
	if (isFilled) {
		for (float scaler = 0; scaler <= scale; scaler += 0.3) {
			for (float angles = 0; angles < 25 * scaler;
			     angles += cStepSize) {
				Display_DrawPixel((u32)(cx + cosf(angles) * scaler),
				(u32)(cy + sinf(angles) * scaler), color);
			}
		}
	} else {
		for (float angles = 0; angles < 25 * scale;
		     angles += cStepSize) {
			Display_DrawPixel((u32)(cx + cosf(angles) * PI * scale), (u32)(cy + sinf(angles) * PI * scale),color);
		}
	}
}
// clang-format on

void Display_FillScreen(u32 color) {
	Display_DrawRect(0,0, global_res.width, global_res.height, color, true);
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
	for (const char* c = text; *c != '\0'; ++c) {
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

void Display_DrawTri(u32 pos1x, u32 pos1y, u32 pos2x, u32 pos2y, u32 pos3x, u32 pos3y, const u32 color) {
	Display_DrawLine(pos1x, pos1y, pos2x, pos2y, color);
	Display_DrawLine(pos2x, pos2y, pos3x, pos3y, color);
	Display_DrawLine(pos3x, pos3y, pos1x, pos1y, color);
}

/*void Display_DrawSprite(LibPos pos, LibSprite& spr)
{
	s8 local = spr.Data();
}*/

void Display_SetColors(const u32 foreground, const u32 background) {
	localColor.Foreground = foreground;
	localColor.Background = background;
}

TextColor Display_GetColors() {
	return localColor;
}
