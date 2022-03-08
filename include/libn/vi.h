#ifndef LIBN_DISPLAY_H
#define LIBN_DISPLAY_H

/*handle everything visual*/

#include <stdarg.h>
#include <string.h>
#include <malloc.h>
#include <stdbool.h>
#include <libn/types.h>
#include <libn/sprite.h>

#define NUMBER_BUFFERS 2

typedef struct{
	u32 width;
	u32 height;
} Resolution;

typedef struct {
	u32 Foreground;
	u32 Background;
} TextColor;

typedef enum { 
	BD16BPP = 0x2, 
	BD32BPP = 0x3 
} Bitdepth;

typedef enum {
	GAMMA_OFF,
	GAMMA_DITHER = 0x4,
	GAMMA_ENABLE = 0x8,
} Gamma;

typedef enum {
	DIVOT_ENABLE	= 0x10,
	AA_RESAMP_FETCH = 0x100,
	AA_RESAMP_ONLY	= 0x200,
	AA_REPLICATE	= 0x300
} AntiAliasing;

typedef enum { DISPLAY = 0, BACKUP, EXTRA } Buffer;

typedef enum {
	RED	      	  = 0xFF0000FF,
	GREEN	      = 0x00FF00FF,
	WHITE	      = 0xFFFFFFFF,
	BLACK	      = 0x00000000,
	BLUE	      = 0x0000FFFF,
	NAVY_BLUE     = 0x111880FF, 
	DARK_RED      = 0x4B1300FF,
	DEEP_DARK_RED = 0x1F0100FF,
	SKY_BLUE      = 0x1118D7FF,
	ORANGE	      = 0xFFA500FF,
	GOLD	      = 0xFFD700FF,
	YELLOW	      = 0xFFFF00FF,
	CYAN	      = 0x00FFFFFF,
	GREY	      = 0x808080FF,
	GREY_SMOOTH   = 0x202020FF,
	PURPLE	      = 0xFF009BFF
} LibColor;

typedef struct {
	float x, y, z;
	float w;
} LibPos3D;

typedef struct {
	u32 x, y;
} LibPos;

extern Resolution global_res;
extern u32 buffer_list[NUMBER_BUFFERS];	

s32 *Display_GetBuffer(s32 frameBuffer);
void Display_SetActiveBuffer(s32 frameBuffer);
void Display_SetDrawingBuffer(s32 frameBuffer);
s32 *Display_GetActiveBuffer();
void Display_SwapBuffers();
void Display_Initialize(Resolution res, Bitdepth bd, AntiAliasing aa,
		Gamma gamma);
s32 Display_FrameWidth();
s32 Display_FrameHeight();
void Display_FillScreen(u32 color);
void Display_SetColors(const u32 foreground, const u32 background);
TextColor Display_GetColors();
void Display_DrawRect(const u16 bx, const u16 by, const u16 xd, const u16 yd, const u32 color, bool filled);
void Display_DrawLine(s32 x1, s32 y1, s32 x2, s32 y2, const u32 color);
void Display_DrawTri(u32 pos1x, u32 pos1y, u32 pos2x, u32 pos2y, u32 pos3x, u32 pos3y, const u32 color);
void Display_DrawCircle(const u16 cx, const u16 cy, u32 scale, const u32 color, bool filled, float stepSize);
void Display_DrawPixel(s32 x, s32 y, const u32 color);
void Display_DrawText(s32, s32, const char* text);
void Display_DrawCharacter(const LibPos textPosition, const unsigned char ch);
void Display_DrawTextFormat(const LibPos textPosition, const char* format, ...);
void Display_InitInterrupts();

#endif