#include <c++/11.2.0/cstdlib>
#include <c++/11.2.0/cstdio>
#include <c++/11.2.0/cstdint>
#include <c++/11.2.0/cstdbool>
#include <c++/11.2.0/cassert>

#include <any>

#pragma GCC diagnostic ignored "-Wdeprecated-enum-enum-conversion"
#pragma GCC diagnostic ignored "-Wunused-parameter"


namespace LibN64
{
    namespace Controller 
    {
        template<class VoidType> requires std::is_pointer<VoidType>::value void SI_Write(VoidType dram_address);
        template<class VoidType> requires std::is_pointer<VoidType>::value void SI_Read(VoidType dram_address);
        void SI_WriteController();
        void SI_ReadController();
    }

    namespace DMA {
        void Wait();
        template<class T>
        requires std::is_copy_assignable<T>::value && std::copyable<T>
        void Read(T RAM, int CART, int length);

        template<class T, class B>
        requires std::is_copy_assignable<T>::value && std::copyable<T>
              && std::is_copy_assignable<B>::value && std::copyable<B>
        void Write(T RAM, B CART, int length); 
    }

    namespace Display {
        struct Resolution
        {
            uint32_t width; 
            uint32_t height;
        } global_res;

        struct TextColor
        {
            static uint32_t Foreground;
            static uint32_t Background;
        };
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

        constexpr int MakeColor(int r, int g, int b, int a) 
        {
            return (r << 24) 		   | 
            (((g & 0x00FFFFFF) << 16)) |
            (((b & 0xFF00FFFF) << 8))  | 
            ((a & 0xFFFF00FF));
        }

        enum LibColor 
        {
            RED			    = MakeColor(0xFF, 0x00, 0x00, 0xFF),
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
            GREY_SMOOTH	    = MakeColor(0x20, 0x20, 0x20, 0xFF),
            PURPLE			= MakeColor(0xFF, 0x00, 0x9B, 0xFF)
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
        LibPos cPos = {10,10};

        template<class SpecifiedType>
        requires std::integral<SpecifiedType> || std::floating_point<SpecifiedType>
        struct Lib2DVec
        {
            public:
                SpecifiedType first_element;
                SpecifiedType second_element;
                
        };
    	std::array<int*, 2> buffer_list;
        int *active_buffer;
        void Initialize(Resolution res, Bitdepth bd, AntiAliasing aa, Gamma gamma = GAMMA_OFF, bool dBuf = true);
        void SetActiveBuffer(int num);
        void SetVI_DRAM(const std::any addr);
        bool bFilling = false;
        void FillScreen(auto color);
        void SetColors(uint32_t foreground, uint32_t background);
        void DrawRect(uint32_t x, uint32_t y, auto xd, auto yd, auto color);
        void DrawPixel(uint32_t x, uint32_t y, auto color);
        void DrawText(uint32_t x, uint32_t y, const std::string text);
        void DrawCharacter(uint32_t x, uint32_t y, unsigned char ch);
        template<class T, class ...Args>
	    requires (!std::is_member_function_pointer<T>::value) || (!std::is_member_function_pointer<Args...>::value)
	    void DrawTextFormat(uint32_t x, uint32_t y,  const std::string format, T arg, Args... args);
            
    }
}

#include "libn_font.h"
#include "libn_regs.h"
#include "libn_controller.h"

#include "libn_dma_pi.h"
#include "libn_frame.h"
#include "libn_display.h"
#include "libn_sprite.h"
#include "libn_stdlib.h"
#include "libn_timer.h"
