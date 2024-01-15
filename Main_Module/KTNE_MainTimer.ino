#include <SPI.h>
#include <Wire.h>
//#include <SoftwareSerial.h>



///////////////////////////////////////////////
// Timer Preset. How much time the player starts off with.
// note to self: remove when DIP sw support is done

#define TIMER_MINUTES 5
#define TIMER_SECONDS 0

///////////////////////////////////////////////


//=========== I2C and SPI Stuff ==========//

#define numberofDigits 4
int number=0;

int AddressArray[] = {9,10};  // Array that stores the I2C addresses of slave modules. 

//========== KTNE Setup ==========//

  //---------- Pins ----------//
  
  byte Module[] = { 4,5,6,7,8 }; // Pins for KINE module inputs
  const byte StrikePin = 12; // Pulled LOW when you mess up a module.
  const byte EndPin = A0; // Pin to signal Timer End
  const byte DisarmPin = 9; // Pin to signal Sucessful disarm.
  
  const byte DisplayEnable= 10; // pin used to enable the active slave


  const int clockPin = A1; // CLK pin for SR - pin 11 on chip.
  const int latchPin = A2; // Latch pin for SR - pin 12 on chip. 
  const int dataPin = A3; // Data pin for SR - pin 14 on chip.

  const int StrikeLED1 = 2; // Two LEDs to indicate how many strikes you have.
  const int StrikeLED2 = 3;

  
  //---------- Variables ----------//

  int minutes = TIMER_MINUTES;
  int seconds = TIMER_SECONDS; 
  unsigned long previousMillis = 0;

  unsigned int SecondLength = 1000; // "Length" of 1 second. reduce this number to speed up the timer.

  bool ClearStates[sizeof(Module)]; // Clear States for the modules.

  byte Strikes = 0;
  bool PrevStrikePinState = 1;
  
  

  
void setup(){

  Serial.begin(9600);

//---------- Variable Initalizations (If Needed) ----------//
  for(int i = 0; i < sizeof(ClearStates); i++){
    ClearStates[i] = 0;
  }

//---------- MAX7219 and SPI Stuff ----------//
  Wire.begin();
  SPI.begin(); //initialize SPI
  pinMode(DisplayEnable, OUTPUT);
  digitalWrite(DisplayEnable, LOW); //select Slave

  //prepare the 7219 to display 7-segment data
  sendCommand (DisplayEnable,12,1); //normal mode (default is shutdown mode)
  sendCommand (DisplayEnable,15,0); //Display test off
  sendCommand (DisplayEnable,10,15); //set medium intensity (range is 0-15)
  sendCommand (DisplayEnable, 11, numberofDigits); //7219 digit scan limit command
  sendCommand (DisplayEnable, 9, 255); //decode command, use standard 7-segment digits
  digitalWrite(DisplayEnable, HIGH); //deselect slave
  

//---------- Pin Mode Declarations ----------//
  for(int i = 0; i < sizeof(Module); i++){
      pinMode (Module[i], INPUT_PULLUP);
  }
  
  pinMode(StrikeLED1, OUTPUT);
  pinMode(StrikeLED2, OUTPUT);
  pinMode(StrikePin, INPUT_PULLUP);
  
  pinMode(EndPin, OUTPUT);
  pinMode(DisarmPin, OUTPUT);

  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

//---------- Delayed Start ----------//

  digitalWrite(EndPin, LOW);
  digitalWrite(StrikeLED1, LOW);
  digitalWrite(StrikeLED2, LOW);  
  for(int i=1; i < (numberofDigits + 1); i++){sendCommand(DisplayEnable,i,0x1a);}
  delay(3000); // initial delay to give other modules init time.
  
  Serial.println("Start");
}

//=0o0o0o0o0o0o0o0o0o0=] START OF MAIN LOOP [=0o0o0o0o0o0o0o0o0o0=//

