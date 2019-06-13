/*
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
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

/*
 *  ======== DK_TM4C129X.c ========
 *  This file is responsible for setting up the board specific items for the
 *  DK_TM4C129X board.
 *
 *  The following defines are used to determine which TI-RTOS peripheral drivers
 *  to include:
 *     TI_DRIVERS_EMAC_INCLUDED
 *     TI_DRIVERS_GPIO_INCLUDED
 *     TI_DRIVERS_I2C_INCLUDED
 *     TI_DRIVERS_SDSPI_INCLUDED
 *     TI_DRIVERS_SPI_INCLUDED
 *     TI_DRIVERS_UART_INCLUDED
 *     TI_DRIVERS_USBMSCHFATFS_INCLUDED
 *     TI_DRIVERS_WATCHDOG_INCLUDED
 *     TI_DRIVERS_WIFI_INCLUDED
 *  These defines are created when a useModule is done on the driver in the
 *  application's .cfg file. The actual #define is in the application
 *  generated header file that is brought in via the xdc/cfg/global.h.
 *  For example the following in the .cfg file
 *     var GPIO = xdc.useModule('ti.drivers.GPIO');
 *  Generates the following
 *     #define TI_DRIVERS_GPIO_INCLUDED 1
 *  If there is no useModule of ti.drivers.GPIO, the constant is set to 0.
 *
 *  Note: a useModule is generated in the .cfg file via the graphical
 *  configuration tool when the "Add xxx to my configuration" is checked
 *  or "Use xxx" is selected.
 */

#include <stdint.h>
#include <stdbool.h>
#include <inc/hw_memmap.h>
#include <inc/hw_types.h>
#include <inc/hw_ints.h>
#include <inc/hw_gpio.h>

#include <driverlib/gpio.h>
#include <driverlib/flash.h>
#include <driverlib/sysctl.h>
#include <driverlib/i2c.h>
#include <driverlib/ssi.h>
#include <driverlib/udma.h>
#include <driverlib/pin_map.h>
#include <driverlib/pwm.h>

#include <xdc/std.h>
#include <xdc/cfg/global.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

#include "DK_TM4C129X.h"
#include "PultGlobalDefinitions.h"

#if defined(ccs)
#pragma DATA_ALIGN(1024)
#elif defined(ewarm)
#pragma data_alignment=1024
#elif defined(gcc)
__attribute__ ((aligned (1024)))
#endif
static tDMAControlTable DK_TM4C129X_DMAControlTable[32];
static bool DMA_initialized = false;

/* Hwi_Struct used in the initDMA Hwi_construct call */
static Hwi_Struct hwiStruct;


/*Void DK_TM4C129X_initServoPwm() {
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
	GPIOPinConfigure(GPIO_PK4_M0PWM6);
	GPIOPinTypePWM(GPIO_PORTK_BASE, GPIO_PIN_4);
	PWMGenConfigure(PWM0_BASE, PWM_GEN_3, PWM_GEN_MODE_UP_DOWN |
	                    PWM_GEN_MODE_NO_SYNC);
	PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, 12000);
	int period = PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3);
	PWMPulseWidthSet(PWM0_BASE, PWM_OUT_6,
	                     PWMGenPeriodGet(PWM0_BASE, PWM_GEN_3) / 4);
	PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, true);
	PWMGenEnable(PWM0_BASE, PWM_GEN_3);
}*/

#include <ti/drivers/PWM.h>
#include <ti/drivers/pwm/PWMTiva.h>
#include <driverlib/pwm.h>

PWMTiva_Object pwmTivaObjects[EK_TM4C1294XL_PWMCOUNT];

/* PWM configuration structure */
#pragma DATA_SECTION(".const:pwmTivaHWAttrs")
const PWMTiva_HWAttrs pwmTivaHWAttrs[EK_TM4C1294XL_PWMCOUNT] = {
    {
        PWM0_BASE,
        PWM_OUT_0,
        PWM_GEN_MODE_DOWN | PWM_GEN_MODE_DBG_RUN
    },
    {
        PWM0_BASE,
        PWM_OUT_1,
        PWM_GEN_MODE_DOWN | PWM_GEN_MODE_DBG_RUN
    },
};

#pragma DATA_SECTION(".const:PWM_config")
extern const PWM_Config PWM_config[] = {
    {&PWMTiva_fxnTable, &pwmTivaObjects[0], &pwmTivaHWAttrs[0]},
    {&PWMTiva_fxnTable, &pwmTivaObjects[1], &pwmTivaHWAttrs[1]},
    {NULL, NULL, NULL}
};

