#include <AutoBrightness.h>
#include <Config.h>
#include <Display.h>

#define LDR_PIN A0

void AutoBrightness::setup() 
{
    lightLevel = analogRead(LDR_PIN);
    loadBrightness();
}   

void AutoBrightness::loadBrightness() 
{        
    Display& display = Display::getInstance();
    int brightness = 0;

    if(lightLevel < 80) {
        brightness = 5;
    } else if(lightLevel < 120) {
        brightness = 10;
    } else if(lightLevel < 200) {
        brightness = 30;
    } else if (lightLevel < 300) {
        brightness = 50;
    } else {
        brightness = 70;
    }

    display.setBrightness(brightness);
}

void AutoBrightness::loop()
{
    unsigned long currentMillis = millis();
    
    if ((unsigned long)(currentMillis - prevTime) >= 5000) { // Every 5 seconds
        lightLevel = analogRead(LDR_PIN);
        
        Serial.println(lightLevel);
        
        loadBrightness();

        prevTime = currentMillis;
    }
}