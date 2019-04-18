/*
 * WheelBuilder.hpp
 *
 *  Created on: 17 апр. 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef EXTRENALDEVICESYNCHRO_DIGITALWHEEL_WHEELBUILDER_HPP_
#define EXTRENALDEVICESYNCHRO_DIGITALWHEEL_WHEELBUILDER_HPP_

#include "Libs/ExchangeLib/Data/OutSerial.hpp"
#include "Protocol.hpp"

namespace ExtrenalDevices {


template <typename TData>
class DigitalWheelBuilder {
private:
    LensDb::LensPack & pack;

public:
    DigitalWheelBuilder(LensDb::LensPack & pack):pack(pack) {

    }

    inline void buildPack (TData & paket) {
        ExchangeLib::OutSerial outBytes(&pack[0], pack.getMaxSize());
        outBytes.write<TData>(paket);
        pack.setPackBufSize(outBytes.getWritedCount());
        }

    };

    }



#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_WHEELBUILDER_HPP_ */
