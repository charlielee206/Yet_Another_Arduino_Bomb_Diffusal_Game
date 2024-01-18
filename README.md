# Yet_Another_Arduino_Bomb_Diffusal_Game
This is yet another arduino bomb diffusal game.</br>
There are many others like it on the web, but this is mine. I made it. And I am (kind of) proud of it :) </br>

## Features
- **Entirely Modular**</br>
  The orders of the modules do not matter. Which module is inserted does not matter. Every module can be replaced with another module with little to no modifications to hardware or software. This can come in handy when dealing with a grooup of people with a large variation in game skill; Just swap out the modules to make the game Harder/Easier. Donezo.
- **Dumb, but Smart**</br>
This is not a joke. This is not here for the memes or the lulz. Unlike other modular bomb diffusal games, modules in this game do not *have* to keep communication with the main controller module in order to properly work. Each module has to connect two wires: the module clear pin and/or the strike pin, to ground. This does not have to be done with a microcontroller. Theoretically, each module could be completely mechanical, just completing a circuit from the main controller to ground. However, this does not mean that the modules cannot talk to the main controller. The master arduino perodically sends out current game info (remaining time and the number of strikes) to connected modules via I2C. This information can be recieved and used accordingly. Currently, there is no way for the modules to talk to the master controller, but this can be easily modified without hardware modifications. This makes making modules Much easier, but also gives more possibilities if you wish to make a more complicated module. 
- **I made it**</br>
What more do I need to say?

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
## Making your own modules
Each module should have 2 basic outputs: a 'strike' output and a 'clear' output.</br>
both should be pulled HIGH (or not connected at all) when not in use.</br>
The 'strike' pin should be pulled to ground and back to its original state when the player makes an error on the module. This will tell the main arduino to register a strike, and act accordingly. </br>
The 'clear' pin should be pulled to ground when the player successfully disarms the module. The main arduino will ignore any changes to the 'clear' input once the specific module is marked clear. This is to help prevent bugs or faulty wiring making the game unbeatable.</br> 
However, the 'strike' pin is still active even after the module is cleared. (All of the modules share the 'strike' pin input.) Please be wary of this information when designing your own modules.</br></br>
The main arduino also acts as a master I2C controller. At the moment, it broadcasts information about remaining time and strikes to every address written in the code. To add your own module with I2C, you must add the slave device's address to the main arduino's code.
