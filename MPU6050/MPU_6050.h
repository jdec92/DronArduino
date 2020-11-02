/*
 * MPU_6050.h
 *
 *      Author: Juande Estrella
 */

#ifndef MPU_6050_H_
#define MPU_6050_H_

#include "../GlobalVariable.h"
#include "Axis.h"
#include <Wire.h>

struct MPU6050 {
private:
    int sampleNumber;
    float pitchAngle, rollAngle, temperature;
    long loopTimer, executionTime;
    bool init;
    uint8_t address, sda, scl;
    Axis gyroscope;
    Axis accelerometer;
    Axis offsetGyroscope;
    Axis offsetAccelerometer;

public:
    MPU6050(uint8_t address, uint8_t sda, uint8_t scl, int sampleNumber);

    bool initialize();

    void calculateOffset(bool isCalculateGyroscope, bool isCalculateAccelerometer);

    Axis recalibrateGyroscope();

    void readSensors();

    void processAccelerometer();

    void calculateAngle();

    void transmitData(uint8_t reg, uint8_t data);

    void transmitAndWaitResponse(uint8_t reg, int numberByte);

    void updateLoopTimer();

    void updateExecutionTimer(long loopTimer);

    float getPitchAngle();

    float getRollAngle();

    float getLoopTimer();

    float getExecutionTime();

    void show(bool showAccelerometre, bool showAnglee);
};


#endif /* MPU_6050_H_ */
