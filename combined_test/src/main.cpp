/*

#include <Arduino.h>
#include <OneWire.h>
#include <Wire.h>
#include "DFRobot_INA219.h"

// ==== DS18B20 SETUP ====
#define DS18S20_Pin 25
OneWire ds(DS18S20_Pin);

// ==== INA219 SETUP ====
DFRobot_INA219_IIC ina219(&Wire, INA219_I2C_ADDRESS4);
float ina219Reading_mA = 158;
float extMeterReading_mA = 160;

// ==== TEMP TASK ====
float getTemp() {
  byte data[12];
  byte addr[8];

  if (!ds.search(addr)) {
    ds.reset_search();
    return -1000;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("CRC is not valid!");
    return -1000;
  }

  if (addr[0] != 0x10 && addr[0] != 0x28) {
    Serial.println("Device is not recognized");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // start conversion

  vTaskDelay(750 / portTICK_PERIOD_MS); // wait for conversion

  ds.reset();
  ds.select(addr);
  ds.write(0xBE); // read scratchpad

  for (int i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];
  float tempRead = ((MSB << 8) | LSB);
  return tempRead / 16.0;
}

void temperatureTask(void *parameter) {
  while (1) {
    float temp = getTemp();
    Serial.print("[TEMP] ");
    Serial.print(temp);
    Serial.println(" °C");

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// ==== INA219 TASK ====
void ina219Task(void *parameter) {
  while (1) {
    float voltage = ina219.getBusVoltage_V();
    float current = ina219.getCurrent_mA();
    float power   = ina219.getPower_mW();
    float shunt   = ina219.getShuntVoltage_mV();

    Serial.println("[INA219]");
    Serial.print("Voltage: "); Serial.print(voltage, 2); Serial.println(" V");
    Serial.print("Current: "); Serial.print(current, 1); Serial.println(" mA");
    Serial.print("Power:   "); Serial.print(power, 1);   Serial.println(" mW");
    Serial.print("Shunt:   "); Serial.print(shunt, 3);   Serial.println(" mV");
    Serial.println("");

    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

// ==== SETUP ====
void setup() {
  Serial.begin(115200);
  delay(1000);

  // Init I2C
  Wire.begin();

  // Init INA219
  while (!ina219.begin()) {
    Serial.println("INA219 begin failed");
    delay(2000);
  }
  ina219.linearCalibrate(ina219Reading_mA, extMeterReading_mA);

  // Create tasks
  xTaskCreate(temperatureTask, "Temperature Task", 2048, NULL, 1, NULL);
  xTaskCreate(ina219Task, "INA219 Task", 2048, NULL, 1, NULL);
}

void loop() {
  // Nothing here when using FreeRTOS
}

*/





// asd

// #include <Arduino.h>
// #include <OneWire.h>
// #include <Wire.h>
// #include "DFRobot_INA219.h"
// #include <ArduinoJson.h>
// #include <sys/time.h>


// #define DS18S20_Pin 25
// OneWire ds(DS18S20_Pin);
// DFRobot_INA219_IIC ina219(&Wire, INA219_I2C_ADDRESS4);

// float ina219Reading_mA = 158;
// float extMeterReading_mA = 160;

// struct SensorData {
//   float temperature = -1000.0;
//   float power = 0.0;
// };

// SensorData sensorData;
// SemaphoreHandle_t dataMutex;

// float getTemp() {
//   byte data[12];
//   byte addr[8];

//   if (!ds.search(addr)) {
//     ds.reset_search();
//     return -1000;
//   }

//   if (OneWire::crc8(addr, 7) != addr[7]) return -1000;
//   if (addr[0] != 0x10 && addr[0] != 0x28) return -1000;

//   ds.reset();
//   ds.select(addr);
//   ds.write(0x44, 1);
//   vTaskDelay(750 / portTICK_PERIOD_MS);

//   ds.reset();
//   ds.select(addr);
//   ds.write(0xBE);

//   for (int i = 0; i < 9; i++) {
//     data[i] = ds.read();
//   }

//   ds.reset_search();
//   int16_t raw = (data[1] << 8) | data[0];
//   return raw / 16.0;
// }

// void temperatureTask(void *parameter) {
//   while (1) {
//     float temp = getTemp();
//     if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
//       sensorData.temperature = temp;
//       xSemaphoreGive(dataMutex);
//     }
//     vTaskDelay(2000 / portTICK_PERIOD_MS);
//   }
// }

// void ina219Task(void *parameter) {
//   while (1) {
//     float power = ina219.getPower_mW();
//     if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
//       sensorData.power = power;
//       xSemaphoreGive(dataMutex);
//     }
//     vTaskDelay(2000 / portTICK_PERIOD_MS);
//   }
// }

// void serialSenderTask(void *parameter) {
//   StaticJsonDocument<192> doc;

