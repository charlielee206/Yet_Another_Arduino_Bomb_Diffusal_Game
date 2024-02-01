#include "U8g2_for_Adafruit_GFX.h"

//font file with custom character subset to minimize space. 
//Contains Alphanumerics and cjk letters used in the program. 
//When changing the words, you will need to generate a custom font if you want different words not english or in the wordlist.


#include "KTNE_Wordlist_16.h" //smol
#include "KTNE_Wordlist_24.h" //large
#define SMOLFONT KTNE_Custom_16
#define LARGEFONT KTNE_Custom_24

#include <XPT2046_Touchscreen.h>
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>

//==========] Colours [==========//
#define HIGH_CONTRAST_MODE //uncomment this to enable high contrase mode. Looks terrible, but at least it's readable.

#ifdef HIGH_CONTRAST_MODE
  #define BGCOLOUR 0x0000
  #define DISPLAYCOLOUR 0x0000
  #define DISPLAYTEXTCOLOUR 0xFFFF
  #define BUTTONCOLOUR 0x0000
  #define BORDERCOLOUR 0xFFFF
  #define TEXTCOLOUR 0xFFFF

#endif

#ifndef HIGH_CONTRAST_MODE
  #define BGCOLOUR 	0x2966
  #define DISPLAYCOLOUR 0x42AC
  #define DISPLAYTEXTCOLOUR 0xE77D
  #define BUTTONCOLOUR 0xB4EF
  #define BORDERCOLOUR 0x632C
  #define TEXTCOLOUR 0x3143
  
  
#endif



U8G2_FOR_ADAFRUIT_GFX u8g2_for_adafruit_gfx;

  #define TFT_CS 10
  #define TFT_RST 8
  #define TFT_DC 9
  #define TOUCH_CS 7
  #define TIRQ_PIN 2
  
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen ts(TOUCH_CS, TIRQ_PIN);
int xval = 0;
int yval = 0;

const String DisplayList[6] = {
  "빈칸",
  " ",
  "일본어",
  "にほんご", 
  "없어",
  "화면"
};
const String WordList[19] = {
  "뭐" ,
  "몰라", 
  "안보여", 
  "어디" ,
  "모르겠어", 
  "잠만",
  "이거",
  "첫번째",
  "응",
  "알았어",
  "아니",
  "했어",
  "다음",
  "가운데",
  "눌러",
  "버튼",
  "잠깐만",
  "다시",
  "어"
};
const byte Sequence[19][19] = {
  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18},
  { 16, 7, 12, 10, 1, 18, 2, 5, 0, 3, 4, 9, 13, 15, 6, 11, 8, 14, 17},
  { 14, 2, 7, 15, 8, 3, 16, 13, 0, 9, 6, 12, 11, 5, 4, 10, 17, 18, 1},
  { 8, 6, 18, 1, 7, 13, 17, 11, 12, 15, 2, 9, 3, 4, 16, 10, 5, 0, 14},
  { 18, 12, 0, 14, 7, 1, 9, 11, 13, 3, 2, 4, 6, 8, 10, 15, 17, 5, 16},
  { 16, 1, 9, 0, 12, 7, 14, 4, 2, 15, 17, 5, 11, 6, 13, 18, 3, 8, 10},
  { 10, 12, 8, 17, 6, 15, 9, 3, 16, 14, 13, 18, 2, 1, 11, 7, 0, 5, 4},
  { 4, 15, 3, 16, 9, 7, 8, 6, 18, 17, 12, 10, 13, 2, 11, 0, 14, 1, 5},
  { 14, 6, 12, 11, 7, 17, 1, 5, 0, 13, 16, 4, 3, 9, 8, 10, 2, 15, 18},
  { 18, 0, 6, 7, 17, 2, 11, 9, 1, 16, 14, 13, 10, 4, 12, 5, 8, 15, 3},
  { 0, 12, 11, 5, 14, 13, 8, 10, 16, 9, 15, 18, 2, 4, 7, 3, 17, 6, 1},
  { 11, 15, 13, 8, 9, 17, 3, 7, 6, 18, 10, 2, 0, 16, 5, 4, 12, 14, 1},
  { 9, 5, 4, 3, 1, 10, 8, 18, 6, 14, 17, 7, 12, 16, 13, 11, 0, 2, 15},
  { 16, 14, 0, 11, 18, 17, 12, 15, 8, 10, 4, 9, 3, 6, 2, 7, 1, 13, 5},
  { 10, 11, 4, 15, 1, 17, 13, 0, 2, 3, 18, 14, 7, 6, 16, 12, 5, 9, 8},
  { 17, 15, 18, 5, 1, 13, 0, 6, 10, 14, 12, 11, 3, 16, 8, 4, 2, 7, 9},
  { 6, 18, 14, 5, 1, 2, 11, 7, 15, 4, 0, 12, 9, 13, 10, 17, 3, 8, 16},
  { 12, 15, 5, 7, 2, 18, 8, 0, 14, 17, 9, 1, 10, 3, 11, 4, 13, 16, 6},
  { 16, 1, 15, 11, 2, 13, 6, 9, 3, 5, 7, 12, 4, 0, 8, 10, 14, 18, 17}
};