/*
 *  ======== EK_TM4C1294XL_initPWM ========
 */
void EK_TM4C1294XL_initPWM(void)
{
    /* Enable PWM peripherals */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

    GPIOPinConfigure(GPIO_PF1_M0PWM1);
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);

    PWM_init();
}


/*
 *  ======== DK_TM4C129X_errorDMAHwi ========
 */
static Void DK_TM4C129X_errorDMAHwi(UArg arg)
{
    System_printf("DMA error code: %d\n", uDMAErrorStatusGet());
    uDMAErrorStatusClear();
    System_abort("DMA error!!");
}

/*
 *  ======== DK_TM4C129X_initDMA ========
 */
void DK_TM4C129X_initDMA(void)
{
    Error_Block eb;
    Hwi_Params  hwiParams;

    if(!DMA_initialized){

        Error_init(&eb);

        Hwi_Params_init(&hwiParams);
        Hwi_construct(&(hwiStruct), INT_UDMAERR, DK_TM4C129X_errorDMAHwi,
                      &hwiParams, &eb);
        if (Error_check(&eb)) {
            System_abort("Couldn't construct DMA error hwi");
        }

        SysCtlPeripheralEnable(SYSCTL_PERIPH_UDMA);
        uDMAEnable();
        uDMAControlBaseSet(DK_TM4C129X_DMAControlTable);

        DMA_initialized = true;
    }
}

/*
 *  ======== DK_TM4C129X_initGeneral ========
 */
void DK_TM4C129X_initGeneral(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOH);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOK);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOL);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOM);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOP);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOQ);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOR);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOS);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOT);
}

#if TI_DRIVERS_EMAC_INCLUDED
#include <ti/drivers/EMAC.h>
#include <ti/drivers/emac/EMACSnow.h>

/*
 *  Required by the Networking Stack (NDK). This array must be NULL terminated.
 *  This can be removed if NDK is not used.
 *  Double curly braces are needed to avoid GCC bug #944572
 *  https://bugs.launchpad.net/gcc-linaro/+bug/944572
 */
NIMU_DEVICE_TABLE_ENTRY  NIMUDeviceTable[2] = {{EMACSnow_NIMUInit}, {NULL}};

/* EMAC objects */
EMACSnow_Object emacObjects[DK_TM4C129X_EMACCOUNT];

/*
 *  EMAC configuration structure
 *  Set user/company specific MAC octates. The following sets the address
 *  to ff-ff-ff-ff-ff-ff. Users need to change this to make the label on
 *  their boards.
 */
