/*
 * LensPackParser.hpp
 *
 *  Created on: 1 мар. 2019 г.
 *      Author: Cheverev kirill
 */

#ifndef LENSPARAM_LENSPACKPARSER_HPP_
#define LENSPARAM_LENSPACKPARSER_HPP_
#include "LensData.hpp"
#include "LensPack.hpp"
#include "LensCrc.hpp"
#include "stddef.h"
#include "stdint.h"
#include "Libs/ExchangeLib/Data/InSerial.hpp"
#include "Libs/Containers/String.hpp"
#include "LensCrc.hpp"

namespace LensDb {


template <size_t maxPackLen>
class LensPackParser {


private:
    LensPack & pack;

public:
    inline LensPackParser (LensPack & pack): pack(pack) {        }

    inline uint16_t parsePackSize () {
        uint16_t size;
        ExchangeLib::InSerial inBytes(&pack[0],maxPackLen);
        inBytes.read<uint16_t>(size);
        return size;
    }

    inline bool parseObjective (LensObjective& lensObjective ) {
        ExchangeLib::InSerial inBytes(&pack[0],maxPackLen, headerSize);
        char *name=&lensObjective.name()[0];
        uint8_t sizeName;
        inBytes.read<uint8_t>(sizeName);
        inBytes.read<char>(name,sizeName );// читаем имя
        lensObjective.name().setLength(sizeName);
        readAxis(lensObjective.zoom(),inBytes);
        readAxis(lensObjective.iris(),inBytes);
        readAxis(lensObjective.focus(),inBytes);
        return checkCrc(&pack[0],inBytes.readedCount());
        }

private:

    inline void readAxis (LensAxis& axis,ExchangeLib::InSerial& inBytes) {
        LensPoint*  points= axis.points();
        uint8_t size;
        inBytes.read<uint8_t>(size);
        inBytes.read<LensPoint>(points,size);
        axis.setSize(size);
        }
    };


}


#endif /* LENSPARAM_LENSPACKPARSER_HPP_ */
