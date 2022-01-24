
namespace LibN64 
{
    class Frame
    {
    private:
        Display::Resolution     r;
        Display::Bitdepth       bd;
        Display::AntiAliasing   aa;
        Display::Gamma          g;
        bool bRunning = false;

    public:
        Frame(Display::Resolution res, Display::Bitdepth bitdepth, Display::AntiAliasing antialiasing) 
        : r(res), bd(bitdepth), aa(antialiasing) {}

        void Begin() 
        { 
            bRunning = true; 

            Display::Initialize(r, bd, aa);
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