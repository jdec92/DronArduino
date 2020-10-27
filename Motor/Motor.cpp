/*
 * Motor.cpp
 *
 *      Author: Juande Estrella
 */

#include "Motor.h"

Motor::Motor(uint8_t pin){
	this->pin = pin;
}

void Motor::initialize(){
	servo.attach(pin, pulseMinMotor, pulseMaxMotor);
}

void Motor::updateValue(float value){
	 servo.writeMicroseconds(value);
}

void Motor::controller(float throttle,float pidPitch,float pidRoll,float pidYaw){
	if(throttle < 1060){
		updateValue(throttle);
	}else{
		Serial.println("+++++++++++++Motor++++++++++++");
		Serial.print(throttle);
		Serial.print("\t");
		Serial.print(pidPitch);
		Serial.print("\t");
		Serial.print(pidRoll);
		Serial.print("\t");
		Serial.println(pidYaw);
		Serial.println("+++++++++++++Motor++++++++++++");

		float pulso = throttle + pidPitch + pidRoll + pidYaw;
		if(pulso < 1030){ //Evitamos que en pleno vuelo se pare por completo el motor
			pulso = 1025;
		}
		if(pulso > pulseMaxMotor){  //Evitamos meter valores superiores al motor
			pulso = pulseMaxMotor;
		}
		updateValue(pulso);
	}
}



