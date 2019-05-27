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

enum {maxCounter=3};


class DigitalWheelManager {


private:
    WheelProtocol::WheelProtacol & protokol;
    Rs485Driver2 & driver;
    LensDb::LensPack & packIn;
    int32_t speedWheel;
    int8_t counter;
    bool conect;
    WheelProtocol::WheelsType wheelId;

public:

    DigitalWheelManager (Rs485Driver2 & driver, WheelProtocol::WheelProtacol & protokol,
                         WheelProtocol::WheelsType wheelId, LensDb::LensPack & packIn) :
    driver(driver),
    protokol(protokol),
    wheelId(wheelId),
    packIn(packIn){
        counter=0;
        speedWheel=0;
        conect=false;
        }

    void exchenge (WheelProtocol::ReadCommand comandID  ) {
        protokol.creatPaket(wheelId, comandID);
        driver.write((UInt8*)&protokol.pack()[0],protokol.pack().getSize());
        driver.read((UInt8*)&packIn[0], 14);
        if (comandID==WheelProtocol::WHEEL_SPEED_REQUEST) {
            if (protokol.createDataSpeed()) {
                counter++;
                if (counter>maxCounter) {
                conect=true;
                speedWheel=protokol.getSpeed();
                counter=maxCounter; }
                }
            else {
                counter--;
                if (counter<0) {
                    counter=0;
                    speedWheel=0.0;
                    conect=false;
                    }
                }
            }
        }

    int32_t getSpeed() {
        return speedWheel;
        }

    bool getConnect () { return conect; }

private:

    void updateWheelPanData () {

        }

    void updateWheelRollData () {

        }

    void updateWheelTiltData () {

        }

};




}

#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_EXCHANGEWHEELMANAGER_HPP_ */
