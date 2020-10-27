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

struct ControlPixy{
	private:
		Pixy2 pixy;
	public:
		void initialize();
		void updateBlocks();
		void updatePulse(Bluetooth bt);

};

#endif /* CAMERA_PIXY_C_ */