unsigned char macAddress[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

const EMACSnow_HWAttrs emacHWAttrs[DK_TM4C129X_EMACCOUNT] = {
    {EMAC0_BASE, INT_EMAC0, macAddress}
};

const EMAC_Config EMAC_config[] = {
    {
        &EMACSnow_fxnTable,
        &emacObjects[0],
        &emacHWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== DK_TM4C129X_initEMAC ========
 */
void DK_TM4C129X_initEMAC(void)
{
    uint32_t ulUser0, ulUser1;

    /* Get the MAC address */
    FlashUserGet(&ulUser0, &ulUser1);
    if ((ulUser0 != 0xffffffff) && (ulUser1 != 0xffffffff)) {
        System_printf("Using MAC address in flash\n");
        /*
         * Convert the 24/24 split MAC address from NV ram into a 32/16 split MAC
         * address needed to program the hardware registers, then program the MAC
         * address into the Ethernet Controller registers.
         */
        macAddress[0] = ((ulUser0 >>  0) & 0xff);
        macAddress[1] = ((ulUser0 >>  8) & 0xff);
        macAddress[2] = ((ulUser0 >> 16) & 0xff);
        macAddress[3] = ((ulUser1 >>  0) & 0xff);
        macAddress[4] = ((ulUser1 >>  8) & 0xff);
        macAddress[5] = ((ulUser1 >> 16) & 0xff);
    }
    else if (macAddress[0] == 0xff && macAddress[1] == 0xff &&
             macAddress[2] == 0xff && macAddress[3] == 0xff &&
             macAddress[4] == 0xff && macAddress[5] == 0xff) {
        System_abort("Change the macAddress variable to match your boards MAC sticker");
    }

    GPIOPinConfigure(GPIO_PF1_EN0LED2);
    GPIOPinConfigure(GPIO_PK4_EN0LED0);
    GPIOPinConfigure(GPIO_PK6_EN0LED1);
    GPIOPinTypeEthernetLED(GPIO_PORTF_BASE, GPIO_PIN_1);
    GPIOPinTypeEthernetLED(GPIO_PORTK_BASE, GPIO_PIN_4);
    GPIOPinTypeEthernetLED(GPIO_PORTK_BASE, GPIO_PIN_6);

    /* Once EMAC_init is called, EMAC_config cannot be changed */
    EMAC_init();
}
#endif /* TI_DRIVERS_EMAC_INCLUDED */

#include <ti/drivers/GPIO.h>
#include <ti/drivers/GPIO.h>

/* GPIO configuration structure */
#ifdef PULT_DEVELOPING_BOARD

const GPIO_HWAttrs gpioHWAttrs[EK_TM4C1294XL_GPIOCOUNT] = {
			{GPIO_PORTL_BASE, GPIO_PIN_4, GPIO_OUTPUT},
			{GPIO_PORTN_BASE, GPIO_PIN_3, GPIO_INPUT},
			{GPIO_PORTN_BASE, GPIO_PIN_2, GPIO_INPUT},
			{GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_INPUT},
			{GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_INPUT},
			{GPIO_PORTP_BASE, GPIO_PIN_1, GPIO_INPUT},

			{GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_INPUT},
			{GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_OUTPUT},
			{GPIO_PORTS_BASE, GPIO_PIN_3, GPIO_OUTPUT},
			{GPIO_PORTL_BASE, GPIO_PIN_5, GPIO_OUTPUT},
			{GPIO_PORTS_BASE, GPIO_PIN_0, GPIO_INPUT},
			{GPIO_PORTN_BASE, GPIO_PIN_7, GPIO_INPUT},
			{GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_INPUT},

			{GPIO_PORTQ_BASE, GPIO_PIN_3, GPIO_OUTPUT},
			{GPIO_PORTM_BASE, GPIO_PIN_7, GPIO_OUTPUT},

};

extern const GPIO_Config GPIO_config[] = {
    {&gpioHWAttrs[0]},
    {&gpioHWAttrs[1]},
    {&gpioHWAttrs[2]},
    {&gpioHWAttrs[3]},
    {&gpioHWAttrs[4]},

	{&gpioHWAttrs[5]},
	{&gpioHWAttrs[6]},
	{&gpioHWAttrs[7]},
	{&gpioHWAttrs[8]},
	{&gpioHWAttrs[9]},
	{&gpioHWAttrs[10]},
	{&gpioHWAttrs[11]},
	{&gpioHWAttrs[12]},

	{&gpioHWAttrs[13]},
	{&gpioHWAttrs[14]},
    {NULL}
};

/*
 *  ======== EK_TM4C1294XL_initGPIO ========
 */
void DK_TM4C129X_initGPIO(void)
{
	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_4);
	GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_3);
	GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_2);
	GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_1);
	GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_0);
	GPIOPinTypeGPIOInput(GPIO_PORTP_BASE, GPIO_PIN_1);
	GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_5);

	GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTS_BASE, GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_5);
	GPIOPinTypeGPIOInput (GPIO_PORTS_BASE, GPIO_PIN_0);
	GPIOPinTypeGPIOInput (GPIO_PORTN_BASE, GPIO_PIN_7);
	GPIOPinTypeGPIOInput (GPIO_PORTF_BASE, GPIO_PIN_3);

	GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_7);


    GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_2, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_3, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTP_BASE, GPIO_PIN_1, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_STRENGTH_12MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_1);  //PL1 Fn1
    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_PIN_7);  //PK7 Fn2
    GPIOPadConfigSet(GPIO_PORTK_BASE, GPIO_PIN_7, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);


/* Once GPIO_init is called, GPIO_config cannot be changed */
    GPIO_init();
}
#else

