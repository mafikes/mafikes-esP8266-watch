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

    if(lightLevel < 40) {
        brightness = 2;
    } else if(lightLevel < 60) {
        brightness = 10;
    } else if(lightLevel < 80) {
        brightness = 20;
    } else if(lightLevel < 120) {
        brightness = 30;
    } else if(lightLevel < 200) {
        brightness = 40;
    } else {
        brightness = 60;
    }

    display.setBrightness(brightness);
}

void AutoBrightness::loop()
{
    unsigned long currentMillis = millis();
    
    if ((unsigned long)(currentMillis - prevTime) >= 5000) { // Every 5 seconds
        lightLevel = analogRead(LDR_PIN);

        Serial.print("Auto Light Level: ");
        Serial.println(lightLevel);

        loadBrightness();

        prevTime = currentMillis;
    }
}