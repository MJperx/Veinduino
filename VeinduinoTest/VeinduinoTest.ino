#include <Arduino.h>
#include "Veinduino.h"

Veinduino* veinduino;
char user_input;

void setup() {
    veinduino = new Veinduino(9,8,7,A0,A1,2,3);
}

void loop() {
  while(Serial.available()){
    user_input = Serial.read();
    if (user_input == '1') {
      veinduino->goMotor(3000);
    } else if (user_input == '2') {
      veinduino->goMotorBack(3000);
    } else if (user_input == '3'){
      veinduino->beginPull();
    } else {
      Serial.println("Error");
    }
    veinduino->resetMotor();
  }
  if (digitalRead(veinduino->_rev) == LOW){
    veinduino->goMotorBack(3000);
  }
}
