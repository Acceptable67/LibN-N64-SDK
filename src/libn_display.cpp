/*handle everything visual*/
#include <libn_display.h>
#include <libn_regs.h>
#include <cstdint>
#include <libn_font.h>
#include <string>

#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"

CreateGlobalRegister(VI);
CreateGlobalRegister(MI);
CreateGlobalRegister(DP);

namespace LibN64::Display 
{
	static Resolution global_res; 
	static int* local_buffer = reinterpret_cast<int*>(FRAMEBUFFER_ADDR);
	
	static TextColor localColor;

	#ifdef LIBN_DOUBLE_BUFFER
		*static std::array<int*, 2> buffer_list;
		bool dbuffering = false;
		static int*                active_buffer;

		auto GetBuffer(Buffer b)
		{
			return buffer_list[b];
		}

		void SetActiveBuffer(const int num) 
		{
			active_buffer = buffer_list[num];
		}
	#endif

	void Initialize(Resolution res, Bitdepth bd, AntiAliasing aa, Gamma gamma, [[maybe_unused]] bool dBuf) 
	{
		localColor.Foreground = LibColor::WHITE;
		localColor.Background = LibColor::BLACK;

		res.width 			= res.width;
		res.height 			= res.height;

		#ifdef LIBN_DOUBLE_BUFFER
			for(int i = 0; i < 2; i++) 
			{
				buffer_list[i] = new int[res.width * res.height];
			}

			active_buffer = GetBuffer(Buffer::DISPLAY);

			VI_REG->origin      = buffer_list[0] | 0xA0000000;
		#else	
			VI_REG->origin 		= FRAMEBUFFER_ADDR;
		#endif

		VI_REG->status 		= bd | aa | gamma;
		VI_REG->width 		= res.width;
		VI_REG->vint 		= 0x2;
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

		*reinterpret_cast<u32*>(((PIF_RAM)-sizeof(int))+0x3c) = 0x8;

		global_res = res;
	}

	void checkVI_Int() {
		if((MI_REG->intr & MI_REG->mask) & 0x08) {
			VI_REG->currentvl = VI_REG->currentvl;
		}
	}

	auto SetVI_Int(auto line) {
		MI_REG->mask = 0x0080;
		VI_REG->vint = line;
	}

	void SetVI_DRAM(const std::any addr) {
		VI_REG->origin = std::any_cast<int>(addr) | 0xA0000000;
	}

	void DrawPixel(LibPos pos, const u32 color) 
	{
		*(local_buffer + (pos.y * global_res.width + pos.x)) = color;
	}

	void DrawRect(LibPos pos, const auto xd, const auto yd, const auto color) 
	{
		for(u32 i = 0; i < xd; i++) 
		{
			for(u32 d = 0; d < yd; d++) 
			{
				DrawPixel({pos.x + i, pos.y + d}, color);
			}
		}
	}

	void FillScreen(u32 color) 
	{
		DrawRect({0,0},global_res.width, global_res.height, color);
	}

	/*8x8 taken from LibDragon*/
	void DrawCharacter(const LibPos pos, unsigned char ch) 
	{
		u32 trans = ((localColor.Background & 0xff) == 0) ? 1 : 0; 
		for(u32 row = 0; row < font_width; row++) {
			unsigned char c = __font_data[(ch * font_width) + row];
			for(u32 col = 0; col < font_height; col++) 
			{
				if(trans) 
				{
					if(c & 0x80) 
					{
						DrawPixel({pos.x + col, pos.y + row}, localColor.Foreground);
					}
				} 
				else 
				{
					DrawPixel({pos.x + col, pos.y + row}, (c & 0x80) ? localColor.Foreground : localColor.Background);
				}
			
				c <<= 1;
			}
		}
	}

	void DrawText(LibPos pos, const std::string text) 
	{
		LibPos toffset = {0,0}; 
		for(auto& c : text) 
		{
			if((pos.x + toffset.x) >= global_res.width || c == '\n') 
			{
				pos.y += font_height;
				toffset.x = 0;
			} 
			else if(c == '\t') 
			{
				pos.x += font_width * 4;
			} 
			else
			{
				DrawCharacter({pos.x + toffset.x, pos.y + toffset.y}, c);
				toffset.x += font_width;
			}
		}
	}


	void SetColors(const u32 foreground, const u32 background) 
	{
		localColor.Foreground  = foreground;
		localColor.Background  = background;
	}

	namespace RDP 
	{

		u32 commandBuffer[48];

		size_t pos = 0;
		size_t start = 0;
		void AddCommand( u32 first)
		{	
			if(pos >= 48) 
				return;
				
			commandBuffer[pos] = first;
			pos++;
		}

		void Send()
		{
			if(pos - start == 0) { return;}

		    while(DP_REG->status & 0x600){};

			DP_REG->status = 0x15; //0b00010101

	   		while(DP_REG->status & 0x600){}

			DP_REG->cmd_start = (static_cast<u32>((u32)commandBuffer | 0xA0000000)) ;
			DP_REG->cmd_end   = (static_cast<u32>((u32)commandBuffer | 0xA0000000) + pos) ;

			if(pos > 24) {
				start = 0;
				pos = 0;
			}
			start = pos;
		}	

		void SetOtherModes()
		{
			AddCommand(0x2F102800);
			AddCommand(0x00000000);
			Send();
		}

		void SetClipping( u32 tx, u32 ty, u32 bx, u32 by )
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

		void SetPrimitiveColor(u32 color)
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

		void DrawRectangle(u32 tx, u32 ty, u32 bx, u32 by)
		{
			AddCommand ( (0xF6000000 | ( bx << 14 ) | ( by << 2 )));
			AddCommand(( tx << 14 ) | ( ty << 2 ) );
			Send();
		}


		void Attach(  )
		{	
			AddCommand((0x3F000000 | 0x00180000 | (global_res.width- 1)));
			#ifdef LIBN_DOUBLE_BUFFER 
				AddCommand( reinterpret_cast<int>(buffer_list[Buffer::DISPLAY]));
			#else
				AddCommand(FRAMEBUFFER_ADDR);
			#endif
			
			Send();
		}

		void Sync( )
		{
			AddCommand(0xE7000000); //PIPE
			AddCommand(0x00000000 );
			Send();
		}

		void DrawRectangleSetup( u32 tx, u32 ty, u32 bx, u32 by, auto color )
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

		void ClearScreen(u32 color) 
		{
			DrawRectangleSetup(0,0, global_res.width, global_res.height, color);
		} 

	}
};
