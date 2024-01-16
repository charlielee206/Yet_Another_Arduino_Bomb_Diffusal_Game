#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define PIN 6
#define NUMPIXELS 2
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

byte address = 9; 
int minutes = 0;
int seconds = 0;
int strikes = 0;
byte RxByte;

byte Darkner = 2.5; // RGB valus get divided by this.

byte ButtonPin = 2;

bool buttonState = 0; 
int lastButtonState = 0; 

unsigned long lastDebounceTime = 0; 
unsigned long debounceDelay = 50;
  
int color = 0;
int RGBValues[5][3] = {{255,0,0},{0,255,0},{0,0,255},{200,0,255},{255,200,0}};



void setup() {
  Serial.begin(9600);
  Wire.begin(address);
  Wire.onReceive(Read_I2C);
  pixels.begin();
  pinMode(ButtonPin, INPUT_PULLUP);
  pinMode(13,OUTPUT); //Clear
  digitalWrite(13,HIGH);
  pinMode(12,OUTPUT); //Strike
  digitalWrite(12,HIGH);
}

void loop() {
  ButtonHandler();

}

void LightPixels(int R, int G, int B){
  for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(R, G, B));
    pixels.show(); 
  }
}

int ButtonHandler(){

  int reading = digitalRead(ButtonPin);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {

    if (reading != buttonState) {
      buttonState = reading;
      if (buttonState == LOW) {
        
        randomSeed(analogRead(0));
        color = random(0, 5);
        Serial.println("Buddon");

        LightPixels(RGBValues[color][0],RGBValues[color][1],RGBValues[color][2]);
        while(buttonState == LOW){
          buttonState = digitalRead(ButtonPin);
        }
        
        switch(color){
          case 0:
            if((minutes == 1)||(seconds%10 == 1)||(seconds/10 == 1)){EndGame();} //Red: Release when 1
            break;
          case 1:
            if((minutes == 3)||(seconds%10 == 3)||(seconds/10 == 3)){EndGame();} //GRN: Release when 3
            break;
          case 2:
            if((minutes == 5)||(seconds%10 == 5)||(seconds/10 == 5)){EndGame();} //BLU: Release when 5
            break;
          case 3:
            if((minutes == 7)||(seconds%10 == 7)||(seconds/10 == 7)){EndGame();} //MGT: Release when 7
            break;
          case 4:
            if((minutes == 9)||(seconds%10 == 9)||(seconds/10 == 9)){EndGame();} //YEL: Release when 9
            break;
        }
        Serial.println("ERROR");
        digitalWrite(12, LOW);
        LightPixels(255,0,0);
        delay(500);
        digitalWrite(12,HIGH);
        pixels.clear();
        pixels.show();
        
        return 1;
      }
    }
  }
    lastButtonState = reading;

  return 0;
}


void EndGame(){
  digitalWrite(13,LOW);
  pixels.clear();
  pixels.show();
  
  for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
    pixels.show();
    delay(100); 
  }
  delay(300);
  for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0, 255, 0));
    pixels.show(); 
    delay(100);
  }
  delay(300);
  for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 255));
    pixels.show();
    delay(100); 
  }
  delay(300);
    for(int i = 0; i < NUMPIXELS; i++){
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
    delay(100); 
  }
  delay(300);
  while(1){}
  
  
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
