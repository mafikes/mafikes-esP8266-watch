#include <DateApp.h>
#include <Config.h>
#include <RTC.h>
#include <Colors.h>

void DateApp::render(Display& display) 
{    
    display.clear();
    DateTime now = RTC::getInstance().now();

    String dateString = String(now.day()) + "." + String(now.month()) + "." + String(now.year()).substring(2,4);
    int dateStringLenght = dateString.length() * 3;
    int16_t positionDate = ((31 - dateStringLenght) / 2);
    display.drawText(dateString, false, {positionDate, 0}, COLOR_WHITE);

    display.show();
}
