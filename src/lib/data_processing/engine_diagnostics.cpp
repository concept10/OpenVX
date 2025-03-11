#include "engine_diagnostics.h"

DiagnosticHistory diagnosticHistory;
bool historyInitialized = false;

int processEngineData(const EngineData &data) {
  // Initialize history if needed
  if (!historyInitialized) {
    initDiagnosticHistory(&diagnosticHistory);
    historyInitialized = true;
  }
  
  // Update history with current data
  updateDiagnosticHistory(&diagnosticHistory, data);

  // Check for error conditions in priority order
  
  // Water temperature check
  if (data.waterTemp > MAX_WATER_TEMP) {
    return CODE_WATER_TEMP_ERROR;
  }
  else if (data.waterTemp < MIN_WATER_TEMP && data.waterTemp > 0) {
    // Cold warning, but not necessarily an error
    // Could be valid in cold water
  }
  
  // Battery voltage check
  if (data.batteryVoltage < MIN_BATTERY_VOLTAGE) {
    return CODE_LOW_BATTERY;
  }
  
  // Knock sensor check
  if (data.knockDetected) {
    return CODE_KNOCK_SENSOR_ERROR;
  }
  
  // Throttle position sensor check
  // Typically, a stuck or erratic throttle position sensor
  if (data.throttlePosition < 0 || data.throttlePosition > 100) {
    return CODE_THROTTLE_POS_ERROR;
  }
  
  // Atmospheric pressure check
  // Standard atmospheric pressure is around 101.3 kPa at sea level
  if (data.atmosphericPressure < 80.0 || data.atmosphericPressure > 110.0) {
    return CODE_ATMOSPHERIC_PRESSURE_ERROR;
  }
  
  // Intake air temperature check
  // Very extreme values would indicate sensor failure
  if (data.intakeAirTemp < -40.0 || data.intakeAirTemp > 120.0) {
    return CODE_INTAKE_AIR_TEMP_ERROR;
  }
  
  // Check for rapid temperature increase (potential overheating)
  if (detectRapidTempIncrease(diagnosticHistory)) {
    return CODE_WATER_TEMP_ERROR; // Pre-emptive warning
  }
  
  // Check for battery drainage
  if (detectBatteryDrainage(diagnosticHistory)) {
    return CODE_LOW_BATTERY; // Pre-emptive warning
  }
  
  // If no errors detected
  return CODE_NORMAL;
}

void initDiagnosticHistory(DiagnosticHistory *history) {
  for (int i = 0; i < HISTORY_LENGTH; i++) {
    history->waterTempHistory[i] = 0;
    history->throttlePosHistory[i] = 0;
    history->batteryVoltageHistory[i] = 0;
  }
  history->historyIndex = 0;
  history->lastHistoryUpdate = 0;
}

void updateDiagnosticHistory(DiagnosticHistory *history, const EngineData &data) {
  // Only update once per second
  if (millis() - history->lastHistoryUpdate < 1000) {
    return;
  }
  
  // Store the current values in the history array
  history->waterTempHistory[history->historyIndex] = data.waterTemp;
  history->throttlePosHistory[history->historyIndex] = data.throttlePosition;
  history->batteryVoltageHistory[history->historyIndex] = data.batteryVoltage;
  
  // Update index for circular buffer
  history->historyIndex = (history->historyIndex + 1) % HISTORY_LENGTH;
  history->lastHistoryUpdate = millis();
}

bool detectRapidTempIncrease(const DiagnosticHistory &history) {
  // Check for a sustained increase of more than 10 degrees over 30 seconds
  int currentIdx = history.historyIndex;
  int thirtySecondsAgoIdx = (currentIdx + HISTORY_LENGTH - 30) % HISTORY_LENGTH;
  
  if (history.waterTempHistory[currentIdx] - history.waterTempHistory[thirtySecondsAgoIdx] > 10.0) {
    return true;
  }
  return false;
}

bool detectBatteryDrainage(const DiagnosticHistory &history) {
  // Check for a steady decline in battery voltage
  float sum = 0;
  for (int i = 0; i < HISTORY_LENGTH - 1; i++) {
    sum += history.batteryVoltageHistory[i] - history.batteryVoltageHistory[i+1];
  }
  
  // If average decline is more than 0.01V per sample
  if (sum / (HISTORY_LENGTH - 1) > 0.01) {
    return true;
  }
  return false;
}
