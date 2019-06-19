/*
 * Digital Wheel.hpp
 *
 *  Created on: 15 апр. 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef EXTRENALDEVICESYNCHRO_DIGITALWHEEL_DIGITAL_WHEEL_HPP_
#define EXTRENALDEVICESYNCHRO_DIGITALWHEEL_DIGITAL_WHEEL_HPP_




#include <Pult/PultClasses.h>
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "Libs/StandartElement/AttAmpStaticChannel.hpp"
#include "Libs/StandartElement/AttAmpDinamicChannel.hpp"


namespace ExtrenalDevices {

enum {
    delitelSpeed=100
};

enum {transmissionMax=5};



Schematic::AttAmpSettings settingsDummi = {
    .amplifierSettings.Vcc=12.0,
    .amplifierSettings.Vee=-12.0,
    .amplifierSettings.stableRatio=1.0,
    .amplifierSettings.resistenceMax=3722,
    .amplifierSettings.adjustableRatio=15.7,
    .attenuatorSettings.Vcc=0.0,
    .attenuatorSettings.Vee=0.0,
    .attenuatorSettings.deadZone=0.0,
    .attenuatorSettings.attenuationK=0.002,
    };

Schematic::AttAmpSettings settingsFirst = {
    .amplifierSettings.Vcc=12.0,
    .amplifierSettings.Vee=-12.0,
    .amplifierSettings.stableRatio=1.0,
    .amplifierSettings.resistenceMax=3722,
    .amplifierSettings.adjustableRatio=15.7,
    .attenuatorSettings.Vcc=0.0,
    .attenuatorSettings.Vee=0.0,
    .attenuatorSettings.deadZone=0.0,
    .attenuatorSettings.attenuationK=0.002,
    };

Schematic::AttAmpSettings settingsSecond = {
    .amplifierSettings.Vcc=120.0,
    .amplifierSettings.Vee=-120.0,
    .amplifierSettings.stableRatio=1.0,
    .amplifierSettings.resistenceMax=3722,
    .amplifierSettings.adjustableRatio=15.7,
    .attenuatorSettings.Vcc=0.0,
    .attenuatorSettings.Vee=0.0,
    .attenuatorSettings.deadZone=0.0,
    .attenuatorSettings.attenuationK=0.02,
    };

Schematic::AttAmpSettings settingsThird = {
    .amplifierSettings.Vcc=230.0,
    .amplifierSettings.Vee=-230.0,
    .amplifierSettings.stableRatio=1.0,
    .amplifierSettings.resistenceMax=3722,
    .amplifierSettings.adjustableRatio=4.0,
    .attenuatorSettings.Vcc=0.0,
    .attenuatorSettings.Vee=0.0,
    .attenuatorSettings.deadZone=0.0,
    .attenuatorSettings.attenuationK=0.2,
    };

Schematic::AttAmpSettings settingsFast = {
    .amplifierSettings.Vcc=230.0,
    .amplifierSettings.Vee=-230.0,
    .amplifierSettings.stableRatio=0.0,
    .amplifierSettings.resistenceMax=3722,
    .amplifierSettings.adjustableRatio=1.0,
    .attenuatorSettings.Vcc=0.0,
    .attenuatorSettings.Vee=0.0,
    .attenuatorSettings.deadZone=0.0,
    .attenuatorSettings.attenuationK=1.0,
    };


Schematic::AttAmpSettings * settings[transmissionMax] = {  &settingsDummi,
                                                           &settingsFirst,
                                                           &settingsSecond,
                                                           &settingsThird,
                                                            &settingsFast};

class WheelChannel:public JoyChanel {
private:

    const float maxValue;
    float speedWheel;
    bool connect;
    uint8_t counter;
    float speedWheel2;
    int32_t speedWheelRaw;
    bool inputRaw;

public:


    Schematic::AttAmpDinamicChannal<transmissionMax> channal;

    WheelChannel(float speedMax,
                 float K,
                 Resistor* speedControl):maxValue(speedMax), JoyChanel(K, offset,speedControl,0.01, 0.015)
                    {
                    speedWheel=0.0,
                    isEnable   =false;
                    connect=false;
                    counter=0;
                    inputRaw=true;
                    for (uint8_t i=0; i<transmissionMax; i++) {
                        channal.setup(i,settings[i]);}

                    }

    inline virtual float getCurrentAdcValue() {
        speedWheel=speedWheelRaw;
        speedWheel*=0.001;
        if (!isEnable)
            return 0.0;
        speedWheel=channal.updateData(speedWheel,speedControl->adcValue);
        if (speedWheel>maxValue)
            speedWheel=maxValue;
        if (speedWheel<-maxValue)
            speedWheel=-maxValue;
        return speedWheel;
        }

    inline volatile float & getSpeedWheel () {
        return speedWheel;
        }

    inline void setUseSpeedRaw (bool inputRaw) {
        this->inputRaw = inputRaw;
        }

    inline int32_t * getSpeedWheelRaw () {
        return &speedWheelRaw;
    }

    inline void setConnect(bool connect) {
        this->connect=connect;
        }

};


}


#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_DIGITAL_WHEEL_HPP_ */
