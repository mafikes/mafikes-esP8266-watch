#include <TimeApp.h>
#include <Config.h>
#include <RTC.h>

String TimeApp::repairDigit(int digit) 
{
    String repairedDigit = String(digit);
    if(digit < 10) repairedDigit = "0"+repairedDigit;
    return repairedDigit;
}

void TimeApp::beforeRender() {
    clockTheme = Config::getInstance().data.watch_type;
    clockColorActive = Config::getInstance().data.watch_type_color;
    clockColor = getColor();
}

DisplayColor TimeApp::getColor()
{
    DisplayColor watchColor = COLOR_WHITE;

    if(clockColorActive == 1) {
      watchColor = COLOR_RED;
    } else if(clockColorActive == 2) {
      watchColor = COLOR_YELLOW;
    } else if(clockColorActive == 3) {
      watchColor = COLOR_ORANGE;
    } else if(clockColorActive == 4) {
      watchColor = COLOR_BLUE;      
    } else if(clockColorActive == 4) {
      watchColor = COLOR_GREEN;
    } else {
      clockColorActive = 0;
      watchColor = COLOR_WHITE;
    }

    return watchColor;
}

void TimeApp::render(Display& display) 
{    
    display.clear();
    DateTime now = RTC::getInstance().now();

    if(clockTheme == 1) {        
        // display.drawText(repairDigit(hour()) + ":" + repairDigit(minute()) + ":" + repairDigit(second()), false, {2, 0}, clockColor);
        display.drawText(repairDigit(now.hour()) + ":" + repairDigit(now.minute()) + ":" + repairDigit(now.second()), false, {2, 0}, clockColor);
    } else {
        // display.drawText(repairDigit(hour()) + ":" + repairDigit(minute()), false, {8, 0}, clockColor);
        display.drawText(repairDigit(now.hour()) + ":" + repairDigit(now.minute()), false, {8, 0}, clockColor);
        clockTheme = 0;
    }

    display.show();
}

void TimeApp::btn1_process()
{
    clockColorActive++;
    clockColor = getColor();

    Config& config = Config::getInstance();
    config.data.watch_type_color = clockColorActive;
    config.save();
}

void TimeApp::btn3_process()
{
    clockTheme++;
    
    Config& config = Config::getInstance();
    config.data.watch_type = clockTheme;
    config.save();
}
