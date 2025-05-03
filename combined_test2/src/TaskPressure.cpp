#include "TaskPressure.h"
#include "TaskShared.h"

extern Measurement sharedMeasurement;
extern SemaphoreHandle_t measurementMutex;

void TaskPressure(void *pvParameters) {
  (void)pvParameters;

  while (true) {
    int pressure = 9;  // Placeholder

    if (xSemaphoreTake(measurementMutex, pdMS_TO_TICKS(100))) {
      sharedMeasurement.pressure = pressure;
      xSemaphoreGive(measurementMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(3000));
  }
}
