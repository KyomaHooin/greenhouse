//
// 2.4 TFT LCD touch "Hothouse" light control 
//

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "TouchScreen.h"
#include <EEPROM.h>

//#include </root/avr/greenhouse/logo.h>

#define BLACK   0x0000
#define WHITE   0xFFFF
#define GREY    0x7BEF
#define RED     0xF800
#define GREEN   0x07E0

#define MINPRESS 20
#define MAXPRESS 1000
//TFT instance
MCUFRIEND_kbv tft;
//TouchScreen instance
TouchScreen ts = TouchScreen(9, A2, A3, 8, 300);
//Touch instance
TSPoint tp;

//CONST/VAR

const uint16_t ID = 0x9325;     // TFT ID
const int CH[2] = {A5,12};      // IRF540N signal pin
const int T[2] = {10,11};       // TP223 signal pin

const int DELAY_TOUCH = 15000;    // 15s touch press
const int DELAY_NOTOUCH = 20000;  // 20s  no TFT touch
const int DELAY_PRESS = 100;      // 10ms screen press

long touchTime[2], fadeTime[2], notouchTime, screenTime; //Timers

bool Token = 1;               // Screen token
bool TMODE[2];                  // TP223 on/off status

//SETUP

void setup() {
  //DEBUG
  Serial.begin(9600);
  //TOUCH/FET
  for (int i = 0; i < 2; i++) {
    pinMode(T[i],INPUT);
    pinMode(CH[i],OUTPUT);
    digitalWrite(CH[i],LOW);
    TMODE[i] = EEPROM.read(i);
  }
  //TFT
  tft.reset();
  tft.begin(ID);
  tft.setRotation(1);//landscape
  drawLogo();
}

//MAIN

void loop() {
  //TOUCH
  for (int i = 0; i < 2; i++) {
    if ( millis() - touchTime[i] > DELAY_TOUCH && digitalRead(T[i]) == HIGH ) {
      if (digitalRead(CH[i])) { digitalWrite(CH[i],LOW); TMODE[i] = 0; } else { digitalWrite(CH[i],HIGH); TMODE[i] = 255; }
      //reset timer
      touchTime[i] = millis();
    }
  }

  //SCREEN
  if ( millis() - screenTime > DELAY_PRESS ) {
    //ScreenTouch coord.
    tp = ts.getPoint(); //p.x p.y //p.z
    // SCREENSAVER TOUCH
    if (400 < tp.x && tp.x < 735 && 505 < tp.y && tp.y < 610 && tp.z > MINPRESS && tp.z < MAXPRESS && Token == 1) {
      Token = 0;
      fixPin();
      tft.fillScreen(0x0000);// crlscr      
      drawChannel();
      colorOnoff();
      drawFaderBar();
      drawFader();
    }
    //CHANNEL TOUCH
    //ONOFF TOUCH    
    //FADER TOUCH
    //RESET
    if (tp.z > MINPRESS && tp.z < MAXPRESS) {// catch sharing pin..
      Serial.print("x: "); Serial.print(tp.x);
      Serial.print(" y: "); Serial.print(tp.y);
      Serial.print(" z: "); Serial.println(tp.z);
      //reset timer
      screenTime = millis();
    }
  }
  //SCREENSAVER
  if ( millis() - screenTime > DELAY_NOTOUCH && Token == 0 ) { fixPin(); drawLogo(); Token = 1; }
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

void drawChannel() {
  for (int i = 0; i < 2; i++) {
    tft.drawRect(65 + i * 100, 20, 90, 90, WHITE);
    tft.drawRect(66 + i * 100, 21, 88, 88, WHITE);
    tft.setCursor(85 + i * 100, 55);
    tft.setTextSize(3);
    tft.print("CH");
    tft.println(i);
    
    tft.drawRect(65 + i * 100, 120, 40, 40, WHITE);
    tft.drawRect(66 + i * 100, 121, 38, 38, WHITE);
    tft.drawRect(115 + i * 100, 120, 40, 40, WHITE);
    tft.drawRect(116 + i * 100, 121, 38, 38, WHITE);
    tft.setCursor(74 + i * 100, 133);
    tft.setTextSize(2);
    tft.println("ON");
    tft.setCursor(118 + i * 100, 133);
    tft.setTextSize(2);
    tft.println("OFF");
  }
}

void drawFaderBar() {
 tft.drawRect(65,180,190,15, WHITE);
 tft.drawRect(66,181,188,13, WHITE);
}

void drawFader() {
 tft.drawRect(75,175,35,25, WHITE);
 tft.drawRect(76,176,33,23, WHITE);
 tft.fillRect(77,177,31,21, BLACK);
}

void colorOnoff() {
  for (int i = 0; i < 2; i++) {
    if (TMODE[i]) {
      tft.drawRect(65 + i * 100, 120, 40, 40, GREEN);
      tft.drawRect(66 + i * 100, 121, 38, 38, GREEN);
    } else {
      tft.drawRect(115 + i * 100, 120, 40, 40, RED);
      tft.drawRect(116 + i * 100, 121, 38, 38, RED);
    }
  }
}

void drawLogo() {
  tft.fillScreen(BLACK);
  tft.setCursor(100, 115);
  tft.setTextSize(2);
  tft.println("TOUCH HERE");
  tft.drawRect(90, 105, 140, 35, WHITE);
  tft.drawRect(91, 106, 138, 33, WHITE);
}

void softPWM(int pin, int freq, int spd) {
  digitalWrite(pin,HIGH);
  delayMicroseconds(spd*freq);
  digitalWrite(pin,LOW);
  delayMicroseconds(spd*(255-freq));
}

