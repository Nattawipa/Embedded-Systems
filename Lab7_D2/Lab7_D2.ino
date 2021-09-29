//6188089 Nattawipa Saetae

#define _SYNC_NTP //Uncomment this if you want the synchronize RTC with the NTP server

#include <WiFi.h>
#include <time.h>
#include <DS3231.h>
#include <Wire.h>
#include "SPI.h"
#include "TFT_eSPI.h"
#include "Free_Fonts.h" // Include the header file attached to this sketch
const char* ssid       = "Teerasak 2.4 G"; //ESP32 not support 5G wifi 
const char* password   = "click123";
TFT_eSPI tft = TFT_eSPI();
unsigned long drawTime = 0;
const char* ntpServer = "th.pool.ntp.org";
//const char* ntpServer = "clock.mahidol.ac.th";
const long  gmtOffset_sec = 3600 * 7; //UTC +7.00
const int   daylightOffset_sec = 0; //0 means no DST observed; otherwise, 3600.

DS3231  rtc;

bool h12Format;
bool ampm;
bool centuryRollover;

struct tm timeinfo;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  
  #ifdef _SYNC_NTP
  //connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println(" CONNECTED");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(TFT_YELLOW);
  
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println("M:" + String(timeinfo.tm_mon) + ", Y:" + String(timeinfo.tm_year));
  rtc.enableOscillator(true, true, 1);
  rtc.setClockMode(h12Format); //24-h format
  rtc.setDoW(timeinfo.tm_wday);
  rtc.setHour(timeinfo.tm_hour);
  rtc.setMinute(timeinfo.tm_min);
  rtc.setSecond(timeinfo.tm_sec);
  rtc.setDate(timeinfo.tm_mday);
  rtc.setMonth(timeinfo.tm_mon + 1); //Month from NTP starts from zero
  rtc.setYear(timeinfo.tm_year - 100); //Year from NTP is an offset from 1900

  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  #endif
}

void loop()
{
  tft.begin();
  tft.fillScreen(TFT_YELLOW);
  #ifdef _SYNC_NTP
  //Show time from NTP
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  int xpos =  20;
  int ypos = 40;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  tft.setTextFont(5); 
  tft.setTextColor(TFT_BLACK);
  tft.setCursor(xpos, ypos);    // Set cursor near top left corner of screen

  tft.setFreeFont(FSB9);
  tft.print("M:" + String(timeinfo.tm_mon));
  tft.print(", Y:" + String(timeinfo.tm_year));
  
  Serial.println("M:" + String(timeinfo.tm_mon) + ", Y:" + String(timeinfo.tm_year));
  #endif

  // Send Day-of-Week
  tft.print("DoW:");
  tft.print(rtc.getDoW());
  tft.print(" ");
  Serial.print("DoW:");
  Serial.print(rtc.getDoW());
  Serial.print(" ");
  
  // Send date
  tft.print("-- Date: ");
  tft.print(rtc.getDate(), DEC);
  tft.print("/");
  tft.print(rtc.getMonth(centuryRollover), DEC);
  tft.print("/");
  tft.print("2");
  
  Serial.print("-- Date: ");
  Serial.print(rtc.getDate(), DEC);
  Serial.print("/");
  Serial.print(rtc.getMonth(centuryRollover), DEC);
  Serial.print("/");
  Serial.print("2"); //This program is still valid until almost the next 1000 years.
  if(centuryRollover){
    Serial.print("1");
    tft.print("1");
  }
  else{
    Serial.print("0");
    tft.print("0");
  }
  tft.print(rtc.getYear(), DEC);
  Serial.print(rtc.getYear(), DEC);

  // Send time
  ypos = 70;
  tft.setCursor(xpos, ypos);  
  tft.print(" -- Time: ");
  tft.print(rtc.getHour(h12Format, ampm), DEC);
  tft.print(":");
  tft.print(rtc.getMinute(), DEC);
  tft.print(":");
  tft.print(rtc.getSecond(), DEC); 

  Serial.print(" -- Time: ");
  Serial.print(rtc.getHour(h12Format, ampm), DEC);
  Serial.print(":");
  Serial.print(rtc.getMinute(), DEC);
  Serial.print(":");
  Serial.print(rtc.getSecond(), DEC);

  //Temperature
   tft.setTextColor(TFT_BLUE);
  tft.println(""); 
  tft.print(" -- RTC Temperature: "); 
  tft.print(rtc.getTemperature()); 
  Serial.print(" -- RTC Temperature: ");
  Serial.println(rtc.getTemperature());

  delay(2000);
}
