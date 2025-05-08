#include <Arduino.h>
#include <Wire.h>
#include "TaskShared.h"
#include "TaskACPower.h"
#include "I2CLock.h"

// External shared variables
extern Measurement sharedMeasurement;
extern DFRobot_ADS1115 ads;
extern SemaphoreHandle_t i2cMutex;

// ADS1115 instance
//DFRobot_ADS1115 ads(&Wire);

// SEN0211 sensor parameters
#define CURRENT_DETECTION_RANGE 20    // Set your sensor's current detection range (5A, 10A, 20A)
#define VOLTAGE_REFERENCE 240.0       // Reference voltage for your AC line (220V, 110V, etc.)
#define CALIBRATION_FACTOR 1.070      // Calibration factor: actual/measured (4.76/4.45)

// Input pins on ADS1115
const int CURRENT_PIN = 0;  // ADS1115 A0 for current measurement

// Number of samples to average for more stable readings
const int SAMPLES = 10;

float readACCurrentValue(int pinNumber) {
    float peakVoltage = 0;
    float voltageRMS = 0;
    float currentValue = 0;
    
    // Take multiple samples for more stable reading
    const int SAMPLES = 10;

    if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {
      for (int i = 0; i < SAMPLES; i++) {
        int rawReading = ads.readVoltage(pinNumber); // Reading in mV
        peakVoltage += rawReading;
        vTaskDelay(2 / portTICK_PERIOD_MS);
  ; // Small delay between readings
      }
      xSemaphoreGive(i2cMutex);
    }


    
    // Average the readings
    peakVoltage = peakVoltage / SAMPLES;
    
    // Convert peak to RMS (root mean square) - using 0.707 factor
    voltageRMS = peakVoltage * 0.707;
    
    // Convert the ADC millivolts to actual volts
    voltageRMS = voltageRMS / 1000.0; // Convert mV to V
    
    // Account for circuit amplification
    voltageRMS = voltageRMS / 2;
    
    // Convert voltage to current using detection range
    currentValue = voltageRMS * CURRENT_DETECTION_RANGE;
    
    // Apply calibration factor to correct the reading
    currentValue = currentValue * CALIBRATION_FACTOR;
    
    return currentValue;
  }
  

// Function to calculate power based on current and fixed voltage
float calculatePower(float current) {
  return VOLTAGE_REFERENCE * current;
}

// Task function for AC power measurement
void TaskACPower(void *pvParameters) {
  (void)pvParameters;
  
  // // Initial delay to allow system to stabilize
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  

  while (true) {
    float ACPower = 300.0;
    float current = readACCurrentValue(0);
    ACPower = current * VOLTAGE_REFERENCE;
    

    // if (xSemaphoreTake(measurementMutex, pdMS_TO_TICKS(100))) {
      sharedMeasurement.ACPower = ACPower;
      // xSemaphoreGive(measurementMutex);
    // }

    current = readACCurrentValue(1);
    //Serial.println(current);
    //Serial.println("ads1 " + String(current));

    if(current > 0.06){
      sharedMeasurement.flag = 1;
    } else {
      sharedMeasurement.flag = 0;
    }
      
  

     

    // Wait before next reading (2 seconds)
    vTaskDelay(40 / portTICK_PERIOD_MS);
  }
}
