/*
 * MPU_6050.cpp
 *
 *      Author: Juande Estrella
 */

#include "MPU_6050.h"

		MPU6050::MPU6050(uint8_t address,uint8_t sda,uint8_t scl, int sampleNumber){
			this->address = address;
			this->sda = sda;
			this->scl = scl;
			this->sampleNumber = sampleNumber;
			this->temperature = 0.0;
			this->pitchAngle = 0.0;
			this->rollAngle = 0.0;
			this->loopTimer = 0;
			this->executionTime = 0;
			this->init = false;
		}

		bool MPU6050::initialize(){
			bool start = true;
			Wire.begin();
			this->transmitData(0x6B,0x00);
			// PWR_MGMT_1 registro 6B hex	-	00000000 para activar
			this->transmitData(0x1B,sensibilityGyr);
			  // GYRO_CONFIG registro 1B hex - 00001000: 500dps sensibilidad giroscopio
			this->transmitData(0x1C,sensibilityAcc);
			 // ACCEL_CONFIG registro 1C hex - 00010000: +/- 8g sensibilidad acelerometro

			//Test de prueba
			transmitAndWaitResponse(0x1B, 1);
			if (Wire.read() != 0x08) {
				start = false;
			    Serial.println("MPU ERROR");
			}

			this->transmitData(0x1A, 0x04);
			 // LPF (filtro paso bajos) registro 1A hex  -  256Hz(0ms):0x00 - 188Hz(2ms):0x01 - 98Hz(3ms):0x02 - 42Hz(4.9ms):0x03 - 20Hz(8.5ms):0x04 - 10Hz(13.8ms):0x05 - 5Hz(19ms):0x06
			return start;
		}

		void MPU6050::calculateOffset(bool calculateGyroscope,bool calculateAccelerometer){
			Axis sumAcc;
			Axis sumGy;
			for (int i = 0; i < sampleNumber ; i++) {
				this->readSensors();            // Leemos los datos del MPU6050 3000 veces y sacamos el valor medio para obtener los offset
				if(calculateAccelerometer){
					sumAcc.x += this->accelerometer.x;
					sumAcc.y+= this->accelerometer.y;
					sumAcc.z += this->accelerometer.z;
				}
				if(calculateGyroscope){
					sumGy.x += this->gyroscope.x;
					sumGy.y += this->gyroscope.y;
					sumGy.z += this->gyroscope.z;
				}
				delayMicroseconds(1000);
			}
			this->offsetAccelerometer.x = sumAcc.x / sampleNumber;
			this->offsetAccelerometer.y = sumAcc.y / sampleNumber;
			this->offsetAccelerometer.z = sumAcc.z / sampleNumber;
			this->offsetGyroscope.x = sumGy.x / sampleNumber;
			this->offsetGyroscope.y = sumGy.y / sampleNumber;
			this->offsetGyroscope.z = sumGy.z / sampleNumber;
		}

		void MPU6050::readSensors() {
		  transmitAndWaitResponse(0x3B, 14);  //Pedimos 14 bytes

		  this->accelerometer.x = Wire.read() << 8 | Wire.read();
		  this->accelerometer.y = Wire.read() << 8 | Wire.read();
		  this->accelerometer.z = Wire.read() << 8 | Wire.read();
		  this->temperature = Wire.read() << 8 | Wire.read();
		  this->gyroscope.x = Wire.read() << 8 | Wire.read();
		  this->gyroscope.y = Wire.read() << 8 | Wire.read();
		  this->gyroscope.z = Wire.read() << 8 | Wire.read();
		}

		void MPU6050::processAccelerometer(){
			//-----------------------------------------------Correción valores - offset
			accelerometer.x -= offsetAccelerometer.x;
			accelerometer.y -= offsetAccelerometer.y;
			accelerometer.z -= offsetAccelerometer.z;
			accelerometer.z = accelerometer.z + LSBAccelerometer;
		}

		void MPU6050::calculateAngle() {
			Axis recalibrateG = recalibrateGyroscope();
			//-------------------------------------------------------- Calculo del Angulo

			pitchAngle += recalibrateG.x * executionTime / 1000 ;
		    rollAngle += recalibrateG.y * executionTime / 1000 ;

			pitchAngle += rollAngle * sin(recalibrateG.z * executionTime * (pi/(180*LSBGyroscope*1000)));      // tiempo_ejecucion /1000 /65.5 * PI/180
			rollAngle -= pitchAngle * sin(recalibrateG.z * executionTime * (pi/(180*LSBGyroscope*1000)));  //(pi/(180*LSBGyroscope*1000)

			float totalVector = sqrt(pow(accelerometer.x, 2) + pow(accelerometer.y, 2) + pow(accelerometer.z, 2));
			float pitchAngleAcc = asin((float)accelerometer.y / totalVector) * 57.2958;     // 57.2958 = Conversion de radianes a grados 180/PI
			float rollAngleAcc = asin((float)accelerometer.x / totalVector) * -57.2958;

			if(init){
				pitchAngle = pitchAngle * 0.99 + pitchAngleAcc * 0.01;      // Corregimos en drift con filtro complementario
				rollAngle = rollAngle * 0.99 + rollAngleAcc * 0.01;
			}else{
				pitchAngle = pitchAngleAcc;
				rollAngle = rollAngleAcc;
				init = true;
			}
		}

		Axis MPU6050::recalibrateGyroscope(){
			Axis recalibrate;
			recalibrate.x = (gyroscope.x - offsetGyroscope.x) / LSBGyroscope;
			recalibrate.y = (gyroscope.y - offsetGyroscope.y) / LSBGyroscope;
			recalibrate.z = (gyroscope.z - offsetGyroscope.z);
			return recalibrate;
		}

		void MPU6050::updateLoopTimer(){
					this->loopTimer = micros();
		}

		void MPU6050::updateExecutionTimer(long loopTimer){
				executionTime = (micros() - loopTimer) / 1000;
		}

		void MPU6050::transmitData(uint8_t reg,uint8_t data){
					Wire.beginTransmission(address);
					Wire.write(reg);
					Wire.write(data);
					Wire.endTransmission();
		}

		void MPU6050::transmitAndWaitResponse(uint8_t data, int numberByte){
			Wire.beginTransmission(address);
			Wire.write(data);
			Wire.endTransmission();
			Wire.requestFrom(address, numberByte); // Pedimos n numero de bytes // @suppress("Ambiguous problem")
			while (Wire.available() < numberByte);
		}


		float MPU6050::getPitchAngle(){
			return this->pitchAngle;
		}

		float MPU6050::getRollAngle(){
			return this->rollAngle;
		}

		float MPU6050::getLoopTimer(){
			return this->loopTimer;
		}

		float MPU6050::getExecutionTime(){
			return executionTime;
		}

		void MPU6050::show(bool showAccelerometre,bool showAngle){
			if (showAccelerometre) {
				    Serial.print(9.8 * accelerometer.x / LSBAccelerometer);  // Aceleracion en 'm/s2'
				    Serial.print("\t");
				    Serial.print(9.8 * accelerometer.y / LSBAccelerometer); // Aceleracion en 'm/s2'
				    Serial.print("\t");
				    Serial.println(9.8 * accelerometer.z / LSBAccelerometer); // Aceleracion en 'm/s2'
			}

			if (showAngle) {
				    Serial.print(pitchAngle);
				    Serial.print("\t");
				    Serial.println(rollAngle);
			}

		}




