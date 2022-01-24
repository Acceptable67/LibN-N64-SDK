/*handle everything visual*/
#include <stdarg.h>
#include <array>

namespace LibN64::Display 
{
	struct Resolution;
	struct TextColor;

	struct Resolution
	{
		uint32_t width; 
		uint32_t height;
	};

	struct TextColor
	{
		static uint32_t Foreground;
		static uint32_t Background;
	};
	static Resolution global_res;
	static auto* buffer = reinterpret_cast<uint32_t*>(FRAMEBUFFER_ADDR);

	uint32_t TextColor::Foreground = 0xFFFFFFFF;
	uint32_t TextColor::Background = 0x202020FF;

	enum Bitdepth 
	{
		BD16BPP         = 0x2,
		BD32BPP         = 0x3
	};

	enum Gamma 
	{
		GAMMA_OFF,
		GAMMA_DITHER    = 0x4,
		GAMMA_ENABLE    = 0x8,
	};

	enum AntiAliasing 
	{
		DIVOT_ENABLE    = 0x10,
		AA_RESAMP_FETCH = 0x100,
		AA_RESAMP_ONLY  = 0x200,
		AA_REPLICATE    = 0x300
	};

	struct LibPos3D {
		float x, y, z;
		float w;

		bool operator ==(LibPos3D x) const 
		{
			return (x.x == this->x && x.y == this->y && x.z == this->z);
		}

		LibPos3D operator +(LibPos3D x) const 
		{
			LibPos3D tmp = {this->x, this->y, this->z, this->w};
			tmp.x += x.x;
			tmp.y += x.y;
			tmp.z += x.z;
			return tmp;
		}

		LibPos3D operator -(LibPos3D x) const 
		{
			LibPos3D tmp = {this->x, this->y, this->z, this->w};
			tmp.x -= x.x;
			tmp.y -= x.y;
			tmp.z -= x.z;
			return tmp;
		}

		LibPos3D operator *(LibPos3D x) const 
		{
			LibPos3D tmp = {this->x, this->y, this->z, this->w};
			tmp.x *= x.x;
			tmp.y *= x.y;
			tmp.z *= x.z;
			return tmp;
		}
	};

	struct LibPos 
	{ 
		uint32_t x, y; 
		LibPos operator +(LibPos x) const 
		{
			LibPos tmp = { this->x, this->y };
			tmp.x += x.x;
			tmp.y += x.y;
			return tmp;
		}

		LibPos operator -(LibPos x) const 
		{
			LibPos tmp = { this->x, this->x};
			tmp.x -= x.x;
			tmp.y -= x.y;
			return tmp;
		}
				
		bool operator ==(LibPos x) const			
		{
			LibPos tmp = { this->x, this->y };
			return (tmp.x == x.x && tmp.y == x.y);
		}
	};
	static LibN64::Display::LibPos cPos = {10,10};

	template<class SpecifiedType>
	requires std::integral<SpecifiedType> || std::floating_point<SpecifiedType>
	struct Lib2DVec
	{
		public:
			SpecifiedType first_element;
			SpecifiedType second_element;
			
	};

	/*from LIBN64.H*/
	constexpr int MakeColor(int r, int g, int b, int a) 
	{
		return (r << 24) 		   | 
		(((g & 0x00FFFFFF) << 16)) |
		(((b & 0xFF00FFFF) << 8))  | 
		((a & 0xFFFF00FF));
	}

	enum LibColor 
	{
		 RED			= MakeColor(0xFF, 0x00, 0x00, 0xFF),
		 GREEN			= MakeColor(0x00, 0xFF, 0x00, 0xFF),
		 WHITE			= MakeColor(0xFF, 0xFF, 0xFF, 0xFF),
		 BLACK			= MakeColor(0x00, 0x00, 0x00, 0x00),
		 BLUE			= MakeColor(0x00, 0x00, 0xFF, 0xFF),
		 NAVY_BLUE		= MakeColor(0x11, 0x18, 0x80, 0xFF),
		 DARK_RED		= MakeColor(0x4B, 0x13, 0x00, 0xFF),
		 DEEP_DARK_RED	= MakeColor(0x1F, 0x01, 0x00, 0xFF),
		 SKY_BLUE		= MakeColor(0x11, 0x18, 0xD7, 0xFF),
		 ORANGE			= MakeColor(0xFF, 0xA5, 0x00, 0xFF),
		 GOLD			= MakeColor(0xFF, 0xD7, 0x00, 0xFF),
		 YELLOW			= MakeColor(0xFF, 0xFF, 0x00, 0xFF),
		 CYAN			= MakeColor(0x00, 0xFF, 0xFF, 0xFF),
		 GREY			= MakeColor(0x80, 0x80, 0x80, 0xFF),
		 GREY_SMOOTH	= MakeColor(0x20, 0x20, 0x20, 0xFF),
		 PURPLE			= MakeColor(0xFF, 0x00, 0x9B, 0xFF)
	};

	void Initialize(Resolution res, Bitdepth bd, AntiAliasing aa, Gamma gamma = GAMMA_OFF) 
	{
		TextColor::Foreground = LibColor::WHITE;
		TextColor::Background = LibColor::BLACK;

		res.width 			= res.width;
		res.height 			= res.height;

		VI_REG->status 		= bd | aa | gamma;
		VI_REG->origin 		= FRAMEBUFFER_ADDR;
		VI_REG->width 		= res.width;
		VI_REG->vint 		= 0x200;
		VI_REG->currentvl 	= 0x0;
		VI_REG->vtiming 	= 0x3E52239;
		VI_REG->vsync 		= 0x20D;
		VI_REG->hsync 		= 0xC15;
		VI_REG->hsyncleap 	= 0xC150C15;
		VI_REG->hvideo 		= 0x6C02EC;
		VI_REG->vvideo 		= 0x2501FF;
		VI_REG->vburst 		= 0xE0204;
		VI_REG->xscale 		= (0x100*res.width)/160;
		VI_REG->yscale 		= (0x100*res.height)/60;

		*reinterpret_cast<uint32_t*>(((PIF_RAM)-0x4)+0x3c) = 0x8;

		global_res = res;
	}

	void DrawPixel(uint32_t x, uint32_t y, auto color) 
	{
		*(buffer + (y * global_res.width + x)) = color;
	}

	void DrawRect(uint32_t x, uint32_t y, uint32_t xd, uint32_t yd, auto color) 
	{
		for(decltype(xd) i = 0; i < xd; i++) 
		{
			for(decltype(yd) d = 0; d < yd; d++) 
			{
				DrawPixel(x + i, y + d, color);
			}
		}
	}

	void FillScreen(auto color) 
	{
		DrawRect(0,0,global_res.width, global_res.height, color);
	}

	/*8x8 taken from LibDragon*/
	void DrawCharacter(uint32_t x, uint32_t y, unsigned char ch) 
	{
		uint32_t trans = ((TextColor::Background & 0xff) == 0) ? 1 : 0; 
		for(decltype(font_width) row = 0; row < font_width; row++) {
			unsigned char c = __font_data[(ch * font_width) + row];
			for(uint32_t col = 0; col < font_height; col++) 
			{
				if(trans) 
				{
					if(c & 0x80) 
					{
						DrawPixel(x + col, y + row, TextColor::Background);
					}
				} 
				else 
				{
					DrawPixel(x + col, y + row, (c & 0x80) ? TextColor::Foreground : TextColor::Background);
				}
			
				c <<= 1;
			}
		}
	}

	void DrawText(uint32_t x, uint32_t y, const std::string text) 
	{
		LibPos toffset = {0,0}; 
		for(auto& c : text) 
		{
			if((x + toffset.x) >= global_res.width || c == '\n') 
			{
				y += font_height;
				toffset.x = 0;
			} 
			else if(c == '\t') 
			{
				x += font_width * 4;
			} 
			else
			{
				DrawCharacter(x + toffset.x, y + toffset.y, c);
				toffset.x += font_width;
			}
		}
	}

	template<class T, class ...Args>
	requires (!std::is_member_function_pointer<T>::value) || (!std::is_member_function_pointer<Args...>::value)
	void DrawTextFormat(uint32_t x, uint32_t y,  const std::string format, T arg, Args... args) {
		char buf[100];
		snprintf(reinterpret_cast<char*>(buf), sizeof(buf), format.c_str(), arg, args...);
		DrawText(x,y,buf);
	}


	void SetColors(uint32_t foreground, uint32_t background) 
	{
		TextColor::Foreground  = foreground;
		TextColor::Background  = background;
	}

	namespace RDP 
	{

		std::array<uint32_t,1024> commandBuffer;

		size_t pos = 0;
		void AddCommand( uint32_t first)
		{	
			if(pos >= commandBuffer.size()) 
				pos=0; 
				
			commandBuffer[pos] = first;
			pos++;
		}

		void Send()
		{
		    while(DP_REG->status & 0x600){};

			DP_REG->status = 0x15; //0b00010101

	   		while(DP_REG->status & 0x600);

			DP_REG->cmd_start = reinterpret_cast<uint32_t>(commandBuffer.begin()) + pos-1;
			DP_REG->cmd_end   = reinterpret_cast<uint32_t>(commandBuffer.begin()) + pos;
		}	

		void SetOtherModes()
		{
			AddCommand(0x2F102800);
			AddCommand(0x00000000);
			Send();
		}

		void SetClipping( uint32_t tx, uint32_t ty, uint32_t bx, uint32_t by )
		{
			AddCommand((0xED000000 | (tx << 14) | (ty << 2)));
			AddCommand(((bx << 14) | (by << 2)) );
			Send();
		}


		void SetDefaultClipping( void )
		{
			SetClipping(0, 0, global_res.width, global_res.height );
		}

		void EnablePrimitive( void )
		{
			AddCommand(0xEFB000FF);
			AddCommand(0x00004000);
			Send();
		}

		void Init() 
		{
			MI_REG->mask = 0x0800;
		}

		void Close() 
		{
			MI_REG->mask = 0x0400;
		}

		void EnableBlend()
		{
			AddCommand(0xEF0000FF);
			AddCommand(0x80000000);
			Send();
		}

		void SetPrimitiveColor(auto color)
		{
			AddCommand(0xF7000000);
			AddCommand(color );
			Send();
		}	

		void SetBlendColor(auto color)
		{
			AddCommand (0xF9000000);
			AddCommand(color );
			Send();
		}

		void DrawRectangle(uint32_t tx, uint32_t ty, uint32_t bx, uint32_t by)
		{
			AddCommand ( (0xF6000000 | ( bx << 14 ) | ( by << 2 )));
			AddCommand((( tx << 14 ) | ( ty << 2 )) );
			Send();
		}


		void Attach(  )
		{	
			AddCommand((0x3F000000 | 0x00180000 | (global_res.width- 1)));
			AddCommand(FRAMEBUFFER_ADDR );
			Send();
		}

		void Sync( )
		{
			AddCommand(0xE7000000); //PIPE
			AddCommand(0x00000000 );
			Send();
		}

		void DrawRectangleSetup( uint32_t tx, uint32_t ty, uint32_t bx, uint32_t by, auto color )
		{
			RDP::Init();
			RDP::Attach();
			RDP::SetDefaultClipping();
			RDP::Sync();
			RDP::EnableBlend();
			RDP::Sync();
			RDP::SetBlendColor(color);
			RDP::Sync();
			RDP::DrawRectangle(tx, ty, bx, by);
			RDP::Close();
			
		}

		void ClearScreen(auto color) 
		{
			DrawRectangleSetup(0,0, global_res.width, global_res.height, color);
		} 

	}
};
