#include <WeatherApp.h>
#include <Config.h>
#include <Display.h>

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

void WeatherApp::askServer() 
{
    StaticJsonDocument<1200> doc;

    HTTPClient http;

    http.begin(String("http://api.openweathermap.org/data/2.5/weather?id=") + API_WEATHER_LOCATION +("&appid=")+ API_WEATHER_KEY +("&units=metric"));
    
    int httpCode = http.GET();
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

        // icon="";
        // owm_data["weather"][0]["icon"].printTo(icon);
        // icon.remove(0,1);
        // icon.remove(icon.length()-2);
        }        
    }

    http.end();
}

void WeatherApp::setup() 
{
    askServer();
} 

void WeatherApp::print() 
{
    Display::getInstance().clear();
    Display::getInstance().drawText(String(temperature) + " C", true, {8, 0}, {255, 255, 255});
}