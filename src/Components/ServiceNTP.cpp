#include <ServiceNTP.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <RTC.h>
#include <Config.h>

const long utcOffsetInSeconds = 0; // + 2 hours
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "tik.cesnet.cz", utcOffsetInSeconds, 0);

void ServiceNTP::updateTime()
{
    timeClient.setUpdateInterval(0);

    if (timeClient.forceUpdate())
    {
        Serial.println("Time updated NTP.");
        time_t rawtime = timeClient.getEpochTime();
        struct tm *ti;
        ti = localtime(&rawtime);

        // Year
        uint16_t year = ti->tm_year + 1900;
        String yearStr = String(year);

        // Month
        uint8_t month = ti->tm_mon + 1;
        String monthStr = month < 10 ? "0" + String(month) : String(month);

        // Day
        uint8_t day = ti->tm_mday;
        String dayStr = day < 10 ? "0" + String(day) : String(day);

        Serial.print("NTP Date:");
        Serial.print(day);
        Serial.print(".");
        Serial.print(month);
        Serial.print(".");
        Serial.println(year);

        RTC::getInstance().setNewTime(DateTime(year, month, day, timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds()));
    }
    else
    {
        Serial.println("Failed to update time via NTP.");
    }
}

void ServiceNTP::loop()
{
    // Update Time by set time
    unsigned long currentMillis = millis();

    // Update time every 48 hours
    if (
        firstStart ||
        ((unsigned long)(currentMillis - prevTime) >= (((1000 * 60) * 60) * Config::getInstance().data.time_update_interval)))
    {
        updateTime();
        prevTime = currentMillis;
        firstStart = false;
    }
}

void ServiceNTP::setup()
{
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Wifi not connected, NTP cant be load.");
    }

    timeClient.begin();

    timeClient.setTimeOffset(3600 * Config::getInstance().data.time_offset);
}
