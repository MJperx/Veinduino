//
// Created by MJ on 2017. 03. 18..
//

#include "Veinduino.h"

Veinduino::Veinduino(uint8_t step, uint8_t dir, uint8_t ms1,
                     uint8_t ms2, uint8_t ena, uint8_t pump,
                     uint8_t pdsck, uint8_t dout) {

    this->_step = step;
    this->_dir = dir;
    this->_ms1 = ms1;
    this->_ms2 = ms2;
    this->_ena = ena;
    this->_pump = pump;
    this->_pdsck = pdsck;
    this->_dout = dout;
    pinMode(_step, OUTPUT);
    pinMode(_dir,  OUTPUT);
    pinMode(_ms1,  OUTPUT);
    pinMode(_ms2,  OUTPUT);
    pinMode(_ena,  OUTPUT);
    pinMode(_pump, OUTPUT);
    pinMode(_pdsck, OUTPUT);
    pinMode(_dout, INPUT);
    digitalWrite(_ena, LOW);
    HX711SetGain(128);
}

void Veinduino::goMotor(uint16_t step) {
    digitalWrite(_dir, LOW);
    for (int x = 1; x < 1000; x++) {
        digitalWrite(_step, HIGH);
        //delay(1);
        digitalWrite(_step, LOW);
        //delay(1);
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
    digitalWrite(_ms1, LOW);
    digitalWrite(_ms2, LOW);
    digitalWrite(_ena, HIGH);
}

void Veinduino::HX711SetGain(uint8_t gain) {
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
    HX711Read();
}

long Veinduino::HX711Read() {
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

long Veinduino::HX711ReadAvarage() {
    long sum = 0;
    for (uint8_t i = 0; i < times; i++) {
        sum += HX711Read();
    }
    return sum / times;
}

bool Veinduino::ScaleIsReady() {
    return digitalRead(_dout) == LOW;
}