//   while (1) {
//     if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
//       doc["temperature_C"] = sensorData.temperature;
//       doc["power_mW"] = sensorData.power;
//       xSemaphoreGive(dataMutex);
//     }

//     // Use gettimeofday to get seconds + microseconds
//     struct timeval now;
//     gettimeofday(&now, NULL);

//     // Combine into nanoseconds
//     int64_t timestamp_ns = ((int64_t)now.tv_sec * 1000000000LL) + ((int64_t)now.tv_usec * 1000LL);
//     doc["timestamp_ns"] = timestamp_ns;

//     serializeJson(doc, Serial);
//     Serial.println();

//     vTaskDelay(1000 / portTICK_PERIOD_MS);
//   }
// }



// void setTimeFromSerial(const String& timeStr) {
//   struct tm tm;
//   if (sscanf(timeStr.c_str(), "%d-%d-%dT%d:%d:%dZ",
//              &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
//              &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {
//     tm.tm_year -= 1900;
//     tm.tm_mon -= 1;
//     time_t t = mktime(&tm);
//     struct timeval now = { .tv_sec = t, .tv_usec = 0 };
//     settimeofday(&now, NULL);
//     Serial.println("[ESP32] Time synced from Raspberry Pi.");
//   } else {
//     Serial.println("[ESP32] Failed to parse time string.");
//   }
// }

// void waitForTimeSync() {
//   Serial.println("Waiting for time sync from Raspberry Pi...");

//   while (true) {
//     if (Serial.available()) {
//       String input = Serial.readStringUntil('\n');
//       input.trim();
//       if (input.startsWith("TIME:")) {
//         String timeStr = input.substring(5);
//         setTimeFromSerial(timeStr);
//         break;
//       }
//     }
//     delay(100);
//   }
// }


// void setup() {
//   Serial.begin(115200);
//   Wire.begin();

//   waitForTimeSync(); // <-- Important: wait for Pi to send timestamp

//   while (!ina219.begin()) {
//     Serial.println("INA219 init failed");
//     delay(2000);
//   }

//   ina219.linearCalibrate(ina219Reading_mA, extMeterReading_mA);
//   dataMutex = xSemaphoreCreateMutex();

//   xTaskCreate(temperatureTask, "TempTask", 2048, NULL, 1, NULL);
//   xTaskCreate(ina219Task, "PowerTask", 2048, NULL, 1, NULL);
//   xTaskCreate(serialSenderTask, "SerialSender", 2048, NULL, 1, NULL);
// }

// void loop() {
//   // nothing
// }



// // this code is currently perculating

#include <Arduino.h>
#include <OneWire.h>
#include <Wire.h>
#include <DFRobot_ADS1115.h>  // Replace INA219 with ADS1115
#include <ArduinoJson.h>
#include <sys/time.h>

// ADS1115 instance
DFRobot_ADS1115 ads(&Wire);

// SEN0211 sensor parameters
#define CURRENT_DETECTION_RANGE 20    // Set your sensor's current detection range (5A, 10A, 20A)
#define VOLTAGE_REFERENCE 240.0       // Reference voltage for your AC line (220V, 110V, etc.)
#define CALIBRATION_FACTOR 1.070      // Calibration factor: actual/measured

// Input pins on ADS1115
const int CURRENT_PIN = 0;  // ADS1115 A0 for current measurement

#define DS18S20_Pin 25
OneWire ds(DS18S20_Pin);

struct SensorData {
  float temperature = -1000.0;
  float power = 0.0;
  float current = 0.0;
  float voltage = VOLTAGE_REFERENCE;
};

SensorData sensorData;
SemaphoreHandle_t dataMutex;

// Function to read AC current from SEN0211
float readACCurrentValue() {
  float peakVoltage = 0;
  float voltageRMS = 0;
  float currentValue = 0;
  
  // Take multiple samples for more stable reading
  const int SAMPLES = 10;
  for (int i = 0; i < SAMPLES; i++) {
    int rawReading = ads.readVoltage(CURRENT_PIN); // Reading in mV
    peakVoltage += rawReading;
    delay(2); // Small delay between readings
  }
  
  // Average the readings
  peakVoltage = peakVoltage / SAMPLES;
  
  // Convert peak to RMS (root mean square) - using 0.707 factor
  voltageRMS = peakVoltage * 0.707;
  
  // Convert the ADC millivolts to actual volts
  voltageRMS = voltageRMS / 1000.0; // Convert mV to V
  
  // Account for circuit amplification
  voltageRMS = voltageRMS / 2;
  
  // Convert voltage to current using detection range
  currentValue = voltageRMS * CURRENT_DETECTION_RANGE;
  
  // Apply calibration factor to correct the reading
  currentValue = currentValue * CALIBRATION_FACTOR;
  
  return currentValue;
}

