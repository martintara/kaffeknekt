My input: Having a esp32 sensing json data over serial. (different sensor data). How would I send data the other way to set the a rtc clock. Preferably a own task.

To **send data from a Raspberry Pi (or any other host)** _to the ESP32_ over serial to **set an RTC (Real-Time Clock)**, you can implement the following on the ESP32 side:

### **1. Define a task to listen for incoming serial data**

Use a separate FreeRTOS task to read from `Serial` and parse incoming JSON to set the RTC.

---

### **ESP32 Example Sketch:**

```cpp
#include <Arduino.h>
#include <ArduinoJson.h>
#include "RTClib.h"

RTC_DS3231 rtc;

void setClockFromJson(const String &jsonStr) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, jsonStr);

  if (error) {
    Serial.println("Failed to parse JSON for RTC");
    return;
  }

  if (doc.containsKey("rtc")) {
    JsonObject rtcData = doc["rtc"];
    int year = rtcData["year"];
    int month = rtcData["month"];
    int day = rtcData["day"];
    int hour = rtcData["hour"];
    int minute = rtcData["minute"];
    int second = rtcData["second"];

    rtc.adjust(DateTime(year, month, day, hour, minute, second));
    Serial.println("RTC updated successfully.");
  }
}

void serialReceiveTask(void *param) {
  String buffer;
  while (true) {
    while (Serial.available()) {
      char c = Serial.read();
      if (c == '\n') {
        setClockFromJson(buffer);
        buffer = "";
      } else {
        buffer += c;
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void setup() {
  Serial.begin(115200);
  rtc.begin();

  xTaskCreate(
    serialReceiveTask,
    "SerialReceiveTask",
    4096,
    NULL,
    1,
    NULL
  );
}

void loop() {
  // Your sensor reading + sending JSON code here
}
```

---

### **On the Raspberry Pi side (Python example):**

```python
import serial
import json
from datetime import datetime

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=1)

now = datetime.now()
rtc_data = {
    "rtc": {
        "year": now.year,
        "month": now.month,
        "day": now.day,
        "hour": now.hour,
        "minute": now.minute,
        "second": now.second
    }
}

json_str = json.dumps(rtc_data)
ser.write((json_str + '\n').encode('utf-8'))
```

---

### Summary:

- ESP32 has a FreeRTOS task (`serialReceiveTask`) running in the background listening for JSON over serial.
    
- When a complete line (ending in `\n`) is received, it parses and updates the RTC.
    
- The Raspberry Pi sends the current time in a predefined JSON format.
    

Do you also want to add verification or acknowledgment back to the Pi?



My input: Esp32 using freertos


Sure! Here’s a **clean and modular example** of how to structure **multiple FreeRTOS tasks** on an ESP32 using the **Arduino framework**, with:

- Separate source and header files (`.h` / `.cpp`) for each task.
    
- Private data for each task encapsulated in a struct.
    
- A `main.cpp` file that starts the tasks.
    

---

### **Folder Structure**

```
project/
├── src/
│   ├── main.cpp
│   ├── TaskA.h
│   ├── TaskA.cpp
│   ├── TaskB.h
│   └── TaskB.cpp
```

---

### **TaskA.h**

```cpp
#pragma once

#include <Arduino.h>

struct TaskAData {
  int counter;
  int pin;
};

void startTaskA(TaskAData* data);
```

---

### **TaskA.cpp**

```cpp
#include "TaskA.h"

void taskAFunction(void* parameters) {
  TaskAData* data = static_cast<TaskAData*>(parameters);
  pinMode(data->pin, OUTPUT);

  while (true) {
    digitalWrite(data->pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(data->pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    data->counter++;
  }
}

void startTaskA(TaskAData* data) {
  xTaskCreate(
    taskAFunction,
    "TaskA",
    2048,
    data,
    1,
    nullptr
  );
}
```

---

### **TaskB.h**

```cpp
#pragma once

#include <Arduino.h>

struct TaskBData {
  int interval;
  const char* message;
};

void startTaskB(TaskBData* data);
```

