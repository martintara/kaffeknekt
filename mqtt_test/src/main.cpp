// #include <WiFi.h>

// void setup() {
//   Serial.begin(115200);
//   delay(1000);
//   Serial.println("Scanning for networks...");
  
//   int n = WiFi.scanNetworks();
//   if (n == 0) {
//     Serial.println("No networks found.");
//   } else {
//     Serial.println("Networks found:");
//     for (int i = 0; i < n; ++i) {
//       Serial.printf("%d: %s (%d dBm) %s\n", i + 1,
//                     WiFi.SSID(i).c_str(),
//                     WiFi.RSSI(i),
//                     (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Secured");
//     }
//   }
// }

// void loop() {}




#include <WiFi.h>

// WiFi network credentials
const char* ssid = "RaspAP";
const char* password = "ChangeMe";

void setup() {
  Serial.begin(115200);
  delay(1000);
  
  Serial.println("\n\n==== ESP32 WiFi Connection Test - Improved ====");
  
  // Reset WiFi completely
  WiFi.disconnect(true, true); // Disconnect and forget saved networks
  WiFi.mode(WIFI_OFF);
  delay(1000);
  WiFi.mode(WIFI_STA);
  delay(1000);
  
  // Scan for networks
  Serial.println("Scanning for networks...");
  int scanAttempts = 0;
  int networks = 0;
  bool networkFound = false;
  int networkChannel = 0;
  
  // Try scanning multiple times
  while (scanAttempts < 3 && !networkFound) {
    scanAttempts++;
    networks = WiFi.scanNetworks();
    
    if (networks > 0) {
      Serial.printf("Found %d networks:\n", networks);
      for (int i = 0; i < networks; i++) {
        Serial.printf("%d: %s (%d dBm) Ch:%d %s\n", i + 1,
                     WiFi.SSID(i).c_str(),
                     WiFi.RSSI(i),
                     WiFi.channel(i),
                     (WiFi.encryptionType(i) == WIFI_AUTH_OPEN) ? "Open" : "Secured");
        
        if (String(WiFi.SSID(i)) == String(ssid)) {
          networkFound = true;
          networkChannel = WiFi.channel(i);
          Serial.printf("Target network PiAP found on channel %d!\n", networkChannel);
        }
      }
    } else {
      Serial.println("No networks found, trying again...");
      delay(1000);
    }
  }
  
  if (!networkFound) {
    Serial.println("ERROR: Target network not found after multiple scans!");
    return;
  }
  
  // Try direct connection with channel specification
  Serial.printf("Attempting connection to %s on channel %d...\n", ssid, networkChannel);
  
  // The following line is the key change - specify the channel directly
  WiFi.begin(ssid, password, networkChannel);
  
  // Wait for connection
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  
  // Check result
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("SUCCESS! Connected to WiFi network");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Gateway: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS: ");
    Serial.println(WiFi.dnsIP());
  } else {
    Serial.print("FAILED to connect. Status code: ");
    Serial.println(WiFi.status());
    
    // Interpret status code
    switch(WiFi.status()) {
      case WL_NO_SHIELD: Serial.println("WL_NO_SHIELD"); break;
      case WL_IDLE_STATUS: Serial.println("WL_IDLE_STATUS"); break;
      case WL_NO_SSID_AVAIL: Serial.println("WL_NO_SSID_AVAIL - SSID not available"); break;
      case WL_SCAN_COMPLETED: Serial.println("WL_SCAN_COMPLETED"); break;
      case WL_CONNECT_FAILED: Serial.println("WL_CONNECT_FAILED - Wrong password"); break;
      case WL_CONNECTION_LOST: Serial.println("WL_CONNECTION_LOST"); break;
      case WL_DISCONNECTED: Serial.println("WL_DISCONNECTED"); break;
      default: Serial.println("Unknown status"); break;
    }
    
    // Try an alternate approach if the first fails
    Serial.println("\nTrying alternate connection method...");
    WiFi.disconnect(true, true);
    delay(1000);
    WiFi.begin(ssid, password);
    
    startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 20000) {
      delay(500);
      Serial.print("*");
    }
    Serial.println();
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("SUCCESS with alternate method!");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("Both connection attempts failed.");
    }
  }
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi. IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("Not connected to WiFi.");
    // Try to reconnect periodically
    WiFi.begin(ssid, password);
    delay(5000);
  }
  delay(10000);
}