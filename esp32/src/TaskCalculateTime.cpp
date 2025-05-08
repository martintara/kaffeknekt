/**
 * @file TaskCalculateTime.cpp
 * @brief FreeRTOS task that reads time from an RTC over I²C and updates a UNIX timestamp.
 *
 * This task reads the current date and time from an RTC module using I²C,
 * converts it to a UNIX timestamp in nanoseconds, and stores it in the
 * shared `Measurement` structure.
 */

#include "TaskCalculateTime.h"
#include "TaskShared.h"
#include "DFRobot_DS323X.h"
#include "I2CLock.h"
extern SemaphoreHandle_t i2cMutex; /// Mutex for guarding I²C access across tasks.


extern DFRobot_DS323X rtc; /// RTC module used to get the current time.
extern Measurement sharedMeasurement; /// Shared structure containing sensor measurements including timestamp.

/**
 * @brief FreeRTOS task to calculate the current UNIX timestamp from the RTC and store it.
 *
 * Periodically reads the date and time from the RTC via I²C using a mutex to ensure safe access.
 * The retrieved time is converted to a UNIX timestamp in nanoseconds using `mktime()` and stored
 * in `sharedMeasurement.timestamp`.
 *
 * @param pvParameters Unused parameter required by FreeRTOS task signature.
 */
void TaskCalculateTime(void *pvParameters) {
  (void)pvParameters;

  while (true) {
    uint16_t year;
    uint8_t month, date, hour, minute, second;

      


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

  
      sharedMeasurement.timestamp = unix_nanos;


    vTaskDelay(200 / portTICK_PERIOD_MS);
  }
}
