//testprogram for å finne ut av interface for å stille klokka
//testet og fungerer

#include "DFRobot_DS323X.h"
#include <Wire.h>
#include <ArduinoJson.h>

// Define I2C pins for ESP32
#define SDA_PIN 21
#define SCL_PIN 22

DFRobot_DS323X rtc;

// Tasks
void TaskSendTime(void *pvParameters);
void TaskReceiveTime(void *pvParameters);

void setup(void) {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  while (rtc.begin() != true) {
    Serial.println("Failed to init DS3232 RTC chip, please check if the chip connection is fine.");
    delay(1000);
  }

  Serial.println("DS3232 RTC initialized successfully!");

  // Create tasks
  xTaskCreatePinnedToCore(TaskSendTime, "SendTime", 4096, NULL, 1, NULL, 1);
  xTaskCreatePinnedToCore(TaskReceiveTime, "ReceiveTime", 4096, NULL, 1, NULL, 1);
}

void loop() {
  // Nothing here – all logic is in tasks
}

void TaskSendTime(void *pvParameters) {
  (void)pvParameters;

  while (true) {
    StaticJsonDocument<128> doc;

    doc["year"] = rtc.getYear();
    doc["month"] = rtc.getMonth();
    doc["day"] = rtc.getDate();
    doc["hour"] = rtc.getHour();
    doc["minute"] = rtc.getMinute();
    doc["second"] = rtc.getSecond();

    serializeJson(doc, Serial);
    Serial.println();

    vTaskDelay(pdMS_TO_TICKS(2000)); // Wait 2 seconds
  }
}

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

    vTaskDelay(pdMS_TO_TICKS(100)); // Small delay to avoid hogging CPU
  }
}
