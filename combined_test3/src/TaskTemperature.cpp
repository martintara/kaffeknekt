#include <Arduino.h>
#include <Adafruit_MAX31865.h>
#include <SPI.h>
#include "TaskShared.h"

extern Measurement sharedMeasurement;
extern SemaphoreHandle_t measurementMutex;

// ESP32 Firebird 2 pin definitions
#define MAX31865_CS_PIN    14      // CS pin for MAX31865
#define MAX31865_DI_PIN    23     // MOSI pin
#define MAX31865_DO_PIN    19     // MISO pin
#define MAX31865_CLK_PIN   18     // SCK pin

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 thermo = Adafruit_MAX31865(MAX31865_CS_PIN, MAX31865_DI_PIN, MAX31865_DO_PIN, MAX31865_CLK_PIN);

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
#define RNOMINAL  100.0

void TaskTemperature(void *pvParameters) {
  (void)pvParameters;
  
  // Initial delay to allow other initializations to complete
  vTaskDelay(2000 / portTICK_PERIOD_MS);
  
  Serial.println("Temperature sensor task starting...");
  
  // SPI initialization for temperature sensor
  SPI.begin(); //redundant?
  
  // Initialize the MAX31865 with 2WIRE configuration
  thermo.begin(MAX31865_2WIRE);
  
  // Initial reading to check if sensor is working
  uint16_t rtd_initial = thermo.readRTD();
  Serial.print("Initial RTD reading: "); 
  Serial.println(rtd_initial);

  if (rtd_initial == 0) {
    Serial.println("WARNING: Temperature sensor not responding, check connections");
  }
  
  // Task loop
  while (true) {
    // Read raw RTD value
    uint16_t rtd = thermo.readRTD();
    // Serial.print("RTD value: "); 
    // Serial.println(rtd);
    
    float temperature = 22.0; // Default value
    
    // Only process if we got a valid reading
    if (rtd > 0) {
      // Calculate temperature using the library function
      temperature = thermo.temperature(RNOMINAL, RREF) - 7.0;
      
      // Debug output
      float ratio = rtd;
      ratio /= 32768;
      float resistance = RREF * ratio;
      // Serial.print("Resistance: "); 
      // Serial.println(resistance);
      // Serial.print("Temperature: "); 
      // Serial.println(temperature);
      
      // Check for out-of-range values
      if (temperature < -50 || temperature > 200) {
        Serial.println("Temperature out of reasonable range, using default");
        temperature = 22.0;
      }
    } else {
      Serial.println("Invalid RTD reading, using default temperature");
    }
    
    // Check for sensor faults
    uint8_t fault = thermo.readFault();
    if (fault) {
      Serial.print("Fault detected 0x"); 
      Serial.println(fault, HEX);
      
      // Clear fault
      thermo.clearFault();
      
      // Try to reinitialize the sensor
      thermo.begin(MAX31865_2WIRE);
      vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    
    // Update shared data with proper mutex protection
    //if (xSemaphoreTake(measurementMutex, pdMS_TO_TICKS(100))) {
      sharedMeasurement.temperature = temperature;
    //  xSemaphoreGive(measurementMutex);
//} else {
    //  Serial.println("Failed to acquire mutex for temperature update");
   // }
    
    // Wait before next reading (2 seconds)
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}