---

### **TaskB.cpp**

```cpp
#include "TaskB.h"

void taskBFunction(void* parameters) {
  TaskBData* data = static_cast<TaskBData*>(parameters);

  while (true) {
    Serial.println(data->message);
    vTaskDelay(data->interval / portTICK_PERIOD_MS);
  }
}

void startTaskB(TaskBData* data) {
  xTaskCreate(
    taskBFunction,
    "TaskB",
    2048,
    data,
    1,
    nullptr
  );
}
```

---

### **main.cpp**

```cpp
#include <Arduino.h>
#include "TaskA.h"
#include "TaskB.h"

TaskAData taskAData = { .counter = 0, .pin = LED_BUILTIN };
TaskBData taskBData = { .interval = 1000, .message = "Hello from Task B" };

void setup() {
  Serial.begin(115200);
  delay(1000); // Give serial time to start

  startTaskA(&taskAData);
  startTaskB(&taskBData);
}

void loop() {
  // Nothing needed here, tasks run in background
}
```

---

### Notes:

- Each task has its **own struct with private data**.
    
- All tasks are started in `setup()` using the `xTaskCreate` wrapper functions.
    
- You can safely extend this with more tasks, parameters, or synchronization tools (e.g., queues or semaphores).
    

Would you like this project formatted as a PlatformIO or Arduino IDE project as well?


Great — here's how to structure this **multi-task FreeRTOS project using PlatformIO** with private data and clean header/source separation.

---

My input: Platformio

### **1. Folder Structure for PlatformIO**

```
esp32-tasks-project/
├── include/
│   ├── TaskA.h
│   └── TaskB.h
├── src/
│   ├── main.cpp
│   ├── TaskA.cpp
│   └── TaskB.cpp
├── platformio.ini
```

---

### **2. platformio.ini**

```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
```

---

### **3. include/TaskA.h**

```cpp
#pragma once

#include <Arduino.h>

struct TaskAData {
  int counter;
  int pin;
};

void startTaskA(TaskAData* data);
```

---

### **4. include/TaskB.h**

```cpp
#pragma once

#include <Arduino.h>

struct TaskBData {
  int interval;
  const char* message;
};

void startTaskB(TaskBData* data);
```

---

### **5. src/TaskA.cpp**

```cpp
#include "TaskA.h"

void taskAFunction(void* parameters) {
  TaskAData* data = static_cast<TaskAData*>(parameters);
  pinMode(data->pin, OUTPUT);

  while (true) {
    digitalWrite(data->pin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    digitalWrite(data->pin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    data->counter++;
  }
}

void startTaskA(TaskAData* data) {
  xTaskCreatePinnedToCore(
    taskAFunction,
    "TaskA",
    2048,
    data,
    1,
    nullptr,
    1
  );
}
```

---

### **6. src/TaskB.cpp**

```cpp
#include "TaskB.h"

void taskBFunction(void* parameters) {
  TaskBData* data = static_cast<TaskBData*>(parameters);

  while (true) {
    Serial.print("Task B says: ");
    Serial.println(data->message);
    vTaskDelay(data->interval / portTICK_PERIOD_MS);
  }
}

void startTaskB(TaskBData* data) {
  xTaskCreatePinnedToCore(
    taskBFunction,
    "TaskB",
    2048,
    data,
    1,
    nullptr,
    0
  );
}
```

---

### **7. src/main.cpp**

```cpp
#include <Arduino.h>
#include "TaskA.h"
#include "TaskB.h"

TaskAData taskAData = { .counter = 0, .pin = LED_BUILTIN };
TaskBData taskBData = { .interval = 1000, .message = "Hello from Task B" };

void setup() {
  Serial.begin(115200);
  delay(1000);  // Let serial settle

  startTaskA(&taskAData);
  startTaskB(&taskBData);
}

void loop() {
  // Optional: use loop for diagnostics or idle state
}
```

---

### To Build & Upload:

```bash
pio run --target upload
pio device monitor
```

Let me know if you want to add inter-task communication (like queues or mutexes) next.