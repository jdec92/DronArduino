/*
 * Pixy.cpp
 *
 *      Author: Juande Estrella
 */
#include "ControlPixy.h"

void ControlPixy::initialize(){
	pixy.init();
	pixy.changeProg("line");
}

bool ControlPixy::isCompletedJourney(Bluetooth bt){
    bool stop = true;
	//avanzar
	if((pixy.line.vectors[0].m_x0>120)&&(pixy.line.vectors[0].m_x0<190)){
        stop = false;
	    advance(bt);
	}
	//girar a la derecha
	if((pixy.line.vectors[0].m_x0 < 120) && (pixy.line.vectors[0].m_x0 > 10)){
	    stop = false;
	    turnRight(bt);
	}

	//girar a la izquierda
	if((pixy.line.vectors[0].m_x0 > 190)){
	    stop = false;
		turnLeft(bt);
	}

	return stop;
}

void ControlPixy::advance(Bluetooth bt) {
    if(bt.getPitch() > degreeMin){
        bt.modifyPitch(bt.getPitch() - incrementDegree);
    }
}

void ControlPixy::turnLeft(Bluetooth bt) {
    if(bt.getYaw() < degreeMax){
        bt.modifyYaw(bt.getYaw() + incrementDegree);
    }
}

void ControlPixy::turnRight(Bluetooth bt) {
    if(bt.getYaw() > degreeMin){
        bt.modifyYaw(bt.getYaw() - incrementDegree);
    }
}

