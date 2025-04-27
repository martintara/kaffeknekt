// #include <Arduino.h>
// #include "BluetoothSerial.h"

// // Check if Bluetooth is properly supported by the ESP32 board configuration
// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` and enable it
// #endif

// // Initialize Bluetooth Serial
// BluetoothSerial SerialBT;

// // Buffer for receiving messages
// char incomingData[64];
// int dataIndex = 0;

// // LED pin
// const int ledPin = LED_BUILTIN;  // Using built-in LED

// void setup() {
//   // Initialize serial communication for debugging
//   Serial.begin(115200);
//   delay(1000); // Short delay to allow serial monitor to connect
  
//   Serial.println("ESP32 FireBeetle2 Bluetooth Communication");
  
//   // Print Bluetooth MAC Address
//   uint8_t mac[6];
//   esp_read_mac(mac, ESP_MAC_BT);
//   Serial.print("ESP32 Bluetooth MAC Address: ");
//   for (int i = 0; i < 6; i++) {
//     Serial.printf("%02X", mac[i]);
//     if (i < 5) Serial.print(":");
//   }
//   Serial.println();
  
//   // Initialize Bluetooth device name
//   SerialBT.begin("ESP32_FireBeetle2");
//   Serial.println("Bluetooth device started, you can pair with it now!");
  
//   // Initialize LED
//   pinMode(ledPin, OUTPUT);
//   digitalWrite(ledPin, LOW);
  
//   // Flash LED twice to indicate successful startup
//   for (int i = 0; i < 2; i++) {
//     digitalWrite(ledPin, HIGH);
//     delay(200);
//     digitalWrite(ledPin, LOW);
//     delay(200);
//   }
// }

// // Process received commands
// void processCommand(const char* command) {
//   Serial.print("Processing command: ");
//   Serial.println(command);
  
//   // Echo the received command back via Bluetooth
//   SerialBT.print("Received: ");
//   SerialBT.println(command);
  
//   // Convert to uppercase for case-insensitive comparison
//   String cmd = String(command);
//   cmd.toUpperCase();
  
//   // Handle different commands
//   if (cmd == "LED_ON") {
//     digitalWrite(ledPin, HIGH);
//     SerialBT.println("ACK:LED is now ON");
//     Serial.println("LED turned ON");
//   } 
//   else if (cmd == "LED_OFF") {
//     digitalWrite(ledPin, LOW);
//     SerialBT.println("ACK:LED is now OFF");
//     Serial.println("LED turned OFF");
//   }
//   else if (cmd == "TEMP") {
//     // Simulate a temperature reading
//     float temp = 20.0 + (float)random(100) / 10.0;
//     SerialBT.print("TEMP:");
//     SerialBT.println(temp);
//     Serial.print("Sent temperature: ");
//     Serial.println(temp);
//   }
//   else if (cmd == "STATUS") {
//     // Return device status
//     String status = "STATUS:";
//     status += "LED=" + String(digitalRead(ledPin) ? "ON" : "OFF");
//     status += ",UPTIME=" + String(millis() / 1000) + "s";
//     status += ",HEAP=" + String(ESP.getFreeHeap()) + "bytes";
    
//     SerialBT.println(status);
//     Serial.println("Status sent: " + status);
//   }
//   else if (cmd == "HELP") {
//     // Send help information
//     SerialBT.println("HELP:");
//     SerialBT.println("LED_ON - Turn LED on");
//     SerialBT.println("LED_OFF - Turn LED off");
//     SerialBT.println("TEMP - Get temperature");
//     SerialBT.println("STATUS - Get device status");
//     SerialBT.println("HELP - Show this help");
//     Serial.println("Help information sent");
//   }
//   else {
//     SerialBT.println("ERR:Unknown command");
//     Serial.println("Unknown command received");
//   }
// }

// void loop() {
//   // Check for incoming Bluetooth data
//   if (SerialBT.available()) {
//     char inChar = SerialBT.read();
    
//     // Echo characters to Serial monitor for debugging
//     Serial.print(inChar);
    
