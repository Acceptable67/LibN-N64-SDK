/*handle everything visual*/
#include <stdarg.h>
#include <array>
#include <malloc.h>

#define  DISPLAY_BUFFER 0
#define  BACKUP_BUFFER	1
#define  EXTRA_BUFFER	2

#define NUMBER_BUFFERS	2

namespace LibN64::Display 
{
	bool dbuffering = false;

	void Initialize(Resolution res, Bitdepth bd, AntiAliasing aa, Gamma gamma, bool dBuf) 
	{
		TextColor::Foreground = LibColor::WHITE;
		TextColor::Background = LibColor::BLACK;

		res.width 			= res.width;
		res.height 			= res.height;

		dbuffering = dBuf;
		
		for(int i =0; i < NUMBER_BUFFERS; i++) 
		{
			buffer_list[i] = new int[res.width * res.height];
		}

		active_buffer = (buffer_list[DISPLAY_BUFFER]);

		VI_REG->status 		= bd | aa | gamma;
		VI_REG->origin 		= (int)active_buffer | 0xA0000000;
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

	void SetVI_DRAM(const std::any addr) {
		VI_REG->origin = std::any_cast<int>(addr) | 0xA0000000;
	}

	void SetActiveBuffer(int num) 
	{
		active_buffer = buffer_list[num];
	}

	void DrawPixel(uint32_t x, uint32_t y, auto color) 
	{
		*(((dbuffering) ? buffer_list[BACKUP_BUFFER] : buffer_list[DISPLAY_BUFFER]) + (y * global_res.width + x)) = color;
	}

	void DrawRect(uint32_t x, uint32_t y, auto xd, auto yd, auto color) 
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
		char buf[300];
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