const GPIO_HWAttrs gpioHWAttrs[EK_TM4C1294XL_GPIOCOUNT] = {
	{GPIO_PORTL_BASE, GPIO_PIN_4, GPIO_OUTPUT},
	{GPIO_PORTN_BASE, GPIO_PIN_2, GPIO_OUTPUT},
	{GPIO_PORTN_BASE, GPIO_PIN_1, GPIO_OUTPUT},
	{GPIO_PORTN_BASE, GPIO_PIN_0, GPIO_OUTPUT},
	{GPIO_PORTQ_BASE, GPIO_PIN_7, GPIO_OUTPUT},

	{GPIO_PORTM_BASE, GPIO_PIN_5, GPIO_OUTPUT},
	{GPIO_PORTD_BASE, GPIO_PIN_3, GPIO_OUTPUT},
	{GPIO_PORTS_BASE, GPIO_PIN_3, GPIO_OUTPUT},
	{GPIO_PORTL_BASE, GPIO_PIN_5, GPIO_OUTPUT},
	{GPIO_PORTS_BASE, GPIO_PIN_0, GPIO_INPUT},
	{GPIO_PORTN_BASE, GPIO_PIN_7, GPIO_INPUT},
	{GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_INPUT},
	{GPIO_PORTN_BASE, GPIO_PIN_5, GPIO_OUTPUT},

	{GPIO_PORTQ_BASE, GPIO_PIN_3, GPIO_OUTPUT},
	{GPIO_PORTM_BASE, GPIO_PIN_7, GPIO_OUTPUT},
	{GPIO_PORTJ_BASE, GPIO_PIN_7, GPIO_OUTPUT},
	{GPIO_PORTC_BASE, GPIO_PIN_6, GPIO_OUTPUT},
//	{GPIO_PORTM_BASE, GPIO_PIN_6, GPIO_OUTPUT},

};

extern const GPIO_Config GPIO_config[] = {
    {&gpioHWAttrs[0]},
    {&gpioHWAttrs[1]},
    {&gpioHWAttrs[2]},
    {&gpioHWAttrs[3]},
    {&gpioHWAttrs[4]},

	{&gpioHWAttrs[5]},
	{&gpioHWAttrs[6]},
	{&gpioHWAttrs[7]},
	{&gpioHWAttrs[8]},
	{&gpioHWAttrs[9]},
	{&gpioHWAttrs[10]},
	{&gpioHWAttrs[11]},
	{&gpioHWAttrs[12]},

	{&gpioHWAttrs[13]},
	{&gpioHWAttrs[14]},

	{&gpioHWAttrs[15]},

	{&gpioHWAttrs[16]},
/*	{&gpioHWAttrs{17}},*/
    {NULL}
};

/*
 *  ======== EK_TM4C1294XL_initGPIO ========
 */
void DK_TM4C129X_initGPIO(void)
{

/* Setup the Pult rs485 rw pin */
    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_4);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_2);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_1);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_7);

    GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_5);
    GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTS_BASE, GPIO_PIN_3);
    GPIOPinTypeGPIOOutput(GPIO_PORTL_BASE, GPIO_PIN_5);
    GPIOPinTypeGPIOInput (GPIO_PORTS_BASE, GPIO_PIN_0);
    GPIOPinTypeGPIOInput (GPIO_PORTN_BASE, GPIO_PIN_7);
    GPIOPinTypeGPIOInput (GPIO_PORTF_BASE, GPIO_PIN_3);

    GPIOPinTypeGPIOOutput(GPIO_PORTC_BASE, GPIO_PIN_6);

    //пин отладки PN5
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_5);
    //Пины управление регистрами светодиодов
	GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_3);
	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_7);
	//RS485 TXE ALT
	GPIOPinTypeGPIOOutput(GPIO_PORTJ_BASE, GPIO_PIN_7);
	//Preston on
//	GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_6);
//	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_3);
//	GPIOPinTypeGPIOOutput(GPIO_PORTG_BASE, GPIO_PIN_7);

    //входы кнопок борды
    GPIOPinTypeGPIOInput(GPIO_PORTP_BASE, GPIO_PIN_1);  //PP1 Select
    GPIOPadConfigSet(GPIO_PORTP_BASE, GPIO_PIN_1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTN_BASE, GPIO_PIN_3);  //PN3 Up
    GPIOPadConfigSet(GPIO_PORTN_BASE, GPIO_PIN_3, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTE_BASE, GPIO_PIN_5);  //PE5 Dn
    GPIOPadConfigSet(GPIO_PORTE_BASE, GPIO_PIN_5, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTG_BASE, GPIO_PIN_0);  //PG0 Esc
    GPIOPadConfigSet(GPIO_PORTG_BASE, GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_1);  //PL1 Fn1
    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTK_BASE, GPIO_PIN_7);  //PK7 Fn2
    GPIOPadConfigSet(GPIO_PORTK_BASE, GPIO_PIN_7, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);


/* Once GPIO_init is called, GPIO_config cannot be changed */
    GPIO_init();
}
#endif


