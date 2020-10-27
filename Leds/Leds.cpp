/*
 * Leds.cpp
 *
 *      Author: Juande Estrella
*/

#include "Leds.h"

		Leds::Leds(uint8_t pinWhite,uint8_t pinBlue,uint8_t pinRedFront,uint8_t pinGreenFront,uint8_t pinRedBack)
		{
			length  = 5;
			leds[0] = pinWhite;
			leds[1] = pinBlue;
			leds[2] = pinRedFront;
			leds[3] = pinGreenFront;
			leds[4] = pinRedBack;
		}

		void Leds::initialize(){
			for(int i = 0 ;i<length; i++){
				pinMode(leds[i], OUTPUT);
			}
		}

		void Leds::onLedWhite(){
			digitalWrite(leds[0], HIGH);
		}

		void Leds::onLedBlue(){
			digitalWrite(leds[1], HIGH);
		}

		void Leds::onLedFly(){
			digitalWrite(leds[3], HIGH);
			digitalWrite(leds[4], HIGH);

		}

		void Leds::onLedWarning(){
			digitalWrite(leds[2], HIGH);
			digitalWrite(leds[4], HIGH);
		}

		void Leds::offAllLeds(){
			for(int i = 0 ; i < length; i++){
				digitalWrite(leds[i], LOW);
			}
		}

		int Leds::size(){
					return length;
		}

		uint8_t Leds::getLed(int i){
			return leds[i];
		}

