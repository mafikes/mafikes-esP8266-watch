#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Config.h>

#include <Display.h>
#include <Wifi.h>
#include <TimeApp.h>
#include <WeatherApp.h>

Wifi wifi;
TimeApp timeApp;
WeatherApp weatherApp;

void setup() {
  Serial.begin(115200); // Web Server
  Serial.begin(9800); // WS d1 mini

  Display::getInstance().clear(); 
  Display::getInstance().showLogo(); // show on dispaly logo intro
  
  wifi.setup(); // Init wifi
  timeApp.setup(); // Init time
  weatherApp.setup();
}

void loop() {
  wifi.loop();
  weatherApp.print();
  // timeApp.loop();
}