/*
 * ByteList.h
 *
 *  Created on: 26 окт. 2014 г.
 *      Author: CNO
 */

#ifndef BYTEARRAY_H_
#define BYTEARRAY_H_
#include <../GyConCommon/dataTypes.h>

class ByteCollection {
public:
	 virtual bool add(UInt8 byte);
	 virtual UInt8 get(UInt16 id);
	 virtual bool set(UInt16 id, UInt8 value);
	 virtual UInt16 getSize();
	 virtual UInt16 getMaxSize();
	 virtual void clear();
};

class ByteArray:public ByteCollection {
public:
    UInt8 *const bytes;
    UInt16 curSize;
    const UInt16 maximumSize;
    ByteArray(UInt8 *bytes, UInt16 maxSize);
    bool add(UInt8 byte);
    UInt8 get(UInt16 id);
    bool set(UInt16 id, UInt8 value);
    UInt16 getSize();
    UInt16 maxSize();
    void clear();
    bool remove();
};

class LoopByteArray: public ByteCollection {
private:
	void findFirstId();
public:
	UInt16 lastId;
	UInt16 firstId;
	UInt8 *bytes;
	UInt16 size;
	UInt16 maxSize;
	void removeFirst();
	bool add(UInt8 byte);
	UInt8 get(UInt16 id);
	bool set(UInt16 id, UInt8 value);
    UInt16 getSize();
    UInt16 getMaxSize();
	void clear();
	LoopByteArray(UInt8 *bytes, UInt16 maxSize);
};

class OffsetByteArray:public ByteArray {
public:
	UInt16 offset;
	OffsetByteArray(ByteArray* byteArray);
	OffsetByteArray(UInt8 *bytes, UInt16 maxSize);
};

#endif /* BYTEARRAY_H_ */
