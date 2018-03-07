# MCB_ROS

Teensy firmware for closed-loop motor control over Ethernet using ROS

## Getting Started

Follow these steps to setup the toolchain for compiling and uploading firmare to the Teensy.

### Prerequisites
- [Teensy 3.2](https://www.pjrc.com/store/teensy32.html)
- Micro-USB to USB cable

### Toolchain Installation and Setup

1. Download and install [Arduino 1.8.2](https://www.arduino.cc/en/Main/OldSoftwareReleases#previous/)  
   - Scroll down the page and locate the 1.8.2 install file for your OS (Windows/Linux)
   - To make things simpler, install as 'portable'. This will keep all Arduino files within a single directory and is particularly useful if you have another Arduino install that you want to keep intact. Follow the instructions [here](https://www.arduino.cc/en/Guide/PortableIDE).

2. Download and install Teensyduino 1.3.6 ([Windows](https://www.pjrc.com/teensy/td_136/TeensyduinoInstall.exe)) ([Linux 64 bit](https://www.pjrc.com/teensy/td_136/TeensyduinoInstall.linux64))  
   - Full instructions available [here](https://www.pjrc.com/teensy/td_download.html)
   - Note that you must install the udev rules for Linux

3. Download current MCB_ROS firmware  
   - Ensure you have selected the correct branch  
   - Use the 'Clone or Download' button to save as a .zip file  
   - Extract the .zip file to *arduino_directory*/portable/sketchbook  
   - Rename the folder 'MCB_ROS' (Arduino requires the folder name to match the .ino filename)  

4. Download additional libraries
   - Modified version of Adafruit_MCB23008
   - ArduinoSTL
   - Place both folders in *arduino_directory*/portable/sketchbook/libraries

5. Modify compile options  
   - open up *arduino_directory*\hardware\teensy\avr\boards.txt
   - locate the following lines:
   ```
   teensy31.menu.opt.o2std=Faster
   teensy31.menu.opt.o2std.build.flags.optimize=-O2
   ```
   and add **--specs=nosys.specs**:
   ```
   teensy31.menu.opt.o2std=Faster
   teensy31.menu.opt.o2std.build.flags.optimize=-O2 --specs=nosys.specs
   ```
   - save and close

## Compiling and Uploading Firmware
1. Start the Arduino IDE and open MCB_ROS.ino
2. Connect Teensy via USB cable
3. Configure Arduino for the Teensy
   - Tools -> Board -> Teensy 3.2/3.1
   - Tools -> CPU Speed -> 96 MHz (overclock)
   - Tools -> Optimize -> Faster (this ensures we use the compile options we changed in boards.txt)
   - Tools -> Port -> (depends on OS; select whichever Teensy is connected to)

## ROS Setup/Configuration

## Author
**Trevor Bruns**
