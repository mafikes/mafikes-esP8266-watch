#ifndef Temperature_h
#define Temperature_h

#include <WString.h>

class Temperature 
{
    private:
        float temperature = 0;
        int humidity = 0;    
        float heatIndex = 0;
    public:
        void setup();
        void showTemperature();
        void showHumidity();
        void showHeatIndex();        
        void loop();
};

#endif