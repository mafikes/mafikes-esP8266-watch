#include <Wifi.h>
#include <Display.h>
#include <Config.h>
#include <ApplicationManager.h>
#include <ServiceNTP.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <ESP8266mDNS.h>

#include "LittleFS.h"
#include "ArduinoJson.h"

AsyncWebServer server(80);
DNSServer dns;

void Wifi::setup() {
    if(!LittleFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    Serial.println(F("WiFi Setup"));
    
    WiFi.persistent(true); 
    AsyncWiFiManager wifiManager(&server, &dns);
    wifiManager.setTimeout(180); 

    if(!wifiManager.autoConnect("MAFIKESWATCH")) { 
        Display::getInstance().drawText("Failed", true, {0, 0});
        Display::getInstance().show();
        
        Serial.println(F("Failed to connect and timeout occurred")); 
        delay(6000); 
        ESP.restart(); //reset and try again 
    } 

    Config::getInstance().data.ip_address = WiFi.localIP().toString();

    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }

    Serial.println(F("WiFi connected...")); 
    Serial.println(F("Use this URL to connect: http://")); Serial.println(Config::getInstance().data.ip_address +"/");
        
    if (MDNS.begin("mafikeswatch"), WiFi.localIP()) { // Start in local address
        Serial.println("Address mafikeswatch.local started!");
        MDNS.addService("http", "tcp", 80);
    } else {
        Serial.println("Error setting up MDNS responder!");
        return;
    }
    
    // Homepage
    server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

    server.on("/sync-time", HTTP_GET, [](AsyncWebServerRequest *request){
       ServiceNTP::getInstance().updateTime();
       request->send(200, "text/plain", "OK"); 
    });

    server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", String(ESP.getFreeHeap()));
    });
        
    // API
    server.on("/change-view", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("button")) {
            String button = request->getParam("button")->value();

            if(button == "1") {
                ApplicationManager::getInstance().btn1_process();
            } else if (button == "2") {
                ApplicationManager::getInstance().btn2_process();
            } else if (button == "3") {
                ApplicationManager::getInstance().btn3_process();
            }
        }

        request->send(200, "text/plain", "OK");
    });

    server.on("/brightness", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String value = request->getParam("value")->value();
            int brightness = value.toInt();
            ApplicationManager::getInstance().setBrightness(brightness);

            Config& config = Config::getInstance();
            config.data.brightness = brightness;
            config.data.brightness_auto = false;
            config.save();
        }

        request->send(200, "text/plain", "OK");
    });

    server.on("/show-main-app", HTTP_GET, [](AsyncWebServerRequest *request) {
        ApplicationManager::getInstance().showMainApp();
        request->send(200, "text/plain", "OK");
    });

    server.on("/show-text", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String text = request->getParam("value")->value();
            if(text) {
                ApplicationManager::getInstance().showText(text);
            } 
        }

        request->send(200, "text/plain", "OK");
    });

    server.on("/save", HTTP_GET, [](AsyncWebServerRequest *request){
        String string_brightness_auto = request->getParam("brightness_auto")->value();
        int brightness = request->getParam("brightness")->value().toInt();
        int time_offset = request->getParam("time_offset")->value().toInt();
        int view_main_switch_time = request->getParam("view_main_switch_time")->value().toInt();
        int view_app_switch_time = request->getParam("view_app_switch_time")->value().toInt();
        String weather_location = request->getParam("weather_location")->value();
        String weather_key = request->getParam("weather_key")->value();
        int time_update_interval = request->getParam("time_update_interval")->value().toInt();
        
        Config& config = Config::getInstance();

        bool brightness_auto = false;
        if(string_brightness_auto == "true") {
            brightness_auto = true;
        }

        if(!brightness_auto) {
            ApplicationManager::getInstance().setBrightness(brightness);
        }

        config.data.brightness_auto = brightness_auto;  
        config.data.brightness = brightness;  
        config.data.time_offset = time_offset;  
        config.data.view_app_switch_time = view_app_switch_time;  
        config.data.view_main_switch_time = view_main_switch_time;  
        config.data.weather_location = weather_location;  
        config.data.weather_key = weather_key;  
        config.data.time_update_interval = time_update_interval;  
        config.save();
        
        request->send(200);
    });

    server.on("/switch-app-time", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String value = request->getParam("value")->value();
            
            int time = value.toInt();

            Config& config = Config::getInstance();    
            config.data.view_app_switch_time = time;        
            config.save();

            ApplicationManager::getInstance().loadSwichTime();
        }

        request->send(200, "text/plain", "OK");
    });

    server.on("/custom-color-watch", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("red", false) && request->hasParam("green", false) && request->hasParam("blue", false)) {
            String red = request->getParam("red")->value();
            String green = request->getParam("green")->value();
            String blue = request->getParam("blue")->value();
         
            Config& config = Config::getInstance();    
            config.data.watch_color_custom = true;            
            config.data.watch_color[0] = red.toInt();
            config.data.watch_color[1] = green.toInt();
            config.data.watch_color[2] = blue.toInt(); 
            config.save();
        }

        request->send(200, "text/plain", "OK");
    });

    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "OK");
        ESP.reset();
    });

    server.begin();
    Serial.println("HTTP Web server running.");
}

void Wifi::loop() {
    MDNS.update();
}

