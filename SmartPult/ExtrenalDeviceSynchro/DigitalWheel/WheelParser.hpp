/*
 * WheelParser.hpp
 *
 *  Created on: 17 апр. 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef EXTRENALDEVICESYNCHRO_DIGITALWHEEL_WHEELPARSER_HPP_
#define EXTRENALDEVICESYNCHRO_DIGITALWHEEL_WHEELPARSER_HPP_

#include "Libs/ExchangeLib/Data/InSerial.hpp"
#include "Protocol.hpp"

namespace ExtrenalDevices {

template <typename TData>
class DigitalWheelParser    {
    private:
        LensDb::LensPack & pack;

    public:
        DigitalWheelParser(LensDb::LensPack & pack):pack(pack) {

        }

        inline bool parsePack (TData & paket) {
            ExchangeLib::InSerial inBytes(&pack[0],50);
            inBytes.read<TData>(paket);
            pack.setPackBufSize(inBytes.readedCount());
        }
    };


}

#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_WHEELPARSER_HPP_ */
