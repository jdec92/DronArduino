/*
 * ControlMode.cpp
 *
 *      Author: Juande Estrella
 */

#include "ControlMode.h"

ControlMode::ControlMode() {
    this->calculateAccelerometer = true;
    this->calculateGyroscope = true;
    this->modeAcrobatic = false;
    this->modeAutomatic = false;
    this->modeDown = false;
    this->modeUp = false;
    this->isActiveMode = false;
    this->isFinishModeUP = true;
    this->isFinishModeDOWN = true;
    this->pidAnglePitch = 0.0;
    this->pidAngleRoll = 0.0;
    this->actionIntegralPitchW = 0.0;
    this->actionIntegralRollW = 0.0;
    this->actionIntegralYawW = 0.0;
    this->actionDerivativePitchW = 0.0;
    this->actionDerivativeRollW = 0.0;
    this->actionDerivativeYawW = 0.0;
    this->actionIntegralPitchAng = 0.0;
    this->actionIntegralRollAng = 0.0;
    this->actionDerivativePitchAng = 0.0;
    this->actionDerivativeRollAng = 0.0;
    this->outPidPitch = 0.0;
    this->outPidRoll = 0.0;
    this->outPidYaw = 0.0;
}

void ControlMode::PIDAnglee(Bluetooth bt, MPU6050 mpu) {
    //============================================================================= PITCH GYRO ang
    float slogan = (float) bt.getPitch();
    calculatePID(slogan, mpu.getPitchAngle(), actionIntegralPitchAng, actionDerivativePitchAng, KpPitchAngle,
                 KiPitchAngle, KdPitchAngle, limitAngle, pidAnglePitch);

    //============================================================================= ROLL GYRO ang
    slogan = (float) bt.getRoll();
    calculatePID(slogan, mpu.getRollAngle(), actionIntegralRollAng, actionDerivativeRollAng, KpRollAngle,
                 KiRollAngle, KdRollAngle, limitAngle, pidAngleRoll);
}

void ControlMode::PIDSpeed(Bluetooth bt, MPU6050 mpu) {
    float inPitch, inRoll;

    if (modeAcrobatic) {
        inPitch = ((float) bt.getPitch()) / mpu.getExecutionTime();  //convert to m/s²
        inRoll = ((float) bt.getRoll()) / mpu.getExecutionTime();
    } else {
        inPitch = pidAnglePitch;
        inRoll = pidAngleRoll;
    }

    float inYaw = (float) bt.getYaw() / mpu.getExecutionTime();
    Axis recalibrateGyro = mpu.recalibrateGyroscope();
    recalibrateGyro.z = recalibrateGyro.z / LSBGyroscope;

    //============================================================================= PITCH GYRO w
    calculatePID(inPitch, recalibrateGyro.x, actionIntegralPitchW, actionDerivativePitchW, KpPitchW, KiPitchW, KdPitchW,
                 limitVelocity, outPidPitch);

    //============================================================================= ROLL GYRO w
    calculatePID(inRoll, recalibrateGyro.y, actionIntegralRollW, actionDerivativeRollW, KpRollW, KiRollW, KdRollW,
                 limitVelocity, outPidRoll);

    //=============================================================================== YAW GYRO w
    calculatePID(inYaw, recalibrateGyro.z, actionIntegralYawW, actionDerivativeYawW, KpYawW, KiYawW, KdYawW,
                 limitVelocity, outPidYaw);
}

float ControlMode::calculatePID(int slogan, float readMPU, float &actionIntegral, float &actionDerivate, float kp,
                                float ki, float kd, float outLimit, float &outPID) {

    float pidError = slogan - readMPU;

    actionIntegral += (ki * pidError);
    actionIntegral = constrain(actionIntegral, -outLimit, outLimit);

    float actionD = kd * (readMPU - actionDerivate);

    outPID = kp * pidError + actionIntegral - actionD;
    outPID = constrain(outPID, -outLimit, outLimit);

    actionDerivate = readMPU;
}

void ControlMode::actionModeUp(Bluetooth bt) {
    int throttle = bluetooth.getThrottle() + incrementThrottle;
    if (throttle <= degreeMaxUp) {
        bluetooth.modifyThrottle(throttle);
    } else {
        this->isFinishModeUP = true;
    }
}

void ControlMode::actionModeDown(Bluetooth bt) {
    int throttle = bluetooth.getThrottle() - incrementThrottle;
    if (throttle >= degreeMin) {
        bluetooth.modifyThrottle(throttle);
    } else {
        this->isFinishModeDOWN = true;
    }
}

void ControlMode::activateModeAcrobatic() {
    disabledAllMode();
    this->modeAcrobatic = true;

    this->calculateAccelerometer = false;
    this->calculateGyroscope = true;
}

void ControlMode::activateModeStable() {
    disabledAllMode();

    this->calculateAccelerometer = true;
    this->calculateGyroscope = true;
}

void ControlMode::activateModeDown(Bluetooth bt) {
    actionAutomatic(bt);
    this->modeDown = true;

    this->isFinishModeDOWN = false;
}

void ControlMode::activateModeUp(Bluetooth bt) {
    actionAutomatic(bt);
    this->modeUp = true;

    this->isFinishModeUP = false;
}

void ControlMode::activateModeAutomatic(Bluetooth bt) {
    actionAutomatic(bt);
    this->modeAutomatic = true;

    this->isFinishModeUP = false;
    this->isFinishModeDOWN = false;
}

void ControlMode::actionAutomatic(Bluetooth bt) {
    bt.modifyPitch(degreeMin + degreeMax / 2);
    bt.modifyRoll(degreeMin + degreeMax / 2);
    bt.modifyYaw(degreeMin + degreeMax / 2);

    disabledAllMode();

    this->calculateAccelerometer = true;
    this->calculateGyroscope = true;
}

void ControlMode::disabledAllMode() {
    this->isActiveMode = false;
    this->modeAutomatic = false;
    this->modeAcrobatic = false;
    this->modeDown = false;
    this->modeUp = false;
}

void ControlMode::onLedAccordingMode(Leds leds) {
    if (!isActiveMode) {
        if (modeAcrobatic) {
            leds.onLedBlue();
            leds.onLedFly();
        } else if (modeAutomatic) {
            leds.onLedWhite();
        } else if (modeUp) {
            leds.onLedBlue();
        } else if (modeDown) {
            leds.onLedWarning();
        } else {
            leds.onLedFly();
        }
        isActiveMode = true;
    }
}

bool ControlMode::isModeAcrobatic() {
    return this->modeAcrobatic;
}

bool ControlMode::isModeDown() {
    return this->modeDown;
}

bool ControlMode::isModeUp() {
    return this->modeUp;
}

bool ControlMode::isModeAutomatic() {
    return this->modeAutomatic;
}

bool ControlMode::isFinishModeUp() {
    return this->isFinishModeUP;
}

bool ControlMode::isFinishModeDown() {
    return this->isFinishModeDOWN;
}

bool ControlMode::isCalculateAcelerometer() {
    return this->calculateAccelerometer;
}

bool ControlMode::isCalculateGyroscope() {
    return this->calculateGyroscope;
}

float ControlMode::getPidPitch() {
    return this->outPidPitch;
}

float ControlMode::getPidRoll() {
    return this->outPidRoll;
}

float ControlMode::getPidYaw() {
    return this->outPidYaw;
}