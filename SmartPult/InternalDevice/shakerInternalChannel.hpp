/*
 * shakerInternalChannel.hpp
 *
 *  Created on: 12 мая 2020 г.
 *      Author: Кирилл Чеверев
 */

#ifndef INTERNALDEVICE_SHAKERINTERNALCHANNEL_HPP_
#define INTERNALDEVICE_SHAKERINTERNALCHANNEL_HPP_

#include "Libs/StandartElement/PF.hpp"
#include "Libs/StandartElement/RNDGenerator.hpp"
#include <Pult/PultClasses.h>
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"

class IShaker {

public:

    virtual void setResistorValue(float value) = 0;

    virtual Filter::IPolosovoyFilter _filter () = 0;

};

class ShakerChannel:public JoyChanel, public IShaker {

private:
    int16_t inputValue;
    Generation::Generator RNDGenerator;
    Filter::PolosovoyFilter filter;
    float outValue;
    VirtualMenuResistor speedResistor;

public:

    ShakerChannel(float speedMax,
                  float K,
                  Resistor* speedControl, uint8_t IQValue,int16_t oldStepValue,
                  float  friqLeft,float friqRight, float& fCLK):inputValue(inputValue),
                  RNDGenerator(IQValue,oldStepValue),filter(friqLeft,friqRight,fCLK),
                  speedMax(speedMax), JoyChanel(K, offset,speedControl,0.01, 0.015) {

    }

    virtual void setResistorValue(float value) {
        speedResistor.setValue(value);
    }

    inline void setInputeValue(int16_t inputValue)  {
        this->inputValue = inputValue;
    }

    inline virtual float getCurrentAdcValue() {
        RNDGenerator.setInputValue(inputValue);
        RNDGenerator.generate();
        filter.setInputValue(RNDGenerator.getOutValue());
        filter.calculateFilter();
        outValue = filter.getOutValue();
        outValue *= speedResistor.value;
        if (outValue>speedMax)
            outValue=speedMax;
        if (outValue<-speedMax)
            outValue=-speedMax;
        return outValue;
    }

};



#endif /* INTERNALDEVICE_SHAKERINTERNALCHANNEL_HPP_ */
