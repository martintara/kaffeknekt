#include <Arduino.h>
#include <WiFi.h>

const char* ssid = "PiAP";           // Your Pi's AP SSID
const char* password = "81549300"; // Your Pi's AP password

void setup() {
  Serial.begin(115200);
  delay(1000);

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Connected!");
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());

  // Blink built-in LED if available
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  // You can add your MQTT or JSON logic here
}
