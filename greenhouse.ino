//
// 2.4 TFT LCD touch "Hothouse" light control 
//

//CONST

const int CH[3] = {A0,A1,A3};      // N-MOSFET channels signal pin
const int T[3] = {6,7,8};          // touch sensor signal pin
const int PRODLEVA_TOUCH = 15000;  // 15s touch press delay

//VAR

long touchTime[3] = {0,0,0};       // Timer

//SETUP

void setup() {
  //Debug
  Serial.begin(9600);
  //Touch, FET setup
  for (int i = 0; i < 3; i++) {
    pinMode(T[i],INPUT);
    pinMode(CH[i],OUTPUT);
    digitalWrite(CH[i],LOW);
  }
}

//MAIN

void loop() {
  //touch solving
  for (int i = 0; i < 3; i++) {
    if ( millis() - touchTime[i] > PRODLEVA_TOUCH && digitalRead(T[i]) == HIGH ) {
       digitalRead(CH[i]) ? digitalWrite(CH[i],LOW) : digitalWrite(CH[i],HIGH);
       touchTime[i] = millis();
    }
  }
}

