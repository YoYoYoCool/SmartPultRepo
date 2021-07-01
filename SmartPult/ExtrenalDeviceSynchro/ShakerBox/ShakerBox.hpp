/*
 * ShakerBox.hpp
 *
 *  Created on: 21 мая 2021 г.
 *      Author: Кирилл Чеверев
 */

#ifndef EXTRENALDEVICESYNCHRO_SHAKERBOX_SHAKERBOX_HPP_
#define EXTRENALDEVICESYNCHRO_SHAKERBOX_SHAKERBOX_HPP_



#include "stdint.h"
#include "stddef.h"
#include "../ExternalDeviseBase.hpp"

namespace ExternalElement
{



class ShakerBox:public ExternalDeviseBase
{

private:

    float inValue[3];

public:

    float getPanSpeed()        {   return inValue[0];     }

    float getTiltSpeed()       {   return inValue[1];     }

    float getRollSpeed()       {   return inValue[2];     }

    int32_t* getMassive()         {   return (int32_t*)inValue;         }

};

class IGetSpeed
{
protected:

    ShakerBox& speed;

public:

    IGetSpeed(ShakerBox& speed):speed(speed)
       {

       }

    bool isConnect()    { return speed.isConnect(); }

    bool isEnable()     { return speed.isEnable();  }

    virtual float getOutValue() =0;
};

class ShakerBoxGetSpeedPan:public IGetSpeed
{

    public:

    ShakerBoxGetSpeedPan(ShakerBox& speed):IGetSpeed(speed)
    {

    }

    float getOutValue()    {   return speed.getPanSpeed();    }

};

class ShakerBoxGetSpeedTilt: public IGetSpeed
{

    public:

    ShakerBoxGetSpeedTilt(ShakerBox& speed):IGetSpeed(speed)
    {

    }

    float getOutValue()    {   return speed.getTiltSpeed();    }

};

class ShakerBoxGetSpeedRoll:public IGetSpeed
    {

        public:

    ShakerBoxGetSpeedRoll(ShakerBox& speed):IGetSpeed(speed)
        {

        }

        float getOutValue()    {   return speed.getRollSpeed();    }

    };


}


#endif /* EXTRENALDEVICESYNCHRO_SHAKERBOX_SHAKERBOX_HPP_ */
