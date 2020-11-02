/*
 * GlobalVariable.h
 *
 *      Author: Juande Estrella
 *
 *
 */

#ifndef GLOBALVARIABLE_H_
#define GLOBALVARIABLE_H_

#include "Arduino.h"

//===========Global Variables================
#define usCiclo 6000  //ciclo del programa
#define baudrates 115200  //frecuencia del programa
#define pi 3.1416

//================Leds======================
#define pinLedWhite 9
#define pinLedBlue 10
#define pinLedRedFront 11
#define pinLedGreenFront 12
#define pinLedRedBack 13

//================Battery======================
#define pinBattery A0
#define offsetVoltage 5.00
#define voltageMinBattery 10.5

//===============MPU6050=================
//----------PIN-------------
#define sdaMPU A4
#define sclMPU A5
//--------------------

#define sensibilityAcc 0x10
#define LSBAccelerometer 4096
#define sensibilityGyr 0x08
#define LSBGyroscope 65.5
#define numberSampleMPU 3000

//===============Bluetooth=================
//-------- PIN------------
#define rxBluetooth 3
#define txBluetooth 2

//----------Control----------------
#define degreeMinThrottle 0
#define degreeMaxThrottle 180
#define degreeMin -30
#define degreeMax 30
#define incrementThrottle 5
#define incrementDegree 1

//-----------Value Accion---------------
#define throttleUp 'A'
#define throttleDown 'B'
#define pitchUp 'P'
#define pitchDown 'H'
#define rollUp 'R'
#define rollDown 'L'
#define yawUp 'W'
#define yawDown 'Y'

#define changeMaxThrottle 'q'
#define changeMinThrottle 'a'
#define valueModeUp 'z'
#define valueModeDown 'x'
#define valueModeAcrobatic 'c'
#define valueModeStable 'v'
#define valueModeAutomatic 'p'

//===============Motores=================
#define pinM1 4
#define pinM2 5
#define pinM3 6
#define pinM4 7
#define pulseMinMotor 1000    // pulsos de 1000us a 2000 us  980
#define pulseMaxMotor 2100

#define degreeMaxUp 50

//===============PID=================
#define limitVelocity 380.0
#define limitAngle 130.0

#define KpPitchAngle 2.2
#define KiPitchAngle 0.06
#define KdPitchAngle 15

#define KpPitchW 1.9
#define KiPitchW 0.07
#define KdPitchW 2.2

#define KpRollAngle 2.2
#define KiRollAngle 0.06
#define KdRollAngle 15

#define KpRollW 1.9
#define KiRollW 0.07
#define KdRollW 2.0

#define KpYawW 1.5
#define KiYawW 0.05
#define KdYawW 1

#endif /* GLOBALVARIABLE_H_ */
