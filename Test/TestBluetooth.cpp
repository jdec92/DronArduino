/*
 * TestBluetooth.cpp
 *
 *      Author: Juande Estrella
 *
 *
*  The posible baudrates are:
   AT+BAUD1-------1200
    AT+BAUD2-------2400
    AT+BAUD3-------4800
    AT+BAUD4-------9600 - Default for hc-06
    AT+BAUD5------19200
    AT+BAUD6------38400
    AT+BAUD7------57600 - Johnny-five speed
    AT+BAUD8-----115200
    AT+BAUD9-----230400
    AT+BAUDA-----460800
    AT+BAUDB-----921600
    AT+BAUDC----1382400
 *
 *
*En Caso de querer comprobar el funcionamiento del bluetooth, descomentamos el texto y comentamos Main.cpp,recompilamos y subimos el programa


#include "../Bluetooth/Bluetooth.h"

Bluetooth bluetooth(rxBluetooth,txBluetooth);

void setup() {

	bluetooth.initialize();

	Serial.begin(baudrates);

	Serial.println("Si no hemos configurado el Bluetooth");
	Serial.println("descomentar la siguiente linea");
	Serial.println("y cambiar en GlobalVariable.h -> baudrates a 9600");
	Serial.println("Ya que por defecto la velocidad HC-06 es 9600");
	//bluetooth.configure("DronJD", "0000");  //configuramos el nombre, el pin y si es necesario la velocidad del bluetooth HC-06

	Serial.println("Nos conectamos al Bluetooth");
}

void loop() {
	String value = bluetooth.waitForResponse();
	if(!value.equals("")){
		Serial.println("Valor leido: "+value);
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

*/

