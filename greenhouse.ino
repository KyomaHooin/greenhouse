
// 2.4 TFT LCD touch "Hothouse" light control 
//
// TODO:
//
// double touch ?
// bitmap ?

#include <Adafruit_GFX.h>
#include <MCUFRIEND_kbv.h>
#include "TouchScreen.h"
#include <EEPROM.h>
#include </root/avr/greenhouse/openhard.h>

#define BLACK   0x0000
#define WHITE   0xFFFF
#define RED     0xF800
#define GREEN   0x07E0
#define ORANGE  0xFD20
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

const uint16_t ID = 0x9325;       // TFT ID

const int T[2] = {10,11};         // TP223 signal pin
const int CH[2] = {A5};           // IRF540N signal pin

const int DELAY_NOTOUCH = 30000;  // 30s  no TFT touch
const int DELAY_SENSOR = 15000;   // 15s sensor touch press
const int DELAY_PRESS = 100;      // 100ms screen press
const int DELAY_MENU = 1000;      // 500ms screen press

long touchTime[2], fadeTime[2], menuTime, screenTime; //timer

bool TMODE[2];                    //TP223 on/off status
bool CHMODE[2];                   //channel select status
bool PWMODE[2];                   //channel PWM status
bool Token = 1;                   //screen level token

//SETUP

void setup() {
  //DEBUG
  Serial.begin(9600);
  //TOUCH/FET
  for (int i = 0; i < 2; i++) {
    pinMode(T[i],INPUT);
    pinMode(CH[i],OUTPUT);
    digitalWrite(CH[i],LOW);
    //TMODE[i] = EEPROM.read(i);
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
    if ( millis() - touchTime[i] > DELAY_SENSOR && digitalRead(T[i]) == HIGH ) {
      if (digitalRead(CH[i])) { digitalWrite(CH[i],LOW); TMODE[i] = 0; } else { digitalWrite(CH[i],HIGH); TMODE[i] = 255; }
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
  if ( millis() - screenTime > DELAY_NOTOUCH && Token == 0 ) { CHMODE[0]= 0; CHMODE[1] = 0; fixPin(); drawLogo(); Token = 1; }
  //PWM
  //for (int f = 1; f < 254; f++) {
  //  softPWM(CH[0], f , 20);// freq, speed
  //}
  //for (int f = 254; f >  1; f--) {
  //  softPWM(CH[0], f, 20);// freq, speed
  //}
}

//FUNC

void drawMenu() {
  fixPin();
  tft.fillScreen(BLACK);
  colorChann();
  colorOnoff();
  drawFaderBar();
  drawFader();
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

//TFT/Touch sharing pins reset
void fixPin() {
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
}

//Software PWM wave
void softPWM(int pin, int freq, int spd) {
  digitalWrite(pin,HIGH);
  delayMicroseconds(spd*freq);
  digitalWrite(pin,LOW);
  delayMicroseconds(spd*(255-freq));
}

