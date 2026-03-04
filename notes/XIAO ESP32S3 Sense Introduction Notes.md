
# XIAO ESP32S3 Sense
- XIAO - Family of boards, refers to the pin layout and construction of the overall board

- ESP32S3 - The microcontroller on the board

- Sense - denotes that this variant of board comes with sensatory equipment  (Microphone, Camera)


Consist of two parts
1. Camera Module
2. XAIO ESP32S3 board

Camera Module
- OV2640 Cam
- Microphone
- MircroSD Card Slot (32 MB FAT Max)
- 2 additional GPIO pints

Board Module
- ESP32-S3R8 Microcontroller
- 8 MB Pseudo Static RAM & 8 MB Flash Ram
- Onboard Battery Charger & Indicator
- Wifi and Bluetooth
- USB-C
- Antenna conntector
- Board to Board connector (for attaching camera module)

---


XIAO board components

- Reset button
- Boot button

- 2 integrated LEDs

Red LED
- Power LED
- Charger indicator for the battery

Amber LED
- User LED

- U.FL Antenna connector
- Board to board connector (for cam)

---
Pins

Most pins on the board serve multiple purposes (multiplexing)

Pins

- 11 + 2 number of pins

- Digital Pins
- Analog Pins
- Tx/Rx (Uses UART)
- Power 5V (can be used for input and output)
- Power  3V3 (3.3 volts) (output only)
- Ground
- GPIO
- Etc.

Also pins on the bottom of the board

- Ground
- Connections for battery
- Thermal bad for measuring temp
- a bunch of others

Camera module also has 2 GPIO connections
- D11 | A11 | GPIO11
- D12 | A12 | GPIO12


---

XIAO Sense specific component considerations

Stick-on antenna pad
the camera can be upgraded to an OV5640 (tell Brett)
Extra boards can be bought without the cam module (tell Brett)
Need to solder the pins on


--- 
Entering bootloader mode

With a non-conductive pen or tip

Press and hold Boot button
Plug in USB power