#pragma once

#include <stdint.h>

// Message types
#define DISCOVERY_REQUEST 1
#define DISCOVERY_RESPONSE 2
#define SENSOR_DATA 3
#define COORDINATOR_HEARTBEAT 4

// Sensor types
#define WATER_TEMP_SENSOR 1
#define KNOCK_SENSOR 2
#define THROTTLE_POSITION_SENSOR 3
#define BATTERY_VOLTAGE_SENSOR 4
#define ATMOSPHERIC_PRESSURE_SENSOR 5
#define INTAKE_AIR_TEMP_SENSOR 6

// Timeouts
#define NODE_TIMEOUT 10000 // 10 seconds

// Max nodes in the network
#define MAX_NODES 10

// Structures for communication
struct SensorData {
  uint8_t sensorType;
  uint32_t nodeId;
  float value;
  uint32_t timestamp;
};

struct DiscoveryMessage {
  uint8_t messageType;
  uint8_t sensorType;
  uint32_t nodeId;
};

// Global variables for node management
extern int registeredNodes;
extern uint8_t nodeMacAddresses[MAX_NODES][6];
extern uint32_t nodeIds[MAX_NODES];
extern uint8_t nodeTypes[MAX_NODES];
extern unsigned long nodeLastSeen[MAX_NODES];

// Function declarations
bool registerNode(const uint8_t *mac, uint32_t nodeId, uint8_t nodeType);
int findNodeById(uint32_t nodeId);
int findNodeByMac(const uint8_t *mac);
void broadcastToAllNodes(const uint8_t *data, size_t len);
