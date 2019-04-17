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
//#include "Protocol.hpp"

namespace ExtrenalDevices {



enum {
    delitelSpeed=100
};



struct AxisData
{
    volatile bool isActive;
    volatile float value;
};

class WheelChannel:public JoyChanel {
public:

    WheelChannel(float speedMax,
                 float K,
                 Resistor* speedControl,
                 float deadZone,
                 float T):maxValue(speedMax), JoyChanel(K, offset,speedControl,deadZone, T),
                 isActive(true), speedWheel(0.0)
                 {
                    isEnable   =true;
                 }

    volatile float * getUKSpeedIn () {return &adcValue;}



    virtual float getCurrentAdcValue() {
        speedWheel=adcValue;
        speedWheel/=delitelSpeed;
        if (!isEnable) {
            speedWheel=0.0;
            return speedWheel;}
        if (!isActive) {
            speedWheel=0.0;
            return speedWheel;}
        if (speedWheel>maxValue)
            speedWheel=maxValue;
        if (speedWheel<-maxValue)
            speedWheel=-maxValue;
        return speedWheel;

    }
private:

    const float maxValue;
    float speedWheel;

protected:
    volatile float * getUKSpeedOut () {return &speedWheel;}

public:
    bool isActive;

};


}


#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_DIGITAL_WHEEL_HPP_ */
