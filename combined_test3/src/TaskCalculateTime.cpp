//tester å bare kalle rtc når den booter
#include "TaskCalculateTime.h"
#include "TaskShared.h"
#include "DFRobot_DS323X.h"
#include "I2CLock.h"

extern SemaphoreHandle_t i2cMutex;
extern DFRobot_DS323X rtc;
extern Measurement sharedMeasurement;

void TaskCalculateTime(void *pvParameters) {
  (void)pvParameters;

  time_t base_unix_time = 0;
  uint64_t base_millis = 0;

  const uint64_t THREE_HOURS_MS = 3ULL * 60 * 60 * 1000; // 3 hours in milliseconds
  uint64_t last_sync_millis = 0;

  auto syncWithRTC = [&]() {
    if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {
      uint16_t year   = rtc.getYear();
      uint8_t month   = rtc.getMonth();
      uint8_t date    = rtc.getDate();
      uint8_t hour    = rtc.getHour();
      uint8_t minute  = rtc.getMinute();
      uint8_t second  = rtc.getSecond();
      xSemaphoreGive(i2cMutex);

      struct tm timeinfo;
      timeinfo.tm_year = year - 1900;
      timeinfo.tm_mon  = month - 1;
      timeinfo.tm_mday = date;
      timeinfo.tm_hour = hour;
      timeinfo.tm_min  = minute;
      timeinfo.tm_sec  = second;
      timeinfo.tm_isdst = 0;

      base_unix_time = mktime(&timeinfo);
      base_millis = millis();
      last_sync_millis = base_millis;
    }
  };

  // Initial sync
  syncWithRTC();

  while (true) {
    uint64_t now = millis();

    // Re-sync every 3 hours
    if ((now - last_sync_millis) >= THREE_HOURS_MS) {
      syncWithRTC();
    }

    uint64_t elapsed_millis = now - base_millis;
    uint64_t unix_nanos = ((uint64_t)base_unix_time * 1000000000ULL) + (elapsed_millis * 1000000ULL);
    sharedMeasurement.timestamp = unix_nanos;

    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}

