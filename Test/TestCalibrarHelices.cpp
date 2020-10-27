/*
 * TestCalibrarHelices.cpp
 *
 *      Author: Juande Estrella
 *
 *
 *  Importante probrar primero sin helices.
 * Cuando estemos seguro, ir poniendo las helices que vamos a calibrar.
 *
 *
 *  Para X e Y tiene que estar en ±3m/s²
 *  Para Z tiene que estar entre ±5m/s² y ±15m/s²
 *
 * En Caso de querer calibrar las helices, descomentamos el texto y comentamos Main.cpp,recompilamos y subimos el programa
 *

#include "../Bluetooth/Bluetooth.h"
#include "../Motor/Motor.h"
#include "../MPU6050/MPU_6050.h"
#include "../Battery/Battery.h"

Battery bateria(pinBattery);

Bluetooth bluetooth(rxBluetooth,txBluetooth);

Motor motor1(pinM1);
Motor motor2(pinM2);
Motor motor3(pinM3);
Motor motor4(pinM4);

Leds leds(ledWhite, ledBlue, ledRedFront, ledGreenFront, ledRedBack);

MPU6050 mpu6050(0x68, sdaMPU, sclMPU, numberSampleMPU);


bool VisuAcc = true;
bool VisuAng = false;

void setup() {

	leds.initialize();

	if(!mpu6050.initialize()){
		Serial.println("ERROR no se inicia MPU");
	}
	if(bateria.isLowBattery()){
		Serial.println("Batería baja");
	}

	mpu6050.calculateOffset(VisuAng);

	bluetooth.initialize();

	Serial.begin(baudrates);

	motor1.initialize();
	motor2.initialize();
	motor3.initialize();
	motor4.initialize();

	Serial.println("Conectarse al dron y bajar throttle al minimo");
	while (bluetooth.getThrottle() > degreeMinThrottle){
	  bluetooth.updatePulse();
	}

	mpu6050.updateLoopTimer();
	Serial.println("Ya puede iniciar el archivo de matlab, para mostrar las graficas");

}


void loop() {

	if(VisuAng) {
		mpu6050.calculateAngle();
	}

	leds.offAllLeds();
	while (micros()- mpu6050.getLoopTimer() < 2000);
	leds.onLedBlue();
	while (micros() - mpu6050.getLoopTimer() < 4000);



	mpu6050.updateLoopTimer();

	motor1.updateValue(bluetooth.getThrottle());
	motor2.updateValue(bluetooth.getThrottle());
	motor3.updateValue(bluetooth.getThrottle());
	motor4.updateValue(bluetooth.getThrottle());

	bluetooth.updatePulse();

	if(bateria.isLowBattery()){
		Serial.println("Batería baja");
	}

	mpu6050.readSensors();
	mpu6050.processAccelerometer();

	mpu6050.show(true,false);
}


int main(){

	init();

	setup();

	while(true){
		loop();
	}

	return 0;
}
*/



