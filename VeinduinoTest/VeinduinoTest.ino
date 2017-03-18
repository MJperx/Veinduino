#include <Arduino.h>
#include "Veinduino.h"

Veinduino veinduino;

void setup() {
    Serial.begin(115200);

    veinduino = new Veinduino(2,3,4,5,6,7,A0,A1);
}

void loop() {
    veinduino.goMotor(1000);
}