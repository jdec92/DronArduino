/*
 * TestMPU6050.cpp
 *
 *
 *      Author: Juande Estrella
 *
 *
 */
/* En primer lugar descomentar lector i2C, setupLector y loopLector para conocer la dirección del MPU6050
 * En segundo lugar comentar lo anterior descomentado y utilizar el setup y loop, para configurar y leer los datos del MPU6050


#include "../MPU6050/MPU_6050.h"

MPU6050 mpu6050(0x68, sdaMPU, sclMPU,numberSampleMPU);

void setup() {

	mpu6050.initialize();

	Serial.begin(baudrates);

	Serial.println("Importante, no mover calculando el offset del giroscopio y acelerometro");
	mpu6050.calculateOffset(true,true);

	mpu6050.updateLoopTimer();
	Serial.println("Ya se puede mover el dron");
}

void loop() {

	while (micros() - mpu6050.getLoopTimer() < usCiclo);
	mpu6050.updateExecutionTimer(mpu6050.getLoopTimer());
	mpu6050.updateLoopTimer();

	mpu6050.readSensors();
	mpu6050.processAccelerometer();
	mpu6050.calculateAngle();

	mpu6050.show(true, true);



}

void setupLector(){
	Wire.begin();
	Serial.begin(baudrates);
	Serial.println("I2C Scanner");
}


void loopLector(){
	  byte error, address;
	  int nDevices;

	  Serial.println("Scanning...");
	  nDevices = 0;
	  for(address = 1; address < 127; address++ )
	  {
	    Wire.beginTransmission(address);
	    error = Wire.endTransmission();
	    if (error == 0)
	    {
	      Serial.print("I2C device found at address 0x");
	      if (address<16)
	        Serial.print("0");
	      Serial.print(address,HEX);
	      Serial.println("  !");

	      nDevices++;
	    }
	    else if (error==4)
	    {
	      Serial.print("Unknow error at address 0x");
	      if (address<16)
	        Serial.print("0");
	      Serial.println(address,HEX);
	    }
	  }
	  if (nDevices == 0)
	    Serial.println("No I2C devices found\n");
	  else
	    Serial.println("done\n");
	  delay(5000);           // wait 5 seconds for next scan
}

int main(){

	init();

//	setupLector();
	setup();

	while(true){
	//	loopLector();
		loop();
	}

	return 0;
}


*/
