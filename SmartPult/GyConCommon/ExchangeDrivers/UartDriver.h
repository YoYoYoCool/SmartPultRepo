/*
 * RtiUartBlockDriver.h
 *
 *  Created on: 21 но€б. 2014 г.
 *      Author: CNO
 */

#ifndef RTIUARTBLOCKDRIVER_H_
#define RTIUARTBLOCKDRIVER_H_

#include <ti/drivers/UART.h>

#include <../GyConCommon/ExchangeDrivers/UARTTiva.h>


#define UART_DRIVER_ERROR -1
#define UART_DRIVER_RX_TIMEOUT -2

class UartDriver {
private:
    const UInt8 uartId;
    UInt32 baud;
    UInt16 readTimeout;
    UInt16 fifoTimeout;
    const UInt16 writeTimeout;
    volatile UART_Params uartParams;
    UARTTiva_params uartTivaParams;
protected:
    volatile UART_Handle uart;
public:
    UartDriver(UInt8 uartId, UInt32 baud, UInt16 readTimeout);

	Int16 read(UInt8* pack, UInt16 size);

	virtual Int16 write(UInt8* pack, UInt16 size);
	void reset(void);
	bool updateUartParams(UInt32 baud_, UInt16 readTimeout_);
};
#endif /* RTIUARTBLOCKDRIVER_H_ */
