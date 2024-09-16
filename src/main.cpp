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
    ESP.wdtDisable();
    ESP.wdtEnable(WDTO_8S);

    Serial.begin(115200); // Serial Port WS d1 mini

    display.showLogo();

    config.setup();

    display.setBrightness(config.data.brightness);
    autoBrightness.setup();
    display.showLogo();

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
