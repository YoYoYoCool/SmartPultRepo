#include <SPI/SPI_Work.h>

//*****************************************************************************
//
//! \addtogroup ssi_examples_list
//! <h1>SPI Master (spi_master)</h1>
//!
//! This example shows how to configure the SSI0 as SPI Master.  The code will
//! send three characters on the master Tx then polls the receive FIFO until
//! 3 characters are received on the master Rx.
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - SSI0 peripheral
//! - GPIO Port A peripheral (for SSI0 pins)
//! - SSI0Clk - PA2
//! - SSI0Fss - PA3
//! - SSI0Rx  - PA4
//! - SSI0Tx  - PA5
//!
//! The following UART signals are configured only for displaying console
//! messages for this example.  These are not required for operation of SSI0.
//! - UART0 peripheral
//! - GPIO Port A peripheral (for UART0 pins)
//! - UART0RX - PA0
//! - UART0TX - PA1
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - None.
//
//*****************************************************************************

//*****************************************************************************
//
// Number of bytes to send and receive.
//
//*****************************************************************************
#define NUM_SSI_DATA            6

uint32_t pui32DataTx[NUM_SSI_DATA];
uint32_t pui32DataRx[NUM_SSI_DATA];
uint32_t ui32Index;
byte Table_Led_Out[48]; //таблица выходов

//*****************************************************************************
// This function sets up UART0 to be used for a console to display information
// as the example is running.
//*****************************************************************************
/*
void
InitConsole(void)
{
    // Enable GPIO port A which is used for UART0 pins.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    // Configure the pin muxing for UART0 functions on port A0 and A1.
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    // Enable UART0 so that we can configure the clock.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    // Use the internal 16MHz oscillator as the UART clock source.
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_PIOSC);
    // Select the alternate (UART) function for these pins.
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    // Initialize the UART for console I/O.
    UARTStdioConfig(0, 115200, 16000000);
}*/

//*****************************************************************************
// Configure SSI2 in master SPI mode.
//*****************************************************************************
void
InitSPI(void)
{

    //прописать ноги в DK_TM4C129X_initGPIO() !!!!!
    //
        //выходы Latch и Reset для 74HCT595
        GPIOPinTypeGPIOOutput(GPIO_PORTQ_BASE, GPIO_PIN_3); // OUT_LATCH PQ3
        GPIOPinTypeGPIOOutput(GPIO_PORTM_BASE, GPIO_PIN_7); // OUT_RESET PM7
    //===========================


	// Set the clocking to run directly from the external crystal/oscillator.
    //SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    // Set up the serial console to use for displaying messages.
    //InitConsole();
    // Display the setup on the console.
    /*UARTprintf("SSI ->\n");
    UARTprintf("  Mode: SPI\n");
    UARTprintf("  Data: 8-bit\n\n");*/
    // The SSI2 peripheral must be enabled for use.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI2);
    // For this example SSI2 is used with PortG[5:2].
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
    // Configure the pin muxing for SSI2 functions on port G5 and G7.
    GPIOPinConfigure(GPIO_PG7_SSI2CLK);
    GPIOPinConfigure(GPIO_PG5_SSI2XDAT0);

    // Configure the GPIO settings for the SSI pins.  This function also gives
    // control of these pins to the SSI hardware.
    // The pins are assigned as follows:
    //      PG5 - SSI2CLK
    //      PG7 - SSI2Tx
    GPIOPinTypeSSI(GPIO_PORTG_BASE, GPIO_PIN_5 | GPIO_PIN_7);

    // Configure and enable the SSI port for SPI master mode.  Use SSI2,
    // system clock supply, idle clock level low and active low clock in
    // freescale SPI mode, master mode, 1MHz SSI frequency, and 8-bit data.
    // For SPI mode, you can set the polarity of the SSI clock when the SSI
    // unit is idle.  You can also configure what clock edge you want to
    // capture data on.  Please reference the datasheet for more information on
    // the different SPI modes.
    //
    SSIConfigSetExpClk(SSI2_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 40000, 8); //500 кГц

    // Enable the SSI2 module.
    SSIEnable(SSI2_BASE);


    Out_Reset_Set;

    int i;
    for(i=0;i<48;i++) Table_Led_Out[i]=0;
}

void SPI2_Send(void)
{
	int i;

	//обработка данных в качестве паузы

	//задвигаем битики в байтики
	pui32DataTx[5] = Table_Led_Out[0];
	for(i=1;i<8;i++) pui32DataTx[5]+=Table_Led_Out[i]<<i;

	pui32DataTx[4] = Table_Led_Out[8+0];
	for(i=1;i<8;i++) pui32DataTx[4]+=Table_Led_Out[8+i]<<i;

	pui32DataTx[3] = Table_Led_Out[2*8+0];
	for(i=1;i<8;i++) pui32DataTx[3]+=Table_Led_Out[2*8+i]<<i;

	pui32DataTx[2] = Table_Led_Out[3*8+0];
	for(i=1;i<8;i++) pui32DataTx[2]+=Table_Led_Out[3*8+i]<<i;

	pui32DataTx[1] = Table_Led_Out[4*8+0];
	for(i=1;i<8;i++) pui32DataTx[1]+=Table_Led_Out[4*8+i]<<i;

	pui32DataTx[0] = Table_Led_Out[5*8+0];
	for(i=1;i<8;i++) pui32DataTx[0]+=Table_Led_Out[5*8+i]<<i;

    // Send 6 bytes of data.
    for(ui32Index = 0; ui32Index < NUM_SSI_DATA; ui32Index++)
    {
        SSIDataPut(SSI2_BASE, pui32DataTx[ui32Index]);
    }

	Task_sleep(1);

	//смена предыдущих состояний перед записью
	Out_Latch_Set;

	Task_sleep(1);

	//возврат к нулю смены состояний после записи
    Out_Latch_Reset;

}

void SPI_Set_Pin(SPI_LED_Pins Pin)
{
	Table_Led_Out[(byte)Pin]=1;
	SPI2_Send();
}

void SPI_Reset_Pin(SPI_LED_Pins Pin)
{
	Table_Led_Out[(byte)Pin]=0;
	SPI2_Send();
}
