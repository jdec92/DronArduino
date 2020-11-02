/*
 * Bluetooth.cpp
 *
 *      Author: Juande Estrella
 */

#include "Bluetooth.h"

Bluetooth::Bluetooth(uint8_t rx, uint8_t tx) {
    this->BT = new SoftwareSerial(rx, tx);
    this->degreeThrottle = (degreeMinThrottle + degreeMaxThrottle) / 2;
    this->degreePitch = (degreeMax + degreeMin) / 2;
    this->degreeRoll = (degreeMax + degreeMin) / 2;
    this->degreeYaw = (degreeMax + degreeMin) / 2;
}

void Bluetooth::initialize() {
    this->BT->begin(baudrates);
}

void Bluetooth::configure(String name, String pin) {
    // Should respond with OK
    this->BT->print("AT");
    waitForResponse();
    // Should respond with its version
    this->BT->print("AT+VERSION");
    waitForResponse();
    // Set the name to PRACT_NAME
    this->BT->print("AT+NAME" + name);
    waitForResponse();
    // Set response to ok pin
    this->BT->print("AT+PIN" + pin);
    waitForResponse();
    // Set respond with okbaudrate
    this->BT->print("AT+BAUD8");
    waitForResponse();

}

void Bluetooth::updatePulse() {
    String value = waitForResponse();
    if (!value.equals("")) {
        commandChangeThrottle(value);
        degreePitch = commandChangeAngle(degreePitch, pitchUp, pitchDown, value);
        degreeRoll = commandChangeAngle(degreeRoll, rollUp, rollDown, value);
        degreeYaw = commandChangeAngle(degreeYaw, yawUp, yawDown, value);
        commandInitializeMotor(value);
    }
}

void Bluetooth::commandChangeThrottle(String value) {
    if (value.indexOf(throttleUp) != -1) {
        if (degreeThrottle < degreeMaxThrottle) {
            degreeThrottle = degreeThrottle + incrementThrottle;
        }
    } else if (value.indexOf(throttleDown) != -1) {
        if (degreeThrottle > degreeMinThrottle) {
            degreeThrottle = degreeThrottle - incrementThrottle;
        }
    }
}

int Bluetooth::commandChangeAngle(int angle, char valueUp, char valueDown, String value) {
    int aux = angle;
    if (value.indexOf(valueUp) != -1) {
        if (angle < degreeMax) {
            aux = angle + incrementDegree;
        }
    } else if (value.indexOf(valueDown) != -1) {
        if (angle > degreeMin) {
            aux = angle - incrementDegree;
        }
    }
    return aux;
}

void Bluetooth::commandInitializeMotor(String value) {
    if (value.indexOf(changeMinThrottle) != -1) {
        degreeThrottle = degreeMinThrottle;
    } else if (value.indexOf(changeMaxThrottle) != -1) {
        degreeThrottle = degreeMaxThrottle;
    }
}

String Bluetooth::waitForResponse() {
    String value = "";
    while (this->BT->available()) {
        value = this->BT->readString();
    }
    return value;
}

void Bluetooth::modifyThrottle(int value) {
    this->degreeThrottle = value;
}

void Bluetooth::modifyPitch(int value) {
    this->degreePitch = value;
}

void Bluetooth::modifyRoll(int value) {
    this->degreeRoll = value;
}

void Bluetooth::modifyYaw(int value) {
    this->degreeYaw = value;
}

float Bluetooth::getThrottle() {    //return Throttle in miliseconds
    float aux = (float) degreeThrottle / degreeMaxThrottle;
    float ms = aux * pulseMinMotor;
    return ms + pulseMinMotor;
}

int Bluetooth::getPitch() {
    return degreePitch;
}

int Bluetooth::getRoll() {
    return degreeRoll;
}

int Bluetooth::getYaw() {
    return degreeYaw;
}

void Bluetooth::showData() {
    Serial.print(degreeThrottle);
    Serial.print("\t");
    Serial.print(degreePitch);
    Serial.print("\t");
    Serial.print(degreeRoll);
    Serial.print("\t");
    Serial.println(degreeYaw);
}
