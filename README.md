# CANgauge-core
## General Information
This repo is meant to serve as a starting point for users who want to develop their own application on CANgauge. It uses our own low level hardware driver implementation, FreeRTOS for task management and scheduling, and LVGL for graphics. Although a ST-Link or other programmer is recommended it is not required. There is a USB bootloader than can be accessed by holding down the button on the back while connecting power.

_Should you want to develop an application on top of different libraries (STM32 HAL, TouchGFX, etc.) see our datasheet for pinouts and other technical information to help do so. Please contact us if you are having troubles finding anything._
## Setup
### STM32CubeIDE
Download our detailed guide on setup in STM32CubeIDE.
### Arduino IDE
Coming soon.
## Library Configuration
FreeRTOS and LVGL are included in the repo natively vs as a submodule. This is done for simplicity of downloading or forking the repo.
### LVGL
LVGL is currently version 9.5. 
### FreeRTOS
FreeRTOS is on version 11.3.1. 

It is ported to the ARM CM7 r0p1 port. The port files have been moved to the include folder to minimize the amount of include paths needed.
