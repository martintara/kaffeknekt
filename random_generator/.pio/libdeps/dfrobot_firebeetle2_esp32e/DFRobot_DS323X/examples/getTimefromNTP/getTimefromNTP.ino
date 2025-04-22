/*!
 * @file  getTimefromNTP.ino
 * @brief  Get time from ntpServer and show current time ,only work on ESP32
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license  The MIT License (MIT)
 * @author  [yufeng](yufeng.luo@dfrobot.com)
 * @maintainer  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2021-10-25
 * @url  https://github.com/DFRobot/DFRobot_DS323X
 */
#include "DFRobot_DS323X.h"
#include <WiFi.h>
#include "time.h"

DFRobot_DS323X rtc;

const char* ssid       = "WIFI_ID";//wlan information
const char* password   = "WIFI_PASSWORD";

const char* ntpServer = "ntp.ntsc.ac.cn";//local ntp server
const long  gmtOffset_sec = 8*3600;
const int   daylightOffset_sec = 0;


void getTimeFromNTP()
{
    struct tm timeinfo;
    if(!getLocalTime(&timeinfo)){
        Serial.println("Failed to obtain time");
        return;
    }
    // Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
    // Set Set initial time.Due to network and program running delays, 2 seconds were added to the obtained network time as delay compensation when setting.
    rtc.setTime(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec + 2);
}

void setup()
{
    Serial.begin(9600);
    while(rtc.begin() != true){
        Serial.println("Failed to init chip, please check if the chip connection is fine. ");
        delay(1000);
    }
    //connect to WiFi
    Serial.printf("Connecting to %s ", ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" CONNECTED");
    
    //init and get the time
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    getTimeFromNTP();
    //disconnect WiFi as it's no longer needed
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
}

void loop()
{
    Serial.print(rtc.getYear(), DEC);
    Serial.print('/');
    Serial.print(rtc.getMonth(), DEC);
    Serial.print('/');
    Serial.print(rtc.getDate(), DEC);
    Serial.print(" (");
    Serial.print(rtc.getDayOfWeek());
    Serial.print(") ");
    Serial.print(rtc.getHour(), DEC);
    Serial.print(':');
    Serial.print(rtc.getMinute(), DEC);
    Serial.print(':');
    Serial.print(rtc.getSecond(), DEC);
    Serial.println(' ');
    delay(1000);
}