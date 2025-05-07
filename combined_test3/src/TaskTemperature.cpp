#include <Arduino.h>
#include "TaskShared.h"

//#include "I2CLock.h"

// External shared variables
extern Measurement sharedMeasurement;
extern SemaphoreHandle_t i2cMutex;

// ADS1115 instance
//DFRobot_ADS1115 ads(&Wire);

// SEN0211 sensor parameters


// Task function for AC power measurement
void TaskTemperature(void *pvParameters) {
  (void)pvParameters;
  
  // // Initial delay to allow system to stabilize
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  

  while (true) {
    float temperature = 300.0;
    
      
    sharedMeasurement.temperature = temperature;

     

    // Wait before next reading (2 seconds)
    vTaskDelay(40 / portTICK_PERIOD_MS);
  }
}