#if TI_DRIVERS_I2C_INCLUDED
#include <ti/drivers/I2C.h>
#include <ti/drivers/i2c/I2CTiva.h>

/* I2C objects */
I2CTiva_Object i2cTivaObjects[DK_TM4C129X_I2CCOUNT];

/* I2C configuration structure, describing which pins are to be used */
const I2CTiva_HWAttrs i2cTivaHWAttrs[DK_TM4C129X_I2CCOUNT] = {
    {I2C3_BASE, INT_I2C3},
    {I2C6_BASE, INT_I2C6},
    {I2C7_BASE, INT_I2C7}
};

const I2C_Config I2C_config[] = {
    {&I2CTiva_fxnTable, &i2cTivaObjects[0], &i2cTivaHWAttrs[0]},
    {&I2CTiva_fxnTable, &i2cTivaObjects[1], &i2cTivaHWAttrs[1]},
    {&I2CTiva_fxnTable, &i2cTivaObjects[2], &i2cTivaHWAttrs[2]},
    {NULL, NULL, NULL}
};

/*
 *  ======== DK_TM4C129X_initI2C ========
 */
void DK_TM4C129X_initI2C(void)
{
    /* I2C3 Init */
    /* Enable the peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C3);

    /* Configure the appropriate pins to be I2C instead of GPIO. */
    GPIOPinConfigure(GPIO_PG4_I2C3SCL);
    GPIOPinConfigure(GPIO_PG5_I2C3SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTG_BASE, GPIO_PIN_4);
    GPIOPinTypeI2C(GPIO_PORTG_BASE, GPIO_PIN_5);

    /* I2C6 Init */
    /* Enable the peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C6);

    /* Configure the appropriate pins to be I2C instead of GPIO. */
    GPIOPinConfigure(GPIO_PB6_I2C6SCL);
    GPIOPinConfigure(GPIO_PB7_I2C6SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_7);

    /* I2C7 Init */
    /* Enable the peripheral */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C7);

    /* Configure the appropriate pins to be I2C instead of GPIO. */
    GPIOPinConfigure(GPIO_PA4_I2C7SCL);
    GPIOPinConfigure(GPIO_PA5_I2C7SDA);
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_4);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_5);

    I2C_init();
}
#endif /* TI_DRIVERS_I2C_INCLUDED */

#if TI_DRIVERS_SDSPI_INCLUDED
#include <ti/drivers/SDSPI.h>
#include <ti/drivers/sdspi/SDSPITiva.h>

/* SDSPI objects */
SDSPITiva_Object sdspiTivaobjects[DK_TM4C129X_SDSPICOUNT];

/* SDSPI configuration structure, describing which pins are to be used */
const SDSPITiva_HWAttrs sdspiTivaHWattrs[DK_TM4C129X_SDSPICOUNT] = {
    {
        SSI3_BASE,          /* SPI base address */

        GPIO_PORTQ_BASE,    /* The GPIO port used for the SPI pins */
        GPIO_PIN_0,         /* SCK */
        /* MISO is actually on a different port (F), so use a use SCK again */
        GPIO_PIN_0,
        GPIO_PIN_2,         /* MOSI */

        GPIO_PORTH_BASE,    /* Chip select port */
        GPIO_PIN_4,         /* Chip select pin */

        GPIO_PORTQ_BASE,    /* GPIO TX port */
        GPIO_PIN_2,         /* GPIO TX pin */
    }
};

const SDSPI_Config SDSPI_config[] = {
    {&SDSPITiva_fxnTable, &sdspiTivaobjects[0], &sdspiTivaHWattrs[0]},
    {NULL, NULL, NULL}
};

/*
 *  ======== DK_TM4C129X_initSDSPI ========
 */
void DK_TM4C129X_initSDSPI(void)
{
    /* Enable the peripherals used by the SD Card */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);

    /* Configure pad settings */
    GPIOPadConfigSet(GPIO_PORTQ_BASE,
            GPIO_PIN_0 | GPIO_PIN_2,
            GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

    GPIOPadConfigSet(GPIO_PORTF_BASE,
            GPIO_PIN_0,
            GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);

    GPIOPadConfigSet(GPIO_PORTH_BASE,
            GPIO_PIN_4,
            GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD);

    GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
    GPIOPinConfigure(GPIO_PF0_SSI3XDAT1);
    GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);

    /*
     * The SDSPI driver assumes that all the SPI pins are located on the same
     * GPIO port. Since the MISO pin is on PORTF instead of PORTQ, we need to
     * manually change its function to hardware mode.
     */
    GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_DIR_MODE_HW);

    SDSPI_init();
}
#endif /* TI_DRIVERS_SDSPI_INCLUDED */

