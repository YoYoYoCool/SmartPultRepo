/*
 * ExchangeWheelManager.hpp
 *
 *  Created on: 16 апр. 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef EXTRENALDEVICESYNCHRO_DIGITALWHEEL_EXCHANGEWHEELMANAGER_HPP_
#define EXTRENALDEVICESYNCHRO_DIGITALWHEEL_EXCHANGEWHEELMANAGER_HPP_


#include <../GyConCommon/ExchangeDrivers/Rs485Driver.h>
#include <../GyConCommon/ExchangeBasic/BasicProtocol.h>
#include "Protocol.hpp"
#include "LensParam/LensPack.hpp"

namespace ExtrenalDevices {


class DigitalWheelManager {


private:
    WheelProtocol::WheelProtacol protokol;
    Rs485Driver driver;
    LensDb::LensPackStatic<50> _pack;
    int32_t speedWheel;

public:

    DigitalWheelManager (BasicProtocolParams * params) :
    driver(params->uartId, 115200, 5, params->txEnablePin),
    protokol(_pack){

        }

    void exchenge (WheelProtocol::WheelsType wheelId, WheelProtocol::ReadCommand comandID  ) {
        protokol.creatPaket(wheelId, comandID);
        driver.write(&protokol.pack()[0],protokol.pack().getSize());
        Task_sleep(5);
        driver.read(&_pack[0], 14);
        if (comandID==WheelProtocol::WHEEL_SPEED_REQUEST) {
            if (protokol.createDataSpeed()) {
                speedWheel=protokol.getSpeed();
                }
            return;
            }
        }

private:




};




}

#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_EXCHANGEWHEELMANAGER_HPP_ */
