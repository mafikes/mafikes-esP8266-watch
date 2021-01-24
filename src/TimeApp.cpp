#include <TimeApp.h>
#include <Config.h>

#include <time.h>
#include <TimeLib.h>

String TimeApp::repairDigit(int digit) 
{
    String repairedDigit = String(digit);
    if(digit < 10) repairedDigit = "0"+repairedDigit;
    return repairedDigit;
}

void TimeApp::beforeRender() {
}

void TimeApp::displayClock(Display& display)
{
    display.clear();

    if(clockTheme == 1) {
        display.drawText(repairDigit(hour()) + ":" + repairDigit(minute()) + ":" + repairDigit(second()), false, {2, 0}, clockColor);
    } else {
        display.drawText(repairDigit(hour()) + ":" + repairDigit(minute()), false, {8, 0}, clockColor);
        clockTheme = 0;
    }
}

void TimeApp::render(Display& display) 
{
    displayClock(display);
}

void TimeApp::btn1_process()
{
    clockColorActive++;

    if(clockColorActive == 1) {
      clockColor = COLOR_RED;
    } else if(clockColorActive == 2) {
      clockColor = COLOR_YELLOW;
    } else if(clockColorActive == 3) {
      clockColor = COLOR_ORANGE;
    } else if(clockColorActive == 4) {
      clockColor = COLOR_BLUE;      
    } else if(clockColorActive == 4) {
      clockColor = COLOR_GREEN;
    } else {
      clockColorActive = 0;
      clockColor = COLOR_WHITE;
    }
}

void TimeApp::btn3_process()
{
    clockTheme++;
}