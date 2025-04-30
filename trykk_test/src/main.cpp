// trykk test

#include <Wire.h>
#include <Arduino.h>

const uint8_t SENSOR_ADDR = 0x28;
const bool USE_COMMAND_MODE = false;

const uint16_t OUTPUT_MIN = 1638;
const uint16_t OUTPUT_MAX = 14746;
const float PRESSURE_MAX_PSI = 200.0;
const float PSI_TO_BAR = 0.0689476;

void setup() {
  Serial.begin(115200);
  Wire.begin(); // For ESP32: GPIO21 = SDA, GPIO22 = SCL
  delay(100);
}

void loop() {
  if (USE_COMMAND_MODE) {
    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(0x00);
    Wire.write(0x00);
    Wire.endTransmission();
    delay(10);
  }

  Wire.requestFrom(SENSOR_ADDR, (uint8_t)4);

  if (Wire.available() == 4) {
    uint8_t buffer[4];
    for (int i = 0; i < 4; ++i) {
      buffer[i] = Wire.read();
    }

    // Print raw bytes in binary
    for (int i = 0; i < 4; ++i) {
      for (int j = 7; j >= 0; --j) {
        Serial.print((buffer[i] >> j) & 1);
      }
      Serial.print(" ");
    }

    // Extract 14-bit raw pressure
    uint16_t raw_pressure = ((buffer[0] & 0x3F) << 8) | buffer[1];

    // Convert to pressure
    float pressure_psi = ((float)(raw_pressure - OUTPUT_MIN) / (OUTPUT_MAX - OUTPUT_MIN)) * PRESSURE_MAX_PSI;
    float pressure_bar = pressure_psi * PSI_TO_BAR;

    // Print raw decimal and pressure
    Serial.printf("=> Raw: %u, Pressure: %.2f bar\n", raw_pressure, pressure_bar);
  }

  delay(500);
}
