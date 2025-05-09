/**
 * @file TaskACPower.cpp
 * @brief FreeRTOS task to measure AC current using a SEN0211 sensor and update power readings.
 *
 * This task uses an ADS1115 ADC and a SEN0211 current sensor to measure the AC current,
 * calculate power consumption, and update a shared measurement structure.
 * It also sets a status flag based on a secondary current threshold.
 */

#include <Arduino.h>
#include <Wire.h>
#include "TaskShared.h"
#include "TaskACPower.h"
//#include "I2CLock.h"

// External shared variables
extern Measurement sharedMeasurement;
extern DFRobot_ADS1115 ads;
extern SemaphoreHandle_t i2cMutex;


// SEN0211 sensor parameters
#define CURRENT_DETECTION_RANGE 20    // Set your sensor's current detection range (5A, 10A, 20A)
#define VOLTAGE_REFERENCE 240.0       // Reference voltage for your AC line (220V, 110V, etc.)
#define CALIBRATION_FACTOR 1.070      // Calibration factor: actual/measured (4.76/4.45)

// Input pins on ADS1115
const int CURRENT_PIN = 0;  // ADS1115 A0 for current measurement

// Number of samples to average for more stable readings
const int SAMPLES = 10;


/**
 * @brief Reads the RMS AC current value from a specified ADS1115 channel.
 *
 * Takes multiple ADC readings, calculates peak voltage, converts it to RMS voltage,
 * then to current using the sensor's range and calibration factor.
 *
 * @param pinNumber Analog input pin on ADS1115 (e.g., 0 for A0, 1 for A1).
 * @return Measured current in amperes.
 */
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
  

/**
 * @brief Calculates power in watts from current using a fixed voltage reference.
 *
 * @param current Current in amperes.
 * @return Power in watts.
 */
float calculatePower(float current) {
  return VOLTAGE_REFERENCE * current;
}

/**
 * @brief FreeRTOS task that periodically reads AC current and updates power measurements.
 *
 * Uses the SEN0211 sensor and ADS1115 ADC to read current from two input channels.
 * One channel is used to compute the power (ACPower), and the other sets a flag
 * based on whether the current exceeds 0.06A.
 *
 * @param pvParameters Unused parameter required by FreeRTOS.
 */
void TaskACPower(void *pvParameters) {
  (void)pvParameters;
  
  // // Initial delay to allow system to stabilize
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  

  while (true) {
    float ACPower = 300.0;
    float current = readACCurrentValue(0);
    ACPower = current * VOLTAGE_REFERENCE;
    


      sharedMeasurement.ACPower = ACPower;


    current = readACCurrentValue(1);


    if(current > 0.06){
      sharedMeasurement.flag = 1;
    } else {
      sharedMeasurement.flag = 0;
    }
      
  

     

    // Wait before next reading (2 seconds)
    vTaskDelay(40 / portTICK_PERIOD_MS);
  }
}
