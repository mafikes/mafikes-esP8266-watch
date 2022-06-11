#include <Wifi.h>
#include <Display.h>
#include <Config.h>
#include <ApplicationManager.h>

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

    server.on("/brightness-auto", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String value = request->getParam("value")->value();
            bool status;

            if(value.toInt()) {
                status = true;
            } else {
                status = false;
            }

            Config& config = Config::getInstance();
            config.data.brightness_auto = status;  
            config.save();
            
        }

        request->send(200, "text/plain", "OK");
    });

    server.on("/change-weather-location", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String value = request->getParam("value")->value();
            
            Config& config = Config::getInstance();
            config.data.weather_location = value;  
            config.save();            
        }

        request->send(200, "text/plain", "OK");
    });

    server.on("/change-weather-api", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String value = request->getParam("value")->value();
            
            Config& config = Config::getInstance();
            config.data.weather_key = value;  
            config.save();
        }

        request->send(200, "text/plain", "OK");
    });

    server.on("/switch-main-time", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String value = request->getParam("value")->value();
            
            int time = value.toInt();

            Config& config = Config::getInstance();
            
            if(time > 0) {
                config.data.view_auto_switch = true;
                config.data.view_main_switch_time = time;
            } else {
                config.data.view_auto_switch = false;
            }

            config.save();

            ApplicationManager::getInstance().loadSwichTime();
        }

        request->send(200, "text/plain", "OK");
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

    server.on("/time-offset", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String value = request->getParam("value")->value();
            
            int time = value.toInt();

            Config& config = Config::getInstance();    
            config.data.time_offset = time;        
            config.save();
        }

        request->send(200, "text/plain", "OK");
    });

    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest *request) {
        ESP.reset();
        request->send(200, "text/plain", "OK");
    });

    server.begin();
    Serial.println("HTTP Web server running.");
}

void Wifi::loop() {
    MDNS.update();
}

