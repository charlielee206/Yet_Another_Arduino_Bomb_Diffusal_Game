# Morse Code
### This is a recreation of the 'Morse Code' module from the game "Keep Talking and Nobody Explodes"

## Diffusal Manual
```
1.Interpret the signal from the flashing light using the Morse Code chart to spell one of the words in the table.
2. The signal will loop, with a long gap between repetitions.
3.Once the word is identified, set the dial to the corresponding position and press the button.


Note: The 'complicated' version has a display that shows the current dial settings. The 'simple' version does not have this feature, and the dial position has to be estimated via physical markings on the panel. 
```
|      Word         | Dial Position | 
| ------------- | ------------- | 
| shell | 0 |
| halls | 1 |
| slick | 2 |
| trick | 3 |
| boxes | 4 |
| leaks | 5 |
| strobe | 6 |
| bistro | 7 |
| flick | 8 |
| bombs | 9 |
| break | 10 |
| brick | 11 |
| steak | 12 |
| sting | 13 |
| vector | 14 |
| beats | 15 |


## BOM
1x Arduino</br>
1x Potentiometer </br>
1x LED </br>
1x Momentary Switch/Button</br>
1x 4-digit 7 Segment Display (optional) </br>
1x MAX7219 LED matrix Driver / Backpack (optional) </br>
1x Analog DC Voltmeter (Really Optional) </br>

*The parts marked as "optional" is not required in the simple version of the game.</br>
**The Analog Voltmeter is just there for aesthetic purposes.

## Wiring
Pin 2: LED</br>
Pin 3: Button </br>

Pin 12: Strike Pin (To Main Arduino) </br>
Pin 13: Clear Pin (To Main Arduino) </br>

Pin A0: Potentiometer 
