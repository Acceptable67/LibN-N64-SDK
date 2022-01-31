/*handle everything visual*/
#include <libn_display.hpp>
#include <libn_regs.hpp>
#include <cstdint>
#include <libn_font.hpp>
#include <libn_sprite.hpp>
#include <string>
#include <functional>

#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"

CreateGlobalRegister(VI, VI_REG);
CreateGlobalRegister(MI, MI_REG);
CreateGlobalRegister(DP, DP_REG);

namespace LibN64::Display 
{

	[[maybe_unused]] static int* local_buffer = reinterpret_cast<int*>(FRAMEBUFFER_ADDR);
	
	std::array<u32, 2> buffer_list = 
	{ 
		FRAMEBUFFER_ADDR, 
		FRAMEBUFFER_ADDR + 0x01000000
	};

	LibN64::Display::Resolution global_res = {0,0};
	static TextColor localColor;

	static int*                active_buffer;
	static int*				   active_draw_buffer;

	int* GetBuffer(Buffer id)
	{
		return reinterpret_cast<s32*>(buffer_list[id]);
	}

	int* GetActiveBuffer()
	{
		return active_buffer;
	}

	void SetActiveBuffer(Buffer x)
	{
		active_buffer = reinterpret_cast<s32*>(buffer_list[x]);
	}

	void SetDrawingBuffer(Buffer x)
	{
		active_draw_buffer = reinterpret_cast<s32*>(buffer_list[x]);
	}

	void SetVI_IntCallback(std::function<void()> func) 
	{
		if((MI_REG->intr & MI_REG->mask) & 0x08) {
			func();
			VI_REG->currentvl = VI_REG->currentvl;
		}
	}

	void SetVI_Intterupt(u32 line) 
	{
		MI_REG->mask = 0x0080;
		VI_REG->vint = line;
	}

	void SwapBuffers() 
	{
		if(GetActiveBuffer() == GetBuffer(Buffer::DISPLAY)) 
			{
				SetActiveBuffer(BACKUP);
				SetDrawingBuffer(DISPLAY);
			} else {
				SetActiveBuffer(DISPLAY);
				SetDrawingBuffer(BACKUP);
			}
	}

	void Initialize(Resolution res, Bitdepth bd, AntiAliasing aa, Gamma gamma) 
	{
		localColor.Foreground = LibColor::WHITE;
		localColor.Background = LibColor::BLACK;

		res.width 			= res.width;
		res.height 			= res.height;

		active_buffer 	   = GetBuffer(Buffer::DISPLAY);
		active_draw_buffer = GetBuffer(Buffer::BACKUP);

		VI_REG->origin      = reinterpret_cast<u32>(active_buffer);

		VI_REG->status 		= bd | aa | gamma;
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

		*reinterpret_cast<u32*>(((PIF_RAM)-sizeof(int))+0x3c) = 0x8;

		global_res = res;
	}

	void DrawPixel(LibPos pos, const u32 color) 
	{
			*(GetBuffer(DISPLAY) + (pos.y * global_res.width + pos.x)) = color;
		}

	void DrawRect(LibPos pos, const u16 xd, const u16 yd, const u32 color) 
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
	void DrawCharacter(const LibPos pos, u8 ch) 
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

	/*void DrawSprite(LibPos pos, LibSprite& spr) 
	{
		s8 local = spr.Data();
	}*/


	void SetColors(const u32 foreground, const u32 background) 
	{
		localColor.Foreground  = foreground;
		localColor.Background  = background;
	}

	namespace RDP 
	{
		static std::vector<u32> cBuffer;
		
		void AddCommand(u32 command)
		{	
			if(cBuffer.size() >= 1024) { 
				cBuffer.clear();
			}
				
			cBuffer.push_back(command);
		}

		void Send()
		{

		    while(DP_REG->status & 0x600){};

			DP_REG->status = 0x15; //0b00010101

	   		while(DP_REG->status & 0x600){}

			DP_REG->cmd_start = (reinterpret_cast<u32>(cBuffer.data()) 		 | 0xA0000000);
			DP_REG->cmd_end   = (reinterpret_cast<u32>(cBuffer.data() + 285) | 0xA0000000);
			/*I have no idea why this works with + 285. It should just queue the list then execute,
			but it doesn't work that way. It needs a very high 'magic' number to get the screen
			refresh perfect.*/
		}	

		void DebugAddr()
		{
			printf("%08X %08X", (u32)cBuffer.data(), (u32)cBuffer.data() + cBuffer.size());
			printf("Buffer size %d", (u32)cBuffer.size());
		}

		void SendDisplayList()
		{
			Send();
		}

		void SetOtherModes()
		{
			AddCommand(0x2F102800);
			AddCommand(0x00000000);
			
		}

		void SetClipping( u32 tx, u32 ty, u32 bx, u32 by )
		{
			AddCommand((DL_SET_CLIP_AREA | (tx << 14) | (ty << 2)));
			AddCommand(((bx << 14) | (by << 2)) );
			
		}


		void SetDefaultClipping( void )
		{
			SetClipping(0, 0, global_res.width, global_res.height );
		}

		void EnablePrimitive( void )
		{
			AddCommand(DL_ENABLE_PRIM);
			AddCommand(DL_ENABLE_PRIM_2);
		
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
			AddCommand(DL_ENABLE_BLEND);
			AddCommand(DL_ENABLE_BLEND_2);
		
		}

		void SetPrimitiveColor(u32 color)
		{
			AddCommand(DL_SET_PRIM_COL);
			AddCommand(color );
		
		}	

		void SetBlendColor(auto color)
		{
			AddCommand(DL_SET_BLEND_COL);
			AddCommand(color );
		
		}

		void DrawRectangle(u32 tx, u32 ty, u32 bx, u32 by)
		{
			AddCommand((DL_DRAW_RECT| (bx << 14) | (by << 2)));
			AddCommand((tx << 14) | (ty << 2));
		
		}


		void Attach()
		{	
			AddCommand((DL_ATTACH_FB | 0x00180000 | (global_res.width- 1)));
			AddCommand((u32)active_buffer);
			
		}

		void Sync()
		{
			AddCommand(DL_SYNC_PIPE); //PIPE
			AddCommand(DL_NULL_CMD);
		
		}

		void DrawRectangleSetup( u32 tx, u32 ty, u32 bx, u32 by, u32 color )
		{
			
			RDP::Attach();
			RDP::SetDefaultClipping();
			RDP::Sync();
			RDP::EnableBlend();
			RDP::Sync();
			RDP::SetBlendColor(color);
			RDP::Sync();
			RDP::DrawRectangle(tx, ty, bx, by);
			RDP::SendDisplayList();
		
			
		}

		void FillScreen(u32 color) 
		{
			DrawRectangleSetup(0,0, global_res.width, global_res.height, color);
		} 

	}
};
