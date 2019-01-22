/*
 * PultProtocol.h
 *
 *  Created on: 24 дек. 2014 г.
 *      Author: CNO
 */

#ifndef PULTPROTOCOL_H_
#define PULTPROTOCOL_H_
#include <ExchangeUtils/ExchangeData/ExchangeData.h>
#include <ExchangeUtils/ExchangeDrivers/UartDriver.h>
#include <ExchangeUtils/ExchangeUtils.h>
#include <ExchangeUtils/PackParsers/CommonPackParser.h>

class PultProtocol {
private:
	LoopByteArray byteArray;
	ExchangeCrc16 crc16;
	UInt8 txBytes[128];
	ExchangeDriver* driver;
public:
	PultProtocol(ExchangeDriver* driver);
	void transmit(ExchangeDataList* dataList);
};


#endif /* PULTPROTOCOL_H_ */
