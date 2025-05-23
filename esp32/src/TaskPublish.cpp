/**
 * @file TaskPublish.cpp
 * @brief Task that serializes and publishes sensor data over Serial in JSON format.
 *
 * This task reads the most recent sensor measurements and the timestamp from the shared structure
 * and sends the data as a JSON object over the Serial interface at regular intervals.
 */

#include "TaskPublish.h"
#include "TaskShared.h"
#include <ArduinoJson.h>

/// Shared structure containing the latest sensor measurements.
extern Measurement sharedMeasurement;

/**
 * @brief Task that publishes sensor data as JSON over Serial.
 *
 * Every 300 ms, this task creates a JSON object containing pressure, temperature,
 * power, timestamp, and a status flag from the shared measurement structure.
 * It sends the JSON over Serial to the Raspberry Pi.
 *
 * @param pvParameters Unused parameter required by FreeRTOS task signature.
 */
void TaskPublish(void *pvParameters) {
  (void)pvParameters;

  while (true) {
    Measurement localCopy;

    localCopy = sharedMeasurement;

    StaticJsonDocument<128> doc;
    doc["pressure"] = localCopy.pressure;
    doc["temperature"] = localCopy.temperature;
    doc["power"] = localCopy.ACPower;
    doc["timestamp"] = localCopy.timestamp;
    doc["flag"] = localCopy.flag;

    serializeJson(doc, Serial);
    Serial.println(); /// Print line indicates end of message

    vTaskDelay(pdMS_TO_TICKS(300));
  }
}
