/*
 * Rs485Driver.h
 *
 *  Created on: 02 авг. 2015 г.
 *      Author: Александр
 */

#ifndef GYCONCOMMON_EXCHANGEDRIVERS_RS485DRIVER_H_
#define GYCONCOMMON_EXCHANGEDRIVERS_RS485DRIVER_H_

class Rs485Driver: public UartDriver  {
private:
	const UInt8 txEnablePin;
	UInt16 txDelay;
public:
	Rs485Driver(UInt8 uartId, UInt32 baud, UInt16 readTimeout, UInt8 txEnablePin);

	virtual Int16 write(UInt8* pack, UInt16 size);
};





class Rs485Driver2: public UartDriver  {
private:
    const UInt8 txEnablePin;
    UInt16 txDelay;
public:
    Rs485Driver2(UInt8 uartId, UInt32 baud, UInt16 readTimeout, UInt8 txEnablePin);

    virtual Int16 write(UInt8* pack, UInt16 size);
};



#endif /* GYCONCOMMON_EXCHANGEDRIVERS_RS485DRIVER_H_ */
