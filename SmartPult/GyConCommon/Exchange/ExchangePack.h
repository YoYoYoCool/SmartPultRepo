#ifndef EXCHANGE_EXCHANGE_PACK_H_
#define EXCHANGE_EXCHANGE_PACK_H_
#include <../GyConCommon/dataTypes.h>
class Pack_;

class PackElement_ {
public:
	Int16 size;
	Pack_* pack;
	PackElement_() {
		size = 0;
	}
	virtual bool parse(UInt8* firstBytePtr) {return true;}
	virtual Int16 write(UInt8* firstBytePtr) {return 0;}
	virtual void setPack(Pack_* pack) {
		this->pack = pack;
	}
};

class PackHeader: public PackElement_ {
public:
	Int16 packSize;
};

class PackData: public PackElement_ {
public:
	UInt8* content;
	UInt8 contentSize;
	virtual void setSizes(UInt8 contentSize) {
		this->size = contentSize;
		this->contentSize = contentSize;
	}
};

class PackDataReader {
private:
	PackData* data;
	Int16 byteId;
	UInt16 contentSize;
public:
	PackDataReader() {

	}
	PackDataReader(PackData* data) {
		this->data = data;
		reset();
	}

 	void init(PackData* data) {
 		this->data = data;
 		reset();
 	}
 	Int8 parseInt8() {
 	   Int8 res = data->content[byteId++];
 	   return res;
 	}
	Int16 parseInt16() {
	   UInt16 byte0 = (UInt16)data->content[byteId++];
	   UInt16 byte1 = (UInt16)data->content[byteId++]<<8;
	   Int16 res = (byte0 + byte1);
	   return res;
	}
	bool hasBytes(Int16 count) {
		return byteId+count<=contentSize;
	}
	void reset() {
		byteId = 0;
		contentSize = data->contentSize;
	}
};

class PackDataWriter {
private:
	PackData* data;
	Int16 byteId;
public:
	void init(PackData* data) {
		byteId = 0;
		this->data = data;
	}
	void writeInt8(Int8 value) {
	    data->content[byteId++] = value;
	    data->setSizes(byteId);
	}
	void writeInt16(Int16 value) {
        data->content[byteId++] = value;
        data->content[byteId++] = value>>8;
        data->setSizes(byteId);
	}
};

class Pack_ {
public:
	PackHeader* const header;
	PackData* const data;
	PackElement_* const crc;
	UInt8* const buf;
	const UInt16 bufSize;
	Pack_(PackHeader* header, PackData* data, PackElement_* crc, UInt8* packBuf, UInt16 bufSize):
		header(header),
		data(data),
		crc(crc),
		buf(packBuf),
		bufSize(bufSize) {
		header->setPack(this);
		data->setPack(this);
		crc->setPack(this);
	}
};

#endif// EXCHANGE_EXCHANGE_PACK_H_
