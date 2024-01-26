#include <TM1637Display.h>

#define DOT  300 //Time for 1 unit of morse code.
#define LETTER_SPACE 5 // LETTER_SPACE * DOT == delay time between each letter

const int ledPin = 2; 
int ledState = LOW; 

unsigned long previousMillis = 0;  

const int PotPin = A0;
const byte ButtonPin = 3;
const byte MeterPin = 5;
const byte DIO = 4;
const byte CLK = 6;

TM1637Display display(CLK, DIO);

bool buttonState = 0; 
int lastButtonState = 0; 

unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 50;

int CounterValue = 0;
int CurrentLetter = 255;
int WordProgress = 0;

byte ClearPin = 13;
byte StrikePin = 12;

String WordList[16] = { "shell",
                    "halls",
                    "slick",
                    "trick",
                    "boxes",
                    "leaks",
                    "strobe",
                    "bistro",
                    "flick",
                    "bombs",
                    "break",
                    "brick",
                    "steak",
                    "sting",
                    "vector",
                    "beats"};


int WordIndex = 0;
int Letter[6] = {255,255,255,255,255,255};

void setup() {
  Serial.begin(9600);
  // set the digital pin as output:
  pinMode(ledPin, OUTPUT);
  pinMode(ClearPin, OUTPUT);
  digitalWrite(ClearPin, HIGH);
  pinMode(StrikePin, OUTPUT);
  digitalWrite(StrikePin, HIGH);
  pinMode(ButtonPin, INPUT_PULLUP);
  pinMode(MeterPin, OUTPUT);
  
  randomSeed(analogRead(A6));
  WordIndex = random(0,16);
  
 for(int i = 0; i < WordList[WordIndex].length(); i++){ 
   int temp = WordList[WordIndex].charAt(i) - '0';
   temp = temp - 49;
   Letter[i] = temp;
 }
 
  Serial.println("==========");
  Serial.print("Word: "); Serial.println(WordList[WordIndex]);
  Serial.println("==========");
}

void loop() {
 
Display();
 ButtonHandler();
 
 if(WordProgress == 0){
   Bwank(Letter[WordProgress]);
 }
 else if(WordProgress == 1){
   Bwank(Letter[WordProgress]);
 }
 else if(WordProgress == 2){
   Bwank(Letter[WordProgress]);
 }
 else if(WordProgress == 3){
   Bwank(Letter[WordProgress]);
 }
 else if(WordProgress == 4){
   Bwank(Letter[WordProgress]);
 }
 else if(WordProgress == 5){
   Bwank(Letter[WordProgress]);
 }
 
 if(WordProgress == 6){ Bwank(26); }
 

}




void Bwank(int character){
  
  if(character == 255){Serial.println("SkipLetter"); WordProgress++; return 0; Serial.println("Dude What The Fuck");}
  
  
  int Letters[27][8] = {
    {LETTER_SPACE,1,1,3,0,0,0,0}, //A // delay, On, off, on, off, on ...
    {LETTER_SPACE,3,1,1,1,1,1,1}, // B
    {LETTER_SPACE,3,1,1,1,3,1,1}, // C
    {LETTER_SPACE,3,1,1,1,1,0,0}, // D
    {LETTER_SPACE,1,0,0,0,0,0,0}, // E
    {LETTER_SPACE,1,1,1,1,3,1,1}, // F
    {LETTER_SPACE,3,1,3,1,1,0,0}, // G
    {LETTER_SPACE,1,1,1,1,1,1,1}, // H
    {LETTER_SPACE,1,1,1,0,0,0,0}, // I
    {LETTER_SPACE,1,1,3,1,3,1,3}, // J
    {LETTER_SPACE,3,1,1,1,3,0,0}, // K
    {LETTER_SPACE,1,1,3,1,1,1,1}, // L
    {LETTER_SPACE,3,1,3,0,0,0,0}, // M
    {LETTER_SPACE,3,1,1,0,0,0,0}, // N
    {LETTER_SPACE,3,1,3,1,3,0,0}, // O
    {LETTER_SPACE,1,1,3,1,3,1,1}, // P
    {LETTER_SPACE,3,1,3,1,1,1,3}, // Q
    {LETTER_SPACE,1,1,3,1,1,0,0}, // R
    {LETTER_SPACE,1,1,1,1,1,0,0}, // S
    {LETTER_SPACE,3,0,0,0,0,0,0}, // T
    {LETTER_SPACE,1,1,1,1,3,0,0}, // U
    {LETTER_SPACE,1,1,1,1,1,1,3}, // V
    {LETTER_SPACE,1,1,3,1,3,0,0}, // W
    {LETTER_SPACE,3,1,1,1,1,1,3}, // X
    {LETTER_SPACE,3,1,1,1,3,1,3}, // Y
    {LETTER_SPACE,3,1,3,1,1,1,1}, // Z
    {3,3,3,5,0,0,0,0} // space
  };
  
  /*
  for(int i = 0; i < 26; i++){
    Letters[i][0] = Letters[i][0] * 2; 
  }*/
  
  CurrentLetter = character;
  
  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis) >= ((Letters[CurrentLetter][CounterValue])*DOT)) {
    previousMillis = currentMillis;
    if((Letters[CurrentLetter][CounterValue]) != 0){if(character != 26){digitalWrite(ledPin, ledState = !ledState);}}
    
    CounterValue++;
    if(CounterValue >= 8){
      CounterValue = 0;
      CurrentLetter = 255;
      WordProgress++;
      Serial.println("-----");
      Serial.println(character);
      Serial.println("-----");
      Serial.print("LEDStatus = "); Serial.println(ledState);
      if(WordProgress == 7){WordProgress = 0; Serial.println("SKEEP");}
    }
    
  }
  

}


