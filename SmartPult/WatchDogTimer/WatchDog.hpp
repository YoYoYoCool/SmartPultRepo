/*
 * WatchDog.hpp
 *
 *  Created on: 09 дек. 2015 г.
 *      Author: AhrameshinGN
 */

#ifndef WATCHDOGTIMER_WATCHDOG_HPP_
#define WATCHDOGTIMER_WATCHDOG_HPP_
#include <ti/drivers/Watchdog.h>
#define KEY_COUNT 16

enum WatchDogKeys
{
	WD_KEY1= 0x000000AE,
	WD_KEY2= 0x000001AE,
	WD_KEY3= 0x000002AE,
	WD_KEY4= 0x000003AE,
	WD_KEY5= 0x000004AE,
	WD_KEY6= 0x000005AE,
	WD_KEY7= 0x000006AE,
	WD_KEY8= 0x000007AE,
	WD_KEY9= 0x000008AE,
	WD_KEY10=0x000009AE,
	WD_KEY11=0x000010AE,
	WD_KEY12=0x000011AE,
	WD_KEY13=0x000012AE,
	WD_KEY14=0x000013AE,
	WD_KEY15=0x000014AE,
	WD_KEY16=0x000015AE
};

class WatchDog
{
	Watchdog_Params params;
	Watchdog_Handle watchdog;

	volatile bool wdFlags[KEY_COUNT] ;
	volatile unsigned int wdFlagsValue[KEY_COUNT] ;
	volatile unsigned char curCount;

	inline bool compareFlags()
	{
		bool ret=true;
		for(unsigned char i=0;i!=curCount;i++)
		{
			ret=(ret&&wdFlags[i]);
		}
		return ret;
	}

	inline void resetFlags()
	{
		for(unsigned char i=0;i!=curCount;i++)
		{
			wdFlags[i]=false;
		}
	}

	public:
		WatchDog(bool inResetMode, unsigned int reloadValue);
		WatchDog();
		void reset();
		static void callBack(UArg a);
		bool registerKey(unsigned int key);
		void useKey(unsigned int key);
		bool isInitialize();
		bool init(bool inResetMode, unsigned int reloadValue);


};




#endif /* WATCHDOGTIMER_WATCHDOG_HPP_ */
