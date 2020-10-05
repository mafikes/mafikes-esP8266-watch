#include <TimeApp.h>
#include <time.h>
#include <Config.h>
#include <Display.h>

#include <TimeLib.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

static const char ntpServerName[] = "europe.pool.ntp.org";
// static const char ntpServerName[] = "tik.cesnet.cz";

WiFiUDP Udp;
const int timeZone = 2;     // Central European Time
unsigned int localPort = 8888;  // local port to listen for UDP packets
const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets
time_t prevDisplay = 0; 

String TimeApp::repairDigit(int digit) 
{
    String repairedDigit = String(digit);
    if(digit < 10) repairedDigit = "0"+repairedDigit;
    return repairedDigit;
}

void TimeApp::displayClock()
{
    Display::getInstance().clear();
    DisplayColor white = {255, 255, 255};

    if(ClockTheme == 0) { 
        Display::getInstance().drawText(repairDigit(hour()) + ":" + repairDigit(minute()), true, {8, 0}, white);
    } else if(ClockTheme == 1) {
        Display::getInstance().drawText(repairDigit(hour()) + ":" + repairDigit(minute()) + ":" + repairDigit(second()), true, {3, 0}, white);
    }
}

void sendNTPpacket(IPAddress &address)
{
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  Udp.beginPacket(address, 123); 
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
//   Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);

//   Serial.print(ntpServerName);
//   Serial.print(": ");
//   Serial.println(ntpServerIP);

  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }

  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

void TimeApp::setup() {
    Udp.begin(localPort);
    // Serial.println("Local port: ");
    // Serial.println(Udp.localPort());
    // Serial.println("waiting for sync");
    setSyncProvider(getNtpTime);
    setSyncInterval(300);
}


void TimeApp::loop() 
{
    if (timeStatus() != timeNotSet) {
        if (now() != prevDisplay) { //update the display only if time has changed
            prevDisplay = now();
            displayClock();
        }
    }
}
