# The Main Module
This is where the magic happens. check the code comments for more info.

## Parts Needed
I will only document parts needed for the Main controller module here, since it is technically the only module needed to setup a working game. Parts and instructions for each module will (probably) be in their respective sub-folders.
Anyways, Here are the required parts. </br>
- 1x Arduino (Nano in my case, but other arduinos may be used.)
- 1x 74HC595 Shift Register
- 9x LEDs (5x Module disarm/progress LEDs, 2x Strike indicator LEDs, 1x Success LED, 1x Failure LED)
- 9x current limiting resistors for the LEDs
- 1x 4 digit 7 segment display (or in my case, 4x single digit 7 segment displays and 2x display LEDs)
- MAX7219 LED driver (Or a all-in-one SPI 7 segment LED display module)</br>
Please note that this is for a 'bomb' with 5 puzzle modules. The code is also for 5 modules. If you want more modules in your game, please edit the code, BOM and schematics accordingly. I tried to be as specific as possible in the code.

## Quick Wiring Guide
(Based on a 5-module, Arduino Nano build.)</br>
For the main module:</br>
```Pins 0 and 1 are left unused, because they mess up uploading and I did not want to go through the hassle of unplugging and replugging things to the pins every time I uploaded code to the arduino. Feel free to use it in your game. Basically free parking.
Pins 2 and 3 are used to light the strike LEDs.
Pins 4 through 8 are input pins for the modules. Each pin corresponds to a module, and when the pin is pulled to ground, the main controller interprets it as a "solved" module. It will also stop checking that pin for updates after it is cleared.
Pin 9 is the disarm indicator. It is pulled HIGH when all of the modules are cleared and signals game end.</br>
Pin 10 is the Chip Select/ Slave Select pin for SPI. It is connected to the MAX7219 in this case.
Pin 11 and 13 is used to comunicate via SPI from the arduino to the LED driver
Pin 12 is the strike pin. It is pulled to ground when you mess up a module.
Pin A0 is used as a digital out; it is pulled HIGH when you fail the game by either time out or strike out.
Pins A1 through A3 are also digital outputs; They are used to feed module status data to the shift register.
Pins A4 and A5 are used for I2C communication.
```
