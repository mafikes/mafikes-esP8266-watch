#include <TemperatureApp.h>
#include <Config.h>
#include <Display.h>
#include <Colors.h>
#include <WString.h>
#include <Icons.h>
#include "DHT.h"
#include <Helper.h>

#define DHTPIN 14
#define DHTTYPE DHT11

#define arrayLength(array) (sizeof((array))/sizeof((array)[0]))

DHT dht(DHTPIN, DHTTYPE);

TemperatureApp::TemperatureApp(int showView)
{
    view = showView;
}

void TemperatureApp::beforeRender()
{
    dht.begin();
    pinMode(DHTPIN, INPUT_PULLUP);
}

void TemperatureApp::showTemperature(Display& display)
{  
    display.clear(); 
    display.showAnimateIcon(ICON_TEMPERATURE, arrayLength(ICON_TEMPERATURE));
    display.drawTextWithIcon(Helper::getInstance().getStringRounded(temperature, 5, 1) + "C", {2, 0}, COLOR_WHITE);
    display.show();
}

void TemperatureApp::showHumidity(Display& display) 
{
    display.clear();
    display.showAnimateIcon(ICON_TEMPERATURE, arrayLength(ICON_TEMPERATURE));
    display.drawTextWithIcon(Helper::getInstance().getStringRounded(humidity, 5, 1) + "%", {2, 0}, COLOR_WHITE);
    display.show();
}

void TemperatureApp::showHeatIndex(Display& display)
{
    display.clear(); 
    display.showAnimateIcon(ICON_TEMPERATURE, arrayLength(ICON_TEMPERATURE));
    display.drawTextWithIcon(Helper::getInstance().getStringRounded(heatIndex, 5, 1) + "C", {2, 0}, COLOR_WHITE);
    display.show();
} 

void TemperatureApp::readData() 
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

void TemperatureApp::render(Display& display) 
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