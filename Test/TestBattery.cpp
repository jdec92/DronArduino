/*
 * TestBattery.cpp
 *
 *      Author: Juande Estrella
 *
*En Caso de querer comprobar el funcionamiento del divisor, descomentamos el texto y comentamos Main.cpp,recompilamos y subimos el programa


#include "../Battery/Battery.h"

Battery battery(pinBattery);

void setup() {
	Serial.begin(baudrates);
	Serial.println("Medimos con el polimero la bateria, y comprobamos si es el mismo voltaje");
}

void loop() {
	battery.updateVoltage(battery.readSensor());

	Serial.print("Voltaje -> ");
	Serial.println(battery.getVoltage());
	delay(1000);
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

