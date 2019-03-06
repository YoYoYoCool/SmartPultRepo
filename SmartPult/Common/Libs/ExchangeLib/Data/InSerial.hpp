/*
 * InStream.hpp
 *
 *  Created on: 1 мая 2018 г.
 *      Author: KapustinIV
 */

#ifndef EXCHANGELIB_INSTREAM_HPP_
#define EXCHANGELIB_INSTREAM_HPP_

#include <stddef.h>
#include <stdint.h>
#include <string.h>

namespace ExchangeLib {

class InSerial {
private:
    uint8_t* _buf;
    size_t _size;
    size_t _offset;
public:
    inline InSerial(uint8_t* buf, size_t size, size_t offset=0) : _buf(buf), _size(size), _offset(offset) {    }

/*    inline bool readByte(uint8_t& byte) {
        debugAssert(sizeof(uint8_t) < _size - _offset);
        if (sizeof(uint8_t) < _size - _offset) {
            byte = Port::byte(_buf, _offset);
            _offset++;
            return true;
        }
        return false;
    }*/

    template<typename TData>
    inline bool read(TData &data) {
        if (sizeof(TData) < _size) {
            _read(data);
            return true;
        }
        return false;
    }
    template<typename TData>
    inline bool read(TData* dataArr, size_t dataArrSize) {
        if (dataArrSize*sizeof(TData) < _size - _offset) {
            _readArray(dataArr,dataArrSize);
            return true;
            }
        return false;
        }
    template<typename TData>
    inline bool get(TData &data) {
        if (sizeof(TData) < _size- _offset) {
            _get(data);
            return true;
        }
        return false;
    }
    inline bool setReadedCount(size_t readedCount) {
        if (readedCount <= _size) {
            _offset = readedCount;
            return true;
        } else {
            _offset = _size;
            return false;
        }
    }
    inline bool skip(size_t skipCount) {
        if (skipCount > _size - _offset) {_offset = _size ;return false;}
        _offset = _offset + skipCount;
        return true;
    }
    inline size_t readedCount() {
        return _offset;
    }
    inline size_t  getSize() {
        return _size;
    }
private:
    template<typename TData>
    inline void _read(TData &data) {
        memcpy((uint8_t*)&data, (uint8_t*)&_buf[_offset], sizeof(TData));
        _offset = _offset + sizeof(TData);
    }
    template<typename TData>
    inline void _readArray(TData *data, size_t dataArrSize) {
        size_t writeSize = sizeof(TData)*dataArrSize;
        memcpy((uint8_t*)data, (uint8_t*)&_buf[_offset], writeSize);
        _offset = _offset + writeSize;
    }

};
};

#endif
