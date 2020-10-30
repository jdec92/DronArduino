/*
 * Motor.h
 *
 *      Author: Juande Estrella
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "../GlobalVariable.h"
#include "Servo.h"

struct Motor {
private:
    uint8_t pin;
    Servo servo;

public:
    Motor(uint8_t pin);

    void initialize();

    void updateValue(float value);

    void controller(float throttle, float pitch, float roll, float yaw);

};


#endif /* MOTOR_MOTOR_H_ */
