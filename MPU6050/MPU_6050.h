/*
 * MPU_6050.h
 *
 *      Author: Juande Estrella
 */

#ifndef MPU_6050_H_
#define MPU_6050_H_


/*
 * MPU_6050.cpp
 *
 *  Created on: 16 sept. 2020
 *      Author: JD
 */
#include "../GlobalVariable.h"
#include "Axis.h"
#include <Wire.h>

struct MPU6050 {
	private:
		int sampleNumber;
		float pitchAngle,rollAngle,temperature;
		long loopTimer,executionTime;
		bool init;
		uint8_t address,sda,scl;
		Axis gyroscope;
		Axis accelerometer;
		Axis offsetGyroscope;
		Axis offsetAccelerometer;

	public:
		MPU6050(uint8_t address,uint8_t sda,uint8_t scl, int sampleNumber);
		bool initialize();

		void calculateOffset(bool calculateGyroscope,bool calculateAccelerometer);

		Axis recalibrateGyroscope();

		void readSensors();
		void processAccelerometer();
		void calculateAngle();

		void transmitData(uint8_t reg,uint8_t data);
		void transmitAndWaitResponse(uint8_t reg,int numberByte);

		void updateLoopTimer();
		void updateExecutionTimer(long loopTimer);

		float getExecutionTime();
		float getLoopTimer();
		float getPitchAngle();
		float getRollAngle();

		void show(bool showAccelerometre,bool showAnglee);
};


#endif /* MPU_6050_H_ */
