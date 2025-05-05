#include <Arduino.h>
#include <Adafruit_MAX31865.h>
#include <SPI.h>

// ESP32 Firebird 2 pin definitions
// You may need to adjust these pins based on your specific board layout
#define MAX31865_CS_PIN    21     // CS pin for MAX31865
#define MAX31865_DI_PIN    23    // MOSI pin
#define MAX31865_DO_PIN    19    // MISO pin
#define MAX31865_CLK_PIN   18    // SCK pin

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 thermo = Adafruit_MAX31865(MAX31865_CS_PIN, MAX31865_DI_PIN, MAX31865_DO_PIN, MAX31865_CLK_PIN);

// Alternative: Use hardware SPI with just the CS pin
// Default hardware SPI pins on ESP32:
// MOSI: 23, MISO: 19, SCK: 18
// Adafruit_MAX31865 thermo = Adafruit_MAX31865(MAX31865_CS_PIN);

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0 //102.2

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // Wait for serial port to connect
  }
  
  Serial.println("ESP32 Firebird 2 with MAX31865 PT100 Sensor Test!");
  
  // Initialize the MAX31865 with the appropriate wiring mode
  thermo.begin(MAX31865_2WIRE);  // set to 2WIRE, 3WIRE, or 4WIRE as necessary
}

void loop() {
  // Read the raw 16-bit value from the sensor
  uint16_t rtd = thermo.readRTD();
  Serial.print("RTD value: "); Serial.println(rtd);
  
  // Calculate the resistance ratio
  float ratio = rtd;
  ratio /= 32768;
  Serial.print("Ratio = "); Serial.println(ratio, 8);
  
  // Calculate the actual resistance
  float resistance = RREF * ratio;
  Serial.print("Resistance = "); Serial.println(resistance, 8);
  
  // Calculate the temperature
  float temperature = thermo.temperature(RNOMINAL, RREF) - 7.0;
  Serial.print("Temperature = "); Serial.println(temperature);
  
  // Check and print any faults
  uint8_t fault = thermo.readFault();
  if (fault) {
    Serial.print("Fault 0x"); Serial.println(fault, HEX);
    if (fault & MAX31865_FAULT_HIGHTHRESH) {
      Serial.println("RTD High Threshold");
    }
    if (fault & MAX31865_FAULT_LOWTHRESH) {
      Serial.println("RTD Low Threshold");
    }
    if (fault & MAX31865_FAULT_REFINLOW) {
      Serial.println("REFIN- > 0.85 x Bias");
    }
    if (fault & MAX31865_FAULT_REFINHIGH) {
      Serial.println("REFIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_RTDINLOW) {
      Serial.println("RTDIN- < 0.85 x Bias - FORCE- open");
    }
    if (fault & MAX31865_FAULT_OVUV) {
      Serial.println("Under/Over voltage");
    }
    thermo.clearFault();
  }
  
  Serial.println();
  delay(1000);
}