// // random data script generert av claude ai
// #include <Arduino.h>
// #include <ArduinoJson.h>
// #include "DFRobot_DS323X.h"
// #include <Wire.h>
// //#include <time.h>

// // Define I2C pins for ESP32
// #define SDA_PIN 21
// #define SCL_PIN 22

// // Time interval between data transmissions (500 ms = 0.5 seconds)
// const unsigned long INTERVAL = 500;
// unsigned long previousMillis = 0;
// unsigned long startTime = 0;
// bool timerrStarted = false;

// // Sensor data ranges
// const float TEMP_LOW_MIN = 3.0; // Low temperature range min
// const float TEMP_LOW_MAX = 5.0; // Low temperature range max
// const float TEMP_HIGH_MIN = 10.0; // High temperature range min
// const float TEMP_HIGH_MAX = 11.0; // High temperature range max
// const float PRESSURE_MIN = 3.0; // Minimum pressure in bar
// const float PRESSURE_MAX = 11.0; // Maximum pressure in bar
// const int WATT_MIN = 800; // Minimum AC wattage
// const int WATT_MAX = 1200; // Maximum AC wattage

// // Flag for temperature direction
// char tempFlag = '0'; // Default flag value
// bool inHighRange = false; // Track which range we're in
// float lastTemperature = 0.0; // Store the last temperature

// // Initialize RTC object
// DFRobot_DS323X rtc;

// // Function to get timestamp in InfluxDB format (Unix time in nanoseconds)
// uint64_t getInfluxDBTimestamp() {
//   // Get time components from RTC
//   struct tm timeinfo;
//   timeinfo.tm_year = rtc.getYear() - 1900; // Years since 1900
//   timeinfo.tm_mon = rtc.getMonth() - 1; // Months are 0-11
//   timeinfo.tm_mday = rtc.getDate(); // Day of month (1-31)
//   timeinfo.tm_hour = rtc.getHour(); // Hours (0-23)
//   timeinfo.tm_min = rtc.getMinute(); // Minutes (0-59)
//   timeinfo.tm_sec = rtc.getSecond(); // Seconds (0-59)
  
//   // Convert to Unix timestamp (seconds since Jan 1, 1970)
//   time_t unixTime = mktime(&timeinfo);
  
//   // Convert to nanoseconds (InfluxDB format)
//   uint64_t influxTime = (uint64_t)unixTime * 1000000000ULL;
//   return influxTime;
// }

// void setup() {
//   // Initialize serial communication at 115200 baud rate
//   Serial.begin(115200);
  
//   // Initialize I2C with ESP32 specific pins
//   Wire.begin(SDA_PIN, SCL_PIN);
  
//   // Initialize RTC module
//   while(rtc.begin() != true){
//     delay(1000);
//   }
  
//   // Set RTC to 24-hour mode
//   rtc.setHourSystem(rtc.e24hours);
  
//   // Seed the random number generator
//   randomSeed(analogRead(0));
  
//   // Brief delay to ensure everything is ready
//   delay(100);
// }

// void loop() {
//   // Get current time
//   unsigned long currentMillis = millis();
  
//   // Start the timer if it hasn't started yet
//   if (!timerrStarted) {
//     startTime = currentMillis;
//     timerrStarted = true;
//   }
  
//   // Calculate elapsed time in seconds
//   unsigned long elapsedSeconds = (currentMillis - startTime) / 1000;
  
//   // Check if it's time to send data
//   if (currentMillis - previousMillis >= INTERVAL) {
//     previousMillis = currentMillis;
    
//     // Determine which temperature range to use based on elapsed time
//     float temperature;
//     bool wasInHighRange = inHighRange; // Remember previous state
    
//     // First 10 seconds: temperature between 3 and 5
//     // Last 10 seconds: temperature between 10 and 11
//     // After 20 seconds, cycle repeats
//     if ((elapsedSeconds % 20) < 10) {
//       temperature = random(TEMP_LOW_MIN * 100, TEMP_LOW_MAX * 100) / 100.0;
//       inHighRange = false;
//     } else {
//       temperature = random(TEMP_HIGH_MIN * 100, TEMP_HIGH_MAX * 100) / 100.0;
//       inHighRange = true;
//     }
    
//     // Check for transitions and set flag
//     if (wasInHighRange && !inHighRange) {
//       // Transitioning from high to low
//       tempFlag = 'D';
//     } else if (!wasInHighRange && inHighRange) {
//       // Transitioning from low to high
//       tempFlag = 'U';
//     } else {
//       // No transition occurred
//       tempFlag = '0';
//     }
    
//     // Generate other random sensor values within defined ranges
//     float pressure = random(PRESSURE_MIN * 100, PRESSURE_MAX * 100) / 100.0; // With 2 decimal precision
//     int power = random(WATT_MIN, WATT_MAX);
    
