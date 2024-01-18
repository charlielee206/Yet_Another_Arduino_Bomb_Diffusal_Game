#include <SPI.h>

const int slaveSelect= 10; //pin used to enable the active slave

int QuestionArray[5] = {0,0,0,0,0}; //numbers displayed on main screen

int MasterArray[5][4] = {{1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4},{1,2,3,4}}; // Array[Stage][4] // Saves what numbers are displayed in what order 
int PositionArray[5] = {0,0,0,0,0}; // A place to log what buttons are pressed //Not zero indexing
int LabelArray[5] = {0,0,0,0,0}; // A place to log what label buttons are pressed
int CurrentStage = 0;

const byte Button[] = {2,3,4,5};
bool buttonState[] = {0,0,0,0}; 
int lastButtonState[] = {0,0,0,0}; 

unsigned long lastDebounceTime[] = {0,0,0,0}; 
unsigned long debounceDelay = 50;

int input = 0;

const byte StageLED[5] = {A1,A2,A3,A4,A5};

const byte ClearPin = 6;
const byte StrikePin = 12;
const byte ClearLED = 7;

const byte DifficultySelect[2] = {8,9};
byte Difficulty = 0;

void setup()
{
  Serial.begin(9600);
  Serial.println("");Serial.println("");Serial.println("");Serial.println("");Serial.println("");Serial.println("");Serial.println("");Serial.println("");Serial.println("");Serial.println("");Serial.println("");
  Serial.println("===== START =====");
  SPI.begin(); //initialize SPI
  pinMode(slaveSelect, OUTPUT);
  digitalWrite(slaveSelect, LOW); //select Slave
  
  pinMode(ClearPin, OUTPUT);
  digitalWrite(ClearPin, HIGH);
  pinMode(StrikePin, OUTPUT);
  digitalWrite(StrikePin, HIGH);
  pinMode(ClearLED, OUTPUT);
  digitalWrite(ClearLED, LOW);
  
  for(int i = 0; i < 2; i++){
    pinMode(DifficultySelect[i], INPUT_PULLUP);
  }
  
  if(digitalRead(DifficultySelect[0] == LOW) && digitalRead(DifficultySelect[1] == LOW)){
    Difficulty = 2; // End at stage 3
  }
  if(digitalRead(DifficultySelect[0] == HIGH) && digitalRead(DifficultySelect[1] == LOW)){
    Difficulty = 1; // End at stage 4
  }
  if(digitalRead(DifficultySelect[0] == LOW) && digitalRead(DifficultySelect[1] == HIGH)){
    Difficulty = 1; // End at stage 4
  }
  if(digitalRead(DifficultySelect[0] == HIGH) && digitalRead(DifficultySelect[1] == HIGH)){
    Difficulty = 0; // Full Send
  }
  
  
  for(int i = 0; i < 5; i++){
    pinMode(StageLED[i], OUTPUT);
    digitalWrite(StageLED[i], LOW);
  }

  for(int i = 0; i < 4; i++){
    pinMode(Button[i], INPUT_PULLUP);
  }

//prepare the 7219 to display 7-segment data
  sendCommand (12,1); //normal mode (default is shutdown mode)
  sendCommand (15,0); //Display test off
  sendCommand (10,15); //set medium intensity (range is 0-15)
  sendCommand (11, 5); //7219 digit scan limit command
  sendCommand (9, 255); //decode command, use standard 7-segment digits
  digitalWrite(slaveSelect, HIGH); //deselect slave

//
  NumberSetup();
}

void loop(){

  Stage0();
  if(CurrentStage == 1){
    Stage1();
    Serial.println("");
  }
  if(CurrentStage == 2){
    Stage2();
    Serial.println("");
  }
  if(CurrentStage == 3){
    if(Difficulty == 2){Success();}
    Stage3();
    Serial.println("");
  }
  if(CurrentStage == 4){
    if(Difficulty == 1){Success();}
    Stage4();
  }

  if(CurrentStage == 5){
    Success();
  }
  CurrentStage = 0;
  Serial.println("----------");
  Serial.println("EndOfTurn");
  Serial.println("----------");
  NumberSetup();

  digitalWrite(StageLED[4], LOW);
  /*
  for(int i = 0; i < 5; i++){
    for(int j = 0; j < 10; j++){
      DisplayDigit(i,j);
      delay(100);
    }
  }*/

}


