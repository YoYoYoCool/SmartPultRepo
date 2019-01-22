/*
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  RedistributionsBoard_PULT_GYCON_UART of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __BOARD_H
#define __BOARD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DK_TM4C129X.h"
#include <inc/hw_memmap.h>
#include <../GyConCommon/DataTypes.h>
#include <driverlib/adc.h>
#include <driverlib/sysctl.h>
#include <ti/sysbios/knl/Task.h>

#include <ti/drivers/SDSPI.h>
//#include <xdc/runtime/System.h>

#define Board_initGeneral    	DK_TM4C129X_initGeneral
#define Board_initGPIO       	DK_TM4C129X_initGPIO
#define Board_initUART       	DK_TM4C129X_initUART
#define Board_initSDSPI		 	DK_TM4C129X_initSDSPI

#define Board_WATCHDOG 			DK_TM4C129X_WATCHDOG0
#define Board_initWDT   		DK_TM4C129X_initWatchdog

#define GyConBoard_initPWM      EK_TM4C1294XL_initPWM
#define GyConBoard_BrightPwm    EK_TM4C1294XL_PWM1

#define Board_PULT_GYCON_UART 	EK_TM4C1294XL_UART4
#define Board_PULT_ALT_UART 	EK_TM4C1294XL_UART5
#define Board_PULT_GYCON_MOTION_SYNC 	EK_TM4C1294XL_UART7
#define Board_PULT_RS485RW  	EK_TM4C1294XL_PULT_RS485RW
#define Board_PULTALT_RS485RW  	EK_TM4C1294XL_PULT_ALT_RS485RW
#define Board_SDSPI0			DK_TM4C129X_SDSPI0
#define Board_RS485_READ_MODE (0)
#define Board_RS485_WRITE_MODE (~0)
#define Board_RS485x3_TXE       EK_TM4C1294XL_RS485_3_TXE

#define Board_PIN_LOW (0)
#define Board_PIN_HIGH (~0)

#define ADC_SEQUENSE0 0
#define ADC_SEQUENCE_PRIORITY0 0

#define ADC_PAN_CHANNAL ADC_CTL_CH0
#define ADC_DUTCH_CHANNAL ADC_CTL_CH20
#define ADC_TILT_CHANNAL ADC_CTL_CH18

#define ADC_PAN_WHEEL_CHANNAL ADC_CTL_CH3
#define ADC_ZOOM_CHANNAL ADC_CTL_CH19

#define ADC_SLOW_CHANNAL ADC_CTL_CH2

#define ADC_TILT_WHEEL_CHANNAL ADC_CTL_CH1

#define SIGNAL_PAN 0
#define SIGNAL_DUTCH 1
#define SIGNAL_TILT 2
#define SIGNAL_PAN_WHEEL 3
#define SIGNAL_ZOOM 4
#define SIGNAL_SLOW 5
#define SIGNAL_TILT_WHEEL 6


#define Debug_Pin_PN5 EK_TM4C1294XL_Debug_Pin_PN5

#define Board_SIGNAL_MUX0  EK_TM4C1294XL_SIGNAL_MUX0
#define Board_SIGNAL_MUX1  EK_TM4C1294XL_SIGNAL_MUX1
#define Board_SIGNAL_MUX2  EK_TM4C1294XL_SIGNAL_MUX2
#define Board_SIGNAL_MUX3  EK_TM4C1294XL_SIGNAL_MUX3

#define Board_BUTTON_MUX0  EK_TM4C1294XL_BUTTON_1_16_MUX0
#define Board_BUTTON_MUX1  EK_TM4C1294XL_BUTTON_1_16_MUX1
#define Board_BUTTON_MUX2  EK_TM4C1294XL_BUTTON_1_16_MUX2
#define Board_BUTTON_MUX3  EK_TM4C1294XL_BUTTON_1_16_MUX3

#define Board_BUTTON_1_16_READ_PIN  EK_TM4C1294XL_BUTTON_1_16_READ_PIN
#define Board_BUTTON_17_32_READ_PIN  EK_TM4C1294XL_BUTTON_17_32_READ_PIN
#define Board_BUTTON_33_48_READ_PIN  EK_TM4C1294XL_BUTTON_33_48_READ_PIN
//Инициализация SDSPI Fatfs
class FileSystemState
{
	SDSPI_Handle sdspiHandle;
	SDSPI_Params sdspiParams;

	public:
		volatile bool isReady;
		FileSystemState(){isReady=false;}
		bool initFS()
		{
			SDSPI_Params_init(&sdspiParams);
			sdspiHandle = SDSPI_open(Board_SDSPI0, 0, &sdspiParams);
			if (sdspiHandle == NULL) {isReady=false;return false;}
			isReady=true;
			return true;
		}
};
//---------------------------------------------------------------------------
//Управление регистрами
//----------------------------------------------------------------------------
#define BoardLedLatchPin EK_TM4C1294XL_LED_LATCH
#define BoardLedResetPin EK_TM4C1294XL_LED_RESET


//PL4
class RegisterControl
{
	public:
		void upLedLatch()	{	GPIO_write(BoardLedLatchPin, Board_PIN_HIGH);}
		void downLedLatch()	{	GPIO_write(BoardLedLatchPin, Board_PIN_LOW);}
		void upLedReset()  	{	GPIO_write(BoardLedResetPin, Board_PIN_HIGH);}
		void downLedReset()	{	GPIO_write(BoardLedResetPin, Board_PIN_LOW);}
};

//-----------------------------------------------------------------------------

class SignalsReader {
private:
	UInt8 id;
	UInt32 adcResult[7];
public:
	SignalsReader() {
		SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
		ADCClockConfigSet(ADC0_BASE, ADC_CLOCK_SRC_PIOSC|ADC_CLOCK_RATE_FULL, 1);
		ADCHardwareOversampleConfigure(ADC0_BASE, 16);
		ADCSequenceDisable(ADC0_BASE, ADC_SEQUENSE0);
		ADCSequenceConfigure(ADC0_BASE, ADC_SEQUENSE0, ADC_TRIGGER_PROCESSOR, ADC_SEQUENCE_PRIORITY0);
		ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENSE0, 0, ADC_PAN_CHANNAL);
		ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENSE0, 1, ADC_DUTCH_CHANNAL);
		ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENSE0, 2, ADC_TILT_CHANNAL);
		ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENSE0, 3, ADC_PAN_WHEEL_CHANNAL);
		ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENSE0, 4, ADC_ZOOM_CHANNAL);
		ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENSE0, 5, ADC_SLOW_CHANNAL );
		ADCSequenceStepConfigure(ADC0_BASE, ADC_SEQUENSE0, 6, ADC_TILT_WHEEL_CHANNAL | ADC_CTL_END);

		ADCSequenceEnable(ADC0_BASE, ADC_SEQUENSE0);
		//ADCIntRegister(ADC0_BASE, 3, &adcHwi);
		ADCIntDisable(ADC0_BASE, ADC_SEQUENSE0);
		id = 0;
	}
	void setMultiplexer(UInt8 id) {
        UInt8 bit0 = (id&1);
 		UInt8 bit1 = (id&2);
		UInt8 bit2 = (id&4);
		UInt8 bit3 = (id&8);
		if (bit0) {
			GPIO_write(Board_SIGNAL_MUX0, Board_PIN_HIGH);
		} else {
			GPIO_write(Board_SIGNAL_MUX0, Board_PIN_LOW);
		}
		if (bit1) {
			GPIO_write(Board_SIGNAL_MUX1, Board_PIN_HIGH);
		} else {
			GPIO_write(Board_SIGNAL_MUX1, Board_PIN_LOW);
		}
		if (bit2) {
			GPIO_write(Board_SIGNAL_MUX2, Board_PIN_HIGH);
		} else {
			GPIO_write(Board_SIGNAL_MUX2, Board_PIN_LOW);
		}
		if (bit3) {
			GPIO_write(Board_SIGNAL_MUX3, Board_PIN_HIGH);
		} else {
			GPIO_write(Board_SIGNAL_MUX3, Board_PIN_LOW);
		}
		this->id = id;
	}
	UInt32* read() {
		ADCProcessorTrigger(ADC0_BASE, ADC_SEQUENSE0);
		{
	        Task_sleep(1);
		} while(ADCBusy(ADC0_BASE));
	    ADCSequenceDataGet(ADC0_BASE, ADC_SEQUENSE0, adcResult);
        return adcResult;
	}
	UInt8 getMultiplexer() {
       return id;
	}
};

class ButtonsReader {
private:
	UInt8 id;
public:
	ButtonsReader() {
		id = 0;
	}
	void setMultiplexer(UInt8 id) {
		UInt8 bit0 = (id & 1);
		UInt8 bit1 = (id & 2);
		UInt8 bit2 = (id & 4);
		UInt8 bit3 = (id & 8);
		if (bit0) {
			GPIO_write(Board_BUTTON_MUX0, Board_PIN_HIGH);
		} else {
			GPIO_write(Board_BUTTON_MUX0, Board_PIN_LOW);
		}
		if (bit1) {
			GPIO_write(Board_BUTTON_MUX1, Board_PIN_HIGH);
		} else {
			GPIO_write(Board_BUTTON_MUX1, Board_PIN_LOW);
		}
		if (bit2) {
			GPIO_write(Board_BUTTON_MUX2, Board_PIN_HIGH);
		} else {
			GPIO_write(Board_BUTTON_MUX2, Board_PIN_LOW);
		}
		if (bit3) {
			GPIO_write(Board_BUTTON_MUX3, Board_PIN_HIGH);
		} else {
			GPIO_write(Board_BUTTON_MUX3, Board_PIN_LOW);
		}
		this->id = id;
	}

	bool read(Uint8 pin) {
       if (!GPIO_read(pin)) {
    	   return false;
       } else {
    	   return true;
       }
	}

	UInt8 getMultiplexer() {
	   return id;
	}
};

#ifdef __cplusplus
}
#endif

#endif /* __BOARD_H */
