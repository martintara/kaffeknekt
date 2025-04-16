// random data script generert av claude ai

#include <Arduino.h>
#include <ArduinoJson.h>
#include "DFRobot_DS323X.h"
#include <Wire.h>
#include <time.h>

// Define I2C pins for ESP32
#define SDA_PIN 21
#define SCL_PIN 22

// Time interval between data transmissions (500 ms = 0.5 seconds)
const unsigned long INTERVAL = 500;
unsigned long previousMillis = 0;

// Sensor data ranges
const float TEMP_MIN = 20.0;     // Minimum temperature in °C
const float TEMP_MAX = 35.0;     // Maximum temperature in °C
const float PRESSURE_MIN = 6.0;  // Minimum pressure in bar
const float PRESSURE_MAX = 10.0; // Maximum pressure in bar
const int WATT_MIN = 800;        // Minimum AC wattage
const int WATT_MAX = 1200;       // Maximum AC wattage

// Initialize RTC object
DFRobot_DS323X rtc;

// Function to get timestamp in InfluxDB format (Unix time in nanoseconds)
uint64_t getInfluxDBTimestamp() {
  // Get time components from RTC
  struct tm timeinfo;
  timeinfo.tm_year = rtc.getYear() - 1900; // Years since 1900
  timeinfo.tm_mon = rtc.getMonth() - 1;    // Months are 0-11
  timeinfo.tm_mday = rtc.getDate();        // Day of month (1-31)
  timeinfo.tm_hour = rtc.getHour();        // Hours (0-23)
  timeinfo.tm_min = rtc.getMinute();       // Minutes (0-59)
  timeinfo.tm_sec = rtc.getSecond();       // Seconds (0-59)
  
  // Convert to Unix timestamp (seconds since Jan 1, 1970)
  time_t unixTime = mktime(&timeinfo);
  
  // Convert to nanoseconds (InfluxDB format)
  uint64_t influxTime = (uint64_t)unixTime * 1000000000ULL;
  
  return influxTime;
}

void setup() {
  // Initialize serial communication at 115200 baud rate
  Serial.begin(115200);
  
  // Initialize I2C with ESP32 specific pins
  Wire.begin(SDA_PIN, SCL_PIN);
  
  // Initialize RTC module
  while(rtc.begin() != true){
    delay(1000);
  }
  
  // Set RTC to 24-hour mode
  rtc.setHourSystem(rtc.e24hours);
  
  // Seed the random number generator
  randomSeed(analogRead(0));
  
  // Brief delay to ensure everything is ready
  delay(100);
}

void loop() {
  // Get current time
  unsigned long currentMillis = millis();
  
  // Check if it's time to send data
  if (currentMillis - previousMillis >= INTERVAL) {
    previousMillis = currentMillis;
    
    // Generate random sensor values within defined ranges
    float temperature = random(TEMP_MIN * 100, TEMP_MAX * 100) / 100.0;  // With 2 decimal precision
    float pressure = random(PRESSURE_MIN * 100, PRESSURE_MAX * 100) / 100.0;  // With 2 decimal precision
    int ac_watt = random(WATT_MIN, WATT_MAX);
    
    // Create JSON document
    StaticJsonDocument<256> doc;
    
    // Add sensor data to JSON
    doc["temperature"] = temperature;
    //doc["temperature_unit"] = "C";
    doc["pressure"] = pressure;
    //doc["pressure_unit"] = "bar";
    doc["ac_watt"] = ac_watt;
    //doc["watt_unit"] = "watt";
    doc["dc_watt"] = ac_watt;
    doc["timestamp"] = getInfluxDBTimestamp();
    
    // Serialize JSON to serial port
    serializeJson(doc, Serial);
    Serial.println(); // Add newline for readability
  }
}