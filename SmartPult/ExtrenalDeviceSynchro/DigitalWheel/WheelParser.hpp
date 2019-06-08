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


    public:

        inline bool parsePack (TData & paket, uint8_t * buf) {
            ExchangeLib::InSerial inBytes(buf,sizeof(TData)+1);
            inBytes.read<TData>(paket);
            }
    };


}

#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_WHEELPARSER_HPP_ */
