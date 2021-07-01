/*
 * ExternalDeviseBase.hpp
 *
 *  Created on: 21 мая 2021 г.
 *      Author: Кирилл Чеверев
 */

#ifndef EXTRENALDEVICESYNCHRO_EXTERNALDEVISEBASE_HPP_
#define EXTRENALDEVICESYNCHRO_EXTERNALDEVISEBASE_HPP_



#include "stdint.h"
#include "stddef.h"


class ExternalDeviseBase
{
private:

    bool enable;
    bool connect;
    int8_t connectCounter;
    int8_t offsetCounterValue;

public:

    ExternalDeviseBase():
        enable(false),
        connect(false),
        connectCounter(0),
        offsetCounterValue(10)
        {

        }

    void updateConnect(uint32_t connect)
        {
            if (connect==1)
            {
                connectCounter++;
                if (connectCounter>offsetCounterValue)
                {
                    connectCounter=offsetCounterValue;
                    this->connect = true;
                }
            }
            else
            {
                connectCounter--;
                if (connectCounter<0)
                {
                    connectCounter=0;
                    this->connect = false;
                }
            }
        }

        void disableExternalDevice()      {   enable = false;     }

        void enableExternalDevice()       {   enable = true;     }

        bool isEnable()     {   return enable;      }

        bool isConnect()    {   return connect;     }
};


#endif /* EXTRENALDEVICESYNCHRO_EXTERNALDEVISEBASE_HPP_ */
