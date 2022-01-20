/*handle everything visual*/
#include <stdarg.h>

namespace LibN64::Display 
{
	struct Resolution
	{
		uint32_t width; 
		uint32_t height;
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


	template<class SpecifiedType>
	requires std::integral<SpecifiedType> || std::floating_point<SpecifiedType>
	struct Lib2DVec
	{
		public:
		std::pair<SpecifiedType, SpecifiedType> values;
		
		SpecifiedType& First() {
			return values.first;
		}

		SpecifiedType& Second() {
			return values.second;
		}
	};

	static uint32_t  *buffer = reinterpret_cast<uint32_t*>(FRAMEBUFFER_ADDR);

	uint32_t fg_color = 0xFFFFFFFF;
	uint32_t bg_color = 0x00000000;

	static Resolution global_res;
	void Initialize(Resolution res) 
	{
		res.width 			= res.width;
		res.height 			= res.height;
		VI_REG->status 		= BD32BPP | AA_REPLICATE;
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

	void DrawPixel(uint32_t x, uint32_t y, uint32_t color) 
	{
		*(buffer + (y * global_res.width + x)) = color;
	}

	void DrawRect(uint32_t x, uint32_t y, uint32_t xd, uint32_t yd, auto color) 
	{
		for(auto i = 0; i < xd; i++) {
			for(auto d = 0; d < yd; d++) {
				DrawPixel(x + i, y + d, color);
			}
		}
	}

	void FillScreen(auto color) 
	{
		DrawRect(0,0,global_res.width, global_res.height, color);
	}

	void DrawCharacter(uint32_t x, uint32_t y, unsigned char ch) 
	{
		uint32_t trans = ((bg_color & 0xff) == 0) ? 1 : 0; 
		for(uint32_t row = 0; row < 8; row++) {
			unsigned char c = __font_data[(ch * 8) + row];
			for(uint32_t col = 0; col < 8; col++) 
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

	void DrawText(uint32_t x, uint32_t y, const std::string text) 
	{
		uint32_t xoffset = 0, yoffset = 0;
		for(auto& c : text) 
		{
			DrawCharacter(x + xoffset, y + yoffset, c);
			xoffset += 8;
		}
	}

	template<class T>
	void DrawTextFormat(uint32_t x, uint32_t y,  const std::string format, T arg) {
		/*va_list args;
		va_start(args, format);

		char buffer[300];
		sprintf(buffer, format, args);
		DrawText(x, y, buffer);

		va_end(args);	*/
		char buf[100];
		snprintf(buf, 100, format.c_str(), arg);
		DrawText(x,y,buf);
	}



	void SetColors(uint32_t foreground, uint32_t background) {
		fg_color = foreground;
		bg_color = background;
	}

	/*LIBDRAGONS RDP UNTIL I WRITE MY OWN*/
	namespace RDP 
	{
		uint32_t rdp_start, rdp_end;
		uint32_t rdp_ringbuffer[4096 / 4];
		void AddCommand( uint32_t data )
		{
			/* Add data to queue to be sent to RDP */
			rdp_ringbuffer[rdp_end / 4] = data;
			rdp_end += 4;
		}

		uint32_t xy, yy;
		void Send( void )
		{
	
			while( (((volatile uint32_t *)0xA4100000)[3] & 0x600) ) ;
			/* Clear XBUS/Flush/Freeze */
			((uint32_t *)0xA4100000)[3] = 0x15;

			while( (((volatile uint32_t *)0xA4100000)[3] & 0x600) ) ;

			/* Send start and end of buffer location to kick off the command transfer */
			((volatile uint32_t *)0xA4100000)[0] = ((uint32_t)rdp_ringbuffer | 0xA0000000) + rdp_start;
			((volatile uint32_t *)0xA4100000)[1] = ((uint32_t)rdp_ringbuffer | 0xA0000000) + rdp_end;

			if( rdp_end > (4096 - 1024) )
			{
				/* Wrap around before a command can be split */
				rdp_start = 0;
				rdp_end = 0;
			}
			else
			{
				/* Advance the start to not allow clobbering current command */
				rdp_start = rdp_end;
			}
		}	


		void SetClipping( uint32_t tx, uint32_t ty, uint32_t bx, uint32_t by )
		{
			/* Convert pixel space to screen space in command */
			AddCommand( 0xED000000 | (tx << 14) | (ty << 2) );
			AddCommand( (bx << 14) | (by << 2) );
			Send();
		}

		/**
		 * @brief Set the hardware clipping boundary to the entire screen
		 */
		void SetDefaultClipping( void )
		{
			/* Clip box is the whole screen */
			SetClipping( 0, 0, global_res.width, global_res.height );
		}

		/**
		 * @brief Enable display of 2D filled (untextured) rectangles
		 *
		 * This must be called before using #rdp_draw_filled_rectangle.
		 */
		void EnablePrimitive( void )
		{
			/* Set other modes to fill and other defaults */
			AddCommand( 0xEFB000FF );
			AddCommand( 0x00004000 );
			Send();
		}

		void Init( void ) 
		{
			MI_REG->mask = 0x0800;
		}

		void Close(void) 
		{
			MI_REG->mask = 0x0400;
		}

		/**
		 * @brief Enable display of 2D filled (untextured) triangles
		 *
		 * This must be called before using #rdp_draw_filled_triangle.
		 */
		void EnableBlend( void )
		{
			AddCommand( 0xEF0000FF );
			AddCommand( 0x80000000 );
			Send();
		}

		void SetPrimitiveColor( uint32_t color )
		{
			/* Set packed color */
			AddCommand( 0xF7000000 );
			AddCommand( color );
			Send();
		}

		void SetBlendColor( uint32_t color )
		{
			AddCommand( 0xF9000000 );
			AddCommand( color );
			Send();
		}

		void DrawRectangle( uint32_t tx, uint32_t ty, uint32_t bx, uint32_t by )
		{
			if( tx < 0 ) { tx = 0; }
			if( ty < 0 ) { ty = 0; }

			AddCommand( 0xF6000000 | ( bx << 14 ) | ( by << 2 ) ); 
			AddCommand( ( tx << 14 ) | ( ty << 2 ) );
			Send();
		}


		void Attach(  )
		{
			uint32_t __bitdepth = BD32BPP;
			
			/* Set the rasterization buffer */
			AddCommand( 0xFF000000 | ((__bitdepth == 2) ? 0x00100000 : 0x00180000) | (global_res.width- 1) );
			AddCommand( FRAMEBUFFER_ADDR );
			Send();
		}

		void Sync( uint32_t sync )
		{
			switch( sync )
			{
				case 0:
					AddCommand( 0xE9000000 );
					break;
				case 1:
					AddCommand( 0xE7000000 );
					break;
				case 2:
					AddCommand( 0xE8000000 );
					break;
				case 3:
					AddCommand( 0xE6000000 );
					break;
			}
			AddCommand( 0x00000000 );
			Send();
		}

		inline void DrawRectangleSetup( uint32_t tx, uint32_t ty, uint32_t bx, uint32_t by, uint32_t color )
		{
			RDP::Attach();
			RDP::SetDefaultClipping();
			RDP::EnableBlend();
			RDP::Sync(1);
			RDP::SetBlendColor(color);
			RDP::Sync(1);
			RDP::DrawRectangle(tx, ty, bx, by);
			RDP::Sync(1);
		}

	}
};
