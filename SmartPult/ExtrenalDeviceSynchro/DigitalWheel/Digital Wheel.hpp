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
#include "Libs/TrigonometricFunction.hpp"
//#include "Protocol.hpp"

namespace ExtrenalDevices {

enum {
    delitelSpeed=100
};

enum {transmissionMax=4};

class WheelChannel:public JoyChanel {
private:

    const float maxValue;
    float speedWheel;
    uint8_t transmission;
    bool connect;
    uint8_t counter;

public:

    Trigonametric::Trigonometric triganometric;

    WheelChannel(float speedMax,
                 float K,
                 Resistor* speedControl,
                 float deadZone,
                 float T):maxValue(speedMax), JoyChanel(K, offset,speedControl,deadZone, T),
                  speedWheel(0.0), triganometric(0)
                    {
                    transmission=0;
                    isEnable   =true;
                    connect=false;
                    counter=0;
                    }

    virtual float getCurrentAdcValue() {
        speedWheel=triganometric.updateData(speedWheel);
        if (!isEnable)
            return 0.0;
        if (!connect)
            return 0.0;
        if (speedWheel>maxValue)
            speedWheel=maxValue;
        if (speedWheel<-maxValue)
            speedWheel=-maxValue;
        return speedWheel;
        }

    void setSpeed (float speedWheel) {
        this->speedWheel=speedWheel;
        this->speedWheel*=0.01;
        }

inline void setConnect(bool connect) {
    this->connect=connect;


}








};


}


#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_DIGITAL_WHEEL_HPP_ */
