/*
 * LensCrc.hpp
 *
 *  Created on: 27 февр. 2019 г.
 *      Author: KapustinIV
 */

#ifndef MODULES_LENSDB_LENSCRC_HPP_
#define MODULES_LENSDB_LENSCRC_HPP_

#include "stdint.h"
#include "stddef.h"

namespace LensDb {

inline uint16_t findCrc(uint8_t* pack, size_t crcPos) {
    uint16_t TMP, CRCL, CRCH, CRC = 0xffff;
    CRCL = CRC;
    CRCH = (CRC >> 8);
    for (uint16_t i = 0; i < crcPos; i++) {
        uint16_t dataTmp = pack[i];
        dataTmp = (dataTmp ^ CRCL);
        TMP = (dataTmp << 4);
        dataTmp = (TMP ^ dataTmp);
        TMP = (dataTmp >> 5);
        TMP &= 0x07;
        CRCL = CRCH;
        CRCH = (dataTmp ^ TMP);
        TMP = (dataTmp << 3);
        CRCL = (CRCL ^ TMP);
        TMP = (dataTmp >> 4);
        TMP &= 0x0F;
        CRCL = (CRCL ^ TMP);
    }
    CRC = (CRCH << 8) + (CRCL & 0x00FF);
    return CRC;
}

inline bool checkCrc(uint8_t* buf, size_t crcPos) {
    uint16_t rxCrc = (uint16_t) buf[crcPos]
                        + (uint16_t) (buf[crcPos+1] << 8);
    uint16_t findedCrc = findCrc(buf, crcPos);
    return rxCrc == findedCrc;
}


} /* namespace LensDb */

#endif /* MODULES_LENSDB_LENSCRC_HPP_ */
