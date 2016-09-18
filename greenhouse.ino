//
// 2.4 TFT LCD touch "Hothouse" light control 
//

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "TouchScreen.h"
#include </root/avr/greenhouse/logo.h>

//TFT instance
MCUFRIEND_kbv tft;
//Touch instance
TouchScreen ts = TouchScreen(9, A2, A3, 8, 300);
TSPoint tp;
//CONST/VAR
const uint16_t ID = 0x9325;// TFT ID
//const int CH[3] = {A0,A1,A2};      // IRF540N signal pin
const int CH = A5;                   // IRF540N signal pin
//const int T[3] = {A3,A4,A5};       // TP223 signal pin
const int T = 10;                    // TP223 signal pin
//Delay
const int DELAY_TOUCH = 15000;    // 15s touch press
const int DELAY_NOTOUCH = 20000;    // 20s  no TFT touch
const int DELAY_PRESS = 100;     // 10ms screen press
//Timer
long touchTime,notouchTime, screenTime, fadeTime[3];
//Screen token
bool iniToken = 1;

//SETUP

void setup() {
  //DEBUG
  Serial.begin(9600);
  //TOUCH/FET
  //for (int i = 0; i < 3; i++) {
  //  pinMode(T[i],INPUT);
    pinMode(T,INPUT);
  //  pinMode(CH[i],OUTPUT);
  pinMode(CH,OUTPUT);
  //  digitalWrite(CH[i],LOW);
  digitalWrite(CH,LOW);
  //}
  //TFT
  tft.reset();
  tft.begin(ID);
  tft.setRotation(1);// LANDSCAPE
  drawLogo();
}

//MAIN

void loop() {
  //TOUCH
  //for (int i = 0; i < 3; i++) {
    //if ( millis() - touchTime[i] > DELAY_TOUCH && digitalRead(T[i]) == HIGH ) {
    if ( millis() - touchTime > DELAY_TOUCH && digitalRead(T) == HIGH ) {        
      //digitalRead(CH[i]) ? digitalWrite(CH[i],LOW) : digitalWrite(CH[i],HIGH);
      digitalRead(CH) ? digitalWrite(CH,LOW) : digitalWrite(CH,HIGH);
      //reset timer
      //touchTime[i] = millis();
      touchTime = millis();
    }
  //}

  //SCREEN
  if ( millis() - screenTime > DELAY_PRESS ) {
    //ScreenTouch coord.
    tp = ts.getPoint(); //p.x p.y //p.z
    // SCREENSAVER TOUCH
    if (400 < tp.x && tp.x < 735 && 505 < tp.y && tp.y < 610 && tp.z > ts.pressureThreshhold && iniToken == 1) {
      iniToken = 0;
      fixPin();
      //default screen
      tft.fillScreen(0x0000);// crlscr      
      drawChannel(0);
      drawChannel(1);
      drawChannel(2);
      drawFaderBar();
    }
    //CHANNEL TOUCH
    
    //ONOFF TOUCH
    
    //FADER TOUCH
    //Serial.print("x: "); Serial.print(p.x);
    //Serial.print(" y: "); Serial.print(p.y);
    //Serial.print(" z: "); Serial.println(p.z);
    //Serial.println("Touch!");
    //reset timer
    screenTime = millis();
  }

  //SCREENSAVER
  
  //PWM
  //for (int f = 1; f < 254; f++) {
  //  softPWM(CH[0], f , 20);// freq, speed
  //}
  //for (int f = 254; f >  1; f--) {
  //  softPWM(CH[0], f, 20);// freq, speed
  //}
}

void fixPin() {
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

void drawChannel(int chn) {
 tft.drawRect(15 + chn * 100, 10, 90, 90, 0xFFFF);// CH0
 tft.drawRect(16 + chn * 100, 11, 88, 88, 0xFFFF);
 tft.fillRect(17 + chn * 100, 12, 86, 86, 0x001F); //blue
 tft.setCursor(35 + chn * 100, 45);
 tft.setTextSize(3);
 tft.print("CH");
 tft.println(chn);
 tft.drawRect(15 + chn * 100, 110, 40, 40, 0xFFFF);
 tft.drawRect(16 + chn * 100, 111, 38, 38, 0xFFFF);
 tft.drawRect(65 + chn * 100, 110, 40, 40, 0xFFFF);
 tft.drawRect(66 + chn * 100, 111, 38, 38, 0xFFFF);
 tft.setCursor(24 + chn * 100, 123);
 tft.setTextSize(2);
 tft.println("ON");
 tft.setCursor(68 + chn * 100, 123);
 tft.setTextSize(2);
 tft.println("OFF");
}

void drawFaderBar() {
 tft.drawRect(15,180,294,15, 0xFFFF);// LINE
 tft.drawRect(16,181,292,13, 0xFFFF);
}

void drawFader() {
 tft.drawRect(25,175,35,25, 0xFFFF);// FADER
 tft.drawRect(26,176,33,23, 0xFFFF);
 tft.fillRect(27,177,31,21, 0xF800);// red
}

void drawLogo() {
  tft.fillScreen(0x0000);// BLACK
  tft.setCursor(100, 115);
  tft.setTextSize(2);
  tft.println("TOUCH HERE");
  tft.drawRect(90, 105, 140, 35, 0xFFFF);
}
//void softPWM(int pin, int freq, int spd) {
//  digitalWrite(pin,HIGH);
//  delayMicroseconds(spd*freq);
//  digitalWrite(pin,LOW);
//  delayMicroseconds(spd*(255-freq));
//}

