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

void Temperature::beforeRender()
{
    dht.begin();
    pinMode(DHTPIN, INPUT_PULLUP);
}

void Temperature::showTemperature(Display& display)
{  
    display.clear(); 
    display.showIcon(ICON_TEMPERATURE, arrayLength(ICON_TEMPERATURE), 0);
    display.drawTextWithIcon(Helper::getInstance().getStringRounded(temperature) + "C", {2, 0}, COLOR_WHITE);
    display.show();
}

void Temperature::showHumidity(Display& display) 
{
    display.clear();
    display.showIcon(ICON_TEMPERATURE, arrayLength(ICON_TEMPERATURE), 0); 
    display.drawTextWithIcon(Helper::getInstance().getStringRounded(humidity) + "%", {2, 0}, COLOR_WHITE);
    display.show();
}

void Temperature::showHeatIndex(Display& display)
{
    display.clear(); 
    display.showIcon(ICON_TEMPERATURE, arrayLength(ICON_TEMPERATURE), 0);
    display.drawTextWithIcon(Helper::getInstance().getStringRounded(heatIndex) + "C", {2, 0}, COLOR_WHITE);
    display.show();
} 

// TODO: calculate it every min.3 seconds
void Temperature::readData() 
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

void Temperature::btn1_process()
{
    view++;
}

void Temperature::render(Display& display) 
{
    readData();

    if(view == 1) {
        showHeatIndex(display);
    } else if(view == 2) {
        showHumidity(display);
    } else {
        showTemperature(display);
        view = 0;
    }
}