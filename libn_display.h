/*handle everything visual*/
#include <stdarg.h>

namespace LibN64::Display 
{
	struct resolution_t 
	{
		int width; 
		int height;
	};
	static uint32_t   *buffer = (uint32_t*)(FRAMEBUFFER_ADDR);

	int fg_color = 0xFFFFFFFF;
	int bg_color = 0x00000000;

	static resolution_t global_res;
	void Initialize(resolution_t res) 
	{
		res.width = res.width;
		res.height = res.height;
		VI_REG->status = 0x0003 | 0x00300 | 0x00080;
		VI_REG->origin = FRAMEBUFFER_ADDR;
		VI_REG->width = res.width;
		VI_REG->vint = 0x200;
		VI_REG->currentvl = 0x0;
		VI_REG->vtiming = 0x3E52239;
		VI_REG->vsync = 0x20D;
		VI_REG->hsync = 0xC15;
		VI_REG->hsyncleap = 0xC150C15;
		VI_REG->hvideo = 0x6C02EC;
		VI_REG->vvideo = 0x2501FF;
		VI_REG->vburst = 0xE0204;
		VI_REG->xscale = (0x100*res.width)/160;
		VI_REG->yscale = (0x100*res.height)/60;

		global_res = res;
	}

	void DrawPixel(int x, int y, uint32_t color) 
	{
		*(buffer + (y * global_res.width + x)) = color;
	}

	void DrawRect(int x, int y, int xd, int yd, uint32_t color) 
	{
		for(int i =0;i<xd;i++) {
			for(int d = 0;d<yd;d++) {
				DrawPixel(x + i, y + d, color);
			}
		}
	}

	void FillScreen(uint32_t color) 
	{
		DrawRect(0,0,global_res.width, global_res.height, color);
	}

	void DrawCharacter(int x, int y, unsigned char ch) 
	{
		int trans = (bg_color & 0xff == 0) ? 1 : 0; 
		for(int row = 0; row < 8; row++) {
			unsigned char c = __font_data[(ch * 8) + row];
			for(int col = 0; col < 8; col++) 
			{
				if(trans) 
				{
					if(c & 0x80) 
					{
						DrawPixel(x + col, y + row, fg_color);
					}
				} 
				else 
				{
					DrawPixel(x + col, y + row, (c & 0x80) ? fg_color : bg_color);
				}
			
				c <<= 1;
			}
		}
	}

	void DrawText(int x, int y, const std::string text) 
	{
		int xoffset = 0, yoffset = 0;
		for(auto& c : text) 
		{
			DrawCharacter(x + xoffset, y + yoffset, c);
			xoffset += 8;
		}
	}

	template<class T>
	void DrawTextFormat(int x, int y,  const char* format, T arg) {
		/*va_list args;
		va_start(args, format);

		char buffer[300];
		sprintf(buffer, format, args);
		DrawText(x, y, buffer);

		va_end(args);	*/
		char buf[100];
		snprintf(buf, 100, format, arg);
		DrawText(x,y,buf);
	}



	void SetColors(uint32_t foreground, uint32_t background) {
		fg_color = foreground;
		bg_color = background;
	}
};
