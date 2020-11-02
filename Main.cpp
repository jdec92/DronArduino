/*
 *	Main.cpp
 *
 *
 *      Author: Juande Estrella
 *
 *  Importante probrar primero sin helices.
 *
 *
 *
*/

#include "Motor/Motor.h"
#include "Battery/Battery.h"
#include "ControlMode/ControlMode.h"
#include "Camera/ControlPixy.h"

Battery bateria(pinBattery);

Bluetooth bluetooth(rxBluetooth, txBluetooth);

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

    if (!mpu6050.initialize() || bateria.isLowBattery()) {
        leds.offAllLeds();
        leds.onLedWarning();
        Serial.println("ERROR no se inicia MPU o la Bater�a esta baja");
        while (true)delay(10);
    }

    pixy.initialize();

    mpu6050.calculateOffset(controlMode.isCalculateGyroscope(), controlMode.isCalculateAcelerometer());

    motor1.initialize();
    motor2.initialize();
    motor3.initialize();
    motor4.initialize();

    Serial.print("Conectarse al dron y bajar throttle al minimo");
    while (bluetooth.getThrottle() != pulseMinMotor) {
        bluetooth.updatePulse();
    }

    Serial.println("OK");
    leds.offAllLeds();
    mpu6050.updateLoopTimer();
    mpu6050.updateExecutionTimer(mpu6050.getLoopTimer());
}

void updateTime() {
    while (micros() - mpu6050.getLoopTimer() < usCiclo);
    mpu6050.updateExecutionTimer(mpu6050.getLoopTimer());
    mpu6050.updateLoopTimer();
}

void updateEngines() {
    motor1.controller(bluetooth.getThrottle(), controlMode.getPidPitch(), -controlMode.getPidRoll(),
                      -controlMode.getPidYaw());
    motor2.controller(bluetooth.getThrottle(), controlMode.getPidPitch(), controlMode.getPidRoll(),
                      controlMode.getPidYaw());
    motor3.controller(bluetooth.getThrottle(), -controlMode.getPidPitch(), controlMode.getPidRoll(),
                      -controlMode.getPidYaw());
    motor4.controller(bluetooth.getThrottle(), -controlMode.getPidPitch(), -controlMode.getPidRoll(),
                      controlMode.getPidYaw());
}

void processDataController() {
    if (controlMode.isCalculateAcelerometer()) {
        mpu6050.processAccelerometer();
    }

    if (controlMode.isCalculateGyroscope()) {
        mpu6050.calculateAngle();
    }

    if (!controlMode.isModeAcrobatic()) {
        controlMode.PIDAnglee(bluetooth, mpu6050);
    }
    controlMode.PIDSpeed(bluetooth, mpu6050);
}

void actionAutomatic() {
    if (controlMode.isModeUp()) {
        controlMode.actionModeUp(bluetooth);
    }

    if (controlMode.isModeDown()) {
        controlMode.actionModeDown(bluetooth, bateria.isLowBattery());
    }

    if (controlMode.isModeAutomatic()) {
        if (controlMode.isFinishModeUp()) {  //Comprobamos si esta a la altura correcta el dron
            if (pixy.isCompletedJourney(bluetooth)) {  //Comprobamos si hemos recorrido el trayecto completo
                controlMode.actionModeDown(bluetooth, bateria.isLowBattery());
            }
        } else {    //Si no hemos despegado o estamos a media altura despega hasta la altura indicada
            controlMode.actionModeUp(bluetooth);
            controlMode.activateModeAutomatic(bluetooth);
        }
    }
}

void readValueBluetooth() {
    if (!controlMode.isModeUp() && !controlMode.isModeDown() && !controlMode.isModeAutomatic()) {
        String value = bluetooth.updatePulse();
        controlMode.commandChangeMode(bluetooth, value);
    } else {
        controlMode.commandChangeMode(bluetooth, bluetooth.waitForResponse());
    }
}

void loop() {
    controlMode.onLedAccordingMode(leds);

    updateTime();

    mpu6050.readSensors();

    processDataController();

    updateEngines();

    actionAutomatic();

    readValueBluetooth();
}

int main() {

    init();

    setup();

    while (true) {
        loop();
    }

    return 0;
}

