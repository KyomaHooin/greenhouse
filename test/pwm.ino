//
// Fading test
//

//CONST

const int CH[3] = {A0,A1,A2};      // N-MOSFET channels signal pin

//SETUP

void setup() {
  //FET setup
  for (int i = 0; i < 3; i++) {
    pinMode(CH[i],OUTPUT);
    digitalWrite(CH[i],LOW);
  }
}

//MAIN

void loop() {
  for (int f = 1; f < 254; f++) {
    softPWM(CH[0], f, 20);// frequency, speed
  }
  for (int f = 254; f >  1; f--) {
    softPWM(CH[0], f, 20);
  }
}

void softPWM(int pin, int freq, int spd) {
  digitalWrite(pin,HIGH);
  delayMicroseconds(spd*freq);
  digitalWrite(pin,LOW);
  delayMicroseconds(spd*(255-freq));  
}

