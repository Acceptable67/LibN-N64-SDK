
#ifndef LIBN_FRAME_H
#define LIBN_FRAME_H

#include <libn_controller.h>
#include <libn_display.h>

namespace LibN64 
{
    class Frame
    {
    private:
        Display::Resolution     r;
        Display::Bitdepth       bd;
        Display::AntiAliasing   aa;
        Display::Gamma          g;
        bool bDoubleBuf;
        bool bRunning = false;

    public:
        Frame(const Display::Resolution res, const Display::Bitdepth bitdepth, const Display::AntiAliasing antialiasing, const bool DoubleBuffering = false);

        void Begin();

        virtual void FrameUpdate();
        virtual void OnCreate();
        virtual void KeyAPressed();
        virtual void KeyBPressed();
        virtual void KeyZPressed();
        virtual void KeyDUpPressed();
        virtual void KeyDDownPressed();
        virtual void KeyDLeftPressed();
        virtual void KeyDRightPressed();
        virtual void KeyStartPressed();
        virtual void KeyJoyXPressed(int);
        virtual void KeyJoyYPressed(int);

        u32 ScreenWidth();
        u32 ScreenHeight();
    };

}

#endif