#if TI_DRIVERS_SPI_INCLUDED
#include <ti/drivers/SPI.h>
#include <ti/drivers/spi/SPITivaDMA.h>

/* SPI objects */
SPITivaDMA_Object spiTivaDMAobjects[DK_TM4C129X_SPICOUNT];
#if defined(ccs)
#pragma DATA_ALIGN(spiTivaDMAscratchBuf, 32)
#elif defined(ewarm)
#pragma data_alignment=32
#elif defined(gcc)
__attribute__ ((aligned (32)))
#endif
uint32_t spiTivaDMAscratchBuf[DK_TM4C129X_SPICOUNT];

/* SPI configuration structure, describing which pins are to be used */
const SPITivaDMA_HWAttrs spiTivaDMAHWAttrs[DK_TM4C129X_SPICOUNT] = {
    {
        SSI0_BASE,
        INT_SSI0,
        &spiTivaDMAscratchBuf[0],
        0,
        UDMA_CHANNEL_SSI0RX,
        UDMA_CHANNEL_SSI0TX,
        uDMAChannelAssign,
        UDMA_CH10_SSI0RX,
        UDMA_CH11_SSI0TX
    },
    {
        SSI2_BASE,
        INT_SSI2,
        &spiTivaDMAscratchBuf[1],
        0,
        UDMA_SEC_CHANNEL_UART2RX_12,
        UDMA_SEC_CHANNEL_UART2TX_13,
        uDMAChannelAssign,
        UDMA_CH12_SSI2RX,
        UDMA_CH13_SSI2TX
    },
    {
        SSI3_BASE,
        INT_SSI3,
        &spiTivaDMAscratchBuf[2],
        0,
        UDMA_SEC_CHANNEL_TMR2A_14,
        UDMA_SEC_CHANNEL_TMR2B_15,
        uDMAChannelAssign,
        UDMA_CH14_SSI3RX,
        UDMA_CH15_SSI3TX
    }
};

const SPI_Config SPI_config[] = {
    {&SPITivaDMA_fxnTable, &spiTivaDMAobjects[0], &spiTivaDMAHWAttrs[0]},
    {&SPITivaDMA_fxnTable, &spiTivaDMAobjects[1], &spiTivaDMAHWAttrs[1]},
    {&SPITivaDMA_fxnTable, &spiTivaDMAobjects[2], &spiTivaDMAHWAttrs[2]},
    {NULL, NULL, NULL},
};

/*
 *  ======== DK_TM4C129X_initSPI ========
 */
void DK_TM4C129X_initSPI(void)
{
    /* SPI0 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);

    /* Need to unlock PF0 */
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);
    GPIOPinConfigure(GPIO_PA3_SSI0FSS);
    GPIOPinConfigure(GPIO_PA4_SSI0XDAT0);
    GPIOPinConfigure(GPIO_PA5_SSI0XDAT1);

    GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_2 | GPIO_PIN_3 |
                                    GPIO_PIN_4 | GPIO_PIN_5);

    /* SSI2 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);

    GPIOPinConfigure(GPIO_PD3_SSI2CLK);
    GPIOPinConfigure(GPIO_PD2_SSI2FSS);
    GPIOPinConfigure(GPIO_PG5_SSI2XDAT0);
    GPIOPinConfigure(GPIO_PG4_SSI2XDAT1);

    GPIOPinTypeSSI(GPIO_PORTD_BASE, GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeSSI(GPIO_PORTG_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    /* SSI3 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI3);

    GPIOPinConfigure(GPIO_PQ0_SSI3CLK);
    GPIOPinConfigure(GPIO_PQ1_SSI3FSS);
    GPIOPinConfigure(GPIO_PQ2_SSI3XDAT0);
    GPIOPinConfigure(GPIO_PF0_SSI3XDAT1);

    GPIOPinTypeSSI(GPIO_PORTQ_BASE, GPIO_PIN_0 | GPIO_PIN_1 |
                                    GPIO_PIN_2);
    GPIOPinTypeSSI(GPIO_PORTF_BASE, GPIO_PIN_0);

    DK_TM4C129X_initDMA();
    SPI_init();
}
#endif /* TI_DRIVERS_SPI_INCLUDED */

#include <../GyConCommon/ExchangeDrivers/UARTTiva.h>

