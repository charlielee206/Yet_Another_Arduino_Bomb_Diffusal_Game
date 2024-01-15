# Simon Says
### This is a recreation of the 'Simon Says' module from the game "Keep Talking and Nobody Explodes"

## Diffusal Manual
```
1. One of the four coloured buttons will flash.
2. Using the correct table below, press the button with the
corresponding colour.
3. The original button will flash, followed by another. Repeat this sequence
in order using the colour mapping.
4. The sequence will lengthen by one each time you correctly enter a sequence
until the module is disarmed.

Note: When using the 'Shrimple' version of the game, The lights correspond to the buttons. the following table is not needed.
```
|               | Red Flash | Blue Flash | Green Flash | Yellow Flash | 
| ------------- | ------------- | ------------- | ------------- | ------------- | 
| No Strikes  | Blue  | Red | Yellow | Green | 
| 1 Strike  | Yellow  | Green | Blue | Red |
| 2 Strikes  | Green  | Red | Yellow | Blue |

## BOM
4x Light-up arcade buttons (1 of each colour: Blue, Red, Green, Yellow) </br>
4x LEDs (Included in the arcade buttons) </br>

## Wiring
Pin 2: Blue LED</br>
Pin 3: Red LED </br>
Pin 4: Green LED </br>
Pin 5: Yellow LED </br>

Pin 6: Blue Button (to ground) </br>
Pin 7: Red Button (to ground) </br>
Pin 8: Green Button (to ground) </br>
Pin 9: Yellow Button (to ground) </br>

Pin 12: Strike Pin (To Main Arduino) </br>
Pin 13: Clear Pin (To Main Arduino) </br>

Pin A4, A5: I2C pins (To Main Arduino)
