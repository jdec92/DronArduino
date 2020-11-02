/*
 * ControlMode.h
 *
 *      Author: Juande Estrella
 */

#ifndef CONTROLMODE_CONTROLMODE_H_
#define CONTROLMODE_CONTROLMODE_H_

#include "../GlobalVariable.h"
#include "../Bluetooth/Bluetooth.h"
#include "../MPU6050/MPU_6050.h"
#include "../Leds/Leds.h"

struct ControlMode {
private:
    float pidAnglePitch, pidAngleRoll;
    float actionIntegralPitchW, actionIntegralRollW, actionIntegralYawW;
    float actionDerivativePitchW, actionDerivativeRollW, actionDerivativeYawW;
    float actionIntegralPitchAng, actionIntegralRollAng;
    float actionDerivativePitchAng, actionDerivativeRollAng;
    float outPidPitch, outPidRoll, outPidYaw;
    bool modeAcrobatic, modeUp, modeDown, modeAutomatic, isActiveMode;
    bool isFinishModeUP, isFinishModeDOWN;
    bool calculateAccelerometer, calculateGyroscope;
public:
    ControlMode();

    void PIDAnglee(Bluetooth bt, MPU6050 mpu);

    void PIDSpeed(Bluetooth bt, MPU6050 mpu);

    float calculatePID(int slogan, float readMPU, float &actionIntegral, float &actionDerivate,
                       float kp, float ki, float kd, float outLimit, float &outPID);

    void commandChangeMode(Bluetooth bt, String value);

    void actionModeUp(Bluetooth bt);

    void actionModeDown(Bluetooth bt, bool isLowBattery);

    void activateModeAcrobatic();

    void activateModeStable();

    void activateModeDown(Bluetooth bt);

    void activateModeUp(Bluetooth bt);

    void activateModeAutomatic(Bluetooth bt);

    void actionAutomatic(Bluetooth bt);

    void disabledAllMode();

    void onLedAccordingMode(Leds leds);

    bool isModeAcrobatic();

    bool isModeDown();

    bool isModeUp();

    bool isModeAutomatic();

    bool isFinishModeUp();

    bool isFinishModeDown();

    bool isCalculateAcelerometer();

    bool isCalculateGyroscope();

    float getPidPitch();

    float getPidRoll();

    float getPidYaw();

};

#endif /* CONTROLMODE_CONTROLMODE_H_ */
