#include <Temperature.h>
#include <Config.h>
#include <Display.h>
#include <Colors.h>
#include <WString.h>
#include <Icons.h>
#include "DHT.h"
#include <Helper.h>

#define DHTPIN 4
#define DHTTYPE DHT11

#define arrayLength(array) (sizeof((array))/sizeof((array)[0]))

DHT dht(DHTPIN, DHTTYPE);

Helper helper;

void Temperature::setup() 
{
    dht.begin();
    pinMode(DHTPIN, INPUT_PULLUP);
}   

void Temperature::showTemperature()
{  
    Display::getInstance().clear(); 
    Display::getInstance().showIcon(ICON_TEMPERATURE, arrayLength(ICON_TEMPERATURE), 0);
    Display::getInstance().drawTextWithIcon(helper.getStringRounded(temperature) + "C", {2, 0}, COLOR_WHITE);
    Display::getInstance().show();
}

void Temperature::showHumidity() 
{
    Display::getInstance().clear();
    Display::getInstance().showIcon(ICON_TEMPERATURE, arrayLength(ICON_TEMPERATURE), 0); 
    Display::getInstance().drawTextWithIcon(String(humidity, 1) + "%", {6, 0}, COLOR_WHITE);
    Display::getInstance().show();
}

void Temperature::showHeatIndex()
{
    Display::getInstance().clear(); 
    Display::getInstance().showIcon(ICON_TEMPERATURE, arrayLength(ICON_TEMPERATURE), 0);
    Display::getInstance().drawTextWithIcon(helper.getStringRounded(heatIndex) + "C", {2, 0}, COLOR_WHITE);
    Display::getInstance().show();
} 

// TODO: calculate it every min.3 seconds
void Temperature::loop() 
{    
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      temperature = 0;
      humidity = 0;
      heatIndex = 0;
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    heatIndex = dht.computeHeatIndex(temperature, humidity, false);
}