//     // If we receive a newline or carriage return, process the command
//     if (inChar == '\n' || inChar == '\r') {
//       if (dataIndex > 0) {  // Only process if we have data
//         incomingData[dataIndex] = '\0';  // Null-terminate the string
//         processCommand(incomingData);
//         dataIndex = 0;  // Reset buffer index
//       }
//     } else {
//       // Add character to buffer if there's room
//       if (dataIndex < sizeof(incomingData) - 1) {
//         incomingData[dataIndex++] = inChar;
//       }
//     }
//   }
  
//   // Check for incoming Serial data (for debugging and testing)
//   if (Serial.available()) {
//     char inChar = Serial.read();
    
//     // If we receive a newline, process the command from Serial too
//     if (inChar == '\n' || inChar == '\r') {
//       if (dataIndex > 0) {
//         incomingData[dataIndex] = '\0'; 
//         Serial.print("Processing Serial command: ");
//         Serial.println(incomingData);
//         processCommand(incomingData);
//         dataIndex = 0;
//       }
//     } else {
//       // Add character to buffer
//       if (dataIndex < sizeof(incomingData) - 1) {
//         incomingData[dataIndex++] = inChar;
//       }
//     }
//   }
  
//   // Send heartbeat message every 5 seconds
//   static unsigned long lastHeartbeat = 0;
//   if (millis() - lastHeartbeat > 5000) {
//     SerialBT.println("HEARTBEAT:" + String(millis()));
//     Serial.println("Heartbeat sent at " + String(millis()) + "ms");
//     lastHeartbeat = millis();
    
//     // Blink the LED briefly as a visual indicator
//     if (digitalRead(ledPin) == LOW) {  // Only blink if LED is off
//       digitalWrite(ledPin, HIGH);
//       delay(10);
//       digitalWrite(ledPin, LOW);
//     }
//   }
  
//   // Small delay to prevent CPU hogging
//   delay(20);
// }




#include <Arduino.h>
#include "BluetoothSerial.h"
#include <ArduinoJson.h>

