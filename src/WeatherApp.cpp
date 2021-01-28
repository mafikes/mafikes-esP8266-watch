#include <WeatherApp.h>
#include <Config.h>
#include <Display.h>
#include <Colors.h>
#include <WString.h>
#include <Icons.h>
#include <Helper.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define arrayLength(array) (sizeof((array))/sizeof((array)[0]))

void WeatherApp::showIcon(Display& display) 
{
    if(lastTemperatureIcon != temperatureIcon) {
        display.resetIconAnimation();
    } 

    if(temperatureIcon == "01") {
        display.showAnimateIcon(ICON_SUN, arrayLength(ICON_SUN));
    } else if (temperatureIcon == "02") {      
        display.showAnimateIcon(ICON_PARTLYCLOUD, arrayLength(ICON_PARTLYCLOUD));
    } else if (temperatureIcon == "03" || temperatureIcon == "04") {      
        display.showAnimateIcon(ICON_CLOUDY, arrayLength(ICON_CLOUDY));
    } else if (temperatureIcon == "09" || temperatureIcon == "10") {
        display.showAnimateIcon(ICON_RAIN, arrayLength(ICON_RAIN));
    } else if (temperatureIcon == "11") {
        display.showAnimateIcon(ICON_THUNDER, arrayLength(ICON_THUNDER));
    } else if (temperatureIcon == "13") {  
        display.showAnimateIcon(ICON_SNOW, arrayLength(ICON_SNOW));
    } else if (temperatureIcon == "50") {
        display.showAnimateIcon(ICON_MIST, arrayLength(ICON_MIST));
    } else {
        display.showAnimateIcon(ICON_SUN, arrayLength(ICON_SUN));
    }
    
    lastTemperatureIcon = temperatureIcon;
}

void WeatherApp::askServer() 
{
    DynamicJsonDocument doc(1200);
    HTTPClient http;
    WiFiClient client;

    http.begin(("http://api.openweathermap.org/data/2.5/weather?id=") + API_WEATHER_LOCATION +("&appid=")+ API_WEATHER_KEY +("&units=metric"));

    int httpCode = http.GET();
    Serial.println(httpCode);

    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {      
            String payload = http.getString();
            DeserializationError error = deserializeJson(doc, payload);

            if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.c_str());
                return;
            }
            
            temperature = doc["main"]["temp"];     
            const char* icon = doc["weather"][0]["icon"];
            temperatureIcon = icon;
            temperatureIcon.remove(2, 1);

            downloaded = true;
            Display::getInstance().resetIconAnimation();
        }        
    }

    http.end();
}

void WeatherApp::beforeRender() 
{
    Display::getInstance().resetIconAnimation();
    Display::getInstance().resetLoading();
}

void WeatherApp::render(Display& display) 
{
    display.clear();

    if(!downloaded) {
        askServer();        
        display.showLoading();
    } else {    
        showIcon(display);              
        display.drawTextWithIcon(Helper::getInstance().getStringRounded(temperature, 5, 0) + "C", {0, 0}, COLOR_WHITE);      
    }

    display.show();
}