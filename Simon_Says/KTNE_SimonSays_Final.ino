
int CurrentStage = 0;

byte address = 10; 
int minutes = 0;
int seconds = 0;
int strikes = 0;

#include <Wire.h>
byte RxByte;

byte StrikePin = 12;

//==========] blinkLED Stuff [==========//
  const byte LED[] = {2,3,4,5};
  bool LEDStatus[] = {0,0,0,0,0,0};

  int LightSequence[6] = {2,3,4,5,2,3};
  int ButtonSequence[6] = {6,7,8,9,6,7};

  int blinkcounter = 0;

  unsigned long ms;        //time from millis()
  unsigned long msLast = 0;

//=====] ButtonHandler Stuff [=====//

  const byte Button[] = {6,7,8,9};
  bool buttonState[] = {0,0,0,0}; 
  int lastButtonState[] = {0,0,0,0}; 

  unsigned long lastDebounceTime[] = {0,0,0,0}; 
  unsigned long debounceDelay = 50;
  
  int PlayerButtonSequence[6] = {0,0,0,0,0,0};


void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(13,OUTPUT);
  digitalWrite(13, HIGH);
    
  pinMode(StrikePin, OUTPUT);
  digitalWrite(StrikePin, HIGH);
    
  Wire.begin(address);
  Wire.onReceive(Read_I2C);
  
  AnswerGenerator();
}

void loop()
{
  Read_I2C;
  if(CurrentStage == 6){
    
    digitalWrite(13, LOW);
    
    Serial.println("Simos Says Module Clear!");
    Serial.println("Yipee!");
    
    digitalWrite(2,HIGH);
    delay(150);
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    delay(150);
    digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
    delay(150);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    delay(150);
    digitalWrite(5,LOW);
    digitalWrite(2,HIGH);
    delay(150);
    delay(150);
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    delay(150);
    digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
    delay(150);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    delay(150);
    digitalWrite(5,LOW);
    digitalWrite(2,HIGH);
    delay(150);
    digitalWrite(2,LOW);
    
    while(1){}
  }
  
  ButtonGameplay(CurrentStage);
  blinkLED(CurrentStage);
}

void blinkLED(int Progress){
  ms = millis();
  if(ms - msLast >= 500){
    switch (blinkcounter/2){
      case 0:
        digitalWrite(LightSequence[0], LEDStatus[0] = !LEDStatus[0]);
        blinkcounter++; //0,1
        if((blinkcounter/2) > Progress ){
          blinkcounter = 12;
        }
        break;
      case 1:
        digitalWrite(LightSequence[1], LEDStatus[1] = !LEDStatus[1]);
        blinkcounter++; //2,3
        if((blinkcounter/2) > Progress ){
          blinkcounter = 12;
        }
        break;
      case 2:
        digitalWrite(LightSequence[2], LEDStatus[2] = !LEDStatus[2]);
        blinkcounter++; //4,5
        if((blinkcounter/2) > Progress ){
          blinkcounter = 12;
        }
        break;
      case 3:
        digitalWrite(LightSequence[3], LEDStatus[3] = !LEDStatus[3]);
        blinkcounter++; //6,7
        if((blinkcounter/2) > Progress ){
          blinkcounter = 12;
        }
        break;
      case 4:
        digitalWrite(LightSequence[4], LEDStatus[4] = !LEDStatus[4]);
        blinkcounter++; //8,9
        if((blinkcounter/2) > Progress ){
          blinkcounter = 12;
        }
        break;
      case 5:
        digitalWrite(LightSequence[5], LEDStatus[5] = !LEDStatus[5]);
        blinkcounter++; //10,11
        if((blinkcounter/2) > Progress ){
          blinkcounter = 12;
        }
        break;
      default:
        blinkcounter++;
        Serial.print("Strikes: "); Serial.println(strikes);
        if(blinkcounter >= 14){blinkcounter = 0;}  
    }    
  msLast = ms;
  }   
}

