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
		float pidAnglePitch,pidAngleRoll;
		float actionIntegralPitchW,actionIntegralRollW,actionIntegralYawW;
		float actionDerivativePitchW,actionDerivativeRollW,actionDerivativeYawW;
		float actionIntegralPitchAng,actionIntegralRollAng;
		float actionDerivativePitchAng,actionDerivativeRollAng;
		float outPidPitch,outPidRoll,outPidYaw;
		bool modeAcrobatic,modeUp,modeDown,modeAutomatic,isActiveMode;
		bool isFinishModeUP,isFinishModeDOWN;
		bool calculateAccelerometer;
		bool calculateGyroscope;
	public:
		ControlMode();
		void PIDSpeed(Bluetooth bt,MPU6050 mpu);
		void PIDAnglee(Bluetooth bt,MPU6050 mpu);
		void activateModeAcrobatic();
		void activateModeStable();
		void activateModeDown(Bluetooth bt);
		void activateModeUp(Bluetooth bt);
		void activateModeAutomatic(Bluetooth bt);
		void onLedAccordingMode(Leds leds);

		void actionModeUp(Bluetooth bt);
		void actionModeDown(Bluetooth bt);

		float calculateOutPID(float pidError,float valueGiroscope,float kp,float actionIntegral,float kd,float actionDerivate,int limit);

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
