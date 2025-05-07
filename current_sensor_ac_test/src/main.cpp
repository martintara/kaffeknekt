// /*!
//  * @file readACCurrent.
//  * @n This example reads Analog AC Current Sensor using ADS1115 ADC module.
//  * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (https://www.dfrobot.com)
//  * @licence     The MIT License (MIT)
//  * @get from https://www.dfrobot.com
//  * Adapted for ESP32 with ADS1115 I2C ADC module
// */

// #include <Arduino.h>
// #include <Wire.h>
// #include <Adafruit_ADS1X15.h>

// // Create an ADS1115 ADC instance
// Adafruit_ADS1115 ads;

// #define ACTectionRange 20     // Current sensor detection range (5A, 10A, 20A)

// // ADS1115 has internal reference voltage of 2.048V by default
// // We'll read raw values and convert based on actual measurements

// float readACCurrentValue()
// {
//   float ACCurrtntValue = 0;
//   float peakVoltage = 0;  
//   float voltageVirtualValue = 0;  // Vrms
  
//   // Take multiple samples to find peak voltage
//   for (int i = 0; i < 5; i++)
//   {
//     // Read from ADS1115 channel 0 (A0)
//     // ads.readADC_SingleEnded(0) returns a 16-bit value (0-32767 for ±2.048V range)
//     peakVoltage += ads.readADC_SingleEnded(0);
//     delay(1);
//   }
  
//   peakVoltage = peakVoltage / 5;   
//   voltageVirtualValue = peakVoltage * 0.707;    // Convert peak to RMS (Vrms = Vpeak * 0.707)
  
//   /*The circuit is amplified by 2 times, so it is divided by 2.*/
//   // Convert ADC value to voltage: 32767 = 2.048V (default ADS1115 voltage range when gain is set to GAIN_ONE)
//   voltageVirtualValue = (voltageVirtualValue / 32767.0 * 2.048) / 2;  
  
//   ACCurrtntValue = voltageVirtualValue * ACTectionRange;
//   return ACCurrtntValue;
// }

// void setup() 
// {
//   Serial.begin(115200);
//   pinMode(2, OUTPUT);  // ESP32 built-in LED
  
//   // Initialize I2C communication
//   Wire.begin();
  
//   // Initialize ADS1115
//   if (!ads.begin()) {
//     Serial.println("Failed to initialize ADS1115!");
//     while (1);
//   }
  
//   // Set gain to +/-2.048V since current sensor output is small
//   // You can adjust this if needed
//   ads.setGain(GAIN_ONE);  // +/-2.048V range (default)
  
//   Serial.println("ADS1115 initialized!");
//   Serial.println("SEN0211 Current Sensor with ADS1115 Test");
//   Serial.println("--------------------------------------");
// }

// void loop() 
// {
//   float ACCurrentValue = readACCurrentValue(); // Read AC Current Value
//   Serial.print("Current: ");
//   Serial.print(ACCurrentValue);
//   Serial.println(" A");
  
//   digitalWrite(2, HIGH);
//   delay(500);
//   digitalWrite(2, LOW);
//   delay(500);
// }

// #include <Arduino.h>
// #include <Wire.h>
// #include <DFRobot_ADS1115.h>

// // ADS1115 instance
// DFRobot_ADS1115 ads(&Wire);

// // SEN0211 sensor parameters
// #define CURRENT_DETECTION_RANGE 20    // Set your sensor's current detection range (5A, 10A, 20A)
// #define VOLTAGE_REFERENCE 230.0       // Reference voltage for your AC line (220V, 110V, etc.)
// // Function prototypes
// float readACCurrentValue();
// void displayReadings();
// // Input pins on ADS1115
// const int CURRENT_PIN = 0;  // ADS1115 A0 for current measurement

// // Variables for power calculation
// float current = 0;
// float voltage = VOLTAGE_REFERENCE;  // Using fixed voltage reference
// float power = 0;

// // Timing variables
// unsigned long lastDisplay = 0;
// const int DISPLAY_INTERVAL = 1000;  // Display readings every 1 second
// const int SAMPLES = 10;  // Number of samples to average

// void setup() {
//   Serial.begin(115200);
//   Serial.println("ESP32 SEN0211 AC Wattmeter Monitor");
  
//   // Initialize I2C for ESP32
//   Wire.begin();
  
//   // Configure ADS1115
//   ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);  // 0x48
//   ads.setGain(eGAIN_ONE);   // 1x gain (±4.096V range) - good for 0.2-2.8V range
//   ads.setMode(eMODE_SINGLE);      // Single-shot mode
//   ads.setRate(eRATE_128);         // 128 samples per second
//   ads.setOSMode(eOSMODE_SINGLE);  // Start a single-conversion
  
//   // Initialize ADS1115 without checking return value
//   ads.init();
  
//   // Check if ADS1115 is connected after initialization
//   if (ads.checkADS1115()) {
//     Serial.println("ADS1115 initialized successfully!");
//   } else {
//     Serial.println("Failed to initialize ADS1115. Check connections!");
//     while (1); // Stop if unable to initialize
//   }
// }

// void loop() {
//   // Check if ADS1115 is connected
//   if (ads.checkADS1115()) {
//     // Read current from SEN0211
//     current = readACCurrentValue();
    
//     // Calculate power (W = V * A)
//     power = voltage * current;
    
//     // Display readings at specified interval
//     if (millis() - lastDisplay >= DISPLAY_INTERVAL) {
//       displayReadings();
//       lastDisplay = millis();
//     }
//   } else {
//     Serial.println("ADS1115 Disconnected!");
//     delay(1000);
//   }
// }

// float readACCurrentValue() {
//   float peakVoltage = 0;
//   float voltageRMS = 0;  // RMS voltage (Vrms)
//   float currentValue = 0;
  
