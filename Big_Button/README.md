# The Big Button
### This is a recreation of the 'The Button' module from the game "Keep Talking and Nobody Explodes"
A quick simple version of the button module from KTNE. will add DIP switch support to change between preset modes.

## Diffusal Manual
```
1. Press and hold down the button.
2. The LED next to the button will light up.
3. Release the button according to the chart given below.

```
|  Light Colour  | Red | Green | Blue | Purple | Yellow | 
| ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| Release when the timer has a :  | 1 | 3 | 5 | 7 | 9 |

as always, this can be re-configured in code.
## BOM
1x Neopixel LED strip (or chained Neopixel LEDs) </br>
1x Momentary Button </br>

## Wiring
Pin 2: Button (to ground)</br>
Pin 6: Neopixel Data Input </br>

Pin 12: Strike Pin (To Main Arduino) </br>
Pin 13: Clear Pin (To Main Arduino) </br>

Pin A4, A5: I2C pins (To Main Arduino)

## Requirements
This module makes use of the Adafruit Neopixel Library.
