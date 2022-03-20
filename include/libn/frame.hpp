
#ifndef LIBN_FRAME_H
#define LIBN_FRAME_H

#include <libn/controller.hpp>
#include <libn/vi.hpp>

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
    Frame(const Display::Resolution res, const Display::Bitdepth bitdepth,
        const Display::AntiAliasing antialiasing);
    Frame() = default;

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
    virtual void KeyJoyXPressed(u32);
    virtual void KeyJoyYPressed(u32);

    void SetScreenClear();
    void ExitFrameLoop();

    u32 ScreenWidth();
    u32 ScreenHeight();
};

} // namespace LibN64

#endif