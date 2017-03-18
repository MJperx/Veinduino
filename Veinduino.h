//
// Created by MJ on 2017. 03. 18..
//

#pragma once

#include "Arduino.h"

class Veinduino {
    public:
        Veinduino();

        Veinduino(uint8_t pin, uint8_t dir, uint8_t _ms1,
                  uint8_t ms2, uint8_t ena, uint8_t pump,
                  uint8_t _pdsck, uint8_t _dout);

        void goMotor(uint16_t step);
        void goPump(unsigned long time);
        void resetMotor();
        void HX711SetGain(uint8_t gain);
        long HX711Read();
        long HX711ReadAvarage();
        bool ScaleIsReady();
    private:
        uint8_t _step, _dir, _ms1, _ms2, _ena, _pump, _pdsck, _dout;
        uint8_t pumpState = LOW;
        uint8_t GAIN;
        unsigned long previousPumpMillis = 0;

};

