#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Config.h>

#include <Display.h>
#include <Wifi.h>
#include <TimeApp.h>

Wifi wifi;
TimeApp timeApp;

void setup() {
  Serial.begin(115200); // Web Server
  Serial.begin(9800); // WS d1 mini

  Display::getInstance().clear(); 
  Display::getInstance().showLogo(); // show on dispaly logo intro
  
  wifi.setup(); // Init wifi
  timeApp.setup(); // Init time
}

void loop() {
  
  // Display::getInstance().clear();
  wifi.loop();
  timeApp.loop();

  // drawText("MAFIKES", true, 0, 0);
}