/*
 * Rs485Driver.cpp
 *
 *  Created on: 02 авг. 2015 г.
 *      Author: Александр
 */

#include <../GyConCommon/ExchangeDrivers/UartDriver.h>
#include <../GyConCommon/ExchangeDrivers/Rs485Driver.h>
#include <ti/drivers/GPIO.h>
#include <ti/sysbios/knl/Task.h>
#include <driverlib/uart.h>
#include <stdint.h>

#define Rs485Driver_RS485_READ_MODE (0)
#define Rs485Driver_RS485_WRITE_MODE (~0)



Rs485Driver::Rs485Driver(UInt8 uartId, UInt32 baud, UInt16 readTimeout, UInt8 txEnablePin):
UartDriver(uartId, baud, readTimeout),
txEnablePin(txEnablePin) {
}

 Int16 Rs485Driver::write(UInt8* pack, UInt16 size) {
	GPIO_write(txEnablePin, Rs485Driver_RS485_WRITE_MODE);
	Int16 txCount = UartDriver::write(pack, size);
	UARTTiva_HWAttrs const *hwAttrs = (UARTTiva_HWAttrs *)((UART_Handle)uart)->hwAttrs;
	while (UARTBusy(hwAttrs->baseAddr)) {

	}
	GPIO_write(txEnablePin, Rs485Driver_RS485_READ_MODE);
	return txCount;
}

//----------------------------------------------------------------------------------------------

 Rs485Driver2::Rs485Driver2(UInt8 uartId, UInt32 baud, UInt16 readTimeout, UInt8 txEnablePin):
 UartDriver2(uartId, baud, readTimeout),
 txEnablePin(txEnablePin) {
 }



 Int16 Rs485Driver2::write(UInt8* pack, UInt16 size) {
     GPIO_write(txEnablePin, Rs485Driver_RS485_WRITE_MODE);
     Int16 txCount = UartDriver2::write(pack, size);
     UARTTiva_HWAttrs const *hwAttrs = (UARTTiva_HWAttrs *)((UART_Handle)uart)->hwAttrs;
     while (UARTBusy(hwAttrs->baseAddr)) {

     }
     GPIO_write(txEnablePin, Rs485Driver_RS485_READ_MODE);
     return txCount;
 }

