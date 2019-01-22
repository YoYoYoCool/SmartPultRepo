/*
 * Pack.h
 *
 *  Created on: 09 июня 2015 г.
 *      Author: Александр
 */

#ifndef EXCHANGE_EXCHANGE_H_
#define EXCHANGE_EXCHANGE_H_
#include <../GyConCommon/DataTypes.h>
#include <../GyConCommon/ExchangeDrivers/UartDriver.h>
#include <../GyConCommon/Exchange/ExchangePack.h>

typedef union {
	UInt8 all;
	struct {
		UInt8 driverError :1;
		UInt8 headerError :1;
		UInt8 dataError :1;
		UInt8 crcError :1;
		UInt8 recieveTimeout :1;
		UInt8 sizeError :1;
	} bit;
} RecieverFaults;

Int16 sizeError;
Int16 headerError;
Int16 recieveTimeout;
Int16 dataError;
Int16 crcError;


class Reciever {
public:
	RecieverFaults faults;
	Reciever(UartDriver* driver) :
			driver(driver) {
	}
	bool recievePack(Pack_* pack) {
		PackHeader* header = pack->header;
		PackElement_* data = pack->data;
		PackElement_* crc = pack->crc;
		faults.all = 0;
		Int32 totalReadCount = driver->read(pack->buf, pack->bufSize); //readData(pack, 0, header->size);
		if (totalReadCount < header->size) {
			if (!checkDriverError(totalReadCount)) {
				faults.bit.sizeError = 1;
			}
			return false;
		}
		if (!header->parse(pack->buf)) {
			faults.bit.headerError = 1;
			driver->read(pack->buf, pack->bufSize);
			driver->reset();
			return false;
		}
		if (header->packSize - header->size - crc->size < 0) {
			faults.bit.sizeError = 1;
			return false;
		}
		if (header->packSize > pack->bufSize) {
			faults.bit.sizeError = 1;
			return false;
		}
		if (totalReadCount < header->packSize) {
			Int16 readCount = readData(pack, totalReadCount,
					header->packSize - totalReadCount);
			if (readCount < 0) {
				checkDriverError(readCount);
				return false;
			}
			totalReadCount += readCount;
		}
		if (!data->parse(&pack->buf[header->size])) {
			faults.bit.dataError = 1;
			return false;
		}
		if (!crc->parse(&pack->buf[header->size + data->size])) {
			faults.bit.crcError = 1;
			return false;
		}
		return true;
	}
private:
	Int32 readData(Pack_* pack, Int32 offset, Int32 size) {
		Int16 totalReadCount = 0;
		Int16 readCount = 0;
		while (totalReadCount < size) {
			readCount = driver->read(&pack->buf[offset + totalReadCount],
					size - totalReadCount);
			if (readCount < 0) {
				return readCount;
			}
			totalReadCount += readCount;
		}
		return totalReadCount;
	}
	bool checkDriverError(Int16 error) {
		bool err = false;
		if (error == UART_DRIVER_ERROR) {
			faults.bit.driverError = 1;
			err = true;
		} else if (error == UART_DRIVER_RX_TIMEOUT) {
			faults.bit.recieveTimeout = 1;
			err = true;
		}
		return err;
	}
	UartDriver* const driver;
};

class Transmitter {
public:
	Transmitter(UartDriver* driver) :
			driver(driver) {
	}
	Int16 transmitPack(Pack_* pack) {
		Int16 writeCount = pack->header->write(pack->buf);
		writeCount += pack->data->write(&pack->buf[writeCount]);
		writeCount += pack->crc->write(&pack->buf[writeCount]);
		Int16 res = driver->write(pack->buf, writeCount);
		return writeCount;
	}
private:
	UartDriver* const driver;
	volatile Int16 timeoutCounter;
};

class Exchange {
public:
	Transmitter transmitter;
	Reciever reciever;
	bool connected;
	Int8 disconnectedCounter;
	UInt16 errsCounter;
	UInt16 okCounter;
	Exchange(UartDriver* driver) :
			transmitter(driver), reciever(driver) {
		connected = false;
		disconnectedCounter = 0;
		errsCounter = 0;
		okCounter = 0;
	}
	bool recieve(Pack_* requestPack) {
		okCounter++;
		if (reciever.recievePack(requestPack)) {
			disconnectedCounter = 7;
			connected = true;
			return true;
		} else {
			errsCounter++;
			if (disconnectedCounter <= 0) {
				connected = false;
			} else {
				disconnectedCounter--;
			}
		}
		return false;
	}
	Int16 transmit(Pack_* responcePack) {
		return transmitter.transmitPack(responcePack);
	}

};

//		PackHeader* header = pack->header;
//		PackElement_* data = pack->data;
//		PackElement_* crc = pack->crc;
//		faults.all = 0;
//		Int32 readCount = driver->read(pack->buf, pack->bufSize);
//		if (readCount<0) {
//			faults.bit.driverError = 1; return false;
//		}
//		if (readCount<header->size) {
//			driver->reset();
//			sizeError++;
//			faults.bit.sizeError = 1; return false;
//		}
//		if (!header->parse(pack->buf)) {
//			driver->reset();
//			headerError++;
//			faults.bit.headerError = 1;	return false;
//		}
//		if (header->packSize - header->size - crc->size < 0) {
//			driver->reset();
//			sizeError++;
//			faults.bit.sizeError = 1; return false;
//		}
//		if (header->packSize!=readCount) {
//			driver->reset();
//			recieveTimeout++;
//			faults.bit.recieveTimeout = 1; return false;
//		}
//		if (!data->parse(&pack->buf[header->size])) {faults.bit.dataError = 1;dataError++; return false;}
//		if (!crc->parse(&pack->buf[header->size + data->size])) {faults.bit.crcError = 1;crcError++; return false;}

#endif /* EXCHANGE_EXCHANGE_H_ */
