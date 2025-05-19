/**
 * @file TaskReceiveTime.cpp
 * @brief FreeRTOS task that listens for JSON input over Serial to update the RTC.
 *
 * This task waits for a JSON message containing time information via the serial port.
 * If the JSON is valid and contains a `set_time` command along with time fields, it
 * updates the RTC module using the provided values.
 */

#include "TaskReceiveTime.h"
#include <ArduinoJson.h>
#include "DFRobot_DS323X.h"

/// Global RTC object used to set the time.
extern DFRobot_DS323X rtc;

/**
 * @brief FreeRTOS task to receive and process time-setting commands over Serial.
 *
 * Expects a JSON payload such as:
 * ```json
 * {
 *   "set_time": true,
 *   "year": 2025,
 *   "month": 5,
 *   "day": 8,
 *   "hour": 14,
 *   "minute": 30,
 *   "second": 0
 * }
 * ```
 * If valid, it sets the RTC accordingly and replies with a success message.
 *
 * @param pvParameters Unused parameter required by FreeRTOS task signature.
 */
void TaskReceiveTime(void *pvParameters) {
  (void)pvParameters;

  String input;

  while (true) {
    if (Serial.available()) {
      input = Serial.readStringUntil('\n');

      StaticJsonDocument<200> doc;
      DeserializationError err = deserializeJson(doc, input);

      if (err) {
        Serial.println("{\"error\":\"Invalid JSON\"}");
        continue;
      }

      if (doc["set_time"] == true &&
          doc.containsKey("year") &&
          doc.containsKey("month") &&
          doc.containsKey("day") &&
          doc.containsKey("hour") &&
          doc.containsKey("minute") &&
          doc.containsKey("second")) {

        int year = doc["year"].as<int>();
        int month = doc["month"].as<int>();
        int day = doc["day"].as<int>();
        int hour = doc["hour"].as<int>();
        int minute = doc["minute"].as<int>();
        int second = doc["second"].as<int>();

        rtc.setTime(year, month, day, hour, minute, second);

        Serial.println("{\"status\":\"RTC updated\"}");
      }
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
