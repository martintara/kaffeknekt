/**
 * @file main.cpp
 * @brief Main entry point for the system. Initializes hardware and starts tasks.
 *
 * Sets up I2C, ADC (ADS1115), and RTC (DS3232). Creates and pins various sensor-reading and
 * data-publishing tasks to core 1. A shared measurement structure and an I2C mutex
 * are used for safe task communication and peripheral access.
 */

#include <Arduino.h>
#include <Wire.h>
#include "DFRobot_DS323X.h"
#include "TaskShared.h"
#include "TaskACPower.h"
#include "TaskPublish.h"
#include "TaskCalculateTime.h"
#include "TaskReceiveTime.h"
#include "TaskTemperature.h"
#include "TaskPressure.h"

#define SDA_PIN 21
#define SCL_PIN 22

DFRobot_DS323X rtc;  /// Global instance of rtc module
DFRobot_ADS1115 ads(&Wire); /// Global instance of ads object

Measurement sharedMeasurement = {}; /// Creation of shared measurement structure.
SemaphoreHandle_t i2cMutex;

/**
 * @brief ESP32 setup function. Initializes peripherals and starts tasks.
 */
void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  ///Initilizing ads
  ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);  // 0x48
  ads.setGain(eGAIN_ONE);        // ±4.096V range
  ads.setMode(eMODE_SINGLE);     // Single-shot mode
  ads.setRate(eRATE_128);        // 128 samples per second
  ads.setOSMode(eOSMODE_SINGLE); // Start a single conversion
  ads.init();
  /// Initializing rtc
  while (!rtc.begin()) {
    Serial.println("Failed to init DS3232 RTC chip.");
    delay(1000);
  }
  Serial.println("DS3232 RTC initialized successfully!");
  /// Creation of mutex to protect i2c bus
  i2cMutex = xSemaphoreCreateMutex();

  /// Creation of tasks in the system
  xTaskCreatePinnedToCore(TaskCalculateTime, "CalculateTime", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskPressure, "Pressure", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskTemperature, "Temperature", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskACPower, "ACPower", 4096, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskPublish, "Publish", 4096, NULL, 1, NULL, 1);

}

/**
 * @brief ESP32 loop function. Left empty since all functionality is handled by tasks.
 */
void loop() {
  // Tasks do the work
}
