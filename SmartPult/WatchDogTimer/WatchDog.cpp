/*
 * WatchDog.cpp
 *
 *  Created on: 09 дек. 2015 г.
 *      Author: AhrameshinGN
 */

#include "WatchDog.hpp"
#include "../Board.h"

WatchDog::WatchDog(bool inResetMode, unsigned int reloadValue)
{
	Watchdog_Params_init(&params);
	if(inResetMode){params.resetMode =   Watchdog_RESET_ON;}
	else{params.resetMode =   Watchdog_RESET_OFF;}
	params.callbackFxn = callBack;
	watchdog = Watchdog_open(Board_WATCHDOG, &params);
	if (watchdog != NULL)
	{
		Watchdog_setReload(watchdog,reloadValue);
	}
}

WatchDog::WatchDog()
{
	curCount=0;
}

bool WatchDog::registerKey(unsigned int key)
{
	if(curCount>=KEY_COUNT)	{return false;	}

	wdFlags[curCount]=false ;
	wdFlagsValue[curCount] =key;
	curCount++;
	return true;
}


void WatchDog::useKey(unsigned int key)
{
	for(unsigned char i=0;i!=curCount;i++)
	{
		if(wdFlagsValue[i]==key){wdFlags[i]=true;}
	}
	if(compareFlags())
	{
		reset();
	}
}
void WatchDog::reset()
{
	Watchdog_clear(watchdog) ;
	resetFlags();
}
void WatchDog::callBack(UArg a)
{
	int t=0;
	t++;
	t--;
}
bool WatchDog::isInitialize()
{
	if(watchdog==NULL){return false;}
	return true;
}

bool WatchDog::init(bool inResetMode, unsigned int reloadValue)
{
	Watchdog_Params_init(&params);
	if(inResetMode){params.resetMode =   Watchdog_RESET_ON;}
	else{params.resetMode =   Watchdog_RESET_OFF;}
	params.callbackFxn = callBack;
	watchdog = Watchdog_open(Board_WATCHDOG, &params);
	if (watchdog != NULL)
	{
		Watchdog_setReload(watchdog,reloadValue);
		return true;
	}
	return false;
}

