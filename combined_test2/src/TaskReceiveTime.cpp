#include "TaskReceiveTime.h"
#include <ArduinoJson.h>
#include "DFRobot_DS323X.h"

extern DFRobot_DS323X rtc;

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

        int year = doc["year"];
        int month = doc["month"];
        int day = doc["day"];
        int hour = doc["hour"];
        int minute = doc["minute"];
        int second = doc["second"];

        rtc.setTime(year, month, day, hour, minute, second);

        Serial.println("{\"status\":\"RTC updated\"}");
      }
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}
