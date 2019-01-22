;/*
 * ByteList.cpp
 *  Created on: 26 окт. 2014 г.
 *      Author: CNO
 */

#include <../GyConCommon/utils/ByteCollection.h>

bool ByteCollection::add(UInt8 byte) {return false;}
UInt8 ByteCollection::get(UInt16 id) {return 0;}
bool ByteCollection::set(UInt16 id, UInt8 value) {return false;}
UInt16 ByteCollection::getSize() {return 0;}
UInt16 ByteCollection::getMaxSize() {return 0;}
void ByteCollection::clear() {}

ByteArray::ByteArray(UInt8* bytes, UInt16 maxSize) :
		bytes(bytes), maximumSize(maximumSize) {
	this->curSize = 0;
}

bool ByteArray::add(UInt8 byte) {
	if (curSize < maximumSize) {
		bytes[curSize] = byte;
		curSize++;
		return true;
	} else {
		return false;
	}
}
UInt8 ByteArray::get(UInt16 id) {
    return bytes[id];
}

bool ByteArray::set(UInt16 id, UInt8 value) {
    bytes[id] = value;
    return true;
}

UInt16 ByteArray::getSize() {
    return curSize;
}

UInt16 ByteArray::maxSize() {
    return maximumSize;
}

void ByteArray::clear() {
	this->curSize = 0;
}

bool ByteArray::remove() {
	if (curSize > 0) {
		curSize--;
		return true;
	} else {
		return false;
	}
}

LoopByteArray::LoopByteArray(UInt8 *bytes, UInt16 loopSize):
	bytes(bytes),
	maxSize(loopSize) {
	size = 0;
	findFirstId();
}

void LoopByteArray::findFirstId() {
	if (size>0) {
	    Int32 tmp = (Int32)lastId - ((Int32)size - 1);
	    if (tmp<0) tmp = (Int32)maxSize + tmp;
	    firstId = tmp;
	} else {
		firstId = 0;
		lastId = maxSize - 1;
	}
}
void LoopByteArray::removeFirst() {
    if (size>0) size--;
    findFirstId();
}

bool LoopByteArray::add(UInt8 byte) {
	bool addResult = true;
    if (lastId<maxSize-1) {
    	lastId++;
    } else lastId = 0;

    bytes[lastId] = byte;
    if (size<maxSize) {
    	size++;
    } else {
    	addResult = false;
        findFirstId();
    }
    return addResult;
}

UInt8 LoopByteArray::get(UInt16 id) {
	UInt16 effId = firstId+id;
	if (effId>=maxSize) {
		effId=effId - maxSize;
	}
    return bytes[effId];
}

bool LoopByteArray::set(UInt16 id, UInt8 value) {
	UInt16 effId = firstId+id;
	if (effId>=maxSize) {
		effId=effId - maxSize;
	}
    bytes[effId] = value;
    return true;
}

UInt16 LoopByteArray::getSize() {
	return size;
}

UInt16 LoopByteArray::getMaxSize() {
	return maxSize;
}

void LoopByteArray::clear() {
	firstId = 0;
	lastId = maxSize - 1;
	size = 0;
}

OffsetByteArray::OffsetByteArray(ByteArray* byteArray) :
		ByteArray(byteArray->bytes, byteArray->maximumSize) {
	offset = 0;
	curSize = byteArray->curSize;
}

OffsetByteArray::OffsetByteArray(UInt8 *bytes, UInt16 maxSize):
		ByteArray(bytes, maxSize) {
	offset=0;
}

//char OffsetByteArray::get(int id) {
//	if (offset+id<size) {
//       return bytes[offset+id];
//	} else return 0xFF;
//}
