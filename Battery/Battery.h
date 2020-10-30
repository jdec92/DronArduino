/*
 * Battery.h
 *
 *      Author: Juande Estrella
 */

#ifndef BATTERY_H_
#define BATTERY_H_

#include "../GlobalVariable.h"

struct Battery {
private:
    uint8_t pin;
    float voltage;
public:
    Battery(uint8_t pin);

    int readSensor();

    void updateVoltage(int voltage);

    float getVoltage();

    bool isLowBattery();
};


#endif /* BATTERY_BATTERY_H_ */
