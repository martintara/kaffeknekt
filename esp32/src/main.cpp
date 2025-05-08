/**
 * @file main.cpp
 * @brief Main entry point for the FreeRTOS system. Initializes hardware and starts tasks.
 *
 * Sets up I²C, ADC (ADS1115), and RTC (DS3232). Creates and pins various sensor-reading and
 * data-publishing FreeRTOS tasks to core 1. A shared measurement structure and an I²C mutex
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
//#include "TaskBrewTrigger.h"

#define SDA_PIN 21
#define SCL_PIN 22

DFRobot_DS323X rtc;  // global instance
DFRobot_ADS1115 ads(&Wire);

Measurement sharedMeasurement = {};
SemaphoreHandle_t i2cMutex;

/**
 * @brief Arduino setup function. Initializes peripherals and starts FreeRTOS tasks.
 */
void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  // SPI.begin();

  ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);  // 0x48
  ads.setGain(eGAIN_ONE);        // ±4.096V range
  ads.setMode(eMODE_SINGLE);     // Single-shot mode
  ads.setRate(eRATE_128);        // 128 samples per second
  ads.setOSMode(eOSMODE_SINGLE); // Start a single conversion
  ads.init();


  while (!rtc.begin()) {
    Serial.println("Failed to init DS3232 RTC chip.");
    delay(1000);
  }
  Serial.println("DS3232 RTC initialized successfully!");

  i2cMutex = xSemaphoreCreateMutex();


  xTaskCreatePinnedToCore(TaskCalculateTime, "CalculateTime", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskPressure, "Pressure", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskTemperature, "Temperature", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskACPower, "ACPower", 4096, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskPublish, "Publish", 4096, NULL, 1, NULL, 1);
  //xTaskCreatePinnedToCore(TaskReceiveTime, "RecieveTime", 4096, NULL, 1, NULL, 1);
  //xTaskCreatePinnedToCore(TaskBrewTrigger, "BrewTrigger", 4096, NULL, 2, NULL, 1);

}

/**
 * @brief Arduino loop function. Left empty since all functionality is handled by tasks.
 */
void loop() {
  // Tasks do the work
}
