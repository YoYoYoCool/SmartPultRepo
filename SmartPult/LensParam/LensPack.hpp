/*
 * LensPack.hpp
 *
 *  Created on: 25 февр. 2019 г.
 *      Author: KapustinIV
 */

#ifndef MODULES_LENSDB_LENSPACK_HPP_
#define MODULES_LENSDB_LENSPACK_HPP_

#include "stdint.h"
#include "stddef.h"
#include "Libs/Utils/Debug.hpp"

namespace LensDb {

class LensPack {
private:
    uint8_t* packBuf;
    size_t packBufSize;
public:
    LensPack(uint8_t* packBuf, size_t packBufSize) : packBuf(packBuf), packBufSize(packBufSize) {

    }
    inline uint8_t& operator[](uint32_t index) {
        debugAssert(index<packBufSize);
        return packBuf[index];
    }
    inline size_t getSize() {
        return packBufSize;
    }
};

template<size_t maxPackSize>
class LensPackStatic : public LensPack
{
private:
    uint8_t packBuf[maxPackSize];
public:
    LensPackStatic() : LensPack(packBuf, maxPackSize) {
    }
};

} /* namespace LensDb */

#endif /* MODULES_LENSDB_LENSPACK_HPP_ */
