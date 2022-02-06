
#ifndef LIBN_FRAME_H
#define LIBN_FRAME_H

#include <libn/controller.hpp>
#include <libn/vi_display.hpp>

namespace LibN64 {
class Frame {
  private:
    Display::Resolution r;
    Display::Bitdepth bd;
    Display::AntiAliasing aa;
    Display::Gamma g;

    bool bRunning     = false;
    bool bClearScreen = false;

  public:
    constexpr Frame(const Display::Resolution res, const Display::Bitdepth bitdepth,
        const Display::AntiAliasing antialiasing);

    void Begin();

    virtual void FrameUpdate() = 0;
    virtual void OnCreate() = 0;
    virtual void KeyAPressed() = 0;
    virtual void KeyBPressed() = 0;
    virtual void KeyZPressed() = 0;
    virtual void KeyDUpPressed() = 0;
    virtual void KeyDDownPressed() = 0;
    virtual void KeyDLeftPressed() = 0;
    virtual void KeyDRightPressed() = 0;
    virtual void KeyStartPressed() = 0;
    virtual void KeyJoyXPressed(int) = 0;
    virtual void KeyJoyYPressed(int) = 0;

    void SetScreenClear();
    void ExitFrameLoop();

    u32 ScreenWidth();
    u32 ScreenHeight();
};

} // namespace LibN64

#endif