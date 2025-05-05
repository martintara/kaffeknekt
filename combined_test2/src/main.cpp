#include <Arduino.h>
#include <Wire.h>
#include "DFRobot_DS323X.h"
#include "TaskCalculateTime.h"
#include "TaskPressure.h"
#include "TaskTemperature.h"
#include "TaskPublish.h"
#include "TaskReceiveTime.h"
#include "TaskShared.h"

#define SDA_PIN 21
#define SCL_PIN 22

DFRobot_DS323X rtc;  // global instance

Measurement sharedMeasurement = {};
SemaphoreHandle_t measurementMutex;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  while (!rtc.begin()) {
    Serial.println("Failed to init DS3232 RTC chip.");
    delay(1000);
  }
  Serial.println("DS3232 RTC initialized successfully!");

  measurementMutex = xSemaphoreCreateMutex();

  xTaskCreatePinnedToCore(TaskCalculateTime, "SendTime", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskPressure, "Pressure", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskTemperature, "Pressure", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskPublish, "Publish", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskReceiveTime, "Publish", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // Tasks do the work
}
