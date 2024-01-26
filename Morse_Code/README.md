# Morse Code
### This is a recreation of the 'Morse Code' module from the game "Keep Talking and Nobody Explodes"

## Diffusal Manual
```
1.Interpret the signal from the flashing light using the Morse Code chart to spell one of the words in the table.
2. The signal will loop, with a long gap between repetitions.
3.Once the word is identified, set the dial to the corresponding position or frequency and press the button.


Note: The 'complicated' version has a display that shows the current dial settings. The 'simple' version does not have this feature, and the dial position has to be estimated via physical markings on the panel. 
```
|      Word         | Dial Position | Displayed Frequency |
| ------------- | ------------- | ------------- |
| shell | 0 | 89.1 ㎒|
| halls | 1 | 90.3 ㎒|
| slick | 2 | 93.1 ㎒|
| trick | 3 | 94.5 ㎒| 
| boxes | 4 | 95.1 ㎒|
| leaks | 5 | 95.9 ㎒|
| strobe | 6 | 97.3 ㎒|
| bistro | 7 | 97.7 ㎒|
| flick | 8 | 99.1 ㎒|
| bombs | 9 | 100.5 ㎒|
| break | 10 | 101.3 ㎒|
| brick | 11 | 102.7 ㎒|
| steak | 12 | 103.5 ㎒|
| sting | 13 | 104.5 ㎒|
| vector | 14 | 107.3 ㎒|
| beats | 15 | 107.7 ㎒|


## BOM
1x Arduino</br>
1x Potentiometer </br>
1x LED </br>
1x Momentary Switch/Button</br>
1x 4-digit 7 Segment Display (optional) </br>
1x TM1637 LED matrix Driver / Backpack (optional) </br>
1x Analog DC Voltmeter (Really Optional) </br>

*The parts marked as "optional" is not required in the simple version of the game.</br>
**The Analog Voltmeter is just there for aesthetic purposes.

## Wiring
Pin 2: LED</br>
Pin 3: Button </br>

Pin 4: DIO of TM1637</br>
Pin 6: CLK of TM1637</br>

Pin 5: Analog Voltmeter</br>

Pin 12: Strike Pin (To Main Arduino) </br>
Pin 13: Clear Pin (To Main Arduino) </br>

Pin A0: Potentiometer 

## Requirements
This module makes use of the TM1637 library.
