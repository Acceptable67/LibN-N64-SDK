
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

    void SetScreenClear();
    void ExitFrameLoop();

    u32 ScreenWidth();
    u32 ScreenHeight();
};

} // namespace LibN64

#endif