//     // Create JSON document
//     StaticJsonDocument<256> doc;
//     doc["temperature"] = temperature;
//     doc["power"] = power;
//     doc["pressure"] = pressure;
//     doc["timestamp"] = getInfluxDBTimestamp();
    
//     // Only include flag if it's U or D (during transition)
//     if (tempFlag != '0') {
//       doc["flag"] = String(tempFlag);
//     }
    
//     // Serialize JSON to serial port
//     serializeJson(doc, Serial);
//     Serial.println(); // Add newline for readability
    
//     // Update last temperature
//     lastTemperature = temperature;
//   }
// }




// random data script generert av claude ai
#include <Arduino.h>
#include <ArduinoJson.h>
#include "DFRobot_DS323X.h"
#include <Wire.h>
//#include <time.h>

// Define I2C pins for ESP32
#define SDA_PIN 21
#define SCL_PIN 22

// Time interval between data transmissions (500 ms = 0.5 seconds)
const unsigned long INTERVAL = 500;
unsigned long previousMillis = 0;
unsigned long startTime = 0;
bool timerrStarted = false;

// Sensor data ranges
const float PRESSURE_LOW_MIN = 3.0; // Low pressure range min
const float PRESSURE_LOW_MAX = 5.0; // Low pressure range max
const float PRESSURE_HIGH_MIN = 10.0; // High pressure range min
const float PRESSURE_HIGH_MAX = 11.0; // High pressure range max
const float TEMP_MIN = 23.0; // Minimum temperature in bar
const float TEMP_MAX = 26.0; // Maximum temperature in bar
const int WATT_MIN = 1000; // Minimum AC wattage
const int WATT_MAX = 1200; // Maximum AC wattage

// Flag for pressure direction
char pressureFlag = '0'; // Default flag value
bool inHighRange = false; // Track which range we're in
float lastPressure = 0.0; // Store the last pressure

// Initialize RTC object
DFRobot_DS323X rtc;

// Function to get timestamp in InfluxDB format (Unix time in nanoseconds)
uint64_t getInfluxDBTimestamp() {
  // Get time components from RTC
  struct tm timeinfo;
  timeinfo.tm_year = rtc.getYear() - 1900; // Years since 1900
  timeinfo.tm_mon = rtc.getMonth() - 1; // Months are 0-11
  timeinfo.tm_mday = rtc.getDate(); // Day of month (1-31)
  timeinfo.tm_hour = rtc.getHour(); // Hours (0-23)
  timeinfo.tm_min = rtc.getMinute(); // Minutes (0-59)
  timeinfo.tm_sec = rtc.getSecond(); // Seconds (0-59)
  
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
  
  // Start the timer if it hasn't started yet
  if (!timerrStarted) {
    startTime = currentMillis;
    timerrStarted = true;
  }
  
  // Calculate elapsed time in seconds
  unsigned long elapsedSeconds = (currentMillis - startTime) / 1000;
  
  // Check if it's time to send data
  if (currentMillis - previousMillis >= INTERVAL) {
    previousMillis = currentMillis;
    
    // Determine which pressure range to use based on elapsed time
    float pressure;
    bool wasInHighRange = inHighRange; // Remember previous state
    
    // First 10 seconds: pressure between 3 and 5
    // Last 10 seconds: pressure between 10 and 11
    // After 20 seconds, cycle repeats
    if ((elapsedSeconds % 20) < 10) {
      pressure = random(PRESSURE_LOW_MIN * 100, PRESSURE_LOW_MAX * 100) / 100.0;
      inHighRange = false;
    } else {
      pressure = random(PRESSURE_HIGH_MIN * 100, PRESSURE_HIGH_MAX * 100) / 100.0;
      inHighRange = true;
    }
    
    // Check for transitions and set flag
    if (wasInHighRange && !inHighRange) {
      // Transitioning from high to low
      pressureFlag = 'D';
    } else if (!wasInHighRange && inHighRange) {
      // Transitioning from low to high
      pressureFlag = 'U';
    } else {
      // No transition occurred
      pressureFlag = '0';
    }
    
    // Generate other random sensor values within defined ranges
    float temperature = random(TEMP_MIN * 100, TEMP_MAX * 100) / 100.0; // With 2 decimal precision
    int power = random(WATT_MIN, WATT_MAX);
    
    // Create JSON document
    StaticJsonDocument<256> doc;
    doc["temperature"] = temperature;
    doc["power"] = power;
    doc["pressure"] = pressure;
    doc["timestamp"] = getInfluxDBTimestamp();
    
    // Only include flag if it's U or D (during transition)
    if (pressureFlag != '0') {
      doc["flag"] = String(pressureFlag);
    }
    
    // Serialize JSON to serial port
    serializeJson(doc, Serial);
    Serial.println(); // Add newline for readability
    
    // Update last pressure
    lastPressure = pressure;
  }
}