void DisplayDigit(int stage){
  sendCommand(5,QuestionArray[stage]);
  for(int i = 0; i < 4; i++){
    delay(100);
    sendCommand(i+1, MasterArray[stage][i]);
  } 
}

void DigitOff(){
  for(int i = 0; i < 5; i++){
    delay(100);
    sendCommand(i + 1, 0xf);
  } 
  delay(100);
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
          return (i + 1);
        }
      }
    }
    lastButtonState[i] = reading;
  }
  return 0;
}

/*
int ButtonHandler(){
  if(Serial.available()){
    char ch= Serial.read();
    if (ch == '\n'){
      return input; //one of 1,2,3,4
    }
    else{input = ch - '0';}
  }
  return 0;
}
*/

void sendCommand(int command, int value){
  digitalWrite(slaveSelect, LOW); //chip select is active low
  SPI.transfer(command);
  SPI.transfer(value);
  digitalWrite(slaveSelect,HIGH);
}

void NumberSetup(){
  //Serial.print("QArray: ");
  for(int i = 0; i < 5; i++){
    randomSeed(analogRead(0));
    QuestionArray[i] = random(1,5);
    //Serial.print(QuestionArray[i]);
  }
  //Serial.println("");
  
  //Serial.println("MasterArray");
  for(int i = 0; i < 5; i++){
    for(int n = 0; n < 4; n++){
      randomSeed(analogRead(0));
      int r = random(n,4);
      int temp = MasterArray[i][n];
      MasterArray[i][n] = MasterArray[i][r];
      MasterArray[i][r] = temp;
      //Serial.print(MasterArray[i][n]);
    }
    //Serial.println(",");
  }

  
  
  
}

void Success(){
    Serial.println("");
    Serial.println("=-=-=-=-=-=-=-=-=-=");
    Serial.println("Congratz!!!");
    Serial.println("=-=-=-=-=-=-=-=-=-=");
    Serial.println("");
    digitalWrite(ClearPin, LOW);
    digitalWrite(ClearLED, HIGH);
    while(1){}
}

void Strike(){
  Serial.println("");
  Serial.println("===== STRIKE!!! =====");
  Serial.println("");
  digitalWrite(StrikePin, LOW);
  
  for(int i = 0; i < 5; i++){sendCommand(i + 1, 0xf);} 
  delay(100);
  for(int i = 0; i < 5; i++){sendCommand(i + 1, 8);} 
  delay(100);
  for(int i = 0; i < 5; i++){sendCommand(i + 1, 0xf);} 
  delay(100);
  for(int i = 0; i < 5; i++){sendCommand(i + 1, 8);} 
  
  digitalWrite(StrikePin, HIGH);
  for(int i = 0; i < 5; i++){digitalWrite(StageLED[i], LOW);}
  
  CurrentStage = 0;
}

int LabelFinder(int stage, int label){
  for(int i = 0; i < 4; i++){
    if(MasterArray[stage][i] == label){
      return i;
    }
  }
}


void Stage0(){
  CurrentStage++;
  Serial.println("-- STAGE 0 --");
  Serial.println(QuestionArray[0]); // Display Main Number
  Serial.println("-------------");
  for(int i = 0; i < 4; i++){Serial.print(MasterArray[0][i]); Serial.print(",");}
  Serial.println(" ");
  
  DisplayDigit(0);
  
  
  
  int Key = 0;
  
  while(Key == 0){
    Key = ButtonHandler();
    }
  
  
  switch(QuestionArray[0]){
    case 1:
      if(Key == 2){ //Button At Second Position
        PositionArray[0] = 2;
        LabelArray[0] = MasterArray[0][1];
      }
      else{Strike();}
      break;
    case 2:
    if(Key == 2){ //Button At Second Position
        PositionArray[0] = 2;
        LabelArray[0] = MasterArray[0][1];
      }
      else{Strike();}
      break;
    case 3:
    if(Key == 3){ //Button At Third Position
        PositionArray[0] = 3;
        LabelArray[0] = MasterArray[0][2];
      }
      else{Strike();}
      break;
    case 4:
    if(Key == 4){ //Button At Fourth Position
        PositionArray[0] = 4;
        LabelArray[0] = MasterArray[0][3];
      }
      else{Strike();}
      break;

  }
  
  DigitOff();
  digitalWrite(StageLED[0], HIGH);
  Serial.print("Button Pressed: "); Serial.println(Key); Serial.print("Label: "); Serial.println(LabelArray[0]);

}

