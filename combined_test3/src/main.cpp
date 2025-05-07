#include <Arduino.h>
#include <Wire.h>
#include "DFRobot_DS323X.h"
#include "TaskShared.h"
#include "TaskACPower.h"
#include "TaskPublish.h"
#include "TaskCalculateTime.h"
#include "TaskReceiveTime.h"

#define SDA_PIN 21
#define SCL_PIN 22

DFRobot_DS323X rtc;  // global instance
DFRobot_ADS1115 ads(&Wire);

Measurement sharedMeasurement = {};
SemaphoreHandle_t i2cMutex;


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
  // if (measurementMutex == NULL) {
  //   Serial.println("Failed to create measurementMutex!");
  //   while (true);  // Stop everything to prevent undefined behavior
  // }

  xTaskCreatePinnedToCore(TaskCalculateTime, "CalculateTime", 4096, NULL, 1, NULL, 1);
  //xTaskCreatePinnedToCore(TaskPressure, "Pressure", 4096, NULL, 1, NULL, 1);
  //xTaskCreatePinnedToCore(TaskTemperature, "Temperature", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskACPower, "ACPower", 4096, NULL, 2, NULL, 1);
  xTaskCreatePinnedToCore(TaskPublish, "Publish", 4096, NULL, 1, NULL, 1);
  //xTaskCreatePinnedToCore(TaskReceiveTime, "RecieveTime", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // Tasks do the work
}
