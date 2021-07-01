/*
 * Pedal.hpp
 *
 *  Created on: 18 мая 2021 г.
 *      Author: Кирилл Чеверев
 */

#ifndef EXTRENALDEVICESYNCHRO_DIGITALPEDAL_PEDAL_HPP_
#define EXTRENALDEVICESYNCHRO_DIGITALPEDAL_PEDAL_HPP_



#include "stdint.h"
#include "stddef.h"
#include "../ExternalDeviseBase.hpp"

namespace ExternalElement
{

class Pedal:public ExternalDeviseBase
    {
    private:

    float inValue;
    float outVaue;


    public:

    Pedal()
    {

    }

    int32_t* _inValue()
    {
        return(int32_t*)&inValue;
    }

    float getOutValue()
    {
        outVaue = inValue;
        return outVaue;
    }



    };

}

#endif /* EXTRENALDEVICESYNCHRO_DIGITALPEDAL_PEDAL_HPP_ */
