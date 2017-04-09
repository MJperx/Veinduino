#pragma once

#include "Arduino.h"
#include <avr/interrupt.h>

class Veinduino {
    public:
        Veinduino();
        Veinduino(uint8_t step, uint8_t dir, uint8_t ena,
                  uint8_t pump, uint8_t interruptpin,
                  uint8_t reverse, uint8_t pressure);

        uint8_t _rev, _inter, _pressure;
        uint8_t  _pump, _step, _dir, _enable;
        volatile int value = 0;

        void goMotor(uint16_t step);
        void goMotorBack(uint16_t step);
        void goPump(unsigned long time);
        void resetMotor();
        void beginPull();
    private:
        uint8_t pumpState = LOW;
        bool forwardState = false;
        uint8_t GAIN;
    		long OFFSET = 0;
    		float SCALE = 1;
        unsigned long previousPumpMillis = 0;
        unsigned long previousForwardMillis = 0;
};
