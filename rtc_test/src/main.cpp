// rtc_test
// bruk av ai: instruerte claudeai til å ta imot en JSON over serial

#include "DFRobot_DS323X.h"
#include <Wire.h>
#include <ArduinoJson.h>

// Define I2C pins for ESP32
#define SDA_PIN 21
#define SCL_PIN 22

DFRobot_DS323X rtc;
String inputString = "";      // String to hold incoming serial data
bool stringComplete = false;  // Whether the string is complete

// Function prototypes
void processSerialCommand();
void checkSerial();

void setup(void) {
    Serial.begin(115200);
    
    // Initialize I2C with ESP32 specific pins
    Wire.begin(SDA_PIN, SCL_PIN);
    
    // Reserve space for the input string
    inputString.reserve(200);
    
    /*Wait for the chip to be initialized completely, and then exit*/
    while(rtc.begin() != true){
        Serial.println("Failed to init DS3232 RTC chip, please check if the chip connection is fine.");
        delay(1000);
    }
    
    Serial.println("DS3232 RTC initialized successfully!");
    Serial.println("Send JSON time data: {\"command\":\"setTime\",\"year\":2025,\"month\":4,\"day\":14,\"hour\":12,\"minute\":0,\"second\":0}");
    
    // unødvendig?
    rtc.setHourSystem(rtc.e24hours);
}

void loop() {
    // Check for new serial data (ESP32 doesn't automatically call serialEvent)
    checkSerial();
    
    // Process any incoming serial commands
    if (stringComplete) {
        processSerialCommand();
        // Clear the string for the next input
        inputString = "";
        stringComplete = false;
    }
    
    // Display the current time and temperature
    Serial.print("Time: ");
    Serial.print(rtc.getYear(), DEC);//year
    Serial.print('/');
    if (rtc.getMonth() < 10) Serial.print("0");
    Serial.print(rtc.getMonth(), DEC);//month
    Serial.print('/');
    if (rtc.getDate() < 10) Serial.print("0");
    Serial.print(rtc.getDate(), DEC);//date
    Serial.print(" (");
    Serial.print(rtc.getDayOfWeek());//day of week
    Serial.print(") ");
    if (rtc.getHour() < 10) Serial.print("0");
    Serial.print(rtc.getHour(), DEC);//hour
    Serial.print(':');
    if (rtc.getMinute() < 10) Serial.print("0");
    Serial.print(rtc.getMinute(), DEC);//minute
    Serial.print(':');
    if (rtc.getSecond() < 10) Serial.print("0");
    Serial.print(rtc.getSecond(), DEC);//second
    Serial.println();
   
    delay(1000);
}

// Check for serial data (needed for ESP32)
void checkSerial() {
    while (Serial.available()) {
        char inChar = (char)Serial.read();
        
        // Add the incoming byte to the string
        if (inChar == '\n') {
            stringComplete = true;
        } else {
            inputString += inChar;
        }
    }
}

// Process serial commands using JSON
void processSerialCommand() {
    inputString.trim();  // Remove any whitespace
    
    // Create JSON document
    // Use StaticJsonDocument for small documents and DynamicJsonDocument for larger ones
    StaticJsonDocument<200> doc;
    
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, inputString);
    
    // Test if parsing succeeds
    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return;
    }
    
    // Check if it's a setTime command
    const char* command = doc["command"];
    if (command && strcmp(command, "setTime") == 0) {
        // Extract time values
        int year = doc["year"];
        int month = doc["month"];
        int day = doc["day"];
        int hour = doc["hour"];
        int minute = doc["minute"];
        int second = doc["second"];
        
        // Check if all values are valid
        if (year >= 2000 && year <= 2099 && month >= 1 && month <= 12 && 
            day >= 1 && day <= 31 && hour >= 0 && hour <= 23 && 
            minute >= 0 && minute <= 59 && second >= 0 && second <= 59) {
            
            // Set the RTC time
            rtc.setTime(year, month, day, hour, minute, second);
            
            // Create JSON response
            StaticJsonDocument<100> responseDoc;
            responseDoc["status"] = "success";
            responseDoc["message"] = "Time set successfully";
            
            serializeJson(responseDoc, Serial);
            Serial.println();
            
            // Check if power loss flag is still set after setting time
            if (rtc.isLostPower()) {
                Serial.println("Warning: Power loss flag still set. This may be a library issue.");
            }
        } else {
            // Create JSON error response
            StaticJsonDocument<100> responseDoc;
            responseDoc["status"] = "error";
            responseDoc["message"] = "Invalid time values";
            
            serializeJson(responseDoc, Serial);
            Serial.println();
        }
    } else {
        // Create JSON error response
        StaticJsonDocument<100> responseDoc;
        responseDoc["status"] = "error";
        responseDoc["message"] = "Unknown command";
        responseDoc["example"] = "{\"command\":\"setTime\",\"year\":2025,\"month\":4,\"day\":14,\"hour\":12,\"minute\":0,\"second\":0}";
        
        serializeJson(responseDoc, Serial);
        Serial.println();
    }
}
