#ifndef LIBN_DISPLAY_H
#define LIBN_DISPLAY_H

/*handle everything visual*/

#include <stdarg.h>
#include <array>
#include <malloc.h>
#include <libn/types.hpp>
#include <libn/sprite.hpp>
#include <any>
#include <string>
#include <functional>
#include <string_view>

#define NUMBER_BUFFERS 2

namespace LibN64 {
namespace Display {
struct Resolution {
	u32 width;
	u32 height;
};

struct TextColor {
	u32 Foreground;
	u32 Background;
};

enum Bitdepth { 
	BD16BPP = 0x2, 
	BD32BPP = 0x3 
};

enum Gamma {
	GAMMA_OFF,
	GAMMA_DITHER = 0x4,
	GAMMA_ENABLE = 0x8,
};

enum AntiAliasing {
	DIVOT_ENABLE	= 0x10,
	AA_RESAMP_FETCH = 0x100,
	AA_RESAMP_ONLY	= 0x200,
	AA_REPLICATE	= 0x300
};

inline constexpr uint32_t MakeColor(u32 r, u32 g, u32 b, u32 a) {
	return (r << 24) | (((g & 0x00FFFFFF) << 16)) |
	       (((b & 0xFF00FFFF) << 8)) | ((a & 0xFFFF00FF));
}

enum Buffer { DISPLAY = 0, BACKUP, EXTRA };

enum LibColor {
	RED	      	  = MakeColor(0xFF, 0x00, 0x00, 0xFF),
	GREEN	      = MakeColor(0x00, 0xFF, 0x00, 0xFF),
	WHITE	      = MakeColor(0xFF, 0xFF, 0xFF, 0xFF),
	BLACK	      = MakeColor(0x00, 0x00, 0x00, 0x00),
	BLUE	      = MakeColor(0x00, 0x00, 0xFF, 0xFF),
	NAVY_BLUE     = MakeColor(0x11, 0x18, 0x80, 0xFF),
	DARK_RED      = MakeColor(0x4B, 0x13, 0x00, 0xFF),
	DEEP_DARK_RED = MakeColor(0x1F, 0x01, 0x00, 0xFF),
	SKY_BLUE      = MakeColor(0x11, 0x18, 0xD7, 0xFF),
	ORANGE	      = MakeColor(0xFF, 0xA5, 0x00, 0xFF),
	GOLD	      = MakeColor(0xFF, 0xD7, 0x00, 0xFF),
	YELLOW	      = MakeColor(0xFF, 0xFF, 0x00, 0xFF),
	CYAN	      = MakeColor(0x00, 0xFF, 0xFF, 0xFF),
	GREY	      = MakeColor(0x80, 0x80, 0x80, 0xFF),
	GREY_SMOOTH   = MakeColor(0x20, 0x20, 0x20, 0xFF),
	PURPLE	      = MakeColor(0xFF, 0x00, 0x9B, 0xFF)
};

struct LibPos3D {
	float x, y, z;
	float w;

	bool operator==(LibPos3D x) const {
		return (x.x == this->x && x.y == this->y && x.z == this->z);
	}

	LibPos3D operator+(LibPos3D x) const {
		LibPos3D tmp = {this->x, this->y, this->z, this->w};
		tmp.x += x.x;
		tmp.y += x.y;
		tmp.z += x.z;
		return tmp;
	}

	LibPos3D operator-(LibPos3D x) const {
		LibPos3D tmp = {this->x, this->y, this->z, this->w};
		tmp.x -= x.x;
		tmp.y -= x.y;
		tmp.z -= x.z;
		return tmp;
	}

	LibPos3D operator*(LibPos3D x) const {
		LibPos3D tmp = {this->x, this->y, this->z, this->w};
		tmp.x *= x.x;
		tmp.y *= x.y;
		tmp.z *= x.z;
		return tmp;
	}
};

struct LibPos {
	u32 x, y;
	LibPos operator+(LibPos x) const {
		LibPos tmp = {this->x, this->y};
		tmp.x += x.x;
		tmp.y += x.y;
		return tmp;
	}

	LibPos operator-(LibPos x) const {
		LibPos tmp = {this->x, this->x};
		tmp.x -= x.x;
		tmp.y -= x.y;
		return tmp;
	}

	bool operator==(LibPos x) const {
		LibPos tmp = {this->x, this->y};
		return (tmp.x == x.x && tmp.y == x.y);
	}
};

template <class SpecifiedType>
requires std::integral<SpecifiedType> || std::floating_point<SpecifiedType>
struct Lib2DVec {
      public:
	SpecifiedType first_element;
	SpecifiedType second_element; 
};

extern LibN64::Display::Resolution global_res;
extern std::array<u32, NUMBER_BUFFERS> buffer_list;

s32 *GetBuffer(Buffer frameBuffer);
void SetActiveBuffer(Buffer frameBuffer);
void SetDrawingBuffer(Buffer frameBuffer);
s32 *GetActiveBuffer();
void SetVI_IntCallback(std::function<void()> callbackFunction);
void SetVI_Intterupt(u32 vline);
void SwapBuffers();
void Initialize(Resolution res, Bitdepth bd, AntiAliasing aa,
		Gamma gamma = GAMMA_OFF);

void FillScreen(u32 color);
void SetColors(const u32 foreground, const u32 background);
TextColor GetColors();
void DrawRect(LibPos boxPosition, const u16 xd, const u16 yd, const u32 color, bool filled = true);
void DrawLine(LibPos point1, LibPos point2, const u32 color);
void DrawTri(LibPos point1, LibPos point2, LibPos point3, const u32 color);
void DrawCircle(LibPos circlePosition, u32 scale = 1, const u32 color = WHITE, bool filled = true, float stepSize = 0.1);
void DrawPixel(LibPos pixelPosition, const u32 color);
void DrawText(LibPos textPosition, const std::string_view text);
void DrawCharacter(const LibPos textPosition, const unsigned char ch);
void DrawTextFormat(const LibPos textPosition, const std::string format, ...);
	} // namespace Display
}; // namespace LibN64

#endif