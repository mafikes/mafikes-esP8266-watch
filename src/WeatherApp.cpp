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
    if(last_temperature_icon != temperature_icon) {
        display.resetIconAnimation();
    } 

    if(temperature_icon == "01") {
        display.showAnimateIcon(ICON_SUN, arrayLength(ICON_SUN));
    } else if (temperature_icon == "02") {      
        display.showAnimateIcon(ICON_PARTLYCLOUD, arrayLength(ICON_PARTLYCLOUD));
    } else if (temperature_icon == "03" || temperature_icon == "04") {      
        display.showAnimateIcon(ICON_CLOUDY, arrayLength(ICON_CLOUDY));
    } else if (temperature_icon == "09" || temperature_icon == "10") {
        display.showAnimateIcon(ICON_RAIN, arrayLength(ICON_RAIN));
    } else if (temperature_icon == "11") {
        display.showAnimateIcon(ICON_THUNDER, arrayLength(ICON_THUNDER));
    } else if (temperature_icon == "13") {  
        display.showAnimateIcon(ICON_SNOW, arrayLength(ICON_SNOW));
    } else if (temperature_icon == "50") {
        display.showAnimateIcon(ICON_MIST, arrayLength(ICON_MIST));
    } else {
        display.showAnimateIcon(ICON_SUN, arrayLength(ICON_SUN));
    }
    
    last_temperature_icon = temperature_icon;
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
            temperature_icon = icon;
            temperature_icon.remove(2, 1);

            downloaded = true;
            Display::getInstance().resetIconAnimation();
        }        
    }

    http.end();
}

void WeatherApp::beforeRender() 
{
    Display::getInstance().resetIconAnimation();
}

void WeatherApp::render(Display& display) 
{
    display.clear();

    if(!downloaded) {
        askServer();        
    } else {    
        showIcon(display);              
        display.drawText(Helper::getInstance().getStringRounded(temperature, 5, 1) + "C", false, {10, 0}, COLOR_WHITE);      
    }

    display.show();
}