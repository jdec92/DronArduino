/*
 * Leds.h
 *
 *      Author: Juande Estrella
*/

#ifndef LEDS_H_
#define LEDS_H_

#include "../GlobalVariable.h"

struct Leds{
	private:
		int length;
		uint8_t leds[5];

	public:
		Leds(uint8_t pinWhite,uint8_t pinBlue,uint8_t pinRedFront,uint8_t pinGreenFront,uint8_t pinRedBack);

		int  size();
		void initialize();
		uint8_t getLed(int i);

		void onLedWhite();
		void onLedBlue();
		void onLedFly();
		void onLedWarning();
		void offAllLeds();
};


#endif
