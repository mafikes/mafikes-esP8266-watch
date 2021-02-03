#ifndef AutoBrightness_h
#define AutoBrightness_h

#include <WString.h>

class AutoBrightness 
{
    private:
        int lightLevel = 0;    
        int lastLightLevel = 0;
        
        unsigned long prevTime = 0;

        void loadBrightness();    
    public:
        void setup();
        void loop();
};

#endif