/*
 * LedController.cpp
 *
 *  Created on: 01 дек. 2015 г.
 *      Author: AhrameshinGN
 */
#include "LedController.hpp"
#include "../Board.h"

UInt8 rawData[REG_COUNT];

LedController::LedController():
		data(rawData, REG_COUNT)
{

}

LedController::LedController(LedControllerExchangeDriver* dr):
	data(rawData, REG_COUNT),
	driver(dr)
{
	//TODO init driver!!!!!!!!!!!
}

LedData* LedController::getData(){return &data;}
void LedController::invalidate()
{
//	driver->resetDOWN();
//	Task_sleep(1);
//	driver->resetUP();
	driver->send(&data);
}
void LedController::setDriver(LedControllerExchangeDriver* driver)
{
	this->driver=driver;
	this->driver->resetUP();
}
LedControllerExchangeDriver* LedController::getDriver(){return driver;}
void LedController::compareAndTransmit(LedID id, LedState state)
{
	UInt8 byteID=id>>3;
	if(byteID>=data.getDataLen()){return;}
	UInt8 bitID=id-(byteID<<3);
	if(bitID>=8){return;}

	UInt8 tmp=0x01;
	tmp=tmp<<bitID;
	tmp=data.getByte(byteID)&tmp;

	switch(state)
	{
		case LED_UP:
			if(tmp!=0){return;}
			data.setLed(id);
			invalidate();
			return;
		case LED_DOWN:
			if(tmp==0){return;}
			data.resetLed(id);
			invalidate();
			return;
	}

}

LedData::LedData(UInt8* buffer, UInt8 bufferLen ):
	dataLen(bufferLen),
	data(buffer){}

UInt8 LedData::getByte(UInt8 id)
{
	if(id>=dataLen){return 0;}
	return data[id];
}
UInt8 LedData::getDataLen()
{
	return dataLen;
}
bool LedData::setLed(LedID id)
{
	UInt8 byteID=id>>3;
	if(byteID>=dataLen){return false;}
	UInt8 bitID=id-(byteID<<3);
	if(bitID>=8){return false;}
	UInt8 tmp=1;
	tmp=tmp<<bitID;
	data[byteID]|=tmp;
	return true;
}

bool LedData::resetLed(LedID id)
{
	UInt8 byteID=id>>3;
	if(byteID>=dataLen){return false;}
	UInt8 bitID=id-(byteID<<3);
	if(bitID>=8){return false;}
	UInt8 tmp=1;
	tmp=tmp<<bitID;
	tmp=~tmp;
	data[byteID]&=tmp;
	return true;
}

void LedData::resetAll()
{
	for(UInt8 i=0;i!=dataLen;i++)
	{
		data[i]=0;
	}
}

void LedData::setAll()
{
	for(UInt8 i=0;i!=dataLen;i++)
	{
		data[i]=0xFF;
	}
}