byte CorrectButton = 0;
byte DisplayKey = 0; //0~5 random
byte ButtonKey[6] = {0,0,0,0,0,0}; //6 numbers, 0 to 18,

byte Stage = 0;

const byte LED[4] = {3,4,5,6};
const byte ClearPin = A0;
const byte StrikePin = A1;


void setup(void) {
  Serial.begin(9600);
  tft.init(240, 320); 
  ts.begin();
  ts.setRotation(2);
  tft.invertDisplay(0);
  u8g2_for_adafruit_gfx.begin(tft);
  u8g2_for_adafruit_gfx.setFont(SMOLFONT);
  u8g2_for_adafruit_gfx.setForegroundColor(TEXTCOLOUR); 
  
  pinMode(ClearPin, OUTPUT);
  pinMode(StrikePin, OUTPUT);
  digitalWrite(ClearPin, HIGH);
  digitalWrite(StrikePin, HIGH);
  
  randomSeed(analogRead(A6));

  tft.fillScreen(BGCOLOUR);
  for(int i = 0; i < 7; i++){
    RedrawButton(i);
  }
  for(int i = 0; i < 4; i++){pinMode(LED[i],OUTPUT);}

  delay(100);
  NewGameSetup();

}

void loop() {

  
  if (ts.touched()) {
    if(TouchHandler() == CorrectButton + 1){
      
      Serial.println("Correct"); 
      digitalWrite(LED[Stage],HIGH);
      Stage++;
      if(Stage == 4){
        Serial.println("======DONE!======"); 
        digitalWrite(ClearPin, LOW);
        tft.fillScreen(BGCOLOUR);
        tft.fillCircle(120, 123, 50, 0x3731);
        
        tft.fillTriangle(120,150, 150,105, 140,98, 0xFFFF);
        tft.fillTriangle(113,140, 120,150, 140,98, 0xFFFF);
        tft.fillTriangle(120,150, 126,140, 95,120, 0xFFFF);
        tft.fillTriangle(120,150, 90,130, 95,120, 0xFFFF);
        
        u8g2_for_adafruit_gfx.setFont(LARGEFONT);
        u8g2_for_adafruit_gfx.setForegroundColor(DISPLAYTEXTCOLOUR);
        u8g2_for_adafruit_gfx.setFontMode(1); 
        u8g2_for_adafruit_gfx.setCursor(120 - 60, 220);
        u8g2_for_adafruit_gfx.print("CLEAR!");
        while(1){}
      }
      
      Serial.print("Stage: "); Serial.println(Stage);
      
      for(int i = 0; i < 7; i++){
        RedrawButton(i);
      }
      delay(500);
      NewGameSetup();
      delay(1000);

    }
    else{for(int i = 0; i < 4; i++){digitalWrite(LED[i],LOW);} Stage = 0; digitalWrite(StrikePin, LOW); delay(500); digitalWrite(StrikePin, HIGH);}
    
    while(ts.touched()){delay(100);}
    }
}

