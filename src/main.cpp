#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Config.h>

#include <ApplicationManager.h>
#include <Display.h>
#include <Wifi.h>
#include <NTP.h>
#include <Config.h>

Wifi wifi;
NTP ntp;
Config& config = Config::getInstance();
ApplicationManager& applicationManager = ApplicationManager::getInstance();
Display& display = Display::getInstance();

void setup() {
    delay(2000);
    Serial.begin(9600); // Serial Port WS d1 mini

    config.setup();

    display.showLogo(); // show on dispaly logo intro
    display.setBrightness(config.data.brightness);
    
    wifi.setup(); // Init Wifi    
    ntp.setup(); // Init NTP Time 
    applicationManager.setup(); // Init Application Manager
}

void loop() {
  wifi.loop();
  applicationManager.loop();
}