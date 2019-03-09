# Firmware
Firmware for ECU (electronic control unit), Throttle Sensor and Engine based on mbed-os and implemented for Xilinx CM1 and STM32 Nucleo.

The ported mbed os version can be found in https://github.com/andi-h/mbed-os

## Behaviour
This software handles the bus protocol (see https://github.com/SoC-Lab/FailsafeECU/tree/master/Concept) and controls sensors and actors. 
The ECU requests the throttle position from the Throttle Sensor (simulated by a button press) and sends the value to the Engine (simulated by a LED).
