
#include "TaskCalculateTime.h"
#include "TaskShared.h"
#include "DFRobot_DS323X.h"
#include "I2CLock.h"
extern SemaphoreHandle_t i2cMutex;


extern DFRobot_DS323X rtc;
extern Measurement sharedMeasurement;

void TaskCalculateTime(void *pvParameters) {
  (void)pvParameters;

  while (true) {
    uint16_t year;
    uint8_t month, date, hour, minute, second;

      // Grouped I2C reads inside one lock


      if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {
        year   = rtc.getYear();
        month  = rtc.getMonth();
        date   = rtc.getDate();
        hour   = rtc.getHour();
        minute = rtc.getMinute();
        second = rtc.getSecond();
        xSemaphoreGive(i2cMutex);
      }



    struct tm timeinfo;
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon  = month - 1;
    timeinfo.tm_mday = date;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min  = minute;
    timeinfo.tm_sec  = second;
    timeinfo.tm_isdst = 0;

    time_t unix_seconds = mktime(&timeinfo);
    uint64_t unix_nanos = (uint64_t)unix_seconds * 1000000000ULL;

    // if (xSemaphoreTake(measurementMutex, pdMS_TO_TICKS(100))) {
      sharedMeasurement.timestamp = unix_nanos;
      // xSemaphoreGive(measurementMutex);
    // }
    //sharedMeasurement.timestamp = unix_nanos;

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
