#include <Config.h>

#include "LittleFS.h"
#include <SPI.h>
#include "ArduinoJson.h"
#include <WString.h>
#include <Display.h>
#include <Colors.h>

void Config::load() {    
    File configFile = LittleFS.open(configPath, "r");

    // Use arduinojson.org/v6/assistant to compute the capacity.
    StaticJsonDocument<600> doc;

    DeserializationError error = deserializeJson(doc, configFile);
    if(error) {
        Serial.println(F("Failed to read config file."));
    }

    data.brightness = doc["brightness"];
    data.brightness_auto = doc["brightness_auto"];
    data.watch_type = doc["watch_type"];
    data.watch_type_color = doc["watch_type_color"];
    data.view_auto_switch = doc["view_auto_switch"];
    data.view_main_switch_time = doc["view_main_switch_time"];
    data.view_app_switch_time = doc["view_app_switch_time"];
    data.weather_key = doc["weather_key"].as<String>();
    data.weather_location = doc["weather_location"].as<String>();
    data.watch_color_custom = doc["watch_color_custom"];
    Serial.println(doc["watch_color"].as<String>());
    DisplayColor watchColor = {doc["watch_color"][0], doc["watch_color"][1], doc["watch_color"][2]};
    data.watch_color = watchColor;
    // data.watch_color = COLOR_YELLOW;

    configFile.close();

    Serial.println(F("Sucess read config file."));
}

void Config::save()
{
    LittleFS.remove(configPath);

    File file = LittleFS.open(configPath, "w");
    if (!file) {
        Serial.println(F("Config: Failed to create config file."));
        return;
    }

    StaticJsonDocument<500> doc;

    doc["brightness"] = data.brightness;
    doc["brightness_auto"] = data.brightness_auto;
    doc["watch_type"] = data.watch_type;
    doc["watch_type_color"] = data.watch_type_color;
    doc["view_auto_switch"] = data.view_auto_switch;
    doc["view_main_switch_time"] = data.view_main_switch_time;
    doc["view_app_switch_time"] = data.view_app_switch_time;
    doc["weather_key"] = data.weather_key;
    doc["watch_color_custom"] = data.watch_color_custom;

    JsonArray newColor = doc.createNestedArray("watch_color");
    newColor.add(data.watch_color.red);
    newColor.add(data.watch_color.green);
    newColor.add(data.watch_color.blue);   

    if(serializeJson(doc, file) == 0) {
        Serial.println(F("Config: Failed to write file."));
    }

    file.close();
}

void Config::setup() {

    if(!LittleFS.begin()) {
        Serial.println("Config: An Error has occurred while mounting LittleFS.");
        return;
    } 
    
    load();
}