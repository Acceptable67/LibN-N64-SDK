/*handle everything visual*/
#include <cmath>
#include <cstdint>
#include <functional>
#include <libn/font.hpp>
#include <libn/regs.hpp>
#include <libn/sprite.hpp>
#include <libn/vi_display.hpp>
#include <string>
#include <type_traits>
#include <utility>

CreateGlobalRegister(VI, VI_REG);
CreateGlobalRegister(MI, MI_REG);
CreateGlobalRegister(DP, DP_REG);

namespace LibN64::Display {

std::array<u32, 2> buffer_list = {FRAMEBUFFER_ADDR, FRAMEBUFFER_ADDR + 0x01000000};

Resolution global_res	       = {0, 0};
static TextColor localColor;
static int *active_buffer;
static int *active_draw_buffer;

s32 *GetBuffer(Buffer id) {
	return reinterpret_cast<s32 *>(buffer_list[id]);
}

s32 *GetActiveBuffer() {
	return active_buffer;
}

void SetActiveBuffer(Buffer x) {
	active_buffer = reinterpret_cast<s32 *>(buffer_list[x]);
}

void SetDrawingBuffer(Buffer x) {
	active_draw_buffer = reinterpret_cast<s32 *>(buffer_list[x]);
}

void SetVI_IntCallback(std::function<void()> func) {
	if ((MI_REG->intr & MI_REG->mask) & 0x08) {
		func();
		VI_REG->currentvl = VI_REG->currentvl;
	}
}

void SetVI_Intterupt(u32 line) {
	MI_REG->mask = 0x0080;
	VI_REG->vint = line;
}

void SwapBuffers() {
	if (GetActiveBuffer() == GetBuffer(Buffer::DISPLAY)) {
		SetActiveBuffer(BACKUP);
		SetDrawingBuffer(DISPLAY);
	} else {
		SetActiveBuffer(DISPLAY);
		SetDrawingBuffer(BACKUP);
	}
}

// clang-format off
void Initialize(Resolution res, Bitdepth bd, AntiAliasing aa, Gamma gamma) {
	localColor.Foreground = LibColor::WHITE;
	localColor.Background = LibColor::BLACK;

	res.width	      = res.width;
	res.height	      = res.height;

	active_buffer	      = GetBuffer(Buffer::DISPLAY);
	active_draw_buffer    = GetBuffer(Buffer::BACKUP);

	VI_REG->origin	      = reinterpret_cast<u32>(active_buffer);

	VI_REG->status	      = static_cast<u32>(bd) | static_cast<u32>(aa) | static_cast<u32>(gamma);
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

	*reinterpret_cast<u32 *>(((PIF_RAM) - sizeof(int)) + 0x3c) = 0x8;

	global_res						   = res;
}

void DrawPixel(LibPos pos, const u32 color) {
	*(GetBuffer(DISPLAY) + (pos.y * global_res.width + pos.x)) = color;
}

void DrawRect(LibPos pos, const u16 xd, const u16 yd, const u32 color, bool bFilled) {
	if(bFilled) 
	{
		for (u32 i = 0; i < xd; i++) {
			for (u32 d = 0; d < yd; d++) {
				DrawPixel({pos.x + i, pos.y + d}, color);
			}
		}
	} else {
		DrawLine(pos, {pos.x + xd, pos.y}, color); 
		DrawLine({pos.x + xd, pos.y}, {pos.x+xd, pos.y + yd}, color);
		DrawLine({pos.x+xd, pos.y+yd}, {pos.x, pos.y + yd}, color); 
		DrawLine({pos.x, pos.y + yd}, pos, color);
	}
	
}

void DrawCircle(LibPos pos, u32 scale, const u32 color, bool isFilled, float cStepSize) {
	if (isFilled) {
		for (float scaler = 0; scaler <= scale; scaler += 0.3) {
			for (float angles = 0; angles < 25 * scaler;
			     angles += cStepSize) {
				DrawPixel({	static_cast<u32>(pos.x + cosf(angles) * 3.1415f * scaler),
					 	  	static_cast<u32>(pos.y + sinf(angles) * 3.1415f * scaler)},
				    		color);
			}
		}
	} else {
		for (float angles = 0; angles < 25 * scale;
		     angles += cStepSize) {
			DrawPixel({	static_cast<u32>( pos.x + cosf(angles) * 3.1415f * scale),
						static_cast<u32>( pos.y + sinf(angles) * 3.1415f * scale)},
			    		color);
		}
	}
}
// clang-format on

void FillScreen(u32 color) {
	DrawRect({0, 0}, global_res.width, global_res.height, color);
}

/*8x8 taken from LibDragon*/
void DrawCharacter(const LibPos pos, const u8 ch) {
	u32 trans = ((localColor.Background & 0xff) == 0) ? 1 : 0;
	for (u32 row = 0; row < font_width; row++) {
		unsigned char c = __font_data[(ch * font_width) + row];
		for (u32 col = 0; col < font_height; col++) {
			if (trans) {
				if (c & 0x80) { DrawPixel({pos.x + col, pos.y + row}, localColor.Foreground); }
			} else {
				DrawPixel({pos.x + col, pos.y + row},
					  (c & 0x80) ? localColor.Foreground : localColor.Background);
			}

			c <<= 1;
		}
	}
}

void DrawText(LibPos pos, const std::string_view text) {
	LibPos toffset = {0, 0};
	for (auto &c : text) {
		if ((pos.x + toffset.x) >= global_res.width || c == '\n') {
			pos.y += font_height;
			toffset.x = 0;
		} else if (c == '\t' || c == '\r') {
			pos.x += font_width * 4;
		} else {
			DrawCharacter({pos.x + toffset.x, pos.y + toffset.y}, c);
			toffset.x += font_width;
		}
	}
}

/*From libdragon*/
void DrawLine(LibPos a, LibPos b, const u32 color) {
	int dy = b.y - a.y;
	int dx = b.x - a.x;
	int sx, sy;

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

	DrawPixel({a.x, a.y}, color);
	if (dx > dy) {
		int frac = dy - (dx >> 1);
		while (a.x != b.x) {
			if (frac >= 0) {
				a.y += sy;
				frac -= dx;
			}
			a.x += sx;
			frac += dy;
			DrawPixel({a.x, a.y}, color);
		}
	} else {
		int frac = dx - (dy >> 1);
		while (a.y != b.y) {
			if (frac >= 0) {
				a.x += sx;
				frac -= dy;
			}
			a.y += sy;
			frac += dx;
			DrawPixel({a.x, a.y}, color);
		}
	}
}

void DrawTri(LibPos pos1, LibPos pos2, LibPos pos3, const u32 color) {
	DrawLine({pos1.x, pos1.y}, {pos2.x, pos2.y}, color);
	DrawLine({pos2.x, pos2.y}, {pos3.x, pos3.y}, color);
	DrawLine({pos3.x, pos3.y}, {pos1.x, pos1.y}, color);
}

/*void DrawSprite(LibPos pos, LibSprite& spr)
{
	s8 local = spr.Data();
}*/

void SetColors(const u32 foreground, const u32 background) {
	localColor.Foreground = foreground;
	localColor.Background = background;
}

TextColor GetColors() {
	return localColor;
} // namespace RDP
} // namespace LibN64::Display
  // namespace LibN64::Display
