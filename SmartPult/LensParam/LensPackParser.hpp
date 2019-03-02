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

class LensPackParser {
private:

    uint16_t addrName;
    uint16_t addrZoom;
    uint16_t addrIris;
    uint16_t addrFocus;
    uint8_t sizeName;
    uint8_t sizeZoom;
    uint8_t sizeIris;
    uint8_t sizeFocus;

public:
    inline LensPackParser () {        }


    inline bool  validate (uint8_t* buf) {
         return checkCrc(&buf[0],calcAddr(&buf[0]));
        }

    inline void objectiveBuild (uint8_t* buf,LensObjective& lensObjective ) {
        ExchangeLib::InSerial inBytes(buf,4096,1);
        char* name;
        inBytes.read<char>(&name[0],sizeName );// читаем имя
        lensObjective.name().copy(name);
        inBytes.setReadedCount(addrZoom);//
        readAxis(lensObjective.zoom(),inBytes,sizeZoom);
        inBytes.setReadedCount(addrIris);//
        readAxis(lensObjective.iris(),inBytes,sizeIris);
        inBytes.setReadedCount(addrFocus);//
        readAxis(lensObjective.focus(),inBytes,sizeFocus);
        }

private:

    inline void readAxis (LensAxis& axis,ExchangeLib::InSerial& inBytes, uint8_t longAxis ) {
        LensPoint  point;
        for (uint8_t pointId=0; pointId<longAxis; pointId++) {
            inBytes.read<LensPoint>(&point, 1);
            axis.setPoint(pointId, point);
            }
        }

    inline uint16_t calcAddr(const uint8_t* buf) {

        addrName=1;
        sizeName=buf[0];
        addrZoom=sizeName+addrName;//
        sizeZoom=buf[addrZoom];// читаем длинну зума
        addrZoom++; //переход на первый байт нулевой точки
        addrIris=addrZoom+sizeZoom*8;//длинна зума умножается на 8 т.к. значение одной точки сохранено в 8 байтах
        // если зум имеет 2 точки типа float то для хранения этих точек нужно 16 байт
        sizeIris=buf[addrIris];
        addrIris++;
        addrFocus=addrIris+sizeIris*8;
        sizeFocus=buf[addrFocus];
        addrFocus++;
        return (sizeFocus*8)+addrFocus;
        }
};


}


#endif /* LENSPARAM_LENSPACKPARSER_HPP_ */
