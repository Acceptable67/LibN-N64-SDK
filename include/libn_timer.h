namespace LibN64::Timer 
{    
    auto MillisecondsSinceStartup()
    {
        unsigned long long ticks = 0;
        asm("mfc0 %0, $9\n" : "=r"(ticks));
        return ticks / ((93750000/2) / 1000); //take vr4300 clock speed and divide by two
    }

    float SecondsSinceStartup() 
    {
        return MillisecondsSinceStartup()  * 0.001;
    }

    enum TimerType 
     {
        ONE_SHOT,
        CONTINUOUS_CALL,
        TIMER
     };

    /*Essentially a version of multithreading.
      Specified code is called every X milliseconds the user requests it to.
      Can stop, start, reset.*/
    class LibTimer 
    {
    private:
        TimerType localType = CONTINUOUS_CALL;
        bool  bCalled  = false;
        bool  bStarted = false;

        float ticks;
        float execute_at;

        std::function<void()> localfunc;
    public:
        LibTimer(){}
        LibTimer(TimerType type, float tickExecution = 0) : localType(type), execute_at(tickExecution){} 
        float GetSecondsPassed() 
        {
            return ticks;
        }

        float GetMinutesPassed() 
        {
            return ticks / 60;
        }

        float GetMsPassed() 
        {
            return (ticks * 1000);
        }

        /*Optional function that would be called every second
          We could just probe the COP0 for the time and subtract from 
          that to get our value but this will do.*/
        void Update(std::function<void()> callback = []{}) 
        {
            auto TimerCheckAndInc = [&](bool bSetCall) 
            {
                this->ticks += (Display::bFilling) ? 0.1 : 0.05;

                bCalled = bSetCall;
                
                if(localType == TimerType::ONE_SHOT && ticks >= execute_at) 
                {
                    callback();
                    bStarted = false;
                    this->ResetTicks();
                    return;
                } 
                else if(localType == TimerType::CONTINUOUS_CALL && ticks >= execute_at)
                {
                    callback();
                    this->ResetTicks();
                }
            }; 

            if(bStarted) 
            {
                /*there must be a better way to optimize this, but will figure out later*/
				if(((Timer::MillisecondsSinceStartup() / 10) % 2) == 0) 
				{
					if(!bCalled) 
					{
						TimerCheckAndInc(true);
					}
				} 
				else 
                {
					if(bCalled) 
                    {
                        TimerCheckAndInc(false);
					}
				}
		
			}
        }

        void ResetTicks()
        {
            ticks = 0;
        }

        void Start() 
        {
            bStarted = true;
        }

        void Stop() {
            bStarted = false;
        }
    };
};