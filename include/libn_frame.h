
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
        Frame(const Display::Resolution res, const Display::Bitdepth bitdepth, const Display::AntiAliasing antialiasing, const bool DoubleBuffering = true) 
        : r(res), bd(bitdepth), aa(antialiasing), bDoubleBuf(DoubleBuffering) {}

        void Begin() 
        { 
            bRunning = true; 

            Display::Initialize(r, bd, aa, g, bDoubleBuf);
			Display::FillScreen(Display::LibColor::GREY_SMOOTH);
			Display::SetColors(Display::TextColor::Foreground, Display::TextColor::Background);
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
                if(cpad_data->x)     { this->KeyJoyXPressed(*reinterpret_cast<uint32_t*>(cpad_data) & 0x0000FF00);}
                if(cpad_data->y)     { this->KeyJoyYPressed(*reinterpret_cast<uint32_t*>(cpad_data) & 0x000000FF);}
                
                Display::cPos.x = 10;
                Display::cPos.y = 10;

                auto memcpy = [](auto a, auto b, auto size) 
                {
                    for(size_t i = 0; i<size; i++ )
                    {
                        if(a[i] != b[i]) 
                            a[i] = b[i];
                    }
                };

                if(bDoubleBuf) {
                    memcpy(Display::buffer_list[0], Display::buffer_list[1], (Display::global_res.width * Display::global_res.height));
                }
            } 
        }

        virtual void FrameUpdate(){}
        virtual void OnCreate(){}
        virtual void KeyAPressed(){}
        virtual void KeyBPressed(){}
        virtual void KeyZPressed(){}
        virtual void KeyDUpPressed(){}
        virtual void KeyDDownPressed(){}
        virtual void KeyDLeftPressed(){}
        virtual void KeyDRightPressed(){}
        virtual void KeyStartPressed(){}
        virtual void KeyJoyXPressed(int){}
        virtual void KeyJoyYPressed(int){}

        uint32_t ScreenWidth() 
        {
            return r.width;
        }

        uint32_t ScreenHeight()
        {
            return r.height;
        }
    };

}