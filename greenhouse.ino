//
// 2.4 TFT LCD touch "Hothouse" light control 
//

//CONST

const int CH[4] = {A0,A1,A3};    // N-MOSFET channels
const int T[3] = {2,3,4};        // Touch sensor.
const int PRODLEVA_TOUCH = 5000; // 5s prodleva detekce stisku.

//VAR

long touchTime;                  // Timers.

//SETUP
void setup() {
  //Debug
  Serial.begin(9600);
  //Touch, FET setup
  for (int i = 0; i < 3; i++) {
    pinMode(T[i],INPUT);
    pinMode(CH[i], OUTPUT);
    digitalWrite(CH[i], LOW);
  }
  //Touch counter setup.
  //touchTime = millis();
  }
//MAIN
void loop() {
 //touch solving
// for (int i = 0; i < 3; i++) {
 if ( millis() - touchTime > PRODLEVA_TOUCH ) {
   if ( digitalRead(T[0]) == HIGH && digitalRead(CH[0]) == LOW ) {
     Serial.print("Touch! ");
     Serial.print(digitalRead(T[0]));
     Serial.print(" millis: ");
     Serial.print(millis());
     Serial.print(" counter: ");
     Serial.print(touchTime);
     Serial.print(" diff: ");
     Serial.println(millis() - touchTime);
     digitalWrite(CH[0],HIGH);
     touchTime = millis();
   }
 }
 if ( millis() - touchTime > PRODLEVA_TOUCH ) {
   if ( digitalRead(T[0]) == HIGH && digitalRead(CH[0]) == HIGH ) {
     Serial.print("Touch! ");
     Serial.print(digitalRead(T[0]));
     Serial.print(" millis: ");
     Serial.print(millis());
     Serial.print(" counter: ");
     Serial.print(touchTime);
     Serial.print(" diff: ");
     Serial.println(millis() - touchTime);
     digitalWrite(CH[0], LOW);
     touchTime = millis();
   }
 }
//  for (int i = 0; i < 4; i++) {
//    digitalWrite(CH[i], LOW);
//  }
//  delay(10000);
//  for (int i = 0; i < 4; i++) {
//    digitalWrite(CH[i], HIGH);
//  }
//  delay(2000);
}
