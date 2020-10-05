#ifndef TimeApp_h
#define TimeApp_h

#include <WString.h>
#include <Time.h>
#include <TimeLib.h>

class TimeApp 
{
    private:
        int ClockTheme = 1;
        String repairDigit(int);
    public:
        void setup();
        void loop();
        void displayClock();
};

#endif