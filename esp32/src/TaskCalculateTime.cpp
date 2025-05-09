/**
 * @file TaskCalculateTime.cpp
 * @brief Task that maintains a high-precision timestamp using an RTC and system timer.
 *
 * The RTC (DS323X) is queried once at boot to get the wall-clock time,
 * and then `millis()` is used to track elapsed time for sub-second resolution.
 * The time is re-synchronized with the RTC every 3 hours to avoid drift.
 */

#include "TaskCalculateTime.h"
#include "TaskShared.h"
#include "DFRobot_DS323X.h"
//#include "I2CLock.h"

extern SemaphoreHandle_t i2cMutex; /// External I2C mutex shared across tasks.
extern DFRobot_DS323X rtc; /// External real-time clock (RTC) object.
extern Measurement sharedMeasurement; /// Shared measurement structure updated with current timestamp.


/**
 * @brief FreeRTOS task that calculates and updates a high-resolution timestamp.
 *
 * This task initially syncs with the DS323X RTC to obtain the wall time, then uses
 * `millis()` to increment the time with millisecond resolution. Every 3 hours,
 * it re-syncs with the RTC to correct any drift.
 *
 * @param pvParameters Unused task parameter (can be `NULL`).
 */

void TaskCalculateTime(void *pvParameters) {
  (void)pvParameters;

  time_t base_unix_time = 0;
  uint64_t base_millis = 0;

  const uint64_t THREE_HOURS_MS = 3ULL * 60 * 60 * 1000; /// Duration in milliseconds between RTC re-synchronization.
  uint64_t last_sync_millis = 0; /// Timestamp of the last RTC sync in milliseconds since boot.

  /**
   * @brief Synchronizes the base time with the RTC.
   *
   * This internal helper (lambda) reads the current time from the RTC,
   * converts it to Unix time, and stores the current `millis()` value
   * to use as a reference for future calculations.
   */
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

