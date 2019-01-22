/*
 * BasicPack.h
 *
 *  Created on: 12 июня 2015 г.
 *      Author: Александр
 */

#ifndef EXCHANGE_BASIC_BASICPACK_H_
#define EXCHANGE_BASIC_BASICPACK_H_

#include <../GyConCommon/dataTypes.h>
#include <../GyConCommon/Exchange/ExchangePack.h>

class BasicHeader: public PackHeader {
public:
	UInt16Bytes beginTag;
	//UInt8 packSize; // у родителя
	UInt8 address;
	UInt8 askCommand;
	UInt8 writeCommand;
	BasicHeader(UInt16 beginTag) {
		this->beginTag.all = beginTag;
		size = 6;
		packSize = 0;
	}
	bool parse(UInt8* buf) {
		UInt16 id = 0;
		UInt8 tagByte0 = buf[id++];
	    UInt8 tagByte1 = buf[id++];
	    if (tagByte0 != beginTag.bytes.byte0)
	    		return false;
	    if (tagByte1 != beginTag.bytes.byte1)
	    		return false;
	    packSize = buf[id++];
	    if (packSize > pack->bufSize) {
	    	return false;
	    }
	    address = buf[id++];
	    askCommand = buf[id++];
		writeCommand = buf[id++];
		return true;
	}
	Int16 write(UInt8* buf) {
		UInt16 id = 0;
		packSize = size + pack->data->size + pack->crc->size;
		buf[id++] = beginTag.bytes.byte0;
	    buf[id++] = beginTag.bytes.byte1;
	    buf[id++] = packSize;
	    buf[id++] = address;
	    buf[id++] = askCommand;
	    buf[id++] = writeCommand;
		return size;
	}
};
class BasicTag8Data: public PackData {
public:
	const UInt8 dataTag;
	BasicTag8Data(UInt8 dataTag) :
		dataTag(dataTag) {
	}
	bool parse(UInt8* buf) {
		size = pack->header->packSize - pack->crc->size - pack->header->size;
		if (size<=1) {
			//buf = NULL;
			contentSize = 0;
			return true;
		}
		contentSize = size - 1;
		UInt16 id = 0;
		UInt8 tag = buf[id++];
		if (tag != dataTag)
			return false;
		content = &buf[id];
		return true;
	}
	void setContent(UInt8* content, UInt8 contentSize) {
		this->content =  content;
		this->size = contentSize + 1;
		this->contentSize = contentSize;
	}
	virtual void setSizes(UInt8 contentSize) {
		this->size = contentSize + 1;
		this->contentSize = contentSize;
	}
	Int16 write(UInt8* buf) {
//		UInt16 id = 0;
		buf[0] = dataTag;
//		for (int i = 0; i < this->contentSize; i++) {
//			buf[id++] = this->content[i];
//		}
		return size;
	}
	virtual void setPack(Pack_* pack) {
		this->pack = pack;
		setContent(&pack->buf[pack->header->size + 1], 0);
		pack->buf[pack->header->size] = dataTag;
	}
};

class BasicData: public PackData {
public:
	BasicData() {
	}
	bool parse(UInt8* buf) {
		size = pack->header->packSize - pack->crc->size - pack->header->size;
		if (size == 0) {
			//buf = NULL;
			contentSize = 0;
			return true;
		}
		contentSize = size;
		content = &buf[0];
		return true;
	}
	void setContent(UInt8* content, UInt8 contentSize) {
		this->content =  content;
		this->size = contentSize;
		this->contentSize = contentSize;
	}
	virtual void setSizes(UInt8 contentSize) {
		this->size = contentSize;
		this->contentSize = contentSize;
	}
	Int16 write(UInt8* buf) {
		return size;
	}
	virtual void setPack(Pack_* pack) {
		this->pack = pack;
		setContent(&pack->buf[pack->header->size], 0);
	}
};

class BasicCrc: public PackElement_ {
public:
	BasicCrc() {
		size = 2;
	}
	bool parse(UInt8* buf) {
		UInt16 rxCrc = (UInt16) buf[0]
					+ (UInt16) (buf[1] << 8);
		Int16 crcPos = pack->header->packSize - size;
		UInt16 findedCrc = findCrc(pack->buf, crcPos);
		return rxCrc == findedCrc;
	}
	Int16 write(UInt8* buf) {
		Int16 crcPos = pack->header->packSize - size;
		UInt16 crc = findCrc(pack->buf, crcPos);
		buf[0] = crc;
		buf[1] = crc>>8;
		return 2;
	}
	virtual UInt16 findCrc(UInt8* pack, Int16 crcPos) {
		UInt16 TMP, CRCL, CRCH, CRC = 0xffff;
		CRCL = CRC;
		CRCH = (CRC >> 8);
		for (UInt16 i = 0; i < crcPos; i++) {
			Int16 dataTmp = pack[i];
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
};

class BasicDataReader {
private:
	PackData* data;
	Int16 byteId;
	UInt16 contentSize;
public:
	BasicDataReader() {
	}
	BasicDataReader(PackData* data) {
		this->data = data;
		reset();
	}
 	void init(PackData* data) {
 		this->data = data;
 		reset();
 	}
	float parse() {
	   UInt8 format = (UInt16)data->content[byteId++];
	   UInt16 byte0 = (UInt16)data->content[byteId++];
	   UInt16 byte1 = (UInt16)data->content[byteId++]<<8;
	   float divider = 1<<format;
	   float res = (float)((Int16)(byte0 + byte1))/divider;
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

class BasicDataWriter {
private:
	PackData* data;
	Int16 byteId;
public:
	BasicDataWriter(PackData* data) {
		this->data = data;
		reset();
	}
	BasicDataWriter() {
	}
	void init(PackData* data) {
		this->data = data;
		reset();
	}
	void write(float value, UInt8 format) {
		float mult = 1<<format;
		data->content[byteId++] = format;
		Int32 resVal = value*mult;
	    data->content[byteId++] = resVal;
	    data->content[byteId++] = resVal>>8;
	    data->setSizes(byteId);
	}
	void reset() {
		byteId = 0;
	}
};

class BasicPack: public Pack_ {
	BasicCrc crc;
public:
	BasicPack(PackHeader* header, PackData* data, UInt8* packBuf, UInt16 bufSize):
		  Pack_(header, data, &crc, packBuf, bufSize) {

	}
};

#endif /* EXCHANGE_BASIC_BASICPACK_H_ */
