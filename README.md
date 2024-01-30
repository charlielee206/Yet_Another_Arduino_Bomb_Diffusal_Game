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

## How it works
The main game logic is handled by the main module. Remaining time, the number of strikes, cleared modules, clock and clear/strike display...</br>
The main module constantly checks each module if the player has cleared it, or made an error. If any of these actions happen, the main module acts accordingly.
For some advanced modules, other information may be required. The main module broadcasts remaining time and the number of strikes via I2C. This information can be recieved and used accordingly.

## Making your own modules
Each module should have 2 basic outputs: a 'strike' output and a 'clear' output.</br>
both should be pulled HIGH (or not connected at all) when not in use.</br>
The 'strike' pin should be pulled to ground and back to its original state when the player makes an error on the module. This will tell the main arduino to register a strike, and act accordingly. </br>
The 'clear' pin should be pulled to ground when the player successfully disarms the module. The main arduino will ignore any changes to the 'clear' input once the specific module is marked clear. This is to help prevent bugs or faulty wiring making the game unbeatable.</br> 
However, the 'strike' pin is still active even after the module is cleared. (All of the modules share the 'strike' pin input.) Please be wary of this information when designing your own modules.</br></br>
The main arduino also acts as a master I2C controller. At the moment, it broadcasts information about remaining time and strikes to every address written in the code. To add your own module with I2C, you must add the slave device's address to the main arduino's code.
