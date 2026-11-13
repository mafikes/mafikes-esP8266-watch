#include <TimeApp.h>
#include <Config.h>
#include <RTC.h>
#include <ServiceNTP.h>

#define DIGIT_ANIMATION_TIME 300 // ms
#define DIGIT_WIDTH 4 // AwtrixFont digit advance
#define COLON_WIDTH 2 // AwtrixFont colon advance

void TimeApp::drawTime(Display& display, const DateTime& now, int16_t x, bool showSeconds)
{
    char digits[6] = {
        (char)('0' + now.hour() / 10), (char)('0' + now.hour() % 10),
        (char)('0' + now.minute() / 10), (char)('0' + now.minute() % 10),
        (char)('0' + now.second() / 10), (char)('0' + now.second() % 10)
    };

    // X position of each digit in "HH:MM:SS"
    const int16_t digitX[6] = {
        x, (int16_t)(x + DIGIT_WIDTH),
        (int16_t)(x + 2 * DIGIT_WIDTH + COLON_WIDTH), (int16_t)(x + 3 * DIGIT_WIDTH + COLON_WIDTH),
        (int16_t)(x + 4 * DIGIT_WIDTH + 2 * COLON_WIDTH), (int16_t)(x + 5 * DIGIT_WIDTH + 2 * COLON_WIDTH)
    };

    int digitCount = showSeconds ? 6 : 4;

    unsigned long currentMillis = millis();

    // First frame after app start is drawn without animation
    if(!digitsInitialized) {
        digitsInitialized = true;
        memcpy(currentDigits, digits, sizeof(currentDigits));
        memcpy(previousDigits, digits, sizeof(previousDigits));
        for(int i = 0; i < 6; i++) {
            digitChangeTime[i] = currentMillis - DIGIT_ANIMATION_TIME;
        }
    }

    for(int i = 0; i < digitCount; i++) {
        if(digits[i] != currentDigits[i]) {
            previousDigits[i] = currentDigits[i];
            currentDigits[i] = digits[i];
            digitChangeTime[i] = currentMillis;
        }

        unsigned long elapsed = currentMillis - digitChangeTime[i];

        if(elapsed >= DIGIT_ANIMATION_TIME) {
            display.drawChar(currentDigits[i], {digitX[i], 0}, clockColor);
            continue;
        }

        // Ease out, fast start and slow end
        float progress = 1.0f - (float)elapsed / DIGIT_ANIMATION_TIME;
        progress = 1.0f - progress * progress * progress;
        int16_t shift = (int16_t)(progress * 8);

        display.drawChar(previousDigits[i], {digitX[i], shift}, clockColor);
        display.drawChar(currentDigits[i], {digitX[i], (int16_t)(shift - 8)}, clockColor);
    }

    display.drawChar(':', {(int16_t)(x + 2 * DIGIT_WIDTH), 0}, clockColor);

    if(showSeconds) {
        display.drawChar(':', {(int16_t)(x + 4 * DIGIT_WIDTH + COLON_WIDTH), 0}, clockColor);
    }
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
        drawTime(display, now, 2, true);
    } else if(clockTheme == 2) {       
        drawTime(display, now, 8, false);      
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

        drawTime(display, now, 12, false);
    } else if(clockTheme == 4) {
        drawTime(display, now, 8, false);

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

        drawTime(display, now, 13, false);

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
