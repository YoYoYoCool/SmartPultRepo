/*
 * SimpleExchangeDriver.cpp
 *
 *  Created on: 01 дек. 2015 г.
 *      Author: AhrameshinGN
 */
#include "LedController.hpp"
#include <stdint.h>
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "../Board.h"

RegisterControl controller;

SimpleSpiExchangeDriver::SimpleSpiExchangeDriver()
{

}
void SimpleSpiExchangeDriver::init()
{
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
	GPIOPinConfigure(GPIO_PG5_SSI2XDAT0);
    GPIOPinConfigure(GPIO_PG7_SSI2CLK);
    GPIOPinTypeSSI(GPIO_PORTG_BASE, GPIO_PIN_7 | GPIO_PIN_5);

   	UInt32 t=SysCtlClockGet();

    SSIConfigSetExpClk(SSI2_BASE, t, SSI_FRF_MOTO_MODE_0,	SSI_MODE_MASTER, 50000, 8);
    SSIEnable(SSI2_BASE);
    resetUP();

}
void SimpleSpiExchangeDriver::send(LedData* content)
{
	controller.downLedLatch();
	for(UInt8 i=0;i!=content->getDataLen();i++)
	{
		SSIDataPut(SSI2_BASE, content->getByte(content->getDataLen()-i-1));
	}

	for(UInt32 i=0;i!=0xFFF;i++)
	{
		if(!SSIBusy(SSI2_BASE))
		{
			break;
		}
	}
	controller.upLedLatch();

}
void SimpleSpiExchangeDriver::resetUP()
{
	controller.upLedReset();
}
void SimpleSpiExchangeDriver::resetDOWN()
{
	controller.downLedReset();
}

