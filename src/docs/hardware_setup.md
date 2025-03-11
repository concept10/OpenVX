# Hardware Setup Guide

## Required Components

### Coordinator Node
- ESP32 Dev Board
- Power supply (9-12V to USB adapter for marine use)
- Optional: 2.8" TFT display for local readings
- Optional: MicroSD card adapter for data logging
- Weatherproof enclosure

### Sensor Nodes
Each sensor node requires:
- ESP32 Dev Board
- Power supply 
- Appropriate sensor (see below)
- Signal conditioning circuit (if required)
- Weatherproof enclosure

## Sensor Types and Connections

### Water Temperature Sensor
- **Sensor Type**: NTC Thermistor or Yamaha original sensor
- **Connection**: Connect to GPIO34 via voltage divider
- **Power**: 3.3V
- **Circuit**: 10kΩ pull-up resistor to 3.3V

### Throttle Position Sensor
- **Sensor Type**: Potentiometer or original Yamaha sensor
- **Connection**: Connect to GPIO35
- **Power**: 3.3V
- **Note**: Ensure voltage output is within 0-3.3V range

### Battery Voltage Sensor
- **Circuit**: Voltage divider (47kΩ/10kΩ) to bring 12V down to 0-3.3V range
- **Connection**: Connect to GPIO32
- **Protection**: Add zener diode for voltage spike protection

### Atmospheric Pressure Sensor
- **Sensor Type**: BMP280/BME280
- **Connection**: I2C (GPIO21 & GPIO22)
- **Power**: 3.3V

### Intake Air Temperature Sensor
- **Sensor Type**: NTC Thermistor or Yamaha original sensor
- **Connection**: Connect to GPIO33 via voltage divider
- **Power**: 3.3V

### Knock Sensor
- **Sensor Type**: Piezoelectric or Yamaha original sensor
- **Connection**: Requires amplifier circuit, then to GPIO25
- **Processing**: FFT analysis on ESP32 to detect knock frequencies

## Wiring Diagram

```
+-----------------+   
| ESP32 Dev Board |   
+-----------------+   
| GPIO34 - Temperature Sensor
| GPIO35 - Throttle Position
| GPIO32 - Battery Voltage
| GPIO33 - Intake Air Temp
| GPIO25 - Knock Sensor
| GPIO21 - SDA (I2C)
| GPIO22 - SCL (I2C)
+-----------------+
```

## Installation Tips

1. Use shielded cables for all sensor connections to reduce electrical noise
2. Place ESP32s away from ignition components to avoid interference
3. Ensure all connections are weatherproof and vibration-resistant
4. Use heat shrink tubing and marine-grade connectors
5. Consider adding a backup power circuit for reliability

## Testing Your Setup

1. Flash the test firmware for each sensor node
2. Open the serial monitor at 115200 baud
3. Verify sensor readings in the debug output
4. Ensure the coordinator can receive data from all sensor nodes
