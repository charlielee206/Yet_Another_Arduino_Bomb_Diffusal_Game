# Memory
### This is a recreation of the 'Memory' module from the game "Keep Talking and Nobody Explodes"
A recreation of the memory module from KTNE. Also has a switch for difficulty select. This will end the game early.

## Diffusal Manual
```
- Press the correct button to progress the module to the next stage. Complete all stages to disarm the module.
- Pressing an incorrect button will reset the module back to stage 1.
- Button positions are ordered from left to right.
Stage 1:
- If the display is 1, press the button in the second position.
- If the display is 2, press the button in the second position.
- If the display is 3, press the button in the third position.
- If the display is 4, press the button in the fourth position.
Stage 2:
- If the display is 1, press the button labeled "4".
- If the display is 2, press the button in the same position as you pressed in stage 1.
- If the display is 3, press the button in the first position.
- If the display is 4, press the button in the same position as you pressed in stage 1.
Stage 3:
- If the display is 1, press the button with the same label you pressed in stage 2.
- If the display is 2, press the button with the same label you pressed in stage 1.
- If the display is 3, press the button in the third position.
- If the display is 4, press the button labeled "4".
Stage 4:
- If the display is 1, press the button in the same position as you pressed in stage 1.
- If the display is 2, press the button in the first position.
- If the display is 3, press the button in the same position as you pressed in stage 2.
- If the display is 4, press the button in the same position as you pressed in stage 2.
Stage 5:
- If the display is 1, press the button with the same label you pressed in stage 1.
- If the display is 2, press the button with the same label you pressed in stage 2.
- If the display is 3, press the button with the same label you pressed in stage 4.
- If the display is 4, press the button with the same label you pressed in stage 3.
```
as always, this can be re-configured in code.
## BOM
1x Arduino</br>
4x Momentary switches</br>
1x 4-Digit 7 segment display(or 4x single digit 7 segment display)</br>
1x Single digit 7 segment display</br>
1x MAX7219 </br>
6x LEDs (5x Progress indicator LEDs, 1x Module Clear LED)</br>

## Wiring
Pin 2,3,4,5: Input Buttons (To Ground)</br>
Pin 8,9: Difficulty Select Buttons (To Ground)</br>

Pin 10: Chip Select/ Slave Select pin for SPI. It is connected to the MAX7219 in this case.</br>
Pin 11, 13: SPI from the arduino to the LED driver</br>

Pin 6: Clear Pin (To Main Arduino) </br>
Pin 7: Clear LED Pin</br>
Pin 12: Strike Pin (To Main Arduino) </br>

Pin A1,A1,A3,A4,A5: Level Indicatior LEDs</br>

## Requirements
This module makes use of the SPI Library.
