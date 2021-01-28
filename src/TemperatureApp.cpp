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

    showIcon = "";
    lastShowedIcon = "";
    Display::getInstance().resetIconAnimation();
    Display::getInstance().resetLoading();
}

void TemperatureApp::showTemperature(Display& display)
{  
    display.clear();
    
    if (sensorLoad) {
        if(temperature >= 20) {
            display.showAnimateIcon(ICON_TEMPERATURE_RED, arrayLength(ICON_TEMPERATURE_RED));
            showIcon = "temperature_red";
        } else if(temperature < 20) {
            display.showAnimateIcon(ICON_TEMPERATURE_BLUE, arrayLength(ICON_TEMPERATURE_BLUE));            
            showIcon = "temperature_blue";
        }

        display.drawTextWithIcon(Helper::getInstance().getStringRounded(temperature, 5, 0) + "C", {0, 0}, COLOR_WHITE);
    } else {
        display.showLoading();
    }   

    if(lastShowedIcon != showIcon) {
        display.resetIconAnimation();
    } 

    lastShowedIcon = showIcon;
    display.show();
}

void TemperatureApp::showHumidity(Display& display) 
{
    display.clear();
    
     if (sensorLoad) {
        if(humidity >= 39 && humidity <= 60) {        
            display.showAnimateIcon(ICON_SMILE_GOOD, arrayLength(ICON_SMILE_GOOD));
            showIcon = "smile_good";
        } else {
            display.showAnimateIcon(ICON_SMILE_BAD, arrayLength(ICON_SMILE_BAD));
            showIcon = "smile_bad";
        }

        display.drawTextWithIcon(Helper::getInstance().getStringRounded(humidity, 5, 0) + "%", {0, 0}, COLOR_WHITE);

        if(lastShowedIcon != showIcon) {
            display.resetIconAnimation();
        }
        
        lastShowedIcon = showIcon;
    } else {
        display.showLoading();
    }

    display.show();
}

void TemperatureApp::showHeatIndex(Display& display)
{
    display.clear(); 
    display.drawTextWithIcon(Helper::getInstance().getStringRounded(heatIndex, 5, 1) + "C", {2, 0}, COLOR_WHITE);
    display.show();
} 

void TemperatureApp::readData() 
{    
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();

    if (isnan(humidity) || isnan(temperature)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      sensorLoad = false;
      heatIndex = 0;
      temperature = 0;
      humidity = 0;
      return;
    }

    sensorLoad = true;
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