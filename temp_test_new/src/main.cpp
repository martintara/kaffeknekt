#include <Arduino.h>
#include <Adafruit_MAX31865.h>

#define MAX31865_CS 5
#define MAX31865_WIRES MAX31865_4WIRE  // Choose: MAX31865_2WIRE, _3WIRE, or _4WIRE

Adafruit_MAX31865 thermo = Adafruit_MAX31865(MAX31865_CS);

void setup() {
  Serial.begin(115200);
  thermo.begin(MAX31865_WIRES);
}

void loop() {
  float temp = thermo.temperature(100, 430);  // PT100, 430Ω reference resistor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" °C");

  uint8_t fault = thermo.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    thermo.clearFault();
  }

  delay(1000);
}
