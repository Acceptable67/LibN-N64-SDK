
bool running = true;

namespace LibN64 
{
    class Frame
    {
    private:
        Display::Resolution r;
        bool bRunning = false;

    public:
        Frame(Display::Resolution res) : r(res) {}

        void Begin() 
        { 
            bRunning = true; 

            Display::Initialize(r);
			Display::FillScreen(0x202020FF);
			Display::SetColors(0x20DF00FF, 0xFF0000FF);
			Controller::SI_WriteController();

            this->OnCreate();

            while(bRunning)
            {
               this->FrameUpdate();

                Controller::SI_ReadController();
                if(cpad_data->A) { this->KeyAPressed(); }
                if(cpad_data->B) { this->KeyBPressed(); }
                if(cpad_data->Z) { this->KeyZPressed(); }
                if(cpad_data->x) { this->KeyJoyXPressed(*reinterpret_cast<uint32_t*>(cpad_data) & 0x0000FF00);}
                if(cpad_data->y) { this->KeyJoyYPressed(*reinterpret_cast<uint32_t*>(cpad_data) & 0x000000FF);}

            } 
        }

        virtual void FrameUpdate(){}
        virtual void OnCreate(){}
        virtual void KeyAPressed(){}
        virtual void KeyBPressed(){}
        virtual void KeyZPressed(){}
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