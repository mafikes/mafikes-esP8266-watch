#include <NTP.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <RTC.h>

const long utcOffsetInSeconds = 3600 * 2; // + 2 hours

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void NTP::updateTime() {
    Serial.println("Time updated via NTP.");

    timeClient.update();

    time_t rawtime = timeClient.getEpochTime();
    struct tm * ti;
    ti = localtime(&rawtime);

    // Year
    uint16_t year = ti->tm_year + 1900;
    String yearStr = String(year);

    // Month
    uint8_t month = ti->tm_mon + 1;
    String monthStr = month < 10 ? "0" + String(month) : String(month);

    RTC::getInstance().setNewTime(DateTime(year, month, timeClient.getDay(), timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()));
}

void NTP::loop() {
    // Update Time by set time    
    unsigned long currentMillis = millis();
    if ((unsigned long)(currentMillis - prevTime) >= ((1000 * 60) * 60) || firstStart) { 
        updateTime();
        prevTime = currentMillis;   
        firstStart = false;     
    }
}

void NTP::setup() {
    while ( WiFi.status() != WL_CONNECTED ) {
        Serial.println("Wifi not connect, NTP cant be load.");
    }

    timeClient.begin();
}
