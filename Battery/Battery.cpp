/*
 * Battery.cpp
 *
 *
 *      Author: Juande Estrella
 */
#include "Battery.h"

Battery::Battery(uint8_t pin) {
    this->pin = pin;
    this->voltage = 0.0;
}

int Battery::readSensor() {
    return analogRead(pin);
}

void Battery::updateVoltage(int voltage) {
    this->voltage = 2.5 * (voltage * offsetVoltage / 1023);
}

float Battery::getVoltage() {
    return voltage;
}

bool Battery::isLowBattery() {
    updateVoltage(readSensor());
    return voltage < voltageMinBattery;
}
