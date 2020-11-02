/*
 * Bluetooth.h
 *
 *      Author: Juande Estrella
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include "../GlobalVariable.h"
#include "SoftwareSerial.h"


struct Bluetooth {
private:
    SoftwareSerial *BT;
    int degreeThrottle, degreePitch, degreeRoll, degreeYaw;

public:
    Bluetooth(uint8_t rx, uint8_t tx);

    void initialize();

    void configure(String name, String pin);

    String updatePulse();

    void commandChangeThrottle(String value);

    int commandChangeAngle(int angle, char valueUp, char valueDown, String value);

    void commandInitializeMotor(String value);

    String waitForResponse();

    void modifyThrottle(int value);

    void modifyPitch(int value);

    void modifyRoll(int value);

    void modifyYaw(int value);

    float getThrottle();

    int getPitch();

    int getRoll();

    int getYaw();

    void showData();

};


#endif /* BLUETOOTH_BLUETOOTH_H_ */
