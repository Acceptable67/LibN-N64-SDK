#ifndef LIBN_DISPLAY_H
#define LIBN_DISPLAY_H

/*handle everything visual*/
#include <stdarg.h>
#include <array>
#include <malloc.h>
#include <libn_types.h>
#include <any>
#include <string>

namespace LibN64
{
	 namespace Display 
    {
        struct Resolution
        {
            u32 width; 
            u32 height;
        };

        struct TextColor
        {
            u32 Foreground;
            u32 Background;
        };

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

        constexpr uint32_t MakeColor(u32 r, u32 g, u32 b, u32 a) 
        {
            return (r << 24) 		   | 
            (((g & 0x00FFFFFF) << 16)) |
            (((b & 0xFF00FFFF) << 8))  | 
            ((a & 0xFFFF00FF));
        }

        enum Buffer 
        {
            DISPLAY = 0,
            BACKUP,
            EXTRA
        };

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

        struct LibPos3D 
        {
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
            u32 x, y; 
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
        [[maybe_unused]] static LibPos cPos;

        template<class SpecifiedType>
        requires std::integral<SpecifiedType> || std::floating_point<SpecifiedType>
        struct Lib2DVec
        {
            public:
                SpecifiedType first_element;
                SpecifiedType second_element;
                
        };
        

        auto GetBuffer(Buffer);
        void SetActiveBuffer(const int num);
        void checkVI_Int();
        auto SetVI_Int(auto);
        void SetVI_DRAM(const std::any addr);
        void Initialize(Resolution res, Bitdepth bd, AntiAliasing aa, Gamma gamma = GAMMA_OFF, bool dBuf = true);
        void FillScreen(u32 color);
        void SetColors(const u32 foreground, const u32 background);
        void DrawRect(LibPos, const auto xd, const auto yd, const auto color);
        void DrawPixel(LibPos, const u32 color);
        void DrawText(LibPos, const std::string text);
        void DrawCharacter(const LibPos, const unsigned char ch);
        template<class T, class ...Args>
	    requires (!std::is_member_function_pointer<T>::value) || (!std::is_member_function_pointer<Args...>::value)
	    void DrawTextFormat(const LibPos,  const std::string format, T arg, Args... args);

        namespace RDP 
        {
            void AddCommand(u32);
            void Send();
            void Sync();
            void Attach();
            void EnableBlend();
            void EnablePrimitive();
            void SetDefaultClipping();
            void SetClipping(u32, u32, u32, u32);
            void SetBlendColor(const auto);
            void SetPrimitiveColor(const auto);
            void DrawRectangle(u32, u32, u32, u32);
            void DrawRectangleSetup(u32 tx, u32 ty, u32 bx, u32 by, auto color);
            void Close();
            //void ClearScreen(const auto color);
			void ClearScreen(u32 color);
            void Init();
        }
    }
};

#endif