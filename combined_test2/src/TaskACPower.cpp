
#include <Arduino.h> //redundant siden den er i .h?
#include "TaskShared.h"
#include "TaskACPower.h"

extern Measurement sharedMeasurement;
extern SemaphoreHandle_t measurementMutex;


void TaskACPower(void *pvParameters) {
  (void)pvParameters;
  
  
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  
  // Task loop
  while (true) {
    float ACPower = 300;
    // Read raw RTD value
    
    
    // Update shared data with proper mutex protection
    if (xSemaphoreTake(measurementMutex, pdMS_TO_TICKS(100))) {
      sharedMeasurement.ACPower = ACPower;
      xSemaphoreGive(measurementMutex);
    } else {
      Serial.println("Failed to acquire mutex for temperature update");
    }
    
    // Wait before next reading (2 seconds)
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}