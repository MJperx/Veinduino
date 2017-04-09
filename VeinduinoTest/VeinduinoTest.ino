#include <Arduino.h>
#include "Veinduino.h"
#include "HX711.h"

HX711 scale(A1, A0);

Veinduino* veinduino;
char user_input;

const int step = 9;            //STEP pin of the Easy Driver
const int dir = 8;             //DIR pin of the Easy Driver
const int ena = 6;             //ENABLE pin of the Easy Driver
const int pump = 7;            //Controll pin of the pump
const int reverse = 2;         //Pin of the reverse controll button
const int forcestop = 3;       //Pin of the force stop button
const int pressureSens = A5;   //Pin of the pressure sensor

void setup() {
    veinduino = new Veinduino(step, dir, ena, pump, reverse, forcestop, pressureSens); //Constructor of the Veinduino class
    scale.read();              //Read a raw value from the scale
    scale.read_average(20);    //The average of 20 readings from the scale
    scale.get_value(5);        //The average of 5 readings from the scale minus the tare weight (not set yet)
    scale.get_units(5);        //The average of 5 readings from the scale minus tare weight (not set) divided by the scale parameter (not set yet)
    scale.set_scale(2280.f);   //This value is obtained by calibrating the scale with known weights (we calibrated the scale with a 1 kg reference weight)
    scale.tare();              //Reset the scale to 0
    scale.read();              //Read a raw value from the scale
    scale.read_average(20);    //The average of 20 readings from the scale
    scale.get_value(5);        //Print the average of 5 readings from the ADC minus the tare weight, set with tare()
    scale.get_units(5);        //Print the average of 5 readings from the ADC minus tare weight, divided by the SCALE parameter set with set_scale
    sei();                     // Enable global interrupts
    EIMSK |= (1 << INT0);      // Enable external interrupt INT0
    EICRA |= (1 << ISC01);     // Trigger INT0 on falling edge
}

void loop() {
  if(Serial.available()){                     //If the serial connection is available
    user_input = Serial.read();               //Read a character from serial
    if (user_input == '1') {                  //If the character is 1
      veinduino->goMotor(3000);               //The motor will go to the end of the rail
    } else if (user_input == '2') {           //If the character is 2
      veinduino->goMotorBack(3000);           //The motor will go back
    } else if (user_input == '3'){            //If the character is 3
      digitalWrite(veinduino->_dir, HIGH);    //Change the direction of the motor
      for(int x= 1; x < 250; x++) {
          veinduino->goMotor(10);             //The motor will spin
          Serial.println(scale.get_units(2)); //Print the average of 2 readings from the ADC minus the tare weight
      }
      veinduino->resetMotor();                //Stop the motor
    } else if (user_input == '4'){            //If the character is 4
      veinduino->goPump(10000);               //Enable the pump and start measuring the pressure
    }
    veinduino->resetMotor();                  //Stop the motor
  }
  if (digitalRead(veinduino->_rev) == LOW){
    veinduino->goMotorBack(100);
    //Serial.println("da");
  }
}

ISR(INT0_vect)                         //Interrupt Service Routine attached to INT0 vector
{
    digitalWrite(veinduino->_enable,HIGH);  //Disable the motor
    //Serial.println("ad");
}
