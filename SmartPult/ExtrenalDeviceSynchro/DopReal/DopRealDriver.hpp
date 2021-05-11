/*
 * DopRealDriver.hpp
 *
 *  Created on: 21 окт. 2020 г.
 *      Author: Кирилл Чеверев
 */

#ifndef EXTRENALDEVICESYNCHRO_DOPREAL_DOPREALDRIVER_HPP_
#define EXTRENALDEVICESYNCHRO_DOPREAL_DOPREALDRIVER_HPP_



#include "../../GyConCommon/ExchangeDrivers/UartDriver.h"

class DopRealUartDriver
{
private:
    UartDriver* uartDriver;
    uint8_t* buf;
    uint16_t& offsetData;

public:
    DopRealUartDriver(uint8_t* buf,uint16_t& offsetData):buf(buf),
    offsetData(offsetData)
    {

    }

    void setDriver(UartDriver* uartDriver)
    {
        this->uartDriver = uartDriver;
    }

    void writeData()
    {
        uartDriver->write(buf, offsetData);
    }


};

#endif /* EXTRENALDEVICESYNCHRO_DOPREAL_DOPREALDRIVER_HPP_ */
