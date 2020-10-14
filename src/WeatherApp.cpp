#include <WeatherApp.h>
#include <Config.h>
#include <Display.h>
#include <Colors.h>
#include <WString.h>
#include <Icons.h>

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define arrayLength(array) (sizeof((array))/sizeof((array)[0]))

void WeatherApp::askServer() 
{
    DynamicJsonDocument doc(1200);
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
        const char* icon = doc["weather"][0]["icon"];
        temperature_icon = icon;
        temperature_icon.remove(2, 1);
      }        
    }

    http.end();
}

void WeatherApp::beforeRender() 
{
    askServer();

    //TODO: Set for asking every 3 minutes
} 

void WeatherApp::render(Display& display) 
{
    display.clear(); 

    if(temperature_icon == "01") {
      display.showIcon(ICON_SUN, arrayLength(ICON_SUN), 0);
    } else if (temperature_icon == "02") {      
      display.showIcon(ICON_PARTLYCLOUD, arrayLength(ICON_PARTLYCLOUD), 3);
    } else if (temperature_icon == "03" || temperature_icon == "04") {      
      display.showIcon(ICON_CLOUDY, arrayLength(ICON_CLOUDY), 3);
    } else if (temperature_icon == "09" || temperature_icon == "10") {
      display.showIcon(ICON_RAIN, arrayLength(ICON_RAIN), 3);
    } else if (temperature_icon == "11") {
      display.showIcon(ICON_THUNDER, arrayLength(ICON_THUNDER), 3);
    } else if (temperature_icon == "13") {  
      display.showIcon(ICON_SNOW, arrayLength(ICON_SNOW), 3);
    } else if (temperature_icon == "50") {
      display.showIcon(ICON_MIST, arrayLength(ICON_MIST), 3);
    } else {
      display.showIcon(ICON_SUN, arrayLength(ICON_SUN), 3);
    }
    
    display.drawTextWithIcon(String(temperature) + "C", {2, 0}, COLOR_WHITE);
    display.show();
    delay(500);
}