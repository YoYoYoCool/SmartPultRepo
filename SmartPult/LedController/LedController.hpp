/*
 * LedController.hpp
 *
 *  Created on: 01 дек. 2015 г.
 *      Author: AhrameshinGN
 */

#ifndef LEDCONTROLLER_LEDCONTROLLER_HPP_
#define LEDCONTROLLER_LEDCONTROLLER_HPP_
#include "../GyConCommon/dataTypes.h"

#define REG_COUNT 3


enum LedState
{
	LED_UP=0,
	LED_DOWN
};

enum LedID
{
	LED_MOTOR=0,
	LED_JOYSTIC,
	MOTION_LED1,
	MOTION_LED4,
	MOTION_LED2,
	MOTION_LED5,
	MOTION_LED3,
	MOTION_LED6,
	LED9,
	LED10,
	LED11,
	LED12,
	LED13,
	LED14,
	LED15,
	LED16,
	LED17,
	LED18,
	LED19,
	LED20,
	LED21,
	LED22,
	LED23,
	LED24,
	LEDCOUNT
};

class LedData
{
	public:
		LedData(UInt8* buffer, UInt8 bufferLen );
		UInt8 getByte(UInt8 id);
		UInt8 getDataLen();
		bool setLed(LedID id);
		bool resetLed(LedID id);
		void resetAll();
		void setAll();
	private:
		UInt8* data;
		UInt8  dataLen;
};

class LedControllerExchangeDriver
{
	public:
		virtual void init()=0;
		virtual void send(LedData* content)=0;
		virtual void resetUP()=0;
		virtual void resetDOWN()=0;
};

class SimpleSpiExchangeDriver: public LedControllerExchangeDriver
{
	public:
		SimpleSpiExchangeDriver();
		virtual void init();
		virtual void send(LedData* content);
		virtual void resetUP();
		virtual void resetDOWN();
};

class LedController
{
	public:

		LedController();
		LedController(LedControllerExchangeDriver* dr);
		LedData* getData();
		void invalidate();
		void compareAndTransmit(LedID id, LedState state);
		void setDriver(LedControllerExchangeDriver* driver);
		LedControllerExchangeDriver* getDriver();
	private:
		LedControllerExchangeDriver* driver;
		LedData data;
};






#endif /* LEDCONTROLLER_LEDCONTROLLER_HPP_ */