void Stage1(){
  CurrentStage++;
  Serial.println("-- STAGE 1 --");
  Serial.println(QuestionArray[1]); // Display Main Number
  Serial.println("-------------");
  for(int i = 0; i < 4; i++){Serial.print(MasterArray[1][i]); Serial.print(",");}
  Serial.println(" ");
  
  DisplayDigit(1);
  
  int Key = 0;
  
  while(Key == 0){
    Key = ButtonHandler();
    }
  
  
  switch(QuestionArray[1]){
    case 1:
      if(Key == (LabelFinder(1,4)+1)){ //Button Label '4'
        PositionArray[1] = LabelFinder(1,4) + 1;
        LabelArray[1] = MasterArray[1][LabelFinder(1,4)];
      }
      else{Strike();}
      break;
    case 2:
    if(Key == PositionArray[0]){ //Button At Same Position as stage 0
        PositionArray[1] = PositionArray[0];
        LabelArray[1] = MasterArray[1][PositionArray[1]-1];
      }
      else{Strike();}
      break;
    case 3:
    if(Key == 1){ //Button At First Position
        PositionArray[1] = 1;
        LabelArray[1] = MasterArray[1][0];
      }
      else{Strike();}
      break;
    case 4:
    if(Key == PositionArray[0]){ //Button At Same Position as stage 0
        PositionArray[1] = PositionArray[0];
        LabelArray[1] = MasterArray[1][PositionArray[1]-1];
      }
      else{Strike();}
      break;
    }
  
  DigitOff();
  digitalWrite(StageLED[1], HIGH);
  Serial.print("Button Pressed: "); Serial.println(Key); Serial.print("Label: "); Serial.println(LabelArray[1]);
  
}

void Stage2(){
  CurrentStage++;
  Serial.println("-- STAGE 2 --");
  Serial.println(QuestionArray[2]); // Display Main Number
  Serial.println("-------------");
  for(int i = 0; i < 4; i++){Serial.print(MasterArray[2][i]); Serial.print(",");}
  Serial.println(" ");
  
  DisplayDigit(2);
  
  int Key = 0;
  
  while(Key == 0){
    Key = ButtonHandler();
    }
  
  
  switch(QuestionArray[2]){
    case 1:
      if(Key == (LabelFinder(2,LabelArray[1])+1)){ //Button Same Label as Stage 1
        PositionArray[2] = LabelFinder(2,LabelArray[1]) + 1;
        LabelArray[2] = MasterArray[2][LabelFinder(2,LabelArray[1])];
      }
      else{Strike();}
      break;
    case 2:
    if(Key == (LabelFinder(2,LabelArray[0])+1)){ //Button Same Label as Stage 0
        PositionArray[2] = LabelFinder(2,LabelArray[0]) + 1;
        LabelArray[2] = MasterArray[2][LabelFinder(2,LabelArray[0])];
      }
      else{Strike();}
      break;
    case 3:
    if(Key == 3){ //Button At Third Position
        PositionArray[2] = 3;
        LabelArray[2] = MasterArray[2][2];
      }
      else{Strike();}
      break;
    case 4:
    if(Key == (LabelFinder(2,4)+1)){ //Button Label '4'
        PositionArray[2] = LabelFinder(2,4) + 1;
        LabelArray[2] = MasterArray[2][LabelFinder(2,4)];
      }
      else{Strike();}
      break;
    }
  
  DigitOff();
  digitalWrite(StageLED[2], HIGH);
  Serial.print("Button Pressed: "); Serial.println(Key); Serial.print("Label: "); Serial.println(LabelArray[2]);
  
}

