#pragma once
#include <Arduino.h>

class I2CLock {
public:
  I2CLock(SemaphoreHandle_t mutex, TickType_t timeout = pdMS_TO_TICKS(100))
    : _mutex(mutex), _locked(false) {
    if (_mutex != nullptr) {
      _locked = xSemaphoreTake(_mutex, timeout) == pdTRUE;
    }
  }

  ~I2CLock() {
    if (_locked && _mutex != nullptr) {
      xSemaphoreGive(_mutex);
    }
  }

  operator bool() const {
    return _locked;
  }

private:
  SemaphoreHandle_t _mutex;
  bool _locked;
};
