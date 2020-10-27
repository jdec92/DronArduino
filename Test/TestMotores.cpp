/*
 * TestMotores.cpp

 *
 *      Author: Juande Estrella
 *
 *
*  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*  !!!!!!!!!!!!!! RETIRAMOS LAS HELICES - SIN HELICES !!!!!!!!!!!!!!!!!!!!!!!!!!
*  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*	Una vez que tenemos calibrado los motores
*
*	1) Conectad la batería y cerrad el interruptor para alimentar la placa Arduino (esta vez no conectaremos el cable USB).
*    Bajamos el throttle al mínimo y ejecutamos el programa.
*   2) Cuando el led 13 se encienda, encendemos el mando.
*   3) Los motores emitiran unos pitidos pipipi... piiiii.
*   4) Listo, podemos empezar a girar los motores con el movil.

#include "../Bluetooth/Bluetooth.h"
#include "../Leds/Leds.h"
#include "../Motor/Motor.h"
#include "../MPU6050/MPU_6050.h"

long tiempo_ejecucion,escLoopTimer;

Bluetooth bluetooth(rxBluetooth,txBluetooth);

Leds leds(ledWhite,ledBlue,ledRedFront,ledGreenFront,ledRedBack);

Motor motor1(pinM1);
Motor motor2(pinM2);
Motor motor3(pinM3);
Motor motor4(pinM4);

MPU6050 mpu6050(0x68, sdaMPU, sclMPU,numberSampleMPU);

void setup() {

	bluetooth.initialize();

	Serial.begin(baudrates);

	motor1.initialize();
	motor2.initialize();
	motor3.initialize();
	motor4.initialize();

	mpu6050.initialize();

	leds.initialize();
	leds.onLedBlue();

	Serial.print("Conectar mando y bajar throttle al minimo");
    while (bluetooth.getThrottle() > degreeMinThrottle){
	     bluetooth.updatePulse();
	}


	mpu6050.updateLoopTimer();
	leds.offAllLeds();
	Serial.println("Starting config");
}

void loop() {

	while (micros() - mpu6050.getLoopTimer() < usCiclo );
	tiempo_ejecucion = (micros() - mpu6050.getLoopTimer()) / 1000;
	mpu6050.updateLoopTimer();

	leds.onLedFly();

	motor1.updateValue(bluetooth.getThrottle());
	motor2.updateValue(bluetooth.getThrottle());
	motor3.updateValue(bluetooth.getThrottle());
	motor4.updateValue(bluetooth.getThrottle());

	bluetooth.updatePulse();
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


