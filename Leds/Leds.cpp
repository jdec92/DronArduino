/*
 * Leds.cpp
 *
 *      Author: Juande Estrella
*/

#include "Leds.h"

Leds::Leds() {
    length = 5;
    leds[0] = pinLedWhite;
    leds[1] = pinLedBlue;
    leds[2] = pinLedRedFront;
    leds[3] = pinLedGreenFront;
    leds[4] = pinLedRedBack;
}

void Leds::initialize() {
    for (int i = 0; i < length; i++) {
        pinMode(leds[i], OUTPUT);
    }
}

void Leds::onLedWhite() {
    digitalWrite(leds[0], HIGH);
}

void Leds::onLedBlue() {
    digitalWrite(leds[1], HIGH);
}

void Leds::onLedFly() {
    digitalWrite(leds[3], HIGH);
    digitalWrite(leds[4], HIGH);
}

void Leds::onLedWarning() {
    digitalWrite(leds[2], HIGH);
    digitalWrite(leds[4], HIGH);
}

void Leds::offAllLeds() {
    for (int i = 0; i < length; i++) {
        digitalWrite(leds[i], LOW);
    }
}
