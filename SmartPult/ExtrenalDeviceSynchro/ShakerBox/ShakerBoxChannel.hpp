/*
 * ShakerBoxChannel.hpp
 *
 *  Created on: 24 мая 2021 г.
 *      Author: Кирилл Чеверев
 */

#ifndef EXTRENALDEVICESYNCHRO_SHAKERBOX_SHAKERBOXCHANNEL_HPP_
#define EXTRENALDEVICESYNCHRO_SHAKERBOX_SHAKERBOXCHANNEL_HPP_



#include <Pult/PultClasses.h>
#include "ShakerBox.hpp"
#include "stddef.h"
#include "stdint.h"


namespace ExtrenalDevices
{

    class ShekerBoxChannel:public JoyChannelBase
    {
    private:

        ExternalElement::IGetSpeed& shakerChannel;

    public:

        ShekerBoxChannel(ExternalElement::IGetSpeed& shakerChannel):shakerChannel(shakerChannel)
            {

            }

        float getCurrentAdcValue()
        {
            if (!isEnable)
                return 0.0;
            if (!shakerChannel.isConnect())
                return 0.0;
            return shakerChannel.getOutValue();
        }

        float getCalibrationOffsetValue()
        {
            return 0.0;
        }
    };
}


#endif /* EXTRENALDEVICESYNCHRO_SHAKERBOX_SHAKERBOXCHANNEL_HPP_ */
