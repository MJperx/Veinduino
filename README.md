# Veinduino Library

Arduino code for school project. The library can control a stepper motor with an EasyDriver and measure kilogram with HX711.

# Installation
At the top of this website (https://github.com/madar94/Veinduino), choose Clone or download -> Download ZIP.

Follow the instructions on https://www.arduino.cc/en/Guide/Libraries to import this library into your Arduino IDE.

## Examples

The VeinduinoTest directory contains the Veinduino library and some example sketches
illustrating how the library can be used with various time sources:

- `VeinduinoTest.ino` gets the measured values
<p>The library includes a example:</p>

	#include <Arduino.h>
	#include "Veinduino.h"
	
	Veinduino* veinduino;
	char user_input;
	void setup() {
      veinduino = new Veinduino(9,8,7,A0,A1,2,3);
      // 9 -> Step pin of the EasyDriver
      // 8 -> Dir pin of the EasyDriver
      // 7 -> Pin of the pump
      // A0 -> SCK pin of the HX711
      // A1 -> DT pin of the HX711
      // 2 -> 'Set motor to begin position' button
      // 3 -> Interupt button
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


<h2>Authorship and License</h2>

<a rel="license" href="http://creativecommons.org/licenses/by/3.0/"><img alt="Creative Commons License" id="license" src="http://i.creativecommons.org/l/by/3.0/80x15.png" /></a>

<p>This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/3.0/">Creative Commons Attribution 3.0 Unported License</a>; it is written by Jozsef Madar. (Do <a href="mailto:madar94@gmail.com">let me know</a> if you find it interesting or useful! Or, should you find bugs, a report or a <a href="http://help.github.com/pull-requests/">pull request</a> would be welcome.)</p>
