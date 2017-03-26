//
// Created by MJ on 2017. 03. 18..
//

#pragma once

#include "Arduino.h"
#include <avr/interrupt.h>

class Veinduino {
    public:
        Veinduino();
        Veinduino(uint8_t step, uint8_t dir, uint8_t pump,
                  uint8_t pdsck, uint8_t dout, uint8_t interruptpin, uint8_t reverse);

        uint8_t _rev;

        long ScaleRead();
        long ScaleReadAvarage(byte times);
        long ScaleGetOffset();
        double ScaleGetValue(byte times = 1);
    		float ScaleGetUnits(byte times = 1);
        float ScaleGetScale();
        bool ScaleIsReady();
    		void ScaleTare(byte times = 10);
    		void ScaleSetScale(float scale = 1.f);
    		void ScaleSetOffset(long offset = 0);
    		void ScalePowerDown();
    		void ScalePowerUp();
        void ScaleSetGain(uint8_t gain);
        void goMotor(uint16_t step);
        void goMotorBack(uint16_t step);
        void goPump(unsigned long time);
        void resetMotor();
        void beginPull();
    private:
        uint8_t _step, _dir, _pump, _pdsck, _dout, _inter;
        uint8_t pumpState = LOW;
        bool forwardState = false;
        uint8_t GAIN;
    		long OFFSET = 0;
    		float SCALE = 1;
        unsigned long previousPumpMillis = 0;
        unsigned long previousForwardMillis = 0;
};
