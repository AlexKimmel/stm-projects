# UART echo with HAL+BSP

This code right now is a copy of the UART_echo and receives data over UART and sends it right back.

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

When done, you can quit minicom with CTRL-A X.

## Dependencies

- Arm embedded toolchain
- OpenOCD
- GNU make
- minicom

## File structure

See the [HAL blink folder](../LED_blink/) for an explanation of the files.
