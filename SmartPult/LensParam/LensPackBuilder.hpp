/*
 * LensPackBuilder.hpp
 *
 *  Created on: 27 февр. 2019 г.
 *      Author: KapustinIV
 */

#ifndef MODULES_LENSDB_LENSPACKBUILDER_HPP_
#define MODULES_LENSDB_LENSPACKBUILDER_HPP_

#include "LensData.hpp"
#include "LensPack.hpp"
#include "Libs/ExchangeLib/Data/OutSerial.hpp"
#include "Libs/Containers/String.hpp"
#include "LensCrc.hpp"

namespace LensDb {

template<size_t maxAxisPoints, size_t maxPackLen>
class LensPackBuilder
{
private:
    LensPackStatic<maxPackLen> pack;
public:
    LensPackBuilder() {
    }
    inline LensPack& buildPack(LensObjective& lensObjective) {
        ExchangeLib::OutSerial outBytes(&pack[0], pack.getSize());
        uint8_t sizeName=lensObjective.name().getLength();
        outBytes.write<uint8_t>(sizeName);//пишем длинну имени
        outBytes.write<const char>(&lensObjective.name()[0], lensObjective.name().getLength());//пишем имя
        writeAxis(lensObjective.zoom(), outBytes);//пишем зум
        writeAxis(lensObjective.iris(), outBytes);//пишем диафрагму
        writeAxis(lensObjective.focus(), outBytes);//пишем фокус
        uint16_t crc = findCrc(&pack[0], outBytes.getWritedCount()); //Считаем контрольную сумму
        outBytes.write<uint16_t>(crc); //Пишем контрольную сумму
        return pack;
    }
private:

    inline void writeAxis(LensAxis& axis, ExchangeLib::OutSerial& outBytes) {
        uint8_t pointsCount = axis.getSize();
        LensPoint* points =  axis.points();
        outBytes.write<uint8_t>(pointsCount); //Пишем количество точек
        outBytes.write<LensPoint>(points, pointsCount);//Пишем точки
    }


};

} /* namespace LensDb */

#endif /* MODULES_LENSDB_LENSPACKBUILDER_HPP_ */
