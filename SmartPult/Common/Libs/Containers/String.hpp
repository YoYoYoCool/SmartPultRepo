/*
 * String.cpp
 *
 *  Created on: 30 мая 2017 г.
 *      Author: I
 */

#ifndef EXCHANGE_UTILS_CONTAINERS_STRING_H_
#define EXCHANGE_UTILS_CONTAINERS_STRING_H_



#include <stdlib.h>
#include <stdint.h>
#include <algorithm>
#include "Libs/Utils/Debug.hpp"

namespace Containers {

static inline size_t strLen(const char* str, size_t maxStrLen);

class String
{
private:
    size_t length;
    size_t maxLength;
    char*  strBuf;
    //dummyStrBuf используется для защиты если размер внешнего буфера <=0,
    //чтобы система продолжила работу без переполнения буфера(см. initLength)
    static char dummyStrBuf[2];
public:

/*================================ Конструкторы ===================================*/
    inline String(char* strBuf, size_t bufSize, size_t strLength = 0) {
    	init(strBuf, bufSize, strLength);
    }
    inline String(char* strBuf, size_t bufSize, const char* str) {
    	init(strBuf, bufSize);
    	size_t strLength = strLen(str, maxLength);
    	std::memcpy(strBuf, str, strLength);
    	strBuf[strLength] = 0;
    	this->length = strLength;
    }
/*================================ Безопасные методы ===================================*/
    inline size_t copy(const String& fromStr) {
    	return _copy(fromStr.strBuf, fromStr.length);
    }
    inline size_t copy(const char* fromStr) {
    	size_t fromStrLen = strLen(fromStr, maxLength);
        return _copy(fromStr, fromStrLen);
    }
    inline void concat(const String& str) {
    	_copy(str.strBuf, str.length, length);
    }
    inline void concat(const char* str) {
    	size_t strLength = strLen(str, maxLength);
    	_copy(str, strLength, length);
    }
    inline void insert(const String& fromStr, size_t offset = 0) {
    	_insert(fromStr.strBuf, fromStr.length, offset);
    }
    inline void insert(const char* fromStr, size_t offset = 0) {
    	size_t strLength = strLen(fromStr, maxLength);
    	_insert(fromStr, strLength, offset);
    }

    inline bool isEqual(const String & str) const {
    	return isEqual(str.strBuf, str.length);
    }
    inline bool isEqual(const char* str) const {
    	size_t strLength = strLen(str, maxLength);
    	return isEqual(str, strLength);
    }
    inline void fill(char c) {
    	fill(c, maxLength);
    }
    inline size_t fill(char c, size_t count, size_t thisOffset = 0) {
    	thisOffset = std::min(thisOffset, maxLength);
    	size_t leftSize = maxLength - thisOffset;
    	debugAssert(count<=leftSize);
    	size_t fillLength = std::min(leftSize, count);
    	std::memset(&strBuf[thisOffset], c, fillLength);
    	this->length = thisOffset + fillLength;
    	strBuf[this->length] = 0;
    	return fillLength;
    }
    inline char getChar(uint32_t id) const {
    	if (id < this->length) {
    		return strBuf[id];
    	} else {
    	    debugAssert(false);
    		return 0;
    	}
    }
    inline void setChar(uint32_t id, const char c) {
    	if (id < this->maxLength ) {
    		strBuf[id] = c;
    	} else{
    	    debugAssert(false);
    	}
    }
    //Оператор предназначен для доступа к внутреннему массиву для оптимизации вывода
    //Предполагает использование только для конечного вывода строки.
    //Работа со строкой во внешней системе крайне не рекомендуется
    const char& operator[](uint32_t id) {
        debugAssert(id<maxLength);
    	id = std::min(id, maxLength-1);
    	return strBuf[id];
    }
    inline void clear() {
    	length = 0;
    	strBuf[length] = 0;
    }
    inline void setLength(size_t length) {
        debugAssert(length<=maxLength);
        this->length = std::min(length, maxLength);
        strBuf[this->length] = 0;
    };
    inline size_t getLength() const {
        return length;
    }
    inline size_t getMaxLength() const {
        return maxLength;
    }
private:
//int32_t - использован для защиты от отрицательного размера
//так как размер может быть отрицательным результатом вычислений внешней системы
    inline void init(char* strBuf, int32_t bufSize, int32_t length = 0) {
    	int32_t maxLength = std::max<int32_t>(bufSize-1, 0);
    	debugAssert(maxLength>0);
    	if (maxLength>0) {
    		this->strBuf = strBuf;
    		this->maxLength = maxLength;
    	} else {
    		this->strBuf = dummyStrBuf;
    		this->maxLength = 1;
    	}
    	debugAssert(length<=this->maxLength);
    	this->length = std::min<size_t>(this->maxLength, length);
    	strBuf[this->length] = 0;
    	strBuf[this->maxLength] = 0;
    }
    inline size_t _copy(const char* fromStr, size_t count, size_t thisOffset = 0) {
       	thisOffset = std::min(thisOffset, maxLength);
        size_t leftSize = maxLength - thisOffset;
        debugAssert(count<=leftSize);
        size_t copyLength = std::min(leftSize, count);
        std::memcpy(&strBuf[thisOffset], fromStr, copyLength);
        this->length = thisOffset + copyLength;
        strBuf[this->length] = 0;
        return copyLength;
    }
    inline size_t _insert(const char* fromStr, size_t count, size_t thisOffset = 0) {
       	thisOffset = std::min(thisOffset, length);
        size_t leftSize = length - thisOffset;
        debugAssert(leftSize>0);
        debugAssert(count<=leftSize);
        size_t copyLength = std::min(leftSize, count);
        std::memcpy(&strBuf[thisOffset], fromStr, copyLength);
        strBuf[this->length] = 0;
        return copyLength;
    }
    inline bool isEqual(const char* str, size_t strLength) const {
    	if (this->length != strLength) return false;
    	for (uint32_t i = 0; i < length; i++) {
    		if (strBuf[i] != str[i]) return false;
    	}
    	return true;
    }
};

static inline size_t strLen(const char* str, size_t maxStrLen) {
	uint32_t i;
	for (i = 0; i < maxStrLen; i++) {
		if (str[i] == 0) break;
	}
	debugAssert(i<maxStrLen || str[maxStrLen] == 0);
	return i;
}

}
#endif /* EXCHANGE_UTILS_CONTAINERS_STRING_H_ */