int ButtonHandler(){
  for(int i = 0; i < 4; i++){
    int reading = digitalRead(ButtonPin);
    if (reading != lastButtonState) {
      lastDebounceTime = millis();
    }
    if ((millis() - lastDebounceTime) > debounceDelay) {

      if (reading != buttonState) {
        buttonState = reading;
        if (buttonState == LOW) {
          Serial.print("========== Reading: "); Serial.print(analogRead(A0)); Serial.println(" ==========");
          Serial.print("========== Bracket: "); Serial.print(analogRead(A0)/64); Serial.println(" ==========");
          if((analogRead(A0)/64) == WordIndex){
            Serial.println("CLEAR!");
            digitalWrite(ClearPin, LOW);
            for(int i = 0; i < 5; i++){
              uint8_t data[4] = { 0x40, 0x40, 0x40, 0x40 };
              display.setSegments(data);
              delay(200);
              uint8_t clear[4] = { 0x00, 0x00, 0x00, 0x00 };
              display.setSegments(clear);
              delay(200);
            }
            uint8_t data[4] = { 0x73, 0x77, 0x6d, 0x6d };
            display.setSegments(data);
            while(1){}
          }
          else if((analogRead(A0)/64) != WordIndex){
            Serial.println("Wrong!");
            digitalWrite(StrikePin, LOW);
            for(int i = 0; i < 5; i++){
              uint8_t data[4] = { 0x71, 0x77, 0x06, 0x38 };
              display.setSegments(data);
              delay(100);
              uint8_t clear[4] = { 0x00, 0x00, 0x00, 0x00 };
              display.setSegments(clear);
              delay(50);}
            digitalWrite(StrikePin, HIGH);
          }
          return ButtonPin;
        }
      }
    }
    lastButtonState = reading;
  }
  return 0;
}

int Display(){
  display.setBrightness(15);
  uint8_t DisplayNumbers[16][4] = {
    {0x3f,	0x7f,	0xef,	0x06}, // 89.1
    {0x3f,	0x6f,	0xbf,	0x4f}, // 90.3
    {0x3f,	0x6f,	0xcf,	0x06}, // 93.1
    {0x3f,	0x6f,	0xe6,	0x6d}, // 94.5
    {0x3f,	0x6f,	0xed,	0x06}, // 95.1
    {0x3f,	0x6f,	0xed,	0x6f}, // 95.9
    {0x3f,	0x6f,	0x87,	0x4f}, // 97.3
    {0x3f,	0x6f,	0x87,	0x07}, // 97.7
    {0x3f,	0x6f,	0xef,	0x06}, // 99.1
    {0x06,	0x3f,	0xbf,	0x6d}, //100.5
    {0x06,	0x3f,	0x86,	0x4f}, // 101.3
    {0x06,	0x3f,	0xdb,	0x07}, // 102.7
    {0x06,	0x3f,	0xcf,	0x6d}, // 103.5
    {0x06,	0x3f,	0xe6,	0x6d}, // 104.5
    {0x06,	0x3f,	0x87,	0x4f}, // 107.3
    {0x06,	0x3f,	0x87,	0x07}  // 107.7
  };
  

  int reading = (analogRead(PotPin)/4);
  analogWrite(MeterPin, reading); 
  reading = reading/16;
  uint8_t data[4] = { 0xff, 0xff, 0xff, 0xff };
  data[0] = DisplayNumbers[reading][0];
  data[1] = DisplayNumbers[reading][1];
  data[2] = DisplayNumbers[reading][2];
  data[3] = DisplayNumbers[reading][3];
  display.setSegments(data);
  
}