float getTemp() {
  byte data[12];
  byte addr[8];

  if (!ds.search(addr)) {
    ds.reset_search();
    return -1000;
  }

  if (OneWire::crc8(addr, 7) != addr[7]) return -1000;
  if (addr[0] != 0x10 && addr[0] != 0x28) return -1000;

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1);
  vTaskDelay(750 / portTICK_PERIOD_MS);

  ds.reset();
  ds.select(addr);
  ds.write(0xBE);

  for (int i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  ds.reset_search();
  int16_t raw = (data[1] << 8) | data[0];
  return raw / 16.0;
}

void temperatureTask(void *parameter) {
  while (1) {
    float temp = getTemp();
    if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
      sensorData.temperature = temp;
      xSemaphoreGive(dataMutex);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void powerTask(void *parameter) {
  while (1) {
    // Check if ADS1115 is connected
    if (ads.checkADS1115()) {
      // Read current from SEN0211
      float current = readACCurrentValue();
      
      // Calculate power (W = V * A)
      float power = sensorData.voltage * current;
      
      if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
        sensorData.current = current;
        sensorData.power = power;
        xSemaphoreGive(dataMutex);
      }
    } else {
      Serial.println("ADS1115 Disconnected!");
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);
  }
}

void serialSenderTask(void *parameter) {
  StaticJsonDocument<256> doc;  // Increased size to accommodate additional fields

  while (1) {
    if (xSemaphoreTake(dataMutex, portMAX_DELAY)) {
      doc["temperature"] = float(sensorData.temperature);
      doc["power"] = sensorData.power;         // Note units changed from mW to W
      //doc["current_A"] = sensorData.current;     // Added current data
      //doc["voltage_V"] = sensorData.voltage;     // Added fixed voltage reference
      doc["pressure"] = 9.00;     // Added current data
      xSemaphoreGive(dataMutex);
    }

    // Use gettimeofday to get seconds + microseconds
    struct timeval now;
    gettimeofday(&now, NULL);

    // Combine into nanoseconds
    int64_t timestamp_ns = ((int64_t)now.tv_sec * 1000000000LL) + ((int64_t)now.tv_usec * 1000LL);
    doc["timestamp"] = timestamp_ns;
    //doc["timestamp"] = 1745742888368398000;
    serializeJson(doc, Serial);
    Serial.println();

    vTaskDelay(1000 / portTICK_PERIOD_MS);
  }
}

void setTimeFromSerial(const String& timeStr) {
  struct tm tm;
  if (sscanf(timeStr.c_str(), "%d-%d-%dT%d:%d:%dZ",
             &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
             &tm.tm_hour, &tm.tm_min, &tm.tm_sec) == 6) {
    tm.tm_year -= 1900;
    tm.tm_mon -= 1;
    time_t t = mktime(&tm);
    struct timeval now = { .tv_sec = t, .tv_usec = 0 };
    settimeofday(&now, NULL);
    Serial.println("[ESP32] Time synced from Raspberry Pi.");
  } else {
    Serial.println("[ESP32] Failed to parse time string.");
  }
}

void waitForTimeSync() {
  Serial.println("Waiting for time sync from Raspberry Pi...");

  while (true) {
    if (Serial.available()) {
      String input = Serial.readStringUntil('\n');
      input.trim();
      if (input.startsWith("TIME:")) {
        String timeStr = input.substring(5);
        setTimeFromSerial(timeStr);
        break;
      }
    }
    delay(100);
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 AC Power Monitor with Temperature Sensor");
  
  Wire.begin();
  waitForTimeSync(); // Wait for Pi to send timestamp

  // Configure ADS1115
  ads.setAddr_ADS1115(ADS1115_IIC_ADDRESS0);  // 0x48
  ads.setGain(eGAIN_ONE);        // 1x gain (±4.096V range)
  ads.setMode(eMODE_SINGLE);     // Single-shot mode
  ads.setRate(eRATE_128);        // 128 samples per second
  ads.setOSMode(eOSMODE_SINGLE); // Start a single-conversion
  
  // Initialize ADS1115
  ads.init();
  
  // Check if ADS1115 is connected after initialization
  if (ads.checkADS1115()) {
    Serial.println("ADS1115 initialized successfully!");
  } else {
    Serial.println("Failed to initialize ADS1115. Check connections!");
    // Continue anyway - it might connect later
  }

  dataMutex = xSemaphoreCreateMutex();

  xTaskCreate(temperatureTask, "TempTask", 2048, NULL, 1, NULL);
  xTaskCreate(powerTask, "PowerTask", 2048, NULL, 1, NULL);
  xTaskCreate(serialSenderTask, "SerialSender", 2048, NULL, 1, NULL);
}

void loop() {
  // nothing - all work done in tasks
}




