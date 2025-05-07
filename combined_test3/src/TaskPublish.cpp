#include "TaskPublish.h"
#include "TaskShared.h"
#include <ArduinoJson.h>

extern Measurement sharedMeasurement;
extern SemaphoreHandle_t measurementMutex;

void TaskPublish(void *pvParameters) {
  (void)pvParameters;

  while (true) {
    Measurement localCopy;

   // if (xSemaphoreTake(measurementMutex, pdMS_TO_TICKS(100))) {
      localCopy = sharedMeasurement;
      //xSemaphoreGive(measurementMutex);
    //}

    StaticJsonDocument<128> doc;
    //doc["pressure"] = localCopy.pressure;
    doc["temperature"] = localCopy.temperature;
    doc["power"] = localCopy.ACPower;
    doc["timestamp"] = localCopy.timestamp;
    doc["flag"] = localCopy.flag;
    serializeJson(doc, Serial);
    Serial.println();

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}