void RedrawButton(int index){
  /*
  |   0   |
  =========
  | 1 | 2 |
  | 3 | 4 |
  | 5 | 6 |
  */
  #define BORDER_WIDTH 3
  switch(index){
    case 0:
      tft.fillRoundRect(25 - BORDER_WIDTH, 15 - BORDER_WIDTH, 190 + BORDER_WIDTH + BORDER_WIDTH, 60 + BORDER_WIDTH + BORDER_WIDTH, 5, BORDERCOLOUR);
      tft.fillRoundRect(25, 15, 190, 60, 5, DISPLAYCOLOUR);
      break;
    case 1:
      tft.fillRoundRect(15 - BORDER_WIDTH, 105 - BORDER_WIDTH, 100 + BORDER_WIDTH + BORDER_WIDTH, 60 + BORDER_WIDTH + BORDER_WIDTH, 3, BORDERCOLOUR);
      tft.fillRoundRect(15, 105, 100, 60, 5, BUTTONCOLOUR);
      break;
    case 2:
      tft.fillRoundRect(125 - BORDER_WIDTH, 105 - BORDER_WIDTH, 100 + BORDER_WIDTH + BORDER_WIDTH, 60 + BORDER_WIDTH + BORDER_WIDTH, 3, BORDERCOLOUR);
      tft.fillRoundRect(125, 105, 100, 60, 5, BUTTONCOLOUR);
      break;
    case 3:
      tft.fillRoundRect(15 - BORDER_WIDTH, 175 - BORDER_WIDTH, 100 + BORDER_WIDTH + BORDER_WIDTH, 60 + BORDER_WIDTH + BORDER_WIDTH, 3, BORDERCOLOUR);
      tft.fillRoundRect(15, 175, 100, 60, 5, BUTTONCOLOUR);
      break;
    case 4:
      tft.fillRoundRect(125 - BORDER_WIDTH, 175 - BORDER_WIDTH, 100 + BORDER_WIDTH + BORDER_WIDTH, 60 + BORDER_WIDTH + BORDER_WIDTH, 3, BORDERCOLOUR);
      tft.fillRoundRect(125, 175, 100, 60, 5, BUTTONCOLOUR);
      break;
    case 5:
      tft.fillRoundRect(15 - BORDER_WIDTH, 245 - BORDER_WIDTH, 100 + BORDER_WIDTH + BORDER_WIDTH, 60 + BORDER_WIDTH + BORDER_WIDTH, 3, BORDERCOLOUR);
      tft.fillRoundRect(15, 245, 100, 60, 5, BUTTONCOLOUR);
      break;
    case 6:
      tft.fillRoundRect(125 - BORDER_WIDTH, 245 - BORDER_WIDTH, 100 + BORDER_WIDTH + BORDER_WIDTH, 60 + BORDER_WIDTH + BORDER_WIDTH, 3, BORDERCOLOUR);
      tft.fillRoundRect(125, 245, 100, 60, 5, BUTTONCOLOUR);

  }
  
}

void WriteText(int index, String DisplayText){
  u8g2_for_adafruit_gfx.setFont(SMOLFONT);
  u8g2_for_adafruit_gfx.setFontMode(1); 
  u8g2_for_adafruit_gfx.setForegroundColor(TEXTCOLOUR);
  
  switch(index){
    case 0:
      u8g2_for_adafruit_gfx.setFont(LARGEFONT);
      u8g2_for_adafruit_gfx.setForegroundColor(DISPLAYTEXTCOLOUR);
      u8g2_for_adafruit_gfx.setFontMode(1); 
      u8g2_for_adafruit_gfx.setCursor(120 - ((DisplayText.length())*5.5), 45+12);
      u8g2_for_adafruit_gfx.print(DisplayText);
      break;
    case 1:
      u8g2_for_adafruit_gfx.setCursor(65 - ((DisplayText.length())*3.55), 135+8);
      u8g2_for_adafruit_gfx.print(DisplayText);
      break;
    case 2:
      u8g2_for_adafruit_gfx.setCursor(175 - ((DisplayText.length())*3.55), 135+8);
      u8g2_for_adafruit_gfx.print(DisplayText);
      break;
    case 3:
      u8g2_for_adafruit_gfx.setCursor(65 - ((DisplayText.length())*3.55), 205+8);
      u8g2_for_adafruit_gfx.print(DisplayText);
      break;
    case 4:
      u8g2_for_adafruit_gfx.setCursor(175 - ((DisplayText.length())*3.55), 205+8);
      u8g2_for_adafruit_gfx.print(DisplayText);
      break;
    case 5:
      u8g2_for_adafruit_gfx.setCursor(65 - ((DisplayText.length())*3.55), 275+8);
      u8g2_for_adafruit_gfx.print(DisplayText);
      break;
    case 6:
      u8g2_for_adafruit_gfx.setCursor(175 - ((DisplayText.length())*3.55), 275+8);
      u8g2_for_adafruit_gfx.print(DisplayText);
      break;
    
    
  }  
  
}

