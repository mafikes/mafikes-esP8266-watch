#include <Wifi.h>
#include <Display.h>

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>

// https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WebServer
ESP8266WebServer server(80);

void handleRoot() {
    server.send(200, "text/plain", "Hello from esp8266!\r\n");
}

void Wifi::setup() {
    Serial.println(F("WiFi Setup")); 
    WiFiManager wifiManager;
    wifiManager.setTimeout(180); 

    if(!wifiManager.autoConnect("MAFIKES WATCH")) { 
        Display::getInstance().drawText("Failed", true, {0, 0});
        Serial.println(F("Failed to connect and timeout occurred")); 
        delay(6000); 
        ESP.restart(); //reset and try again 
    } 

    ipAddress = WiFi.localIP().toString();
    
    Serial.println(F("WiFi connected...")); 
    Serial.println(F("Use this URL to connect: http://")); Serial.println(ipAddress+"/");

    server.on("/", handleRoot);
    
    server.begin();
    Serial.println("HTTP server started");
}

void Wifi::loop() {
    server.handleClient();
    MDNS.update();
}

