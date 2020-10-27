/*
* TestCalibrarMotores.cpp
*
*      Author: Juande Estrella
*
*  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*  !!!!!!!!!!!!!! RETIRAMOS LAS HELICES - SIN HELICES !!!!!!!!!!!!!!!!!!!!!!!!!!
*  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*
*  1) Sin conectar la bateria, alimentamos la placa Arduino con cable USB y cargamos este programa.
*  2) Ejecutamos el programa sin conectar la batería.
*  3) Cuando nos indique el Serial Plotter que nos conectemos al dron y subamos el throttle al maximo, lo realizamos
*  4) Cuando nos lo indique, conectamos la bateria.
*  5) El motor emitira unos pitidos (pi,pi), bajamos el throttle al minimo.
*  6) Escucharemos otros pitidos, pipipi... piiii.
*  7) Terminado, ya podemos manejar los motores.

#include "../Bluetooth/Bluetooth.h"
#include "../Motor/Motor.h"

Bluetooth bluetooth(rxBluetooth,txBluetooth);

Motor motor1(pinM1);
Motor motor2(pinM2);
Motor motor3(pinM3);
Motor motor4(pinM4);

float loop_timer;

void setup() {

	bluetooth.initialize();

	Serial.begin(baudrates);

	motor1.initialize();
	motor2.initialize();
	motor3.initialize();
	motor4.initialize();

	Serial.print("Conectarse al dron y subir throttle al maximo");
	while (bluetooth.getThrottle() < degreeMaxThrottle){
	  bluetooth.updatePulse();
	}

	Serial.println("Conectamos la Bateria");
	Serial.println("El motor emitira unos pitidos, Bajamos el Throttle al minimo");
	Serial.println("Cuando escuchemos varios pitidos, los motores estaran configurados");
	Serial.println("Ya podremos manejar los motores");
}

void loop() {
	while (micros() - loop_timer < usCiclo);
	loop_timer = micros();
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