void loop(){
  unsigned long currentMillis = millis();
//----------] Timer Function [----------// 

  if(currentMillis - previousMillis >= SecondLength){
    previousMillis = currentMillis;

    if(seconds == 0){
      seconds = 59;
      if(minutes == 0){
        FailState();    // End of timer; Enter Failstate.  
      }
      else{
        minutes--;        
      }      
    }
    else{
      seconds--;
    }
  //Serial.print("Seconds:"); Serial.println(seconds);
    BroadcastValues(AddressArray, sizeof(AddressArray));
  }

  //BroadcastValues(9);
  sendCommand(DisplayEnable,4,seconds%10);
  sendCommand(DisplayEnable,3,seconds/10);
  sendCommand(DisplayEnable,2,minutes%10);
  sendCommand(DisplayEnable,1,minutes/10);


//----------] Module State Detection [----------// 

  for(int i = 0; i < sizeof(ClearStates); i++){
    //ClearStates[i] = !(digitalRead(Module[i]));
        
    if(ClearStates[i] == 0){ // Check only if module is marked as not cleared. "Locking" the states prevents misinputs or HW issues on cleared modules affecting the game. 
      ClearStates[i] = !(digitalRead(Module[i]));
    } 
             
  }

//----------] Clear Progress Display LED [----------//

  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, CalculateSRValue(ClearStates, sizeof(ClearStates)));
  digitalWrite(latchPin, HIGH);

//----------] Check for Game End [----------//
  int StateTotal = 0;
  for(int i = 0; i < sizeof(ClearStates); i++){
    StateTotal = StateTotal + ClearStates[i];      
    
    if(StateTotal == sizeof(ClearStates)){ // *Bomb Has Been Diffused*
      Serial.println("YIPEEEEE");
      digitalWrite(DisarmPin, HIGH);
        for(int j=0;j<10;j++){                      //flashes the numbers.
          for(int i=1; i < (numberofDigits + 1); i++){sendCommand(DisplayEnable,i,0xf);}
          delay(200);
          for(int i=1; i < (numberofDigits + 1); i++){sendCommand(DisplayEnable,i,0x1a);}
          delay(200);
          }
      while(1){} // infloop to stop timer

    }            
  }

//----------] Check and Light Strikes [----------//
  if(PrevStrikePinState != digitalRead(StrikePin)){
    PrevStrikePinState = digitalRead(StrikePin);
    if(digitalRead(StrikePin) == 0){
      Strikes ++;
    } 
    if(Strikes == 1){
      digitalWrite(StrikeLED1, HIGH);
      SecondLength = SecondLength * 0.8; // Speeds up timer
    }
    else if(Strikes == 2){
      digitalWrite(StrikeLED2, HIGH);
      SecondLength = SecondLength * 0.8; // Speeds up timer again
    }   
    else if(Strikes == 3){
      FailState(); //Fail at Strike 3
    }
  }
}

//=0o0o0o0o0o0o0o0o0o0=] END OF MAIN LOOP [=0o0o0o0o0o0o0o0o0o0=//

void sendCommand(int pin, int command, int value){ // Function to send commands over SPI.
  digitalWrite(pin, LOW); //chip select is active low
  SPI.transfer(command);
  SPI.transfer(value);
  digitalWrite(pin,HIGH);
}

void BroadcastValues(int *address, int Length){
  for(int i = 0; i < Length; i++){ 
    Wire.beginTransmission(address[i]);
    Wire.write(255);
    Wire.write(minutes);
    Wire.write(seconds);
    Wire.write(Strikes);
    Wire.endTransmission();
  }
}

int CalculateSRValue(bool *States, int Length){ // Function to turn the ClearStates array into a number ready to feed into the shift register.
  int OutputNumber = 0;

  for(int i = 0; i < Length; i++){
    OutputNumber = OutputNumber << 1;
    OutputNumber = OutputNumber + States[i];
  }

  OutputNumber = OutputNumber << (8 - Length); // display offset correction. The LEDs are connected to the MSB of the counter. we have to shift the value all the way up to display it properly. 
  
  OutputNumber = OutputNumber >> 1; // Display offset correction again. This is here because I was stupid and wired the first LED to pin1, not pin 15 or 14 or whatever the proper MSB is.

  return OutputNumber;
}

void FailState(){ // (In TF2 announcer voice) You Failed!!!! *Booing sound effect plays*
  for(int j=0;j<10;j++){   //flashes the numbers.
    for(int i = 1; i < (numberofDigits + 1); i++){sendCommand(DisplayEnable,i,0xf);}
      delay(200);
      for(int i = 1; i < (numberofDigits + 1); i++){sendCommand(DisplayEnable,i,0);}
        delay(200);
        }
  digitalWrite(EndPin,HIGH);
  while(1){} // end code with infloop.
}




