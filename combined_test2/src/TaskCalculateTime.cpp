// #include "TaskSendTime.h"
// #include <ArduinoJson.h>
// #include "DFRobot_DS323X.h"

// extern DFRobot_DS323X rtc;

// void TaskSendTime(void *pvParameters) {
//   (void)pvParameters;

//   while (true) {
//     StaticJsonDocument<128> doc;

//     // Get current time
//     uint16_t year = rtc.getYear();
//     uint8_t month = rtc.getMonth();
//     uint8_t day = rtc.getDate();
//     uint8_t hour = rtc.getHour();
//     uint8_t minute = rtc.getMinute();
//     uint8_t second = rtc.getSecond();

//     // Convert to Unix timestamp (seconds since epoch)
//     struct tm timeinfo;
//     timeinfo.tm_year = year - 1900;
//     timeinfo.tm_mon  = month - 1;
//     timeinfo.tm_mday = day;
//     timeinfo.tm_hour = hour;
//     timeinfo.tm_min  = minute;
//     timeinfo.tm_sec  = second;
//     timeinfo.tm_isdst = 0;

//     time_t unix_seconds = mktime(&timeinfo);
//     int64_t unix_nanos = (int64_t)unix_seconds * 1000000000LL;

//     doc["timestamp"] = unix_nanos;
//     serializeJson(doc, Serial);
//     Serial.println();

//     vTaskDelay(pdMS_TO_TICKS(2000));
//   }
// }


#include "TaskCalculateTime.h"
#include "TaskShared.h"
#include "DFRobot_DS323X.h"

extern DFRobot_DS323X rtc;
extern Measurement sharedMeasurement;
extern SemaphoreHandle_t measurementMutex;

void TaskCalculateTime(void *pvParameters) {
  (void)pvParameters;

  while (true) {
    uint16_t year = rtc.getYear();
    uint8_t month = rtc.getMonth();
    uint8_t day = rtc.getDate();
    uint8_t hour = rtc.getHour();
    uint8_t minute = rtc.getMinute();
    uint8_t second = rtc.getSecond();

    struct tm timeinfo;
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon  = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min  = minute;
    timeinfo.tm_sec  = second;
    timeinfo.tm_isdst = 0;

    time_t unix_seconds = mktime(&timeinfo);
    uint64_t unix_nanos = (uint64_t)unix_seconds * 1000000000ULL;

    if (xSemaphoreTake(measurementMutex, pdMS_TO_TICKS(100))) {
      sharedMeasurement.timestamp = unix_nanos;
      xSemaphoreGive(measurementMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
