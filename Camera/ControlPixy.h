/*
 * Pixy.c
 *
 *      Author: Juande Estrella
 */

#ifndef CAMERA_PIXY_C_
#define CAMERA_PIXY_C_

#include "../GlobalVariable.h"
#include "Pixy2.h"
#include "../Bluetooth/Bluetooth.h"

struct ControlPixy {
private:
    Pixy2 pixy;
public:
    void initialize();

    bool isCompletedJourney(Bluetooth bt);

    void advance(Bluetooth bt);

    void turnRight(Bluetooth bt);

    void turnLeft(Bluetooth bt);


};

#endif /* CAMERA_PIXY_C_ */
