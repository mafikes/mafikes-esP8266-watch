#ifndef RTC_H
#define RTC_H

#include "RTClib.h"

class RTC
{
    private:
        RTC() {}
        RTC(RTC const&);
        void operator = (RTC const&);

    public:        
        static RTC& getInstance() {
            static RTC instance;
            return instance;
        }

        void setNewTime(DateTime);
        DateTime now();
        void setup();
};

#endif