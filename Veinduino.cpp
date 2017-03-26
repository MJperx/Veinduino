//
// Created by MJ on 2017. 03. 18..
//

#include "Veinduino.h"

Veinduino::Veinduino(uint8_t step, uint8_t dir, uint8_t pump,
                     uint8_t pdsck, uint8_t dout, uint8_t interruptpin, uint8_t reverse) {

    this->_step = step;
    this->_dir = dir;
    this->_pump = pump;
    this->_pdsck = pdsck;
    this->_dout = dout;
    this->_inter = interruptpin;
    this->_rev = reverse;
    pinMode(_step, OUTPUT);
    pinMode(_dir,  OUTPUT);
    pinMode(_pump, OUTPUT);
    pinMode(_pdsck, OUTPUT);
    pinMode(_dout, INPUT);
    pinMode(_inter, INPUT);
    pinMode(_rev, INPUT_PULLUP);
    ScaleSetGain(128);
    ScaleSetScale(408.5f);
    ScaleTare();
    Serial.begin(115200);
}


void Veinduino::beginPull() {
  digitalWrite(_dir, HIGH);
  for(int x= 1; x < 3000; x++) {
      digitalWrite(_step,HIGH);
      delay(1);
      digitalWrite(_step,LOW);
      delay(1);
      Serial.println(ScaleGetUnits(10));
      ScalePowerDown();
      delayMicroseconds(10);
      ScalePowerUp();
  }
  resetMotor();
}

void Veinduino::goMotor(uint16_t step) {
    digitalWrite(_dir, HIGH);
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
    unsigned long currentMillis = millis();
    if (currentMillis - previousPumpMillis >= time) {
        previousPumpMillis = currentMillis;
        if (pumpState == LOW) {
            pumpState = HIGH;
        } else {
            pumpState = LOW;
        }
        digitalWrite(_pump, pumpState);
    }
}

void Veinduino::resetMotor() {
    digitalWrite(_step, LOW);
    digitalWrite(_dir, LOW);
}

void Veinduino::ScaleSetGain(uint8_t gain) {
    switch (gain) {
        case 128:
            GAIN = 1;
            break;
        case 64:
            GAIN = 3;
            break;
        case 32:
            GAIN = 2;
            break;
    }
    digitalWrite(_pdsck, LOW);
    ScaleRead();
}

long Veinduino::ScaleRead() {
    unsigned long value = 0;
    uint8_t data[3] = { 0 };
    uint8_t filler = 0x00;

    data[2] = shiftIn(_dout, _pdsck, MSBFIRST);
    data[1] = shiftIn(_dout, _pdsck, MSBFIRST);
    data[0] = shiftIn(_dout, _pdsck, MSBFIRST);

    for (uint8_t i = 0; i < GAIN; i++) {
        digitalWrite(_pdsck, HIGH);
        digitalWrite(_pdsck, LOW);
    }

    if (data[2] & 0x80) {
        filler = 0xFF;
    } else {
        filler = 0x00;
    }

    value = ( static_cast<unsigned long>(filler) << 24
              | static_cast<unsigned long>(data[2]) << 16
              | static_cast<unsigned long>(data[1]) << 8
              | static_cast<unsigned long>(data[0]) );

    return static_cast<long>(value);
}

long Veinduino::ScaleReadAvarage(byte times) {
    long sum = 0;
    for (uint8_t i = 0; i < times; i++) {
        sum += ScaleRead();
    }
    return sum / times;
}

bool Veinduino::ScaleIsReady() {
    return digitalRead(_dout) == LOW;
}

double Veinduino::ScaleGetValue(byte times) {
	return ScaleReadAvarage(times) - OFFSET;
}

float Veinduino::ScaleGetUnits(byte times) {
	return ScaleGetValue(times) / SCALE;
}

void Veinduino::ScaleTare(byte times) {
	double sum = ScaleReadAvarage(times);
	ScaleSetOffset(sum);
}

void Veinduino::ScaleSetScale(float scale) {
	SCALE = scale;
}

float Veinduino::ScaleGetScale() {
	return SCALE;
}

void Veinduino::ScaleSetOffset(long offset) {
	OFFSET = offset;
}

long Veinduino::ScaleGetOffset() {
	return OFFSET;
}

void Veinduino::ScalePowerDown() {
	digitalWrite(_pdsck, LOW);
	digitalWrite(_pdsck, HIGH);
}

void Veinduino::ScalePowerUp() {
	digitalWrite(_pdsck, LOW);
}
