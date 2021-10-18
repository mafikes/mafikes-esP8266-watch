#include <Anniversary.h>
#include <Config.h>
#include <RTC.h>
#include <Colors.h>
#include <TimeLib.h>

void Anniversary::beforeRender() {
    DateTime now = RTC::getInstance().now();

    tmElements_t time1 = {0, 0, 0, 0, 1,  10, CalendarYrToTm(1994)}, // 0:00:00 1st Jan 2015
                 time2 = {0, 0, 0, 0, now.day(), now.month(), CalendarYrToTm(now.year())}; // 0:00:00 8th Oct 2015 (today)
    
    uint32_t difference = (uint32_t)(makeTime(time2) - makeTime(time1));
    
    struct elapsedTime_t {
        uint8_t Seconds, Minutes, Hours;
        uint16_t Days;
    } elapsedTime;
    
    elapsedTime.Seconds = difference % 60;
    difference /= 60; // now it is minutes
    
    elapsedTime.Minutes = difference % 60;
    difference /= 60; // now it is hours
    
    elapsedTime.Hours = difference % 24;
    difference /= 24; // now it is days

    elapsedTime.Days = difference;

    days = elapsedTime.Days;
}

void Anniversary::render(Display& display) 
{    
    display.clear();

    display.drawText(String(days), false, {0, 0}, COLOR_WHITE);

    display.show();
}

