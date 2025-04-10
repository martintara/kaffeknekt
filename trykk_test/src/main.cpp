#include <Wire.h>
#include <Arduino.h> 

const uint8_t SENSOR_ADDR = 0x28; // Adjust if needed

void setup() {
  Serial.begin(115200);
  Wire.begin(); // SDA = GPIO21, SCL = GPIO22 on most ESP32 boards
  delay(100);
}

void loop() {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.endTransmission();
  
  Wire.requestFrom(SENSOR_ADDR, (uint8_t)4); // Request 4 bytes

  if (Wire.available() == 4) {
    uint8_t status = Wire.read();
    uint8_t pressure_msb = Wire.read();
    uint8_t pressure_lsb = Wire.read();
    uint8_t checksum = Wire.read(); // Optional, depends on sensor version

    uint16_t raw_pressure = ((status & 0x3) << 14) | (pressure_msb << 8) | pressure_lsb;

    // Convert raw_pressure to engineering units (depends on sensor config!)
    Serial.print("Raw Pressure: ");
    Serial.println(raw_pressure);
  } else {
    Serial.println("Sensor not responding");
  }

  delay(500);
}
