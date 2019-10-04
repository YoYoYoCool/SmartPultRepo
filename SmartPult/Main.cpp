
#include <xdc/std.h>

#include <xdc/runtime/Log.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/System.h>
#include <xdc/cfg/global.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/knl/Task.h>
#include "Board.h"
#include "PULT_Indicator/LCD Classes.h"
#include <Pult/Pult.h>

#include <driverlib/flash.h>
#include "inc/hw_flash.h"
#include "inc/hw_types.h"

#include "ExtrenalDeviceSynchro/RS232Syncro.hpp"

extern "C" {
    void pultExchangeTaskFxn();
    void pultAltExchangeTask();
    void pultDriverTaskFxn();
	void taskFxn();
	void motionSynchroFxn();
	void menu_taskFxn(); //таск для обслуживания меню
}

Pult pult(((Semaphore_Handle*)(&exchangeTaskSemafore)),((Semaphore_Handle*)(&exchangeAltSem)));

bool secure()
{
	if(FlashProtectGet(0x00004000)==FlashExecuteOnly){return true;}
	HWREG(FLASH_FMPRE0)=0xFFFF00FF;
	HWREG(FLASH_FMPPE0)=0xFFFF00FF;
	if(FlashProtectSave()!=0){return false;}
	return true;
}

Int main(){
    Board_initGeneral();
    Board_initGPIO();
    Board_initUART();
    Board_initWDT();
    Board_initSDSPI();
    pult.initWatchDog();
    pultIndikatorInit();
    GyConBoard_initPWM();

    GPIO_write(Board_RS485x3_TXE, Board_RS485_WRITE_MODE);

    BIOS_start();    /* does not return */
    return(0);
}

//Пример использования протокола пульта
void taskFxn()
{
}

//extern MotionSynchroTransmiter* motionSynchroTransmiter;
void motionSynchroFxn()
{
    ExtrSyncroization::ExtrenalDevieExchDriver::exchTask();
}

void pultExchangeTaskFxn() {
	pult.exchangeTask();
}

void pultAltExchangeTask()
{
    pult.exchangeAlternativeTask();
}

void pultDriverTaskFxn() {
    pult.driverTask();
}

void menu_taskFxn() {
	pultIndikator_Task(&pult);
}

