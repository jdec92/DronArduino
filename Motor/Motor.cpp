/*
 * Motor.cpp
 *
 *      Author: Juande Estrella
 */

#include "Motor.h"

Motor::Motor(uint8_t pin) {
    this->pin = pin;
}

void Motor::initialize() {
    servo.attach(pin, pulseMinMotor, pulseMaxMotor);
}

void Motor::updateValue(float value) {  //valor en microsegundos
    servo.writeMicroseconds(value);
}

void Motor::controller(float throttle, float pitch, float roll, float yaw) {
    if (throttle < 1060) {  //valor throttle minimo antes de despegar con lo cual no necesita control estabilidad
        updateValue(throttle);
    } else {    // activamos el control de estabilidad
        float pulso = throttle + pitch + roll + yaw;
        if (pulso < 1030) { //Evitamos que en pleno vuelo se pare por completo el motor
            pulso = 1025;
        }
        if (pulso > pulseMaxMotor) {  //Evitamos meter valores superiores al pulso maximo de motor
            pulso = pulseMaxMotor;
        }
        updateValue(pulso);
    }
}



