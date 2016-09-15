//
// Touch test 
//

//CONST

const int T[3] = {2,3,4};        // Touch sensor pin
const int PRODLEVA_TOUCH = 10000; // 10s prodleva detekce stisku

//VAR

long touchTime;// Touch timer

//SETUP

void setup() {
  //Debug
  Serial.begin(9600);
  //Touch setup
  for (int i = 0; i < 3; i++) {
    pinMode(T[i],INPUT);
  }
  //Touch counter setup.
  touchTime = 0;
}

//MAIN

void loop() {
  //touch solving
  if ( millis() - touchTime > PRODLEVA_TOUCH ) {
    if ( digitalRead(T[0]) == HIGH ) {
      Serial.print("Touch! ");
      touchTime = millis();
    }
  }
}

