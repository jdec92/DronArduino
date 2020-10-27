/*
 * Pixy.cpp
 *
 *      Author: Juande Estrella
 */
#include "ControlPixy.h"

void ControlPixy::initialize(){
	pixy.init();
}

void ControlPixy::updateBlocks(){
	for (int i=0; i<pixy.line.numVectors; i++){
	    pixy.line.vectors[i].print();
	  }
}

void ControlPixy::updatePulse(Bluetooth bt){
	//avanzar
	if((pixy.line.vectors[0].m_x0>120)&&(pixy.line.vectors[0].m_x0<190)){
		if(bt.getPitch() > degreeMin){
			bt.modifyPitch(bt.getPitch() - incrementDegree);
		}
	}

	//girar a la derecha
	if((pixy.line.vectors[0].m_x0 < 120) && (pixy.line.vectors[0].m_x0 > 10)){
		if(bt.getYaw() > degreeMin){
			bt.modifyYaw(bt.getYaw() - incrementDegree);
		}
	}

	//girar a la izquierda
	if((pixy.line.vectors[0].m_x0 > 190)){
		if(bt.getYaw() < degreeMax){
			bt.modifyYaw(bt.getYaw() + incrementDegree);
		}
	}
}

