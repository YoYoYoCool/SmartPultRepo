/*
 * Pedal.hpp
 *
 *  Created on: 18 мая 2021 г.
 *      Author: Кирилл Чеверев
 */

#ifndef EXTRENALDEVICESYNCHRO_DIGITALPEDAL_PEDALCNANNEL_HPP_
#define EXTRENALDEVICESYNCHRO_DIGITALPEDAL_PEDALCNANNEL_HPP_



#include <Pult/PultClasses.h>
#include "Pedal.hpp"
#include "stddef.h"
#include "stdint.h"



namespace ExtrenalDevices
{

class PedalChannel:public JoyChannelBase {
private:

    ExternalElement::Pedal& pedal;
    Resistor& speedResisitor;
    float koaf;

public:
    PedalChannel(ExternalElement::Pedal& pedal,
                 Resistor& speedResisitor,
                 float koaf):
                 pedal(pedal),
                 speedResisitor(speedResisitor),
                 koaf(koaf)
    {

    }

    float getCurrentAdcValue()
    {
        if (!isEnable)
            return 0.0;
        if (!pedal.isConnect())
            return 0.0;
        float value;
        value = pedal.getOutValue();
        value*= koaf;
        value*= speedResisitor.value;
        return value;

    }
    float getCalibrationOffsetValue()
    {
        return 0.0;
    }

};
}

#endif /* EXTRENALDEVICESYNCHRO_DIGITALPEDAL_PEDALCNANNEL_HPP_ */
