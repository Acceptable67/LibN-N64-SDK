#include <libn_display.h>
#include <libn_controller.h>
#include <libn_frame.h>

namespace LibN64 
{
    static auto cpad_data = reinterpret_cast<Controller::Cpad*>(PIF_RAM);

    Frame::Frame(const Display::Resolution res, const Display::Bitdepth bitdepth, const Display::AntiAliasing antialiasing, const bool DoubleBuffering) 
    : r(res), bd(bitdepth), aa(antialiasing), bDoubleBuf(DoubleBuffering) {}

    void Frame::Begin() 
    { 
        bRunning = true; 
        Display::TextColor local = { Display::LibColor::YELLOW, Display::LibColor::BLACK | 0xFF};

        Display::Initialize(r, bd, aa, g, bDoubleBuf);
        Display::FillScreen(Display::LibColor::GREY_SMOOTH);
        Display::SetColors(local.Foreground, local.Background);
        Controller::SI_WriteController();

        this->OnCreate();

        while(bRunning)
        {
            this->FrameUpdate();

            Controller::SI_ReadController();
            if(cpad_data->A)     { this->KeyAPressed(); }
            if(cpad_data->B)     { this->KeyBPressed(); }
            if(cpad_data->Z)     { this->KeyZPressed(); }
            if(cpad_data->start) { this->KeyStartPressed(); }
            if(cpad_data->up)    { this->KeyDUpPressed(); }
            if(cpad_data->down)  { this->KeyDDownPressed(); }
            if(cpad_data->left)  { this->KeyDLeftPressed(); }
            if(cpad_data->right) { this->KeyDRightPressed(); }
            if(cpad_data->x)     { this->KeyJoyXPressed(*reinterpret_cast<u32*>(cpad_data) & 0x0000FF00);}
            if(cpad_data->y)     { this->KeyJoyYPressed(*reinterpret_cast<u32*>(cpad_data) & 0x000000FF);}

            [[maybe_unused]] auto memcpy = [](auto a, auto b, auto size) 
            {
                for(size_t i = 0; i<size; i++)
                {
                    if(a[i] != b[i]) 
                        a[i] = b[i];
                }
            };

            if(bDoubleBuf) {
                // memcpy(Display::buffer_list[Display::DISPLAY], Display::buffer_list[Display::BACKUP], (Display::global_res.width * Display::global_res.height));
            }
        } 
    }

    void Frame::FrameUpdate(){}
    void Frame::OnCreate(){}
    void Frame::KeyAPressed(){}
    void Frame::KeyBPressed(){}
    void Frame::KeyZPressed(){}
    void Frame::KeyDUpPressed(){}
    void Frame::KeyDDownPressed(){}
    void Frame::KeyDLeftPressed(){}
    void Frame::KeyDRightPressed(){}
    void Frame::KeyStartPressed(){}
    void Frame::KeyJoyXPressed(int){}
    void Frame::KeyJoyYPressed(int){}

    u32 Frame::ScreenWidth() 
    {
        return r.width;
    }

    u32 Frame::ScreenHeight()
    {
        return r.height;
    }

}