void Stage3(){
  CurrentStage++;
  Serial.println("-- STAGE 3 --");
  Serial.println(QuestionArray[3]); // Display Main Number
  Serial.println("-------------");
  for(int i = 0; i < 4; i++){Serial.print(MasterArray[3][i]); Serial.print(",");}
  Serial.println(" ");
  
  DisplayDigit(3);
  
  int Key = 0;
  
  while(Key == 0){
    Key = ButtonHandler();
    }
  
  
  switch(QuestionArray[3]){
    case 1:
      if(Key == PositionArray[0]){ //Button At Same Position as stage 0
        PositionArray[3] = PositionArray[0];
        LabelArray[3] = MasterArray[3][PositionArray[3]-1];
      }
      else{Strike();}
      break;
    case 2:
    if(Key == 1){ //Button At First Position
        PositionArray[3] = 1;
        LabelArray[3] = MasterArray[3][0];
      }
      else{Strike();}
      break;
    case 3:
    if(Key == PositionArray[1]){ //Button At Same Position as stage 1
        PositionArray[3] = PositionArray[1];
        LabelArray[3] = MasterArray[3][PositionArray[3]-1];
      }
      else{Strike();}
      break;
    case 4:
    if(Key == PositionArray[1]){ //Button At Same Position as stage 0
        PositionArray[3] = PositionArray[1];
        LabelArray[3] = MasterArray[3][PositionArray[3]-1];
      }
      else{Strike();}
      break;
    }
  
  DigitOff();
  digitalWrite(StageLED[3], HIGH);
  Serial.print("Button Pressed: "); Serial.println(Key); Serial.print("Label: "); Serial.println(LabelArray[3]);
  
}

void Stage4(){
  CurrentStage++;
  Serial.println("-- STAGE 4 --");
  Serial.println(QuestionArray[4]); // Display Main Number
  Serial.println("-------------");
  for(int i = 0; i < 4; i++){Serial.print(MasterArray[4][i]); Serial.print(",");}
  Serial.println(" ");
  
  DisplayDigit(4);
  
  int Key = 0;
  
  while(Key == 0){
    Key = ButtonHandler();
    }
  
  
  switch(QuestionArray[4]){
    case 1:
      if(Key == (LabelFinder(4,LabelArray[0])+1)){ //Button Same Label as Stage 0
        PositionArray[4] = LabelFinder(4,LabelArray[0]) + 1;
        LabelArray[4] = MasterArray[4][LabelFinder(4,LabelArray[0])];
      }
      else{Strike();}
      break;
    case 2:
    if(Key == (LabelFinder(4,LabelArray[1])+1)){ //Button Same Label as Stage 1
        PositionArray[4] = LabelFinder(4,LabelArray[1]) + 1;
        LabelArray[1] = MasterArray[1][LabelFinder(4,LabelArray[1])];
      }
      else{Strike();}
      break;
    case 3:
    if(Key == (LabelFinder(4,LabelArray[2])+1)){ //Button Same Label as Stage 2
        PositionArray[4] = LabelFinder(4,LabelArray[2]) + 1;
        LabelArray[4] = MasterArray[4][LabelFinder(4,LabelArray[2])];
      }
      else{Strike();}
      break;
    case 4:
    if(Key == (LabelFinder(4,LabelArray[3])+1)){ //Button Same Label as Stage 3
        PositionArray[4] = LabelFinder(4,LabelArray[3]) + 1;
        LabelArray[4] = MasterArray[4][LabelFinder(4,LabelArray[3])];
      }
      else{Strike();}
      break;
    }
  
  DigitOff();
  digitalWrite(StageLED[4], HIGH);
  Serial.print("Button Pressed: "); Serial.println(Key); Serial.print("Label: "); Serial.println(LabelArray[4]);
  
}
