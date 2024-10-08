#include <AutoBrightness.h>
#include <Config.h>
#include <Display.h>

#define LDR_PIN A0
const int HIGH_MAX_LIGHT = 1024;

void AutoBrightness::setup() 
{
    lightLevel = analogRead(LDR_PIN);
    loadBrightness();
}   

void AutoBrightness::loadBrightness() 
{        
    Display& display = Display::getInstance();
    int brightness = 0;

    brightness = float((float(lightLevel) / float(1024))*float(100));
    
    if(brightness < 5) {
        brightness = 5;
    }

    display.setBrightness(brightness);
}

void AutoBrightness::loop()
{
    unsigned long currentMillis = millis();
    
    if ((unsigned long)(currentMillis - prevTime) >= 10000) { // Every 10 seconds
        lightLevel = analogRead(LDR_PIN);

        // Serial.print("Auto Light Level: ");
        // Serial.println(lightLevel);    

        loadBrightness();

        prevTime = currentMillis;
    }
}
