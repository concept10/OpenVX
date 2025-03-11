#include <WiFi.h>
#include <esp_now.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "../lib/communication/mesh_network.h"
#include "../lib/data_processing/engine_diagnostics.h"

// Network credentials for AP mode
#define WIFI_SSID "OpenVX_Coordinator"
#define WIFI_PASSWORD "yamaha_vmax225"

// Web server
AsyncWebServer server(80);

// Store latest sensor readings
struct {
  float waterTemp = 0.0;
  float throttlePosition = 0.0;
  float batteryVoltage = 0.0;
  float atmosphericPressure = 0.0;
  float intakeAirTemp = 0.0;
  bool knockDetected = false;
  int errorCode = 1; // Default to 1 (Normal)
  unsigned long lastUpdate = 0;
} engineData;

// ESP-NOW callback function
void onDataReceived(const uint8_t *mac, const uint8_t *data, int len) {
  if (len == sizeof(SensorData)) {
    SensorData *receivedData = (SensorData *)data;
    
    // Update appropriate data based on sensor type
    switch (receivedData->sensorType) {
      case WATER_TEMP_SENSOR:
        engineData.waterTemp = receivedData->value;
        break;
      case THROTTLE_POSITION_SENSOR:
        engineData.throttlePosition = receivedData->value;
        break;
      // Add other sensor types
      // ...
    }
    
    engineData.lastUpdate = millis();
    
    // Process diagnostics
    engineData.errorCode = processEngineData(engineData);
  }
}

void setup() {
  Serial.begin(115200);
  
  // Initialize WiFi in AP mode
  WiFi.mode(WIFI_AP);
  WiFi.softAP(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("AP IP address: ");
  Serial.println(WiFi.softAPIP());
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback
  esp_now_register_recv_cb(onDataReceived);
  
  // Setup web server endpoints
  setupWebServer();
  
  // Start server
  server.begin();
}

void loop() {
  // Check if any node has timed out
  for (int i = 0; i < registeredNodes; i++) {
    if (millis() - nodeLastSeen[i] > NODE_TIMEOUT) {
      // Mark node as offline
    }
  }
  
  // Broadcast coordinator heartbeat periodically
  static unsigned long lastHeartbeat = 0;
  if (millis() - lastHeartbeat > 5000) {
    broadcastHeartbeat();
    lastHeartbeat = millis();
  }
  
  delay(10);
}

void setupWebServer() {
  // Serve static files
  server.serveStatic("/", SPIFFS, "/");
  
  // API endpoint to get engine data
  server.on("/api/engine-data", HTTP_GET, [](AsyncWebServerRequest *request) {
    DynamicJsonDocument doc(1024);
    doc["waterTemp"] = engineData.waterTemp;
    doc["throttlePosition"] = engineData.throttlePosition;
    doc["batteryVoltage"] = engineData.batteryVoltage;
    doc["atmosphericPressure"] = engineData.atmosphericPressure;
    doc["intakeAirTemp"] = engineData.intakeAirTemp;
    doc["knockDetected"] = engineData.knockDetected;
    doc["errorCode"] = engineData.errorCode;
    doc["lastUpdate"] = engineData.lastUpdate;
    
    String response;
    serializeJson(doc, response);
    request->send(200, "application/json", response);
  });
}

void broadcastHeartbeat() {
  // Send heartbeat to all nodes
  uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  esp_now_send(broadcastAddress, (uint8_t *)"HEARTBEAT", 9);
}
