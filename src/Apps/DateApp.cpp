#include <DateApp.h>
#include <Config.h>
#include <RTC.h>
#include <Colors.h>

void DateApp::render(Display& display) 
{    
    display.clear();
    DateTime now = RTC::getInstance().now();

    int positionX = 5;
    if(now.month() >= 10) {
        positionX = 2;
    }
    
    display.drawText(String(now.day()) + "." + String(now.month()) + "." + String(now.year()).substring(2,4), false, {positionX, 0}, COLOR_WHITE);

    display.show();
}
