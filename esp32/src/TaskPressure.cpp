/**
 * @file TaskPressure.cpp
 * @brief Task that reads data from an I2C pressure sensor and updates shared measurement structure.
 *
 * This task communicates with a digital pressure sensor over I2C, converts the raw 14-bit
 * pressure reading to bar, and stores the result in the shared Measurement structure.
 */

#include "TaskPressure.h"
#include "TaskShared.h"

const uint8_t SENSOR_ADDR = 0x28;
const uint16_t OUTPUT_MIN = 1638;
const uint16_t OUTPUT_MAX = 14746;
const float PRESSURE_MAX_PSI = 200.0;
const float PSI_TO_BAR = 0.0689476;

extern Measurement sharedMeasurement;
extern SemaphoreHandle_t i2cMutex; /// Mutex for guarding I²C access across tasks.

/**
 * @brief Task that reads pressure data from a sensor and updates the shared measurement.
 *
 * This task waits 1 second before starting, then enters a loop where it:
 * - Takes the I2C mutex.
 * - Requests 4 bytes of data from a digital pressure sensor at address 0x28.
 * - Parses the 14-bit raw pressure reading.
 * - Converts it to PSI, then to bar.
 * - Releases the mutex and updates the global sharedMeasurement.pressure value.
 *
 * If the sensor doesn't respond properly, an error message is printed over Serial.
 *
 * @param pvParameters Unused parameter required by FreeRTOS task signature.
 */
void TaskPressure(void *pvParameters) {
  (void)pvParameters;

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  while (true) {
    float pressure = 0.0;

    if (xSemaphoreTake(i2cMutex, portMAX_DELAY) == pdTRUE) {

      /// Request data from the pressure sensor
      Wire.requestFrom(SENSOR_ADDR, (uint8_t)4);
    
      if (Wire.available() == 4) {
        uint8_t buffer[4];
        for (int i = 0; i < 4; ++i) {
          buffer[i] = Wire.read();
      }
      
      /// Extract 14-bit raw pressure
      uint16_t raw_pressure = ((buffer[0] & 0x3F) << 8) | buffer[1];
      
      /// Convert raw data to pressure
      float pressure_psi = ((float)(raw_pressure - OUTPUT_MIN) / (OUTPUT_MAX - OUTPUT_MIN)) * PRESSURE_MAX_PSI;
      pressure = pressure_psi * PSI_TO_BAR;
      

    } else {
      Serial.println("Failed to read pressure sensor data");
    }
    xSemaphoreGive(i2cMutex);
    }

    sharedMeasurement.pressure = pressure;

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

