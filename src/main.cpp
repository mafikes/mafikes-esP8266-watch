#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Config.h>

#include <ApplicationManager.h>
#include <Display.h>
#include <Wifi.h>
#include <TimeApp.h>
#include <WeatherApp.h>
#include <Temperature.h>
#include <TouchButton.h>

Wifi wifi;
// TouchButton touchButton;
// ApplicationManager& applicationManager = ApplicationManager::getInstance();
ApplicationManager applicationManager;

void setup() {
  delay(1000);

  // Reset ESP watchdog
  // ESP.wdtDisable();
  // ESP.wdtEnable(WDTO_8S);

  Serial1.begin(115200);
  Serial.begin(9800); // WS d1 mini
  
  Display::getInstance().showLogo();
  Display::getInstance().showLogo(); // show on dispaly logo intro
  Display::getInstance().clear();

  wifi.setup(); // Init Wifi
  applicationManager.setup(); // Init Application Manager

  // timeApp.setup(); // Init Time
  // weatherApp.setup(); // Init Weather
  // temperature.setup(); // Init Temperature DTH 

}


void loop() {
  Serial.println(ESP.getFreeHeap());
  wifi.loop();
  applicationManager.loop();

  // temperature.loop();
  // temperature.showTemperature();
  // weatherApp.print();
  // timeApp.loop();
}