/*
 * OutStream.hpp
 *
 *  Created on: 3 мая 2018 г.
 *      Author: KapustinIV
 */

#ifndef EXCHANGELIB_OUTSTREAM_HPP_
#define EXCHANGELIB_OUTSTREAM_HPP_

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include "Libs/Utils/Debug.hpp"
#include "string.h"
#include "Libs/Containers/String.hpp"

namespace ExchangeLib {
class OutSerial
{
private:
    uint8_t* buf;
    size_t _maxSize;
    size_t offset;
public:
    inline OutSerial(uint8_t* buf, size_t size, size_t offset=0 ) : buf(buf), _maxSize(size), offset(offset) {
    }

    template<typename TData>
    inline bool write(TData& data) {
        debugAssert(sizeof(TData) < _maxSize - offset);
        if (sizeof(TData) < _maxSize - offset) {
             _write(data);
             return true;
        }
        return false;
    }
    template<typename TData>
    inline bool write(TData* dataArr, size_t dataArrSize) {
        debugAssert(dataArrSize*sizeof(TData) < _maxSize - offset);
        if (dataArrSize*sizeof(TData) < _maxSize - offset) {
            _writeArray(dataArr, dataArrSize);
            return true;
        }
        return false;
    }
    template<typename TData>
    inline bool write(TData** dataArr, size_t dataArrSize) {
        debugAssert(dataArrSize*sizeof(TData) < _maxSize - offset);
        if (dataArrSize*sizeof(TData) < _maxSize - offset) {
            for(size_t i = 0; i < dataArrSize; i++) {
                _write(*dataArr[i]);
            }
            return true;
        }
        return false;
    }
    inline size_t getWritedCount() {
       return offset;
    }
    inline void clear() {
       offset = 0;
    }
    inline size_t getMaxSize() {
       return _maxSize;
    }
    inline void setOffset (size_t offset) {
        this->offset=offset;
        }
    inline bool skip(size_t count) {
        debugAssert(count < _maxSize - offset);
        if (count < _maxSize - offset) {
            offset = offset + count;
            return true;
        }
        return false;
    }
private:
    template<typename TData>
    inline void _write(TData& data) {
        memcpy((uint8_t*)&buf[offset], (uint8_t*)&data, sizeof(TData));
        offset = offset + sizeof(TData);
    }
    template<typename TData>
    inline void _writeArray(TData* dataArray, size_t arraySize) {
        size_t writeSize = sizeof(TData)*arraySize;
        memcpy((uint8_t*)&buf[offset], (uint8_t*)dataArray, writeSize);
        offset = offset + writeSize;
    }

};
};

#endif
