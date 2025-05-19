/**
 * @file TaskShared.h
 * @brief Structure of sensor data.
 * 
 * Detailed description here.
 */

#pragma once
#include <Arduino.h>

// Shared data structure
struct Measurement {
  uint64_t timestamp;
  int flag;
  float pressure;
  float temperature;
  float ACPower;
};

// Global shared instance (defined in main.cpp)
extern Measurement sharedMeasurement;

// Mutex to protect shared access to the Measurements
extern SemaphoreHandle_t measurementMutex;