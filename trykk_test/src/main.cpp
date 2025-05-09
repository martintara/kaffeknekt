#include <Wire.h>
#include <Arduino.h> 

const uint8_t SENSOR_ADDR = 0x28;
const uint16_t OUTPUT_MIN = 1638;
const uint16_t OUTPUT_MAX = 14746;
const bool USE_COMMAND_MODE = false;  // Set to true if your sensor needs a trigger before reading

void setup() {
  Serial.begin(115200);
  Wire.begin();  // For ESP32: GPIO21 = SDA, GPIO22 = SCL
  delay(100);
}

void loop() {
  // If the sensor requires a command to trigger measurement:
  if (USE_COMMAND_MODE) {
    Wire.beginTransmission(SENSOR_ADDR);
    Wire.write(0x00);  // Command MSB
    Wire.write(0x00);  // Command LSB
    Wire.endTransmission();
    delay(10);  // Wait for sensor to respond
  }

  Wire.requestFrom(SENSOR_ADDR, (uint8_t)4);

  if (Wire.available() == 4) {
    uint8_t buffer[4];
    for (int i = 0; i < 4; ++i) {
      buffer[i] = Wire.read();
    }

    uint8_t status = (buffer[0] >> 6) & 0x03;
    if (status != 0) {
      Serial.print("Sensor error! Status: ");
      Serial.println(status);
      delay(500);
      return;
    }

    // Extract 14-bit pressure from buffer[0] (lower 6 bits) and buffer[1]
    uint16_t raw_pressure = ((buffer[0] & 0x3F) << 8) | buffer[1];

    float percent_fs = (float)(raw_pressure - OUTPUT_MIN) / (OUTPUT_MAX - OUTPUT_MIN);
    float pressure_bar = percent_fs * 200.0 * 0.0689476;

    Serial.print("Raw Pressure: ");
    Serial.print(raw_pressure);
    Serial.print(" => ");
    Serial.print(pressure_bar, 2);
    Serial.println(" bar");
  } else {
    Serial.println("Sensor not responding or bad I2C read");
  }

  delay(500);
}