/* UART objects */
UARTTiva_Object uartTivaObjects[EK_TM4C1294XL_UARTCOUNT];

/* UART configuration structure */


const UARTTiva_HWAttrs uartTivaHWAttrs[EK_TM4C1294XL_UARTCOUNT] = {
    {UART4_BASE, INT_UART4},
	{UART7_BASE, INT_UART7},
	{UART5_BASE, INT_UART5}
};

extern const UART_Config UART_config[] = {
    {&UARTTiva_fxnTable, &uartTivaObjects[0], &uartTivaHWAttrs[0]},
	{&UARTTiva_fxnTable, &uartTivaObjects[1], &uartTivaHWAttrs[1]},
	{&UARTTiva_fxnTable, &uartTivaObjects[2], &uartTivaHWAttrs[2]},
    {NULL, NULL, NULL}
    };

/*
 *  ======== EK_TM4C1294XL_initUART ========
 */
void DK_TM4C129X_initUART(void)
{
	/* Enable and configure the peripherals used by the UART4 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART4);
    GPIOPinConfigure(GPIO_PK0_U4RX);
    GPIOPinConfigure(GPIO_PK1_U4TX);
    GPIOPinTypeUART(GPIO_PORTK_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART7);
    GPIOPinConfigure(GPIO_PC5_U7TX);
    GPIOPinConfigure(GPIO_PC4_U7RX);
    GPIOPinTypeUART(GPIO_PORTC_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART5);
    GPIOPinConfigure(GPIO_PH7_U5TX);
    GPIOPinConfigure(GPIO_PH6_U5RX);
    GPIOPinTypeUART(GPIO_PORTH_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    /* Initialize the UART driver */
    UART_init();
}


/*
 *  ======== DK_TM4C129X_initUSB ========
 *  This function just turns on the USB
 */
void DK_TM4C129X_initUSB(DK_TM4C129X_USBMode usbMode)
{
    /* Enable the USB peripheral and PLL */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);
    SysCtlUSBPLLEnable();

    /* Setup pins for USB operation */
    GPIOPinTypeUSBAnalog(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeUSBAnalog(GPIO_PORTL_BASE, GPIO_PIN_6 | GPIO_PIN_7);

    /* Additional configurations for Host mode */
    if (usbMode == DK_TM4C129X_USBHOST) {
        /* Configure the pins needed */
        HWREG(GPIO_PORTD_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;
        HWREG(GPIO_PORTD_BASE + GPIO_O_CR) = 0xff;
        GPIOPinConfigure(GPIO_PD6_USB0EPEN);
        GPIOPinConfigure(GPIO_PD7_USB0PFLT);
        GPIOPinTypeUSBDigital(GPIO_PORTD_BASE, GPIO_PIN_6 | GPIO_PIN_7);
    }
}

#if TI_DRIVERS_USBMSCHFATFS_INCLUDED
#include <ti/drivers/USBMSCHFatFs.h>
#include <ti/drivers/usbmschfatfs/USBMSCHFatFsTiva.h>

/* USBMSCHFatFs objects */
USBMSCHFatFsTiva_Object usbmschfatfstivaObjects[DK_TM4C129X_USBMSCHFatFsCOUNT];

/* USBMSCHFatFs configuration structure, describing which pins are to be used */
const USBMSCHFatFsTiva_HWAttrs usbmschfatfstivaHWAttrs[DK_TM4C129X_USBMSCHFatFsCOUNT] = {
    {INT_USB0}
};

const USBMSCHFatFs_Config USBMSCHFatFs_config[] = {
    {
        &USBMSCHFatFsTiva_fxnTable,
        &usbmschfatfstivaObjects[0],
        &usbmschfatfstivaHWAttrs[0]
    },
    {NULL, NULL, NULL}
};

/*
 *  ======== DK_TM4C129X_initUSBMSCHFatFs ========
 */
void DK_TM4C129X_initUSBMSCHFatFs(void)
{
    /* Initialize the DMA control table */
    DK_TM4C129X_initDMA();

    /* Call the USB initialization function for the USB Reference modules */
    DK_TM4C129X_initUSB(DK_TM4C129X_USBHOST);
    USBMSCHFatFs_init();
}
#endif /* TI_DRIVERS_USBMSCHFATFS_INCLUDED */

#if TI_DRIVERS_WATCHDOG_INCLUDED
#include <ti/drivers/Watchdog.h>
#include <ti/drivers/watchdog/WatchdogTiva.h>

/* Watchdog objects */
WatchdogTiva_Object watchdogTivaObjects[DK_TM4C129X_WATCHDOGCOUNT];

/* Watchdog configuration structure */
const WatchdogTiva_HWAttrs watchdogTivaHWAttrs[DK_TM4C129X_WATCHDOGCOUNT] = {
    /* EK_LM4F120XL_WATCHDOG0 with 1 sec period at default CPU clock freq */
    {WATCHDOG0_BASE, INT_WATCHDOG, 80000000},
};

const Watchdog_Config Watchdog_config[] = {
    {&WatchdogTiva_fxnTable, &watchdogTivaObjects[0], &watchdogTivaHWAttrs[0]},
    {NULL, NULL, NULL},
};

/*
 *  ======== DK_TM4C129X_initWatchdog ========
 *
 * NOTE: To use the other watchdog timer with base address WATCHDOG1_BASE,
 *       an additional function call may need be made to enable PIOSC. Enabling
 *       WDOG1 does not do this. Enabling another peripheral that uses PIOSC
 *       such as ADC0 or SSI0, however, will do so. Example:
 *
 *       SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
 *       SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG1);
 *
 *       See the following forum post for more information:
 *       http://e2e.ti.com/support/microcontrollers/stellaris_arm_cortex-m3_microcontroller/f/471/p/176487/654390.aspx#654390
 */
void DK_TM4C129X_initWatchdog()
{
    /* Enable peripherals used by Watchdog */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WDOG0);

    /* Initialize the Watchdog driver */
    Watchdog_init();
}
#endif /* TI_DRIVERS_WATCHDOG_INCLUDED */

