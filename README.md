# OpenVX
Open Source interface to the Yamaha VMAX 225 Outboard engine


This project aims to provide the hardware, and software interface to the Yamaha VMAX EFI outboard engine for live diagnostic monitoring.  Useful for people that want additional engine information for troubleshooting purposes.

## General
Why would someone want this?

- Diagnostic purposes using real-time engine data

Analog Signals
Digital Events (event.msg, event.message, event.timestamp, event.source, )

###
XOR

*  1 - Normal
* 13 - Incorrect pulser coil input signal
* 14 - No crank position sensor input signal
* 15 - Incorrect engine cooling water temperature sensor input signal
* 17 - Incorrect knocking sensor input signal
* 18 - Incorrect throttle position sensor input signal
* 19 - Low battery input voltage
* 22 - Incorrect atmospheric pressure sensor input signal (out of normal operating range)
* 23 - Incorrect intake air temperature sensor input signal
* 31–44 - Microcomputer processing information
* 31 - No tachometer pulse being output
* 32 - Shift cutoff control (during ignition cutoff operation)
* 33 - Ignition timing is being slightly cor- rected (when starting a cold engine)
* 35 - Fuel injection period is being slightly corrected (when knocking control)
* 36 - Ignition timing is being slightly corrected (when knocking control)
* 41 - Overrevolution control (during ignition cutoff operation)
* 42 - Overheat control/oil empty control
* 43 - Buzzer sounding
* 44 - Engine stop switch control operating

