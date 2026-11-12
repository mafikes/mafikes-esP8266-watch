#include <WeatherApp.h>
#include <Display.h>
#include <Colors.h>
#include <Icons.h>
#include <Helper.h>
#include <Config.h>

#include <WString.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define arrayLength(array) (sizeof((array))/sizeof((array)[0]))
#define WEATHER_RETRY_INTERVAL 30000 // 30s
#define WEATHER_HTTP_TIMEOUT 3000 // 3s, must stay below watchdog

void WeatherApp::askServer() 
{
    Config& config = Config::getInstance();

    requested = true;
    lastRequestTime = millis();

    if(config.data.weather_key.length() == 0 || config.data.weather_location.length() == 0) {
        Serial.println(F("Weather: missing API key or location."));
        return;
    }

    DynamicJsonDocument doc(1800);    
    WiFiClient client;
    HTTPClient http;

    char url[150];
    snprintf(url, sizeof(url), "http://api.openweathermap.org/data/2.5/weather?id=%s&appid=%s&units=metric", 
             config.data.weather_location.c_str(), config.data.weather_key.c_str());

    http.setTimeout(WEATHER_HTTP_TIMEOUT);
    http.begin(client, url);
    int httpCode = http.GET();

    if (httpCode > 0) {
        if (httpCode == HTTP_CODE_OK) {      
            String payload = http.getString();
            DeserializationError error = deserializeJson(doc, payload);

            if (error) {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.c_str());
                http.end();
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

void WeatherApp::beforeRender() 
{
    Display::getInstance().resetIconAnimation();
    Display::getInstance().resetLoading();
}

void WeatherApp::render(Display& display) 
{
    display.clear();

    if(!downloaded) {
        if(!requested || millis() - lastRequestTime >= WEATHER_RETRY_INTERVAL) {
            askServer();
        }
        display.showLoading();        
    } else {    
        showIcon(display);              
        display.drawTextWithIcon(Helper::getInstance().getStringRounded(temperature, 5, 0) + "C", {0, 0}, COLOR_WHITE);      
    }

    display.show();
}
