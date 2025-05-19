/**
 * @file TaskTemperature.h
 * @brief Handles temperature monitoring.
 * 
 * Detailed description here.
 */

#pragma once
#include <Arduino.h>

/**
 * @brief FreeRTOS task that reads temperature sensor data and processes it.
 *
 * @param pvParameters Pointer to parameters passed to the task (unused).
 */
void TaskTemperature(void *pvParameters);