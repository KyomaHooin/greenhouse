//
// 2.4 TFT LCD touch "Hothouse" light control 
//

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "TouchScreen.h"

//VAR

//TFT instance
MCUFRIEND_kbv tft;
//Touch instance
TouchScreen ts = TouchScreen(9, A2, A3, 8, 300);
//CONST/VAR
const uint16_t ID = 0x9325;// TFT ID
//const int CH[3] = {A0,A1,A2};      // IRF540N signal pin
const int CH = A5;                   // IRF540N signal pin
//const int T[3] = {A3,A4,A5};       // TP223 signal pin
const int T = 10;                    // TP223 signal pin
//Delay
const int DELAY_TOUCH = 15000;    // 15s touch press
const int DELAY_SCREEN = 100;     // 10ms screen press
//Timer
long touchTime, screenTime, fadeTime[3];

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
  tft.fillScreen(0x0000);// BLACK
  tft.setCursor(110, 110);
  tft.setTextSize(2);
  tft.println("DEMO CODE");
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
  if ( millis() - screenTime > DELAY_SCREEN ) {
    //ScreenTouch coord.
    TSPoint p = ts.getPoint(); //p.x p.y //p.z
    if (p.z > ts.pressureThreshhold) {
      Serial.print("x: "); Serial.print(p.x);
      Serial.print(" y: "); Serial.print(p.y);
      Serial.print(" z: "); Serial.println(p.z);
    }
    //reset timer
    screenTime = millis();
  }
  //softPWM
  //for (int f = 1; f < 254; f++) {
  //  softPWM(CH[0], f , 20);// freq, speed
  //}
  //for (int f = 254; f >  1; f--) {
  //  softPWM(CH[0], f, 20);// freq, speed
  //}
}

//void softPWM(int pin, int freq, int spd) {
//  digitalWrite(pin,HIGH);
//  delayMicroseconds(spd*freq);
//  digitalWrite(pin,LOW);
//  delayMicroseconds(spd*(255-freq));
//}

