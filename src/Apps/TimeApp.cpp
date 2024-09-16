#include <TimeApp.h>
#include <Config.h>
#include <RTC.h>
#include <ServiceNTP.h>

String TimeApp::repairDigit(int digit) 
{
    String repairedDigit = String(digit);
    if(digit < 10) repairedDigit = "0"+repairedDigit;
    return repairedDigit;
}

void TimeApp::beforeRender() {
    Config& config = Config::getInstance();

    clockTheme = config.data.watch_type;
    clockColorActive = config.data.watch_type_color;
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
    } else if(clockColorActive == 5) {
        watchColor = COLOR_GRAY;
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

    if(Config::getInstance().data.watch_color_custom) {        
        DisplayColor watchCustomColor = {Config::getInstance().data.watch_color[0], Config::getInstance().data.watch_color[1], Config::getInstance().data.watch_color[2]};    
        clockColor = watchCustomColor;         
    } else {
        clockColor = getColor();
    }    

    if(now.hour() == 165) { // BAD SYNC, LOW BATTERY, ETC.
        // ServiceNTP::getInstance().updateTime();
    }

    if(clockTheme == 1) {        
        display.drawText(repairDigit(now.hour()) + ":" + repairDigit(now.minute()) + ":" + repairDigit(now.second()), false, {2, 0}, clockColor);
    } else if(clockTheme == 2) {       
        display.drawText(repairDigit(now.hour()) + ":" + repairDigit(now.minute()), false, {8, 0}, clockColor);      
    } else if(clockTheme == 3) {
        DisplayColor headerColor = clockColor;

        if(headerColor.red == COLOR_WHITE.red && headerColor.blue == COLOR_WHITE.blue && headerColor.green == COLOR_WHITE.green) {
            headerColor = COLOR_RED;
        }

        display.drawLine(0, 0, 8, 0, headerColor);
        display.drawLine(0, 1, 8, 1, headerColor);
        display.drawLine(0, 2, 8, 2, COLOR_WHITE);
        display.drawLine(0, 3, 8, 3, COLOR_WHITE);
        display.drawLine(0, 4, 8, 4, COLOR_WHITE);
        display.drawLine(0, 5, 8, 5, COLOR_WHITE);
        display.drawLine(0, 6, 8, 6, COLOR_WHITE);
        display.drawLine(0, 7, 8, 7, COLOR_WHITE);

        int16_t positionDay = now.day() >= 10 ? 1 : 3;
        display.drawText(String(now.day()), false, {positionDay, 1}, COLOR_BLACK);

        display.drawText(repairDigit(now.hour()) + ":" + repairDigit(now.minute()), false, {12, 0}, clockColor);
    } else if(clockTheme == 4) {
        display.drawText(repairDigit(now.hour()) + ":" + repairDigit(now.minute()), false, {8, 0}, clockColor);

        // Print Day of Week under clock
        int offset = 0;
        for(int day = 0; day <= 6; day++) {
            display.drawLine(2 + offset, 7, 4 + offset, 7, COLOR_WHITE);
            offset += 4;      
        }

        // Draw active day of week
        // now.dayOfTheWeek() Day of week as an integer from 0 (Sunday) to 6 (Saturday).    
        int activeOffset = now.dayOfTheWeek() == 0 ?  (6 * 4) : 4 - (now.dayOfTheWeek() * 4);
        display.drawLine(2 + activeOffset, 7, 4 +activeOffset, 7, clockColor);
    } else {
        DisplayColor headerColor = clockColor;

        if(headerColor.red == COLOR_WHITE.red && headerColor.blue == COLOR_WHITE.blue && headerColor.green == COLOR_WHITE.green) {
            headerColor = COLOR_RED;
        }

        display.drawLine(0, 0, 8, 0, headerColor);
        display.drawLine(0, 1, 8, 1, headerColor);
        display.drawLine(0, 2, 8, 2, COLOR_WHITE);
        display.drawLine(0, 3, 8, 3, COLOR_WHITE);
        display.drawLine(0, 4, 8, 4, COLOR_WHITE);
        display.drawLine(0, 5, 8, 5, COLOR_WHITE);
        display.drawLine(0, 6, 8, 6, COLOR_WHITE);
        display.drawLine(0, 7, 8, 7, COLOR_WHITE);

        int16_t positionDay = now.day() >= 10 ? 1 : 3;
        display.drawText(String(now.day()), false, {positionDay, 1}, COLOR_BLACK);

        display.drawText(repairDigit(now.hour()) + ":" + repairDigit(now.minute()), false, {13, 0}, clockColor);

        // Print Days of Week under clock
        int offset = 9;
        for(int day = 0; day <= 6; day++) {
            display.drawLine(2 + offset, 7, 3 + offset, 7, COLOR_WHITE);
            offset += 3;      
        }

        // Draw active day of week
        // now.dayOfTheWeek() Day of week as an integer from 0 (Sunday) to 6 (Saturday).    
        int activeOffset = now.dayOfTheWeek() == 0 ? 9 + (6 * 3) : 6 + (now.dayOfTheWeek() * 3);
        display.drawLine(2 + activeOffset, 7, 3 + activeOffset, 7, headerColor);

        clockTheme = 0;
    }

    display.show();
}

void TimeApp::btn1_process()
{
    clockColorActive++;
    clockColor = getColor();

    Config& config = Config::getInstance();
    config.data.watch_color_custom = false;
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
