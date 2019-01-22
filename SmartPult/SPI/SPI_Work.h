#ifndef __SPI_WORK_H
#define __SPI_WORK_H


#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "driverlib/sysctl.h"
#include "driverlib/uart.h"
#include "utils/uartstdio.h"
#include <ti/sysbios/knl/Task.h>


typedef uint8_t byte;

// выходы Latch и Reset для 74HCT595
#define Out_Latch_Set       GPIOPinWrite(GPIO_PORTQ_BASE,GPIO_PIN_3,1<<3)    // OUT_LATCH PQ3
#define Out_Latch_Reset     GPIOPinWrite(GPIO_PORTQ_BASE,GPIO_PIN_3,0)    // OUT_LATCH PQ3
#define Out_Reset_Set       GPIOPinWrite(GPIO_PORTM_BASE,GPIO_PIN_7,1<<7)    // OUT_RESET PM7
#define Out_Reset_Reset     GPIOPinWrite(GPIO_PORTM_BASE,GPIO_PIN_7,0)    // OUT_RESET PM7

typedef enum{
	SPI_Led_Pin1 = 0,
	SPI_Led_Pin2,
	SPI_Led_Pin3,
	SPI_Led_Pin4,
	SPI_Led_Pin5,
	SPI_Led_Pin6,
	SPI_Led_Pin7,
	SPI_Led_Pin8,
	SPI_Led_Pin9,
	SPI_Led_Pin10,
	SPI_Led_Pin11,
	SPI_Led_Pin12,
	SPI_Led_Pin13,
	SPI_Led_Pin14,
	SPI_Led_Pin15,
	SPI_Led_Pin16,
	SPI_Led_Pin17,
	SPI_Led_Pin18,
	SPI_Led_Pin19,
	SPI_Led_Pin20,
	SPI_Led_Pin21,
	SPI_Led_Pin22,
	SPI_Led_Pin23,
	SPI_Led_Pin24,
	SPI_Led_Pin25,
	SPI_Led_Pin26,
	SPI_Led_Pin27,
	SPI_Led_Pin28,
	SPI_Led_Pin29,
	SPI_Led_Pin30,
	SPI_Led_Pin31,
	SPI_Led_Pin32,
	SPI_Led_Pin33,
	SPI_Led_Pin_Count
} SPI_LED_Pins;

extern byte Table_Led_Out[48];

extern void InitSPI(void);
extern void SPI2_Send(void);
extern void SPI_Set_Pin(SPI_LED_Pins Pin);
extern void SPI_Reset_Pin(SPI_LED_Pins Pin);



#endif /* __SPI_WORK_H */
