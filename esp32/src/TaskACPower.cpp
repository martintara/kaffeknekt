/**
 * @file TaskACPower.cpp
 * @brief Task that controls gets current measurements from SEN0211 sensor and updates power readings.
 *
 * This task uses an ADS1115 ADC and a SEN0211 current sensor to measure the AC current,
 * calculate power consumption, and updates power variable in a shared measurement structure.
 * It also takes measurement from an aditional SEN0211 sensor to discovere when brewing starts and stops.
 */

#include <Arduino.h>
#include <Wire.h>
#include "TaskShared.h"
#include "TaskACPower.h"

/// External shared variables defined in main
extern SemaphoreHandle_t i2cMutex;
extern Measurement sharedMeasurement;
extern DFRobot_ADS1115 ads;

/// SEN0211 sensor parameters
#define CURRENT_DETECTION_RANGE 20    /// Sets the sensor's current detection range (5A, 10A, 20A)
#define VOLTAGE_REFERENCE 240.0       /// Reference voltage the AC line.
#define CALIBRATION_FACTOR 1.070      /// Calibration factor: actual/measured (4.76/4.45)

/// Input pin on ADS1115
const int CURRENT_PIN = 0;  // ADS1115 A0 for current measurement

// Number of samples to average for more stable readings
const int SAMPLES = 10;

/**
 * @brief Reads the RMS AC current value from a specified ADS1115 channel.
 *
 * Takes multiple ADC readings, calculates peak voltage, converts it to RMS voltage,
 * then to current using the sensor's range and calibration factor.
 *
 * @param pinNumber Analog input pin on ADS1115.
 * @return Measured current in amperes.
 */
float readACCurrentValue(int pinNumber) {
    float peakVoltage = 0;
    float voltageRMS = 0;
    float currentValue = 0;
    
    /// Take multiple samples for more stable reading
    const int SAMPLES = 10;

    if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {
      for (int i = 0; i < SAMPLES; i++) {
        int rawReading = ads.readVoltage(pinNumber); /// Reading in mV
        peakVoltage += rawReading;
        vTaskDelay(2 / portTICK_PERIOD_MS);
      }
      xSemaphoreGive(i2cMutex);
    }
   
    /// Average the readings
    peakVoltage = peakVoltage / SAMPLES;
    
    /// Converts peak to RMS (root mean square) - using 0.707 factor
    voltageRMS = peakVoltage * 0.707;
    
    /// Convert the ADC millivolts to volts
    voltageRMS = voltageRMS / 1000.0;
    
    /// Account for circuit amplification
    voltageRMS = voltageRMS / 2;
    
    /// Convert voltage to current using detection range
    currentValue = voltageRMS * CURRENT_DETECTION_RANGE;
    
    /// Apply calibration factor to correct the reading
    currentValue = currentValue * CALIBRATION_FACTOR;
    
    return currentValue;
  }
  

// slettes ? /**
//  * @brief Calculates power in watts from current using a fixed voltage reference.
//  *
//  * @param current Current in amperes.
//  * @return Power in watts.
//  */
// float calculatePower(float current) {
//   return VOLTAGE_REFERENCE * current;
// }

/**
 * @brief Task that periodically reads AC current and updates power measurements.
 *
 * Uses the SEN0211 sensor and ADS1115 ADC to read current from two input channels.
 * One channel is used to compute the power (ACPower), and the other sets a flag
 * based on whether the current exceeds 0.06A, indicating current brewing status.
 *
 * @param pvParameters Unused parameter required by FreeRTOS.
 */
void TaskACPower(void *pvParameters) {
  (void)pvParameters;
  
  /// Initial delay to allow system to stabilize
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  while (true) {
    float ACPower = 0.0;
    /// Takes a reading from first SEN0211 sensor
    float current = readACCurrentValue(0);
    ACPower = current * VOLTAGE_REFERENCE;
   
    sharedMeasurement.ACPower = ACPower;

    current = readACCurrentValue(1);

    /// This checks if there is a current running to the solinoid, acting as a trigger noticing when we are brewing.
    if(current > 0.06){
      sharedMeasurement.flag = 1;
    } else {
      sharedMeasurement.flag = 0;
    }
    vTaskDelay(40 / portTICK_PERIOD_MS);
  }
}