int TouchHandler(){
  TS_Point p = ts.getPoint();
    xval = map(p.x, 150,4000, 0,240);
    yval = map(p.y, 150, 4000, 0, 320);
    delay(30);
    //Serial.println();
    //Serial.println(xval/120);
    //if(yval > 100){
    //  Serial.println((yval-100)/70);
    //}
    if(yval < 100){return 0;}
    else if((xval/120) == 0 && ((yval-100)/70) == 0){
      return 1;
    }
    else if((xval/120) == 1 && ((yval-100)/70) == 0){
      return 2;
    }
    else if((xval/120) == 0 && ((yval-100)/70) == 1){
      return 3;
    }
    else if((xval/120) == 1 && ((yval-100)/70) == 1){
      return 4;
    }
    else if((xval/120) == 0 && ((yval-100)/70) == 2){
      return 5;
    }
    else if((xval/120) == 1 && ((yval-100)/70) == 2){
      return 6;
    }
    else{return 0;}
    
}

void NewGameSetup(){
  DisplayKey = random(0,6);

  for(int i = 0; i < 6; i++){
    ButtonKey[i] = random(0,19);
    byte duplicate = 1;
    while(duplicate == 1){
      duplicate = 0;
      for(int j = 0; j < i; j++){
        if(ButtonKey[i] == ButtonKey[j]){
          ButtonKey[i] = random(0,19);
          duplicate = 1;
        }
      }
    }
    
  }
  
  Serial.println("==========");
  Serial.print("Display Key: "); Serial.println(DisplayKey);
  Serial.print("Display Text: "); Serial.println(DisplayList[DisplayKey]);
  Serial.print("Button Key: "); for(int i = 0; i < 6; i++){Serial.print(ButtonKey[i]); Serial.print(" ,");}Serial.println();
  Serial.print("Sequence Text: "); for(int i = 0; i < 6; i++){Serial.print(WordList[ButtonKey[i]]); Serial.print(" ,");}Serial.println();
  Serial.print("What Button To Read: "); Serial.println(DisplayKey + 1);
  Serial.print("What The button should say: "); Serial.println(WordList[ButtonKey[DisplayKey]]);
  Serial.print("Reference list on what button to press: "); for(int i = 0; i < 19; i++){Serial.print(Sequence[ButtonKey[DisplayKey]][i]); Serial.print(" ,");}Serial.println();
  Serial.print("Text Reference list on what button to press: "); for(int i = 0; i < 19; i++){Serial.print(WordList[Sequence[ButtonKey[DisplayKey]][i]]); Serial.print(" ,");}Serial.println();
  
  for(int i = 0; i < 19; i++){
    for(int j = 0; j < 6; j++){
      if(Sequence[ButtonKey[DisplayKey]][i] == ButtonKey[j]){
        
        Serial.println("---");
        Serial.print("Matched Button number: "); Serial.println(j);
        Serial.print("Matched Sequence index: "); Serial.println(Sequence[ButtonKey[DisplayKey]][i]);
        Serial.print("Matched Text: "); Serial.println(WordList[Sequence[ButtonKey[DisplayKey]][i]]);
        
        CorrectButton = j;
        goto EXITSEARCHLOOP;
      }
      
    }
    
  }
  EXITSEARCHLOOP:

  
  
  WriteText(0,DisplayList[DisplayKey]);
  delay(50);
  WriteText(1,WordList[ButtonKey[0]]);
  delay(50);
  WriteText(2,WordList[ButtonKey[1]]);
  delay(50);
  WriteText(3,WordList[ButtonKey[2]]);
  delay(50);
  WriteText(4,WordList[ButtonKey[3]]);
  delay(50);
  WriteText(5,WordList[ButtonKey[4]]);
  delay(50);
  WriteText(6,WordList[ButtonKey[5]]);
  delay(50);
}
