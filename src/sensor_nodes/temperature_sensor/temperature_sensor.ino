#include <WiFi.h>
#include <esp_now.h>
#include "../../lib/communication/mesh_network.h"

// Pin definitions
#define TEMP_SENSOR_PIN 34 // ADC pin for temperature sensor

// Coordinator MAC address (will be learned during discovery)
uint8_t coordinatorAddress[6];
bool coordinatorFound = false;

// Calibration values
const float TEMP_OFFSET = -20.0;
const float TEMP_SCALE = 0.1;

// Sensor metadata
SensorData sensorData;

void setup() {
  Serial.begin(115200);
  
  // Initialize as station
  WiFi.mode(WIFI_STA);
  
  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Register callback functions
  esp_now_register_recv_cb(onDataReceived);
  esp_now_register_send_cb(onDataSent);
  
  // Initialize sensor data structure
  sensorData.sensorType = WATER_TEMP_SENSOR;
  sensorData.nodeId = ESP.getEfuseMac() & 0xFFFFFFFF;
  
  // Start discovery process to find coordinator
  startDiscovery();
}

void loop() {
  if (coordinatorFound) {
    // Read temperature sensor
    int rawValue = analogRead(TEMP_SENSOR_PIN);
    
    // Convert to actual temperature value using calibration
    float temperature = convertRawToTemperature(rawValue);
    
    // Update sensor data
    sensorData.value = temperature;
    sensorData.timestamp = millis();
    
    // Send data to coordinator
    esp_now_send(coordinatorAddress, (uint8_t *)&sensorData, sizeof(sensorData));
    
    // Print for debugging
    Serial.printf("Sent temperature: %.2f°C\n", temperature);
  } else {
    // Continue discovery attempts
    if (millis() % 5000 == 0) {
      startDiscovery();
    }
  }
  
  delay(1000); // Send data every second
}

float convertRawToTemperature(int rawValue) {
  // Convert ADC value to voltage
  float voltage = rawValue * 3.3 / 4095.0;
  
  // Convert voltage to temperature using Yamaha sensor characteristics
  // This is a simple linear conversion - will need calibration for exact sensor
  float temperature = (voltage * 100.0) + TEMP_OFFSET;
  
  // Apply calibration
  return temperature * TEMP_SCALE;
}

void startDiscovery() {
  Serial.println("Starting discovery for coordinator...");
  
  // Broadcast a discovery message
  uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  
  // Prepare discovery message
  DiscoveryMessage msg;
  msg.messageType = DISCOVERY_REQUEST;
  msg.sensorType = WATER_TEMP_SENSOR;
  msg.nodeId = sensorData.nodeId;
  
  // Send discovery message
  esp_now_send(broadcastAddress, (uint8_t *)&msg, sizeof(msg));
}

void onDataReceived(const uint8_t *mac, const uint8_t *data, int len) {
  // Check if it's a discovery response
  if (len == sizeof(DiscoveryMessage)) {
    DiscoveryMessage *msg = (DiscoveryMessage *)data;
    
    if (msg->messageType == DISCOVERY_RESPONSE) {
      // Store coordinator MAC
      memcpy(coordinatorAddress, mac, 6);
      coordinatorFound = true;
      Serial.println("Coordinator found!");
    }
  }
}

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("Last packet send status: ");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
