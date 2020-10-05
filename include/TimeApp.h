#ifndef TimeApp_h
#define TimeApp_h

#include <WString.h>
#include <Time.h>
#include <TimeLib.h>

class TimeApp 
{
    private:
        int clockTheme = 1;
        String repairDigit(int);
    public:
        void setup();
        void loop();
        void displayClock();
        void changeTheme(int);
};

#endif