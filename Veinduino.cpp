#include "Veinduino.h"

Veinduino::Veinduino(uint8_t step, uint8_t dir, uint8_t ena,
                     uint8_t pump, uint8_t interruptpin,
                     uint8_t reverse, uint8_t pressure) {

    this->_step = step;
    this->_dir = dir;
    this->_enable = ena;
    this->_pump = pump;
    this->_inter = interruptpin;
    this->_rev = reverse;
    this->_pressure = pressure;

    Serial.begin(115200);

    pinMode(_step, OUTPUT);
    pinMode(_dir,  OUTPUT);
    //pinMode(_enable, OUTPUT);
    pinMode(_pump, OUTPUT);
    pinMode(_inter, INPUT);
    pinMode(_rev, INPUT_PULLUP);
    pinMode(_pressure, INPUT);
    digitalWrite(_pump,LOW);
    digitalWrite(_enable,HIGH);
}

void Veinduino::beginPull() {
  digitalWrite(_enable, LOW);
  digitalWrite(_dir, HIGH);
  for(int x= 1; x < 3000; x++) {
      digitalWrite(_step,HIGH);
      delay(1);
      digitalWrite(_step,LOW);
      delay(1);
  }
  resetMotor();
}

void Veinduino::goMotor(uint16_t step) {
    digitalWrite(_dir, HIGH);
    digitalWrite(_enable, LOW);
    for(int x= 1; x < step; x++) {
        digitalWrite(_step,HIGH);
        delay(1);
        digitalWrite(_step,LOW);
        delay(1);
    }
    resetMotor();
}

void Veinduino::goMotorBack(uint16_t step) {
    digitalWrite(_dir, LOW);
    digitalWrite(_enable, LOW);
    unsigned long currentMillis = millis();
    for (int x = 1; x < step; x++) {
      if (currentMillis - previousPumpMillis >= 1) {
        digitalWrite(_step, HIGH);
        delay(1);
        digitalWrite(_step, LOW);
        delay(1);
      }
    }
    resetMotor();
}

void Veinduino::goPump(unsigned long time) {
    digitalWrite(_pump,HIGH);
    for (int i = 0; i < time; i++){
      Serial.println(analogRead(_pressure));
      delay(1);
    }
    digitalWrite(_pump,LOW);
}

void Veinduino::resetMotor() {
    digitalWrite(_step, LOW);
    digitalWrite(_dir, LOW);
}