//   // Take multiple samples for more stable reading
//   for (int i = 0; i < SAMPLES; i++) {
//     int rawReading = ads.readVoltage(CURRENT_PIN); // Reading in mV
//     peakVoltage += rawReading;
//     delay(2); // Small delay between readings
//   }
  
//   // Average the readings
//   peakVoltage = peakVoltage / SAMPLES;
  
//   // Convert peak to RMS (root mean square) - using 0.707 factor
//   voltageRMS = peakVoltage * 0.707;
  
//   // Convert the ADC millivolts to actual volts
//   // No need to divide by 1024 as readVoltage already returns mV
//   voltageRMS = voltageRMS / 1000.0; // Convert mV to V
  
//   // Account for circuit amplification - adjust this factor based on your sensor
//   // The original code used division by 2 as circuit was amplified by 2 times
//   voltageRMS = voltageRMS / 2;
  
//   // Convert voltage to current using detection range
//   currentValue = voltageRMS * CURRENT_DETECTION_RANGE;
  
//   return currentValue;
// }

// void displayReadings() {
//   Serial.println("------ AC Power Measurements ------");
//   Serial.print("Current: ");
//   Serial.print(current);
//   Serial.println(" A");
  
//   Serial.print("Voltage (fixed): ");
//   Serial.print(voltage);
//   Serial.println(" V");
  
//   Serial.print("Power: ");
//   Serial.print(power);
//   Serial.println(" W");
  
//   Serial.println("---------------------------------");
// }








#include <Arduino.h>
#include <Wire.h>
#include <DFRobot_ADS1115.h>

// ADS1115 instance
DFRobot_ADS1115 ads(&Wire);

// SEN0211 sensor parameters
#define CURRENT_DETECTION_RANGE 20    // Set your sensor's current detection range (5A, 10A, 20A)
#define VOLTAGE_REFERENCE 230.0       // Reference voltage for your AC line (220V, 110V, etc.)
#define CALIBRATION_FACTOR 1.070      // Calibration factor: actual/measured (4.76/4.45)

// Function prototypes
float readACCurrentValue();
void displayReadings();

// Input pins on ADS1115
const int CURRENT_PIN = 1;  // ADS1115 A0 for current measurement

// Variables for power calculation
float current = 0;
float voltage = VOLTAGE_REFERENCE;  // Using fixed voltage reference
float power = 0;

// Timing variables
unsigned long lastDisplay = 0;
const int DISPLAY_INTERVAL = 100;  // Display readings every 1 second
const int SAMPLES = 10;  // Number of samples to average

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 SEN0211 AC Wattmeter Monitor");
  
  // Initialize I2C for ESP32
  Wire.begin();
  
  // Configure ADS1115
  ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);  // 0x48
  ads.setGain(eGAIN_ONE);   // 1x gain (±4.096V range) - good for 0.2-2.8V range
  ads.setMode(eMODE_SINGLE);      // Single-shot mode
  ads.setRate(eRATE_128);         // 128 samples per second
  ads.setOSMode(eOSMODE_SINGLE);  // Start a single-conversion
  
  // Initialize ADS1115 without checking return value
  ads.init();
  
  // Check if ADS1115 is connected after initialization
  if (ads.checkADS1115()) {
    Serial.println("ADS1115 initialized successfully!");
  } else {
    Serial.println("Failed to initialize ADS1115. Check connections!");
    while (1); // Stop if unable to initialize
  }
}

void loop() {
  // Check if ADS1115 is connected
  if (ads.checkADS1115()) {
    // Read current from SEN0211
    current = readACCurrentValue();
    
    // Calculate power (W = V * A)
    power = voltage * current;
    
    // Display readings at specified interval
    if (millis() - lastDisplay >= DISPLAY_INTERVAL) {
      //displayReadings();
      if(current >= 0.05){
        Serial.println("ON");
      } else{
        Serial.println("OFF");
      }
      lastDisplay = millis();
    }
  } else {
    Serial.println("ADS1115 Disconnected!");
    delay(1000);
  }
}

float readACCurrentValue() {
  float peakVoltage = 0;
  float voltageRMS = 0;  // RMS voltage (Vrms)
  float currentValue = 0;
  
  // Take multiple samples for more stable reading
  for (int i = 0; i < SAMPLES; i++) {
    int rawReading = ads.readVoltage(CURRENT_PIN); // Reading in mV
    peakVoltage += rawReading;
    delay(2); // Small delay between readings
  }
  
  // Average the readings
  peakVoltage = peakVoltage / SAMPLES;
  
  // Convert peak to RMS (root mean square) - using 0.707 factor
  voltageRMS = peakVoltage * 0.707;
  
  // Convert the ADC millivolts to actual volts
  // No need to divide by 1024 as readVoltage already returns mV
  voltageRMS = voltageRMS / 1000.0; // Convert mV to V
  
  // Account for circuit amplification - adjust this factor based on your sensor
  // The original code used division by 2 as circuit was amplified by 2 times
  voltageRMS = voltageRMS / 2;
  
  // Convert voltage to current using detection range
  currentValue = voltageRMS * CURRENT_DETECTION_RANGE;
  
  // Apply calibration factor to correct the reading
  currentValue = currentValue * CALIBRATION_FACTOR;
  return currentValue;
}

void displayReadings() {
  Serial.println("------ AC Power Measurements ------");
  Serial.print("Current: ");
  Serial.print(current);
  Serial.println(" A");
  
  Serial.print("Voltage (fixed): ");
  Serial.print(voltage);
  Serial.println(" V");
  
  Serial.print("Power: ");
  Serial.print(power);
  Serial.println(" W");
  
  Serial.println("---------------------------------");

  
}