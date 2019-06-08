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


public:

    inline void buildPack (TData & paket,  uint8_t *buf) {
        ExchangeLib::OutSerial outBytes(buf, sizeof(TData)+1);
        outBytes.write<TData>(paket);

        }

    };

    }



#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_WHEELBUILDER_HPP_ */
