/*
 * UartDriver.cpp
 *
 *  Created on: 21 но€б. 2014 г.
 *      Author: CNO
 */

#include <../GyConCommon/ExchangeDrivers/UartDriver.h>
#include <../GyConCommon/ExchangeDrivers/UARTTiva.h>
#include <driverlib/uart.h>
#include <ti/sysbios/BIOS.h>

UartDriver::UartDriver(UInt8 uartId, UInt32 baud, UInt16 readTimeout):
	uartId(uartId),
	baud(baud),
	readTimeout(readTimeout),
	writeTimeout(writeTimeout) {
	    fifoTimeout = ((2*8*10*1000)/baud) + 1;
	    UART_Params_init((UART_Params*)&uartParams);
//	    uartTivaParams.fifolevel = UART_FIFO_RX1_8;
	    uartTivaParams.fifolevel = UART_FIFO_RX6_8;
	    uartTivaParams.rtSaveRead = true;

	    uartParams.custom = (UARTTiva_params *) &uartTivaParams;
		//uartParams.readTimeout = BIOS_WAIT_FOREVER;
		uartParams.writeTimeout = BIOS_WAIT_FOREVER;
		uartParams.writeDataMode = UART_DATA_BINARY;
		uartParams.readDataMode = UART_DATA_BINARY;
		uartParams.readReturnMode = UART_RETURN_FULL;
		uartParams.readEcho = UART_ECHO_OFF;
		uartParams.baudRate = baud;
		uartParams.readTimeout = readTimeout;
		uart = UART_open(uartId, (UART_Params*)&uartParams);
		if (uart == NULL) {
		    //log
		}
}

bool UartDriver::updateUartParams(UInt32 baud_, UInt16 readTimeout_)
{

    UART_close(uart);

    baud=baud_;
    readTimeout=readTimeout_;

    fifoTimeout = ((2*8*10*1000)/baud) + 1;
    UART_Params_init((UART_Params*)&uartParams);
    uartTivaParams.fifolevel = UART_FIFO_RX6_8;
    uartTivaParams.rtSaveRead = true;

    uartParams.custom = (UARTTiva_params *) &uartTivaParams;
    uartParams.writeTimeout = BIOS_WAIT_FOREVER;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = baud;
    uartParams.readTimeout = readTimeout;
    uart = UART_open(uartId, (UART_Params*)&uartParams);
    if (uart == NULL) { return false; }
    return true;

}

Int16 UartDriver::read(UInt8* buf, UInt16 size) {
	UARTTiva_Object*  object = (UARTTiva_Object*)uart->object;
    Int16 realRead = UART_read(uart, buf, size);
    if (realRead<0) return UART_DRIVER_ERROR;
    if (object->recieveTimeoutFlag) return UART_DRIVER_RX_TIMEOUT;
	return realRead;
}

Int16 UartDriver::write(UInt8* buf, UInt16 size) {
//  UARTTiva_Object*  object = (UARTTiva_Object*)uart->object;
//  UInt16 txTimeout = ((((size*8*10*1000))>>2)/baud) + 1;
//  UInt16 writeTimeout = txTimeout + fifoTimeout;
//  object->writeTimeout = writeTimeout;
    return UART_write(uart, buf, size);
}

void UartDriver::reset(void) {
    UARTTiva_reset(uart);
}

//-------------------------------------------------------
UartDriver2::UartDriver2(UInt8 uartId, UInt32 baud, UInt16 readTimeout):
    uartId(uartId),
    baud(baud),
    readTimeout(readTimeout),
    writeTimeout(writeTimeout) {
        fifoTimeout = ((2*8*10*1000)/baud) + 1;
        UART_Params_init((UART_Params*)&uartParams);
//      uartTivaParams.fifolevel = UART_FIFO_RX1_8;
        uartTivaParams.fifolevel = UART_FIFO_RX7_8;
        uartTivaParams.rtSaveRead = true;

        uartParams.custom = (UARTTiva_params *) &uartTivaParams;
        //uartParams.readTimeout = BIOS_WAIT_FOREVER;
        uartParams.writeTimeout = BIOS_WAIT_FOREVER;
        uartParams.writeDataMode = UART_DATA_BINARY;
        uartParams.readDataMode = UART_DATA_BINARY;
        uartParams.readReturnMode = UART_RETURN_FULL;
        uartParams.readEcho = UART_ECHO_OFF;
        uartParams.baudRate = baud;
        uartParams.readTimeout = readTimeout;
        uart = UART_open(uartId, (UART_Params*)&uartParams);
        if (uart == NULL) {
            //log
        }
}

bool UartDriver2::updateUartParams(UInt32 baud_, UInt16 readTimeout_)
{

    UART_close(uart);

    baud=baud_;
    readTimeout=readTimeout_;

    fifoTimeout = ((2*8*10*1000)/baud) + 1;
    UART_Params_init((UART_Params*)&uartParams);
    uartTivaParams.fifolevel = UART_FIFO_RX7_8;
    uartTivaParams.rtSaveRead = true;

    uartParams.custom = (UARTTiva_params *) &uartTivaParams;
    uartParams.writeTimeout = BIOS_WAIT_FOREVER;
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = baud;
    uartParams.readTimeout = readTimeout;
    uart = UART_open(uartId, (UART_Params*)&uartParams);
    if (uart == NULL) { return false; }
    return true;

}

 Int16 UartDriver2::read(UInt8* buf, UInt16 size) {
    UARTTiva_Object*  object = (UARTTiva_Object*)uart->object;
    Int16 realRead = UART_read(uart, buf, size);
    if (realRead<0) return UART_DRIVER_ERROR;
    if (object->recieveTimeoutFlag) return UART_DRIVER_RX_TIMEOUT;
    return realRead;
}



 Int16 UartDriver2::write(UInt8* buf, UInt16 size) {
//	UARTTiva_Object*  object = (UARTTiva_Object*)uart->object;
//	UInt16 txTimeout = ((((size*8*10*1000))>>2)/baud) + 1;
//	UInt16 writeTimeout = txTimeout + fifoTimeout;
//	object->writeTimeout = writeTimeout;
	return UART_write(uart, buf, size);
}

void UartDriver2::reset(void) {
	UARTTiva_reset(uart);
}
