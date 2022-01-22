namespace LibN64::Timer 
{    
    auto MillisecondsSinceStartup()
    {
        unsigned long long ticks;
        asm("mfc0 %0, $9" : "=r"(ticks));
        return ticks / ((93750000/2) / 1000);
    }

    auto SecondsSinceStartup() 
    {
        return MillisecondsSinceStartup() / 1000;
    }

    class LibTimer 
    {
    private:
        bool called  = false;
        bool started = false;
        uint32_t ticks;
        std::function<void()> localfunc;
    public:

        uint32_t GetSecondsPassed() 
        {
            return ticks;
        }

        uint32_t GetMsPassed() 
        {
            return (ticks * 1000);
        }

        /*Optional function that would be called every second*/
        void Update(std::function<void()> callback = []{}) 
        {
            if(started) 
            {
				if(Timer::SecondsSinceStartup() % 2 == 0) 
				{
					if(!called) 
					{
						ticks+=1;
						called = true;
                        callback();
					}
				} 
				else 
                {
					if(called) 
                    {
						ticks+=1;
						called = false;
                        callback();
					}
				}
		
			}
        }

        void Start() 
        {
            started = true;
        }
    };
};