
// 2.4 TFT LCD touch "Greenhouse" light control 
//
//TODO: SPI shift registers ,EEPROM config
//

#include </root/avr/greenhouse/openhard.h>
#include <MCUFRIEND_kbv.h>
#include <Adafruit_GFX.h>
#include "TouchScreen.h"

#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define YELLOW  0xFFE0

#define MINPRESS 20
#define MAXPRESS 1000

//TFT instance
MCUFRIEND_kbv tft;
//TouchScreen instance
TouchScreen ts = TouchScreen(9, A2, A3, 8, 300);
//Touch instance
TSPoint tp;

//CONST/VAR

const int T[2] = {10};// TP223 signal pin
const int CH[2] = {A5};// IRF540N signal pin

const unsigned int DELAY_NOTOUCH = 60000;// 1m no TFT touch
const int DELAY_SENSOR = 15000;// 15s sensor touch press
const int DELAY_PRESS = 100;// 100ms general screen press
const int DELAY_MENU = 1000;// 1s menu press

long touchTime[2], fadeTime[2], menuTime, screenTime;// timer

bool TMODE[2], CHMODE[2];// TP223 on/off, channel,
int PWMODE[2];// PWM level

bool Token = 1;// screen level token

//SETUP

void setup() {
  //TOUCH/FET
  for (int i = 0; i < 2; i++) {
    pinMode(T[i],INPUT);
    pinMode(CH[i],OUTPUT);
    digitalWrite(CH[i],LOW);
  }
  //TFT
  tft.reset();
  tft.begin(0x9325);// Driver ID
  tft.setRotation(1);// "landscape"
  drawLogo();
}

//MAIN

void loop() {
  //SENSOR
  for (int i = 0; i < 2; i++) {
    if ( millis() - touchTime[i] > DELAY_SENSOR && digitalRead(T[i]) == HIGH ) {
      if (digitalRead(CH[i])) { digitalWrite(CH[i],LOW); TMODE[i] = 0; } else { digitalWrite(CH[i],HIGH); TMODE[i] = 1; PWMODE[i] = 255; }
      //Update menu
      if (Token == 0 ) { fixPin(); colorOnoff(); }
      //reset timer
      touchTime[i] = millis();
    }
  }
  //SCREEN
  if ( millis() - screenTime > DELAY_PRESS ) {
    //ScreenTouch coord.
    tp = ts.getPoint();
    // MENU
    if ( tp.z > MINPRESS && tp.z < MAXPRESS && Token ) { Token = 0; drawMenu(); menuTime = millis(); }
    //CHANNEL
    if (millis() - menuTime > DELAY_MENU) {
      //CH0 TOUCH
      if (595 < tp.x && tp.x < 800 && 245 < tp.y && tp.y < 495 && tp.z > MINPRESS && tp.z < MAXPRESS && !Token ) {
        CHMODE[0] = 1; CHMODE[1] = 0; fixPin(); colorChann();
      }
      //CH2 TOUCH     
      if (350 < tp.x && tp.x < 535 && 245 < tp.y && tp.y < 495 && tp.z > MINPRESS && tp.z < MAXPRESS && !Token ) {
        CHMODE[1] = 1; CHMODE[0] = 0; fixPin(); colorChann();
      }
    }
    //ON-OFF TOUCH
    if (715 < tp.x && tp.x < 770 && 560 < tp.y && tp.y < 645 && tp.z > MINPRESS && tp.z < MAXPRESS && !Token) {// CH0 ON
      if (CHMODE[0] == 1 && TMODE[0] == 0) { digitalWrite(CH[0],HIGH); TMODE[0] = 1; PWMODE[0] = 255; fixPin(); colorOnoff(); }
    }
    if (595 < tp.x && tp.x < 650 && 560 < tp.y && tp.y < 645 && tp.z > MINPRESS && tp.z < MAXPRESS && !Token) {// CH0 OFF
      if (CHMODE[0] == 1 && TMODE[0] == 1) { digitalWrite(CH[0],LOW); TMODE[0] = 0; fixPin(); colorOnoff(); }
    }
    if (465 < tp.x && tp.x < 530 && 560 < tp.y && tp.y < 645 && tp.z > MINPRESS && tp.z < MAXPRESS && !Token) {// CH1 ON
      if (CHMODE[1] == 1 && TMODE[1] == 0) { digitalWrite(CH[1],HIGH); TMODE[1] = 1; PWMODE[1] = 255; fixPin(); colorOnoff(); }
    }
    if (345 < tp.x && tp.x < 410 && 560 < tp.y && tp.y < 645 && tp.z > MINPRESS && tp.z < MAXPRESS && !Token) {// CH1 OFF
      if (CHMODE[1] == 1 && TMODE[1] == 1) { digitalWrite(CH[1],LOW); TMODE[1] = 0; fixPin(); colorOnoff(); }
    }
    //FADER TOUCH
    if (710 < tp.x && tp.x < 780 && 725 < tp.y && tp.y < 810 && tp.z > MINPRESS && tp.z < MAXPRESS && !Token) {// FADE-
      for (int i = 0; i < 2; i++) {
        if (CHMODE[i] == 1 && TMODE[i] == 1) {
          if (5 < PWMODE[i] && PWMODE[i] <= 255) { PWMODE[i] -= 5; analogWrite(CH[i], PWMODE[i]); }
        }
      }
    }
    if (345 < tp.x && tp.x < 405 && 725 < tp.y && tp.y < 810 && tp.z > MINPRESS && tp.z < MAXPRESS && !Token) {// FADE+
      for (int i = 0; i < 2; i++) {
        if (CHMODE[i] == 1 && TMODE[i] == 1) {
          if ( 5 <= PWMODE[i] && PWMODE[i] < 255) { PWMODE[i] += 5; analogWrite(CH[i], PWMODE[i]); }
        }
      }
    }
    //UPDATE
    if (tp.z > MINPRESS && tp.z < MAXPRESS) { screenTime = millis(); }
  }
  //SCREENSAVER
  if (millis() - screenTime > DELAY_NOTOUCH && Token == 0) { CHMODE[0]= 0; CHMODE[1] = 0; fixPin(); drawLogo(); Token = 1; }
}

//FUNC
//Menu
void drawMenu() {
  fixPin();
  tft.fillScreen(BLACK);
  colorChann();
  colorOnoff();
  drawFader();
}
//Fader
void drawFader() {
 tft.drawRect(65,170,40,40, WHITE);// [-]
 tft.drawRect(66,171,38,38, WHITE);
 tft.drawRect(215,170,40,40, WHITE);// [+]
 tft.drawRect(216,171,38,38, WHITE);
 tft.setCursor(80, 183);
 tft.setTextSize(2);
 tft.println("-");
 tft.setCursor(230, 183);
 tft.setTextSize(2);
 tft.println("+");
 tft.drawRect(115,190,90,2, WHITE);// line
 tft.drawRect(115,186,2,10, WHITE);// 1st
 tft.drawRect(137,186,2,10, WHITE);// 2nd
 tft.drawRect(159,186,2,10, WHITE);// 3rd
 tft.drawRect(181,186,2,10, WHITE);// 4th
 tft.drawRect(204,186,2,10, WHITE);// 5th
}
//90x90 square + 10px spacing
void colorChann() {
  for (int i = 0; i < 2; i++) {
    if (CHMODE[i]) {
      tft.drawRect(65 + i * 100, 20, 90, 90, YELLOW);
      tft.drawRect(66 + i * 100, 21, 88, 88, YELLOW);
    } else {
      tft.drawRect(65 + i * 100, 20, 90, 90, WHITE);
      tft.drawRect(66 + i * 100, 21, 88, 88, WHITE);
    }
    tft.setCursor(85 + i * 100, 55);
    tft.setTextSize(3);
    tft.print("CH");
    tft.println(i);   
  }
}
//40x40 square + 10px spacing
void colorOnoff() {
  for (int i = 0; i < 2; i++) {
    if (TMODE[i]) {
      tft.drawRect(65 + i * 100, 120, 40, 40, GREEN);
      tft.drawRect(66 + i * 100, 121, 38, 38, GREEN);
      tft.drawRect(115 + i * 100, 120, 40, 40, WHITE);
      tft.drawRect(116 + i * 100, 121, 38, 38, WHITE);
    } else {
      tft.drawRect(65 + i * 100, 120, 40, 40, WHITE);
      tft.drawRect(66 + i * 100, 121, 38, 38, WHITE);
      tft.drawRect(115 + i * 100, 120, 40, 40, RED);
      tft.drawRect(116 + i * 100, 121, 38, 38, RED);
    }
    tft.setCursor(74 + i * 100, 133);
    tft.setTextSize(2);
    tft.println("ON");
    tft.setCursor(118 + i * 100, 133);
    tft.setTextSize(2);
    tft.println("OFF");
  }
}
//Bitmap PROGMEM logo
void drawLogo() {
  tft.fillScreen(BLACK);
  //draw PROGMEM 16bit color img pixel by pixel..
  int k = 0;
  for (int i=0; i < 64; i++) {
    for (int j=0; j < 64; j++) {
      tft.drawPixel(130 + j, 85 + i, pgm_read_word(&openhard[k]));
      k++;
    }
  }
  tft.setCursor(100, 160);
  tft.setTextSize(1);
  tft.println("GREENHOUSE SYSTEM Ltd.");
}
//TFT-Touch sharing pins reset
void fixPin() {
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