// Check if Bluetooth is properly supported by the ESP32 board configuration
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` and enable it
#endif

// Initialize Bluetooth Serial
BluetoothSerial SerialBT;

// Buffer for receiving messages
char incomingData[128];
int dataIndex = 0;

// LED pin
const int ledPin = LED_BUILTIN;  // Using built-in LED

// Timer for sending sensor data
unsigned long lastSensorUpdate = 0;
const long sensorInterval = 2000;  // Send sensor data every 2 seconds

// Function declarations
void sendSensorData();
void processCommand(const char* command);

void setup() {
  // Initialize serial communication for debugging
  Serial.begin(115200);
  delay(1000); // Short delay to allow serial monitor to connect
  
  Serial.println("ESP32 FireBeetle2 Bluetooth JSON Sensor Data");
  
  // Print Bluetooth MAC Address
  uint8_t mac[6];
  esp_read_mac(mac, ESP_MAC_BT);
  Serial.print("ESP32 Bluetooth MAC Address: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", mac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  
  // Initialize random seed based on analog noise
  randomSeed(analogRead(0));
  
  // Initialize Bluetooth device name
  SerialBT.begin("ESP32_FireBeetle2");
  Serial.println("Bluetooth device started, you can pair with it now!");
  
  // Initialize LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  
  // Flash LED twice to indicate successful startup
  for (int i = 0; i < 2; i++) {
    digitalWrite(ledPin, HIGH);
    delay(200);
    digitalWrite(ledPin, LOW);
    delay(200);
  }
  
  // Send initial sensor data
  sendSensorData();
}

// Generate and send random sensor data in JSON format
void sendSensorData() {
  // Create a JSON document
  // Size calculated with https://arduinojson.org/v6/assistant/
  StaticJsonDocument<256> doc;
  
  // Add timestamp
  doc["timestamp"] = millis();
  
  // Add device info
  doc["device"] = "ESP32_FireBeetle2";
  
  // Generate random sensor values
  JsonObject sensors = doc.createNestedObject("sensors");
  
  // Temperature (20-30°C)
  sensors["temperature"] = 20.0 + (random(100) / 10.0);
  
  // Humidity (30-80%)
  sensors["humidity"] = 30 + random(50);
  
  // Pressure (980-1020 hPa)
  sensors["pressure"] = 980 + random(40);
  
  // Light level (0-1000)
  sensors["light"] = random(1000);
  
  // Battery level (0-100%)
  sensors["battery"] = random(100);
  
  // Motion detection (boolean)
  sensors["motion"] = random(2) == 1;
  
  // Add system status
  JsonObject system = doc.createNestedObject("system");
  system["heap"] = ESP.getFreeHeap();
  system["uptime"] = millis() / 1000;
  system["led"] = digitalRead(ledPin) == HIGH;
  
  // Serialize JSON to string
  String jsonString;
  serializeJson(doc, jsonString);
  
  // Send over Bluetooth
  SerialBT.println(jsonString);
  
  // Debug output
  Serial.println("Sent sensor data: " + jsonString);
  
  // Update timestamp
  lastSensorUpdate = millis();
}

// Process received commands
void processCommand(const char* command) {
  Serial.print("Processing command: ");
  Serial.println(command);
  
  // Create a JSON response document
  StaticJsonDocument<128> response;
  response["type"] = "response";
  response["command"] = command;
  
  // Convert to uppercase for case-insensitive comparison
  String cmd = String(command);
  cmd.toUpperCase();
  
  // Handle different commands
  if (cmd == "LED_ON") {
    digitalWrite(ledPin, HIGH);
    response["status"] = "success";
    response["message"] = "LED is now ON";
    Serial.println("LED turned ON");
  } 
  else if (cmd == "LED_OFF") {
    digitalWrite(ledPin, LOW);
    response["status"] = "success";
    response["message"] = "LED is now OFF";
    Serial.println("LED turned OFF");
  }
  else if (cmd == "GET_SENSORS") {
    // Send sensor data immediately
    sendSensorData();
    return; // Already sent data, no need for additional response
  }
  else if (cmd == "STATUS") {
    // Device status is included in response
    response["status"] = "success";
    response["uptime"] = millis() / 1000;
    response["heap"] = ESP.getFreeHeap();
    response["led"] = digitalRead(ledPin) == HIGH;
  }
  else if (cmd == "HELP") {
    response["status"] = "success";
    response["commands"] = "LED_ON, LED_OFF, GET_SENSORS, STATUS, HELP";
  }
  else {
    response["status"] = "error";
    response["message"] = "Unknown command";
    Serial.println("Unknown command received");
  }
  
  // Serialize and send the response
  String jsonResponse;
  serializeJson(response, jsonResponse);
  SerialBT.println(jsonResponse);
  
  Serial.println("Sent response: " + jsonResponse);
}

void loop() {
  // Check for incoming Bluetooth data
  if (SerialBT.available()) {
    char inChar = SerialBT.read();
    
    // Echo characters to Serial monitor for debugging
    Serial.print(inChar);
    
    // If we receive a newline or carriage return, process the command
    if (inChar == '\n' || inChar == '\r') {
      if (dataIndex > 0) {  // Only process if we have data
        incomingData[dataIndex] = '\0';  // Null-terminate the string
        processCommand(incomingData);
        dataIndex = 0;  // Reset buffer index
      }
    } else {
      // Add character to buffer if there's room
      if (dataIndex < sizeof(incomingData) - 1) {
        incomingData[dataIndex++] = inChar;
      }
    }
  }
  
  // Check for incoming Serial data (for debugging and testing)
  if (Serial.available()) {
    char inChar = Serial.read();
    
    // If we receive a newline, process the command from Serial too
    if (inChar == '\n' || inChar == '\r') {
      if (dataIndex > 0) {
        incomingData[dataIndex] = '\0'; 
        Serial.print("Processing Serial command: ");
        Serial.println(incomingData);
        processCommand(incomingData);
        dataIndex = 0;
      }
    } else {
      // Add character to buffer
      if (dataIndex < sizeof(incomingData) - 1) {
        incomingData[dataIndex++] = inChar;
      }
    }
  }
  
  // Send sensor data periodically
  if (millis() - lastSensorUpdate > sensorInterval) {
    sendSensorData();
  }
  
  // Blink the LED briefly as a visual indicator when sending data
  if (digitalRead(ledPin) == LOW && millis() - lastSensorUpdate < 100) {
    digitalWrite(ledPin, HIGH);
    delay(10);
    digitalWrite(ledPin, LOW);
  }
  
  // Small delay to prevent CPU hogging
  delay(20);
}