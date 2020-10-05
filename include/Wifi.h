#ifndef Wifi_h
#define Wifi_h

#include <WString.h>

class Wifi 
{
    private:
        String ipAddress;
    public:
        void setup();
        void loop();
};

#endif