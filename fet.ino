//
// FET test 
//

//CONST

const int CH[4] = {A0,A1,A3};    // N-MOSFET channels.

//SETUP
void setup() {
  //FET setup
  for (int i = 0; i < 3; i++) {
    pinMode(CH[i], OUTPUT);
    digitalWrite(CH[i], LOW);
  }
}

//MAIN

void loop() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(CH[i], LOW);
  }
  delay(10000);
  for (int i = 0; i < 3; i++) {
    digitalWrite(CH[i], HIGH);
  }
  delay(2000);
}

