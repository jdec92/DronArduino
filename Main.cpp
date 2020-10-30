/*
 *	Main.cpp
 *
 *
 *      Author: Juande Estrella
 *
 *  Importante probrar primero sin helices.
 * Cuando estemos seguro, ir poniendo las helices que vamos a calibrar.
 *
 *
 *
*/

#include "Motor/Motor.h"
#include "Battery/Battery.h"
#include "ControlMode/ControlMode.h"
#include "Camera/ControlPixy.h"

Battery bateria(pinBattery);

Bluetooth bluetooth(rxBluetooth,txBluetooth);

Motor motor1(pinM1);
Motor motor2(pinM2);
Motor motor3(pinM3);
Motor motor4(pinM4);

Leds leds;

MPU6050 mpu6050(0x68, sdaMPU, sclMPU, numberSampleMPU);

ControlPixy pixy;

ControlMode controlMode;

void setup() {
	Serial.begin(baudrates);

	bluetooth.initialize();

	leds.initialize();

	leds.offAllLeds();
	leds.onLedBlue();

	if(!mpu6050.initialize() || bateria.isLowBattery()){
		leds.offAllLeds();
		leds.onLedWarning();
		Serial.println("ERROR no se inicia MPU o la Bater�a esta baja");
		while (true)delay(10);
	}

	pixy.initialize();

	mpu6050.calculateOffset(controlMode.isCalculateGyroscope(),controlMode.isCalculateAcelerometer());

	motor1.initialize();
	motor2.initialize();
	motor3.initialize();
	motor4.initialize();

	Serial.print("Conectarse al dron y bajar throttle al minimo");
	while (bluetooth.getThrottle() != pulseMinMotor){
	  bluetooth.updatePulse();
	}

	Serial.println("OK");
	leds.offAllLeds();
	mpu6050.updateLoopTimer();
	mpu6050.updateExecutionTimer(mpu6050.getLoopTimer());
}


void loop() {
	controlMode.onLedAccordingMode(leds);

	while (micros() - mpu6050.getLoopTimer() < usCiclo);
	mpu6050.updateExecutionTimer(mpu6050.getLoopTimer());
	mpu6050.updateLoopTimer();

	mpu6050.readSensors();
	if(controlMode.isCalculateAcelerometer()){
		mpu6050.processAccelerometer();
	}

	if(controlMode.isCalculateGyroscope()) {
			mpu6050.calculateAngle();
	}

	if(!controlMode.isModeAcrobatic()){
		controlMode.PIDAnglee(bluetooth, mpu6050);
	}
	controlMode.PIDSpeed(bluetooth, mpu6050);

	motor1.controller(bluetooth.getThrottle(), controlMode.getPidPitch(),-controlMode.getPidRoll(), -controlMode.getPidYaw());
	motor2.controller(bluetooth.getThrottle(), controlMode.getPidPitch(), controlMode.getPidRoll(), controlMode.getPidYaw());
	motor3.controller(bluetooth.getThrottle(), -controlMode.getPidPitch(), controlMode.getPidRoll(), -controlMode.getPidYaw());
	motor4.controller(bluetooth.getThrottle(), -controlMode.getPidPitch(),-controlMode.getPidRoll(), controlMode.getPidYaw());

	if(!controlMode.isModeUp() && !controlMode.isModeDown() && !controlMode.isModeAutomatic()){
		bluetooth.updatePulse();
	}

	if(controlMode.isModeUp()){
	    if(controlMode.isFinishModeUp()){
	        leds.offAllLeds();
			controlMode.activateModeStable();
		}else{
	        controlMode.actionModeUp(bluetooth);
	    }
	}

	if(controlMode.isModeDown() || bateria.isLowBattery()){
	    if(controlMode.isFinishModeDown()){
            leds.offAllLeds();
            controlMode.activateModeStable();
		}else{
	        controlMode.actionModeDown(bluetooth);
	    }
	}

	if(controlMode.isModeAutomatic()){
	    if(controlMode.isFinishModeUp()){  //Comprobamos si esta a la altura correcta el dron
            if(pixy.isCompletedJourney(bluetooth)){  //Comprobamos si hemos recorrido el trayecto completo
                if(controlMode.isFinishModeDown()){ //Comprobamos si hemos atarrezidao el dron
                    leds.offAllLeds();
                    controlMode.activateModeStable();
                }else{                  //Si no hemos aterrizado aterriza el dron
                    controlMode.actionModeDown(bluetooth);
                }
            }
	    }else{    //Si no hemos despegado o estamos a media altura despega hasta la altura indicada
	        controlMode.actionModeUp(bluetooth);
	    }
	}
}


int main(){

	init();

	setup();

	while(true){
		loop();
	}

	return 0;
}

