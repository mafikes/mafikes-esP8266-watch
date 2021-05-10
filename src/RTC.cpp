#include <RTC.h>
#include "RTClib.h"
#include <time.h>

RTC_DS3231 rtcTime;

void RTC::setNewTime(DateTime time) {
    rtcTime.adjust(time);
}

DateTime RTC::now() {
    return rtcTime.now();
}

void RTC::setup() {
    if (!rtcTime.begin()) {
        Serial.println("Couldn't find RTC");        
    }
 
    if (rtcTime.lostPower()) {
        Serial.println("RTC lost power, waiting for NTP refresh!");
    }
}
