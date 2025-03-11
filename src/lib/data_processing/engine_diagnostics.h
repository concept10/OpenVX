#pragma once

// Engine diagnostic codes (as per Yamaha VMAX documentation)
#define CODE_NORMAL 1
#define CODE_PULSER_COIL_ERROR 13
#define CODE_NO_CRANK_POSITION 14
#define CODE_WATER_TEMP_ERROR 15
#define CODE_KNOCK_SENSOR_ERROR 17
#define CODE_THROTTLE_POS_ERROR 18
#define CODE_LOW_BATTERY 19
#define CODE_ATMOSPHERIC_PRESSURE_ERROR 22
#define CODE_INTAKE_AIR_TEMP_ERROR 23

// Thresholds for various parameters
#define MIN_BATTERY_VOLTAGE 11.5
#define MAX_WATER_TEMP 95.0
#define MIN_WATER_TEMP 5.0

// Function to process engine data and determine error codes
int processEngineData(const struct EngineData &data);

// Structure to hold engine diagnostic data
struct EngineData {
  float waterTemp;
  float throttlePosition;
  float batteryVoltage;
  float atmosphericPressure;
  float intakeAirTemp;
  bool knockDetected;
  int errorCode;
  unsigned long lastUpdate;
};

// History tracking for diagnostics
#define HISTORY_LENGTH 60  // Keep 60 seconds of history
struct DiagnosticHistory {
  float waterTempHistory[HISTORY_LENGTH];
  float throttlePosHistory[HISTORY_LENGTH];
  float batteryVoltageHistory[HISTORY_LENGTH];
  int historyIndex;
  unsigned long lastHistoryUpdate;
};

// Initialize history tracking
void initDiagnosticHistory(DiagnosticHistory *history);

// Update history with latest readings
void updateDiagnosticHistory(DiagnosticHistory *history, const EngineData &data);

// Analyze trends in sensor readings
bool detectRapidTempIncrease(const DiagnosticHistory &history);
bool detectBatteryDrainage(const DiagnosticHistory &history);
