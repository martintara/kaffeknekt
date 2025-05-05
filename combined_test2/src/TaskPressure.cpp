#include "TaskPressure.h"
#include "TaskShared.h"

const uint8_t SENSOR_ADDR = 0x28;
const uint16_t OUTPUT_MIN = 1638;
const uint16_t OUTPUT_MAX = 14746;
const float PRESSURE_MAX_PSI = 200.0;
const float PSI_TO_BAR = 0.0689476;

extern Measurement sharedMeasurement;
extern SemaphoreHandle_t measurementMutex;

void TaskPressure(void *pvParameters) {
  (void)pvParameters;

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  while (true) {
    float pressure = 0.0;
    
    // Request data from the pressure sensor
    Wire.requestFrom(SENSOR_ADDR, (uint8_t)4);
    
    if (Wire.available() == 4) {
      uint8_t buffer[4];
      for (int i = 0; i < 4; ++i) {
        buffer[i] = Wire.read();
      }
      
      // Extract 14-bit raw pressure
      uint16_t raw_pressure = ((buffer[0] & 0x3F) << 8) | buffer[1];
      
      // Convert to pressure
      float pressure_psi = ((float)(raw_pressure - OUTPUT_MIN) / (OUTPUT_MAX - OUTPUT_MIN)) * PRESSURE_MAX_PSI;
      pressure = pressure_psi * PSI_TO_BAR;
      
      // Debug output
      //Serial.printf("Pressure sensor: Raw: %u, Pressure: %.2f bar\n", raw_pressure, pressure);
    } else {
      Serial.println("Failed to read pressure sensor data");
    }

    if (xSemaphoreTake(measurementMutex, pdMS_TO_TICKS(100))) {
      sharedMeasurement.pressure = pressure;
      xSemaphoreGive(measurementMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}



