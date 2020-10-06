#ifndef WeatherApp_h
#define WeatherApp_h

#include <WString.h>

class WeatherApp 
{
    private:
        float temperature = 0;        
        String icon;
    public:
        void setup();
        void askServer();
        void print();
};

#endif