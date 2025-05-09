/*!
 * @file readACCurrent.
 * @n This example reads Analog AC Current Sensor using ADS1115 ADC module.
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (https://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @get from https://www.dfrobot.com
 * Adapted for ESP32 with ADS1115 I2C ADC module
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Create an ADS1115 ADC instance
Adafruit_ADS1115 ads;

#define ACTectionRange 20     // Current sensor detection range (5A, 10A, 20A)

// ADS1115 has internal reference voltage of 2.048V by default
// We'll read raw values and convert based on actual measurements

float readACCurrentValue()
{
  float ACCurrtntValue = 0;
  float peakVoltage = 0;  
  float voltageVirtualValue = 0;  // Vrms
  
  // Take multiple samples to find peak voltage
  for (int i = 0; i < 5; i++)
  {
    // Read from ADS1115 channel 0 (A0)
    // ads.readADC_SingleEnded(0) returns a 16-bit value (0-32767 for ±2.048V range)
    peakVoltage += ads.readADC_SingleEnded(0);
    delay(1);
  }
  
  peakVoltage = peakVoltage / 5;   
  voltageVirtualValue = peakVoltage * 0.707;    // Convert peak to RMS (Vrms = Vpeak * 0.707)
  
  /*The circuit is amplified by 2 times, so it is divided by 2.*/
  // Convert ADC value to voltage: 32767 = 2.048V (default ADS1115 voltage range when gain is set to GAIN_ONE)
  voltageVirtualValue = (voltageVirtualValue / 32767.0 * 2.048) / 2;  
  
  ACCurrtntValue = voltageVirtualValue * ACTectionRange;
  return ACCurrtntValue;
}

void setup() 
{
  Serial.begin(115200);
  pinMode(2, OUTPUT);  // ESP32 built-in LED
  
  // Initialize I2C communication
  Wire.begin();
  
  // Initialize ADS1115
  if (!ads.begin()) {
    Serial.println("Failed to initialize ADS1115!");
    while (1);
  }
  
  // Set gain to +/-2.048V since current sensor output is small
  // You can adjust this if needed
  ads.setGain(GAIN_ONE);  // +/-2.048V range (default)
  
  Serial.println("ADS1115 initialized!");
  Serial.println("SEN0211 Current Sensor with ADS1115 Test");
  Serial.println("--------------------------------------");
}

void loop() 
{
  float ACCurrentValue = readACCurrentValue(); // Read AC Current Value
  Serial.print("Current: ");
  Serial.print(ACCurrentValue);
  Serial.println(" A");
  
  digitalWrite(2, HIGH);
  delay(500);
  digitalWrite(2, LOW);
  delay(500);
}