#if TI_DRIVERS_WIFI_INCLUDED
#include <ti/drivers/WiFi.h>

#if TI_DRIVERS_WIFI_CC3100
#include <ti/drivers/wifi/WiFiCC3100.h>

/* WiFi objects */
WiFiCC3100_Object wiFiCC3100Objects[DK_TM4C129X_WIFICOUNT];

/* WiFi configuration structure */
const WiFiCC3100_HWAttrs wiFiCC3100HWAttrs[DK_TM4C129X_WIFICOUNT] = {
    {
        GPIO_PORTS_BASE, /* IRQ port */
        GPIO_PIN_2,      /* IRQ pin */
        INT_GPIOS,       /* IRQ port interrupt vector */

        GPIO_PORTQ_BASE, /* CS port */
        GPIO_PIN_7,      /* CS pin */

        GPIO_PORTN_BASE, /* WLAN EN port */
        GPIO_PIN_7       /* WLAN EN pin */
    }
};

const WiFi_Config WiFi_config[] = {
    {
        &WiFiCC3100_fxnTable,
        &wiFiCC3100Objects[0],
        &wiFiCC3100HWAttrs[0]
    },
    {NULL,NULL, NULL},
};

/*
 *  ======== DK_TM4C129X_initWiFi ========
 */
void DK_TM4C129X_initWiFi(void)
{
    /* Configure EN & CS pins to disable CC3100 */
    GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_7);
    GPIOPinTypeGPIOOutput(GPIO_PORTN_BASE, GPIO_PIN_7);
    GPIOPinWrite(GPIO_PORTQ_BASE, GPIO_PIN_7, GPIO_PIN_7);
    GPIOPinWrite(GPIO_PORTN_BASE, GPIO_PIN_7, 0);

    /* Configure SSI2 for CC3100 */
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
    GPIOPinConfigure(GPIO_PG7_SSI2CLK);
    GPIOPinConfigure(GPIO_PG5_SSI2XDAT0);
    GPIOPinConfigure(GPIO_PG4_SSI2XDAT1);
    GPIOPinTypeSSI(GPIO_PORTG_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7);

    /* Configure IRQ pin */
    GPIOPinTypeGPIOInput(GPIO_PORTS_BASE, GPIO_PIN_2);
    GPIOPadConfigSet(GPIO_PORTS_BASE, GPIO_PIN_2, GPIO_STRENGTH_2MA,
                     GPIO_PIN_TYPE_STD_WPD);
    GPIOIntTypeSet(GPIO_PORTS_BASE, GPIO_PIN_2, GPIO_RISING_EDGE);

    SPI_init();
    DK_TM4C129X_initDMA();

    WiFi_init();
}
#endif /* TI_DRIVERS_WIFI_CC3100 */

#endif /* TI_DRIVERS_WIFI_INCLUDED */
