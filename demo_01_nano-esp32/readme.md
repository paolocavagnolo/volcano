# NANO ESP32 with MPY DEMO

## VERY IMPORTANT
The [Grove Shield for Arduino Nano](https://store.arduino.cc/products/grove-shield-for-arduino-nano?queryID=undefined) used for the demo has the RST pin on the left side **linked** with the RST pin on the right side,
thats is ok for a normal Arduino Nano, but for the Nano ESP32, on the right side there is the B1 pin, used to put the board in Bootloader mode.

So, if you press reset on the Nano ESP32 while it is connected to the shield it will enter in bootloader mode. To avoid this I've cut the RST pin on the Nano board

## CONNECTIONS


| _module _     | _port_  |
|---------------|---------|
| potentiometer | **A0**  |
|  light sensor | **A2**  |
|    microphone | **A6**  |
|           IMU | **I2C** |
|        button | **D2**  |
|           led | **D4**  |


## UPLOAD

- Download the sketches in the [src_mpy folder](https://github.com/paolocavagnolo/volcano/tree/main/demo_01_nano-esp32/mpy_src)
- Open the Arduino Lab for MicroPython and connect to the board
- Copy all the files on the board

## LIVE DEMO

- Open the Arduino Lab for Micropython on one side of the monitor, **don't connect to the board**
- Open the Arduino IDE
- On the Arduino IDE select the serial port of the board (probably the only one shown)
- Select Arduino Nano ESP32 as board
- Open the serial plotter on the other side of the monitor

