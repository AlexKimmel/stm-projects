
# Overview

This project demonstrates UART communication between two devices using the STM32H7 microcontroller. The code is designed to receive data from one UART interface, process it, and transmit data through another UART interface. It includes functionality to encode and decode data, manage buffers, and handle UART interrupts.
## Hardware Requirements

  - STM32H7 Nucleo board
  - UART connections to other devices (e.g., another microcontroller or a PC)


## Files and Directories

  - main.c: Contains the main logic and functions for UART communication and data processing.
  - stm32h7xx_hal.h, stm32h7xx_nucleo.h: Header files for the STM32 HAL and board-specific configurations.


# Usage

  After getting the necessary dependencies, connect your board via USB and run

  ```
  make flash
  ```

  After flashing successfully, run

  ```
  make serial
  ```
to open minicom, which allows you to see data coming from, and send data over,
the serial port opened by the board. You may need to adapt the port name
`/dev/ttyACM0` on your device.

Use `pngToBinary.py` to create binary that can be send over with minicom. `binaryToPng.py` can then create the png out of the received binary.

## Dependencies

- Arm embedded toolchain
- OpenOCD
- GNU make
- minicom
