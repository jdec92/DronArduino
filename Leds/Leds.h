/*
 * Leds.h
 *
 *      Author: Juande Estrella
*/

#ifndef LEDS_H_
#define LEDS_H_

#include "../GlobalVariable.h"

struct Leds {
private:
    int length;
    uint8_t leds[5];

public:
    Leds();

    void initialize();

    void onLedWhite();

    void onLedBlue();

    void onLedFly();

    void onLedWarning();

    void offAllLeds();
};

#endif
