#include <WeatherApp.h>
#include <Config.h>
#include <Display.h>
#include <Colors.h>
#include <WString.h>
#include <Icons.h>
#include <Helper.h>
#include <Ticker.h>

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define arrayLength(array) (sizeof((array))/sizeof((array)[0]))

void WeatherApp::prepareIcon() 
{
  if(temperature_icon == "01") {
    Display::getInstance().showIcon(ICON_SUN, 0);
  } else if (temperature_icon == "02") {      
    Display::getInstance().showIcon(ICON_PARTLYCLOUD, 3);
  } else if (temperature_icon == "03" || temperature_icon == "04") {      
    Display::getInstance().showIcon(ICON_CLOUDY, 3);
  } else if (temperature_icon == "09" || temperature_icon == "10") {
    Display::getInstance().showIcon(ICON_RAIN, 3);
  } else if (temperature_icon == "11") {
    Display::getInstance().showIcon(ICON_THUNDER, 3);
  } else if (temperature_icon == "13") {  
    Display::getInstance().showIcon(ICON_SNOW, 3);
  } else if (temperature_icon == "50") {
    Display::getInstance().showIcon(ICON_MIST, 3);
  } else {
    Display::getInstance().showIcon(ICON_SUN, 3);
  }

  delay(300);
}

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
  Display::getInstance().resetIconAnimation();
  askServer();
}

void WeatherApp::render(Display& display) 
{

  display.clear(); 

  prepareIcon();

  display.drawText(Helper::getInstance().getStringRounded(temperature, 5, 1) + "C", false, {10, 0}, COLOR_WHITE);
  display.show();
  
  // display.clear(); 
  
  // if(temperature_icon == "01") {
  //   display.showIcon(ICON_SUN, arrayLength(ICON_SUN), 0);
  // } else if (temperature_icon == "02") {      
  //   display.showIcon(ICON_PARTLYCLOUD, arrayLength(ICON_PARTLYCLOUD), 3);
  // } else if (temperature_icon == "03" || temperature_icon == "04") {      
  //   display.showIcon(ICON_CLOUDY, arrayLength(ICON_CLOUDY), 3);
  // } else if (temperature_icon == "09" || temperature_icon == "10") {
  //   display.showIcon(ICON_RAIN, arrayLength(ICON_RAIN), 3);
  // } else if (temperature_icon == "11") {
  //   display.showIcon(ICON_THUNDER, arrayLength(ICON_THUNDER), 3);
  // } else if (temperature_icon == "13") {  
  //   display.showIcon(ICON_SNOW, arrayLength(ICON_SNOW), 3);
  // } else if (temperature_icon == "50") {
  //   display.showIcon(ICON_MIST, arrayLength(ICON_MIST), 3);
  // } else {
  //   display.showIcon(ICON_SUN, arrayLength(ICON_SUN), 3);
  // }
  
  // display.drawTextWithIcon(Helper::getInstance().getStringRounded(temperature, 5, 1) + "C", {2, 0}, COLOR_WHITE);
  // display.show();
  // delay(500);
}