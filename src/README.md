# Yamaha VMAX 225

This project implements a distributed sensor network using multiple ESP32 modules to monitor various parameters of the Yamaha VMAX 225 outboard engine.

## System Architecture

- **Coordinator Node**: Central ESP32 that aggregates data from all sensors and provides a unified interface
- **Sensor Nodes**: Individual ESP32 modules dedicated to specific engine parameters:
  - Engine cooling water temperature
  - Knock sensor
  - Throttle position
  - Battery voltage
  - Atmospheric pressure
  - Intake air temperature

## Communication

The system uses ESP-NOW for lightweight peer-to-peer communication between nodes, with mesh networking capabilities for extended range.

## Hardware Requirements

- ESP32 development boards (one for coordinator, one for each sensor)
- Appropriate sensors for each parameter
- Power supply solution for each node
- Optional: Display module for coordinator node

## Setup Instructions

1. Flash each ESP32 with its corresponding firmware from this repository
2. Connect sensors according to the pinout diagrams in `/docs/hardware_setup.md`
3. Power up the coordinator node first, then sensor nodes
4. Access the web interface via the coordinator's IP address

## Diagnostic Codes

The system monitors and interprets standard Yamaha diagnostic codes as listed in the main project README.