int ButtonHandler(){
  for(int i = 0; i < 4; i++){
    int reading = digitalRead(Button[i]);
    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }
    if ((millis() - lastDebounceTime[i]) > debounceDelay) {

      if (reading != buttonState[i]) {
        buttonState[i] = reading;
        if (buttonState[i] == LOW) {
          return Button[i];
        }
      }
    }
    lastButtonState[i] = reading;
  }
  return 0;
}

void ButtonGameplay(int Progress){

  int Input = ButtonHandler();
  int CurrentStep = 0;
  if(Input != 0){
    Serial.println("Input Detected!");
    blinkcounter = 0;
    for(int i = 0; i < 6; i++){
      LEDStatus[i] = 0;
      digitalWrite(LightSequence[i],0);
    }
    
    while(CurrentStep <= Progress){
      if(Input == ButtonSequence[CurrentStep]){
        Serial.println(Input);
        Input = 0;
        CurrentStep++;
      }
      else if(Input != 0){
        Serial.println("REEEEEEEE");
        CurrentStage = 0;
        digitalWrite(StrikePin, LOW);
        delay(300);
        digitalWrite(2,HIGH);
        digitalWrite(3,HIGH);
        digitalWrite(4,HIGH);
        digitalWrite(5,HIGH);
        delay(800);
        digitalWrite(2,LOW);
        digitalWrite(3,LOW);
        digitalWrite(4,LOW);
        digitalWrite(5,LOW);
        delay(500);
        digitalWrite(StrikePin, HIGH);
        AnswerGenerator();
        
        return 0;
        }
      Input = ButtonHandler();
    }
    Serial.println("Stage Complete! ");
    CurrentStage++;
    if(CurrentStage == 6){return 0;}
    delay(300);
    digitalWrite(2,HIGH);
    delay(150);
    digitalWrite(2,LOW);
    digitalWrite(3,HIGH);
    delay(150);
    digitalWrite(3,LOW);
    digitalWrite(4,HIGH);
    delay(150);
    digitalWrite(4,LOW);
    digitalWrite(5,HIGH);
    delay(150);
    digitalWrite(5,LOW);
    digitalWrite(2,HIGH);
    delay(150);
    digitalWrite(2,LOW);
    delay(400);
  }
}

void AnswerGenerator(){
  Serial.print("New Answer Sequence: ");
  randomSeed(analogRead(0));
  for(int i = 0; i < 6; i++){
    LightSequence[i] = random(2, 6);
    
    switch(strikes){
      case 0:
        switch(LightSequence[i]){
          case 2:
            ButtonSequence[i] = 7;
            break;
          case 3:
            ButtonSequence[i] = 6;
            break;
          case 4:
            ButtonSequence[i] = 9;
            break;
          case 5:
            ButtonSequence[i] = 8;
            break;
        }      
        break;
        
    case 1:
      switch(LightSequence[i]){
          case 2:
            ButtonSequence[i] = 8;
            break;
          case 3:
            ButtonSequence[i] = 9;
            break;
          case 4:
            ButtonSequence[i] = 6;
            break;
          case 5:
            ButtonSequence[i] = 7;
            break;
            }
        break;
      
      case 2:
        switch(LightSequence[i]){
          case 2:
            ButtonSequence[i] = 9;
            break;
          case 3:
            ButtonSequence[i] = 6;
            break;
          case 4:
            ButtonSequence[i] = 7;
            break;
          case 5:
            ButtonSequence[i] = 8;
            break;
        }
        break;    
    }
    
    
    //ButtonSequence[i] = LightSequence[i] + 4;
    
    Serial.print(ButtonSequence[i]);
  }
  Serial.println(" ");
}

void Read_I2C(int numBytes){
  while(Wire.available()) { 
    RxByte = Wire.read();
    if(RxByte == 255){
      minutes = Wire.read();
      seconds = Wire.read();
      strikes = Wire.read();
    }
  }
}