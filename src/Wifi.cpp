#include <Wifi.h>
#include <Display.h>
#include "Settings.h"

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include "LittleFS.h"
#include <ESP8266mDNS.h>
#include "ArduinoJson.h"

#include <ApplicationManager.h>

AsyncWebServer server(80);
DNSServer dns;

void Wifi::setup() {
    if(!LittleFS.begin()) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    Serial.println(F("WiFi Setup"));
    AsyncWiFiManager wifiManager(&server,&dns);
    wifiManager.setTimeout(180); 

    if(!wifiManager.autoConnect("MAFIKES WATCH")) { 
        Display::getInstance().drawText("Failed", true, {0, 0});
        Serial.println(F("Failed to connect and timeout occurred")); 
        delay(6000); 
        ESP.restart(); //reset and try again 
    } 

    IP_ADDRESS = WiFi.localIP().toString();

    Serial.println(F("WiFi connected...")); 
    Serial.println(F("Use this URL to connect: http://")); Serial.println(IP_ADDRESS+"/");

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
            ApplicationManager::getInstance().setBrightness(value.toInt());
        }

        request->send(200, "text/plain", "OK");
    });

    server.on("/watch-type", HTTP_GET, [](AsyncWebServerRequest *request){
        // TODO: set watch type
        request->send(200, "text/plain", "OK");
    });

    server.on("/watch-color", HTTP_GET, [](AsyncWebServerRequest *request){
        // TODO: set color watch
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

    server.on("/change-weather-api", HTTP_GET, [](AsyncWebServerRequest *request){
        if(request->hasParam("value")) {
            String value = request->getParam("value")->value();
            // TODO: save weather api        
        }

        request->send(200, "text/plain", "OK");
    });

    server.begin();
    Serial.println("HTTP Web server running.");
}

void Wifi::loop() {

}

