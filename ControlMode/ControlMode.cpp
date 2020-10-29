/*
 * ControlMode.cpp
 *
 *      Author: Juande Estrella
 */

#include "ControlMode.h"

ControlMode::ControlMode(){
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

void ControlMode::PIDAnglee(Bluetooth bt,MPU6050 mpu){
	  //============================================================================= PITCH GYRO ang
	  float pidErrorPitch = bt.getPitch() - mpu.getPitchAngle();                                           // error entre lectura y consigna

	  actionIntegralPitchAng += (KiPitchAngle * pidErrorPitch);                                       // Parte integral (sumatorio del error en el tiempo)
	  actionIntegralPitchAng = constrain(actionIntegralPitchAng, -limitAngle, limitAngle); // Limitar parte integral

	  pidAnglePitch = calculateOutPID(pidErrorPitch, mpu.getPitchAngle(), KpPitchAngle, actionIntegralPitchAng, KdPitchAngle, actionDerivativePitchAng,limitAngle);
	  actionDerivativePitchAng = mpu.getPitchAngle();                                                   // Error anterior
	  //============================================================================= ROLL GYRO ang
	  float pidErrorRoll =  bt.getRoll() - mpu.getRollAngle();
	  actionIntegralRollAng += (KiRollAngle * pidErrorRoll);
	  actionIntegralRollAng = constrain(actionIntegralRollAng, -limitAngle, limitAngle);

	  pidAngleRoll = calculateOutPID(pidErrorRoll, mpu.getRollAngle(), KpRollAngle, actionIntegralRollAng, KdRollAngle, actionDerivativeRollAng, limitAngle);
	  actionDerivativeRollAng = mpu.getRollAngle();
}

void ControlMode::PIDSpeed(Bluetooth bt, MPU6050 mpu){
	float inPitch,inRoll;
	float pidErrorPitch,pidErrorRoll,pidErrorYaw;
	 if (modeAcrobatic) {
	    inPitch = ((float) bt.getPitch()) / mpu.getExecutionTime();
	    inRoll = ((float) bt.getRoll()) / mpu.getExecutionTime();

	  }else {
	    inPitch = pidAnglePitch;
	    inRoll = pidAngleRoll;
	  }

	  Axis recalibrateGyro = mpu.recalibrateGyroscope();
	  recalibrateGyro.z = recalibrateGyro.z / LSBGyroscope;

	  float convert = (float) bt.getYaw() / mpu.getExecutionTime();
	  //============================================================================= PITCH GYRO w
	  pidErrorPitch = inPitch -  recalibrateGyro.x;
	  actionIntegralPitchW += (KiPitchW * pidErrorPitch);
	  actionIntegralPitchW = constrain(actionIntegralPitchW, -limitVelocity, limitVelocity);

	  outPidPitch =calculateOutPID(pidErrorPitch, recalibrateGyro.x,KpPitchW,actionIntegralPitchW,KdPitchW,actionDerivativePitchW,limitVelocity);
	  actionDerivativePitchW = recalibrateGyro.x;
	  //============================================================================= ROLL GYRO w
	  pidErrorRoll = inRoll - recalibrateGyro.y;
	  actionIntegralRollW += (KiRollW * pidErrorRoll);
	  actionIntegralRollW = constrain(actionIntegralRollW, -limitVelocity, limitVelocity);

	  outPidRoll =calculateOutPID(pidErrorRoll, recalibrateGyro.y,KpRollW,actionIntegralRollW,KdRollW,actionDerivativeRollW,limitVelocity);
	  actionDerivativeRollW =  recalibrateGyro.y;
	  //=============================================================================== YAW GYRO w
	  pidErrorYaw = convert - recalibrateGyro.z;
	  actionIntegralYawW += (KiYawW * pidErrorYaw);
	  actionIntegralYawW = constrain(actionIntegralYawW, -limitVelocity, limitVelocity);

	  outPidYaw =calculateOutPID(pidErrorYaw, recalibrateGyro.z,KpYawW,actionIntegralYawW,KdYawW,actionDerivativeYawW,limitVelocity);
	  actionDerivativeYawW = recalibrateGyro.z;
}

void ControlMode::actionModeUp(Bluetooth bt) {
    int throttle = bluetooth.getThrottle() + incrementThrottle;
    if(throttle <= degreeMaxUp){
        bluetooth.modifyThrottle(throttle);
    }else{
        this->isFinishModeUP = true;
    }
}

void ControlMode::actionModeDown(Bluetooth bt) {
    int throttle = bluetooth.getThrottle() - incrementThrottle;
    if(throttle >= degreeMin){
        bluetooth.modifyThrottle(throttle);
    }else{
        this->isFinishModeDOWN = true;
    }
}

float ControlMode::calculateOutPID(float pidError,float valueGiroscope,float kp,float actionIntegral,float kd,float actionDerivate,int limit){
		  float actionD = kd * (valueGiroscope - actionDerivate);
		  float outPID = kp * pidError + actionIntegral - actionD;
		  return constrain(outPID, -limit, limit);
}

void ControlMode::activateModeAcrobatic(){
	this->isActiveMode = false;
	this->modeAcrobatic = true;
	this->modeAutomatic = false;
	this->modeDown = false;
	this->modeUp = false;
	this->calculateAccelerometer = false;
	this->calculateGyroscope = true;

}

void ControlMode::activateModeStable(){
	this->isActiveMode = false;
	this->modeAcrobatic = false;
	this->modeAutomatic = false;
	this->modeDown = false;
	this->modeUp = false;
	this->calculateAccelerometer =true;
	this->calculateGyroscope = true;
}

void ControlMode::activateModeUp(Bluetooth bt){
	bt.modifyPitch(degreeMin + degreeMax / 2);
	bt.modifyRoll(degreeMin + degreeMax / 2);
	bt.modifyYaw(degreeMin + degreeMax / 2);
	this->isActiveMode = false;
	this->modeAcrobatic = false;
	this->modeAutomatic = false;
	this->modeDown = false;
	this->modeUp = true;
	this->calculateAccelerometer = true;
	this->calculateGyroscope = true;
	this->isFinishModeUP = false;
}

void ControlMode::activateModeDown(Bluetooth bt){
	bt.modifyPitch(degreeMin + degreeMax / 2);
	bt.modifyRoll(degreeMin + degreeMax / 2);
	bt.modifyYaw(degreeMin + degreeMax / 2);
	this->isActiveMode = false;
	this->modeAcrobatic = false;
	this->modeAutomatic = false;
	this->modeDown = true;
	this->modeUp = false;
	this->calculateAccelerometer =true;
	this->calculateGyroscope = true;
	this->isFinishModeDOWN = false;
}

void ControlMode::activateModeAutomatic(Bluetooth bt){
	bt.modifyPitch(degreeMin + degreeMax / 2);
	bt.modifyRoll(degreeMin + degreeMax / 2);
	bt.modifyYaw(degreeMin + degreeMax / 2);
	this->isActiveMode = false;
	this->modeAcrobatic = false;
	this->modeAutomatic = true;
	this->modeDown = false;
	this->modeUp = false;
	this->calculateAccelerometer =true;
	this->calculateGyroscope = true;
	this->isFinishModeUP = false;
	this->isFinishModeDOWN = false;
}

void ControlMode::onLedAccordingMode(Leds leds){
	if(!isActiveMode){
		if(modeAcrobatic){
			leds.onLedBlue();
			leds.onLedFly();
		}else if(modeAutomatic){
			leds.onLedWhite();
		}else if(modeUp){
			leds.onLedBlue();
		}else if(modeDown){
			leds.onLedWarning();
		}else{
			leds.onLedFly();
		}
		isActiveMode = true;
	}
}

bool ControlMode::isModeAcrobatic(){
	return this->modeAcrobatic;
}

bool ControlMode::isModeDown(){
	return this->modeDown;
}

bool ControlMode::isModeUp(){
	return this->modeUp;
}

bool ControlMode::isModeAutomatic(){
	return this->modeAutomatic;
}

bool ControlMode::isFinishModeUp() {
    return this->isFinishModeUP;
}

bool ControlMode::isFinishModeDown() {
    return this->isFinishModeDOWN;
}

bool ControlMode::isCalculateAcelerometer(){
	return this->calculateAccelerometer;
}

bool ControlMode::isCalculateGyroscope(){
	return this->calculateGyroscope;
}

float ControlMode::getPidPitch(){
	return this->outPidPitch;
}

float ControlMode::getPidRoll(){
	return this->outPidRoll;
}

float ControlMode::getPidYaw(){
	return this->outPidYaw;
}
