
bool running = true;

namespace LibN64 
{
    class Frame
    {
    private:
        Display::resolution_t r;
        bool bRunning = false;

    public:
        Frame(Display::resolution_t res) : r(res) {}

        void Begin() 
        { 
            bRunning = true; 	
            this->OnCreate();

            while(bRunning)
            {
                this->FrameUpdate();
            } 
        }

        virtual void FrameUpdate(){}
        virtual void OnCreate(){}
        
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