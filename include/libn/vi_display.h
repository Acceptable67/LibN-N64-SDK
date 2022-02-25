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
void Display_SetVI_IntCallback(void (*callbackFunction)());
void Display_SetVI_Intterupt(u32 vline);
void Display_SwapBuffers();
void Display_Initialize(Resolution res, Bitdepth bd, AntiAliasing aa,
		Gamma gamma);

void Display_FillScreen(u32 color);
void Display_SetColors(const u32 foreground, const u32 background);
TextColor Display_GetColors();
void Display_DrawRect(LibPos boxPosition, const u16 xd, const u16 yd, const u32 color, bool filled);
void Display_DrawLine(s32 x1, s32 y1, s32 x2, s32 y2, const u32 color);
void Display_DrawTri(LibPos point1, LibPos point2, LibPos point3, const u32 color);
void Display_DrawCircle(LibPos circlePosition, u32 scale, const u32 color, bool filled, float stepSize);
void Display_DrawPixel(s32 x, s32 y, const u32 color);
void Display_DrawText(s32, s32, const char* text);
void Display_DrawCharacter(const LibPos textPosition, const unsigned char ch);
void Display_DrawTextFormat(const LibPos textPosition, const char* format, ...);

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

void RDP_DebugAddr();
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