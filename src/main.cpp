#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Config.h>
#include <ApplicationManager.h>
#include <Display.h>
#include <Wifi.h>
#include <AutoBrightness.h>
#include <RTC.h>
#include <ServiceNTP.h>

Wifi wifi;
Config &config = Config::getInstance();
ServiceNTP &ntp = ServiceNTP::getInstance();
ApplicationManager &applicationManager = ApplicationManager::getInstance();
Display &display = Display::getInstance();
AutoBrightness autoBrightness;
RTC &rtc = RTC::getInstance();

void setup()
{
    // delay(2000);
    Serial.begin(512000); // Serial Port WS d1 mini
    config.setup();

    display.setBrightness(100);
    display.showLogo(); // show on dispaly logo intro
    
    display.setBrightness(config.data.brightness);
    autoBrightness.setup();
    display.showLogo(); // show on dispaly logo intro

    wifi.setup();               // Init Wifi
    rtc.setup();                // Init RTC
    ntp.setup();                // Init NTP Time
    applicationManager.setup(); // Init Application Manager
}

void loop()
{
    wifi.loop();
    ntp.loop();

    if (config.data.brightness_auto)
    {
        autoBrightness.loop();
    }

    applicationManager.loop();
}
