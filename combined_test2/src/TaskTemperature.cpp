#include "TaskTemperature.h"
#include "TaskShared.h"



extern Measurement sharedMeasurement;
extern SemaphoreHandle_t measurementMutex;

void TaskTemperature(void *pvParameters) {
  (void)pvParameters;

  vTaskDelay(1000 / portTICK_PERIOD_MS);

  while (true) {
    float temperature = 22.0;
    
    // Request data from the temperature sensor
    

    if (xSemaphoreTake(measurementMutex, pdMS_TO_TICKS(100))) {
      sharedMeasurement.temperature = temperature;
      xSemaphoreGive(measurementMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}



