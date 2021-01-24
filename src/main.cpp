#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Config.h>

#include <ApplicationManager.h>
#include <Display.h>
#include <Wifi.h>
#include <NTP.h>

Wifi wifi;
NTP ntp;
ApplicationManager& applicationManager = ApplicationManager::getInstance();

void setup() {
  delay(2000);
  Serial.begin(9600); // WS d1 mini
  
  Display::getInstance().showLogo();
  Display::getInstance().showLogo(); // show on dispaly logo intro
  Display::getInstance().clear();

  wifi.setup(); // Init Wifi
  ntp.setup(); // Init NTP Time 
  applicationManager.setup(); // Init Application Manager

  // tickerSwitchAppView.start();
}

// void switchAppView() {
  // applicationManager.nextApp();
// }

void loop() {
  // SHOW to serial FREE RAM
  // Serial.println(ESP.getFreeHeap());

  // tickerSwitchAppView.update();

  wifi.loop();
  applicationManager.loop();
}