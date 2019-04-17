/*
 * ExchangeWheelManager.hpp
 *
 *  Created on: 16 апр. 2019 г.
 *      Author: User9
 */

#ifndef EXTRENALDEVICESYNCHRO_DIGITALWHEEL_EXCHANGEWHEELMANAGER_HPP_
#define EXTRENALDEVICESYNCHRO_DIGITALWHEEL_EXCHANGEWHEELMANAGER_HPP_

#include "LensParam/LensPack.hpp"
#include "Libs/ExchangeLib/Data/OutSerial.hpp"
#include "Libs/ExchangeLib/Data/InSerial.hpp"
#include <../GyConCommon/ExchangeDrivers/Rs485Driver.h>
#include <../GyConCommon/ExchangeBasic/BasicProtocol.h>
#include "Protocol.hpp"

namespace ExtrenalDevices {

class DigitalWheelBuilder {
private:
    LensDb::LensPack & pack;

public:
    DigitalWheelBuilder(LensDb::LensPack & pack):pack(pack) {

    }

    inline void buildPack (wheelProtocol::WheelSimplePackFrame & paket) {
        ExchangeLib::OutSerial outBytes(&pack[0], pack.getSize());
        outBytes.write<wheelProtocol::WheelSimplePackFrame>(paket);
        }

};

class DigitalWheelParser{
private:
    LensDb::LensPack & pack;

public:
    DigitalWheelParser(LensDb::LensPack & pack):pack(pack) {

    }

    inline bool parsePack (wheelProtocol::WheelSimplePackFrame & paket) {
        ExchangeLib::InSerial inBytes(&pack[0],50);
        inBytes.read<wheelProtocol::WheelSimplePackFrame>(paket);
    }
};


class DigitalWheelManager {


private:
    LensDb::LensPackStatic<50> pack;
    DigitalWheelBuilder builder;
    DigitalWheelParser parser;
    wheelProtocol::WheelSimplePackFrame  paket;
    Rs485Driver rs485Driver;

public:

    DigitalWheelManager (BasicProtocolParams * params) : builder(pack),parser(pack),
    rs485Driver(params->uartId, 115200, params->recieveTimeout, params->txEnablePin){

        }

    void exchenge (wheelProtocol::WheelSimplePackFrame &  paket) {
        build(paket);
        rs485Driver.write(&pack[0],pack.getSize());
        }

private:

    inline void build (wheelProtocol::WheelSimplePackFrame & paket) {
        builder.buildPack(paket);
        }

    inline void parse () {
        parser.parsePack(paket);

        }

    inline uint16_t findCrc(uint8_t* pack, size_t crcPos)
            {
            if(crcPos>5)   {       crcPos=5;  }
            uint16_t TMP, crcl, crch, crc = 0xffff;

            crcl = crc;
            crch = (crc >> 8);
            for (uint16_t i = 0; i < crcPos; i++)
                {
                uint16_t dataTmp = pack[i];
                dataTmp = (dataTmp ^ crcl);
                TMP = (dataTmp << 4);
                dataTmp = (TMP ^ dataTmp);
                TMP = (dataTmp >> 5);
                TMP &= 0x07;
                crcl = crch;
                crch = (dataTmp ^ TMP);
                TMP = (dataTmp << 3);
                crcl = (crcl ^ TMP);
                TMP = (dataTmp >> 4);
                TMP &= 0x0F;
                crcl = (crcl ^ TMP);
                }

            crc = (crch << 8) + (crcl & 0x00FF);
            return crc;

};




}

#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_EXCHANGEWHEELMANAGER_HPP_ */
