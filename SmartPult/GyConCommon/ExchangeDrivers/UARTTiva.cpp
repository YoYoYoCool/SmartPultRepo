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

#include <stdint.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/Assert.h>
#include <xdc/runtime/Diags.h>
#include <xdc/runtime/Log.h>
#include <xdc/runtime/Types.h>

#include <../GyConCommon/ExchangeDrivers/UARTTiva.h>

#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include <ti/sysbios/family/arm/m3/Hwi.h>

/* driverlib header files */
#include <inc/hw_memmap.h>
#include <inc/hw_ints.h>
#include <inc/hw_types.h>
#include <driverlib/uart.h>

#if !defined(CCWARE)
#include <driverlib/sysctl.h>
#endif

/* UARTTiva functions */
void         UARTTiva_close(UART_Handle handle);
int          UARTTiva_control(UART_Handle handle, unsigned int cmd, void *arg);
void         UARTTiva_init(UART_Handle handle);
UART_Handle  UARTTiva_open(UART_Handle handle, UART_Params *params);
int          UARTTiva_read(UART_Handle handle, void *buffer, size_t size);
void         UARTTiva_readCancel(UART_Handle handle);
int          UARTTiva_readPolling(UART_Handle handle, void *buffer, size_t size);
int          UARTTiva_write(UART_Handle handle, const void *buffer, size_t size);
void         UARTTiva_writeCancel(UART_Handle handle);
int          UARTTiva_writePolling(UART_Handle handle, const void *buffer,
                                   size_t size);

/* UART function table for UARTTiva implementation */
const UART_FxnTable UARTTiva_fxnTable = {
    UARTTiva_close,
    UARTTiva_control,
    UARTTiva_init,
    UARTTiva_open,
    UARTTiva_read,
    UARTTiva_readPolling,
    UARTTiva_readCancel,
    UARTTiva_write,
    UARTTiva_writePolling,
    UARTTiva_writeCancel
};

static const uint32_t dataLength[] = {
    UART_CONFIG_WLEN_5, /* UART_LEN_5 */
    UART_CONFIG_WLEN_6, /* UART_LEN_6 */
    UART_CONFIG_WLEN_7, /* UART_LEN_7 */
    UART_CONFIG_WLEN_8  /* UART_LEN_8 */
};

static const uint32_t stopBits[] = {
    UART_CONFIG_STOP_ONE,   /* UART_STOP_ONE */
    UART_CONFIG_STOP_TWO    /* UART_STOP_TWO */
};

static const uint32_t parityType[] = {
    UART_CONFIG_PAR_NONE,   /* UART_PAR_NONE */
    UART_CONFIG_PAR_EVEN,   /* UART_PAR_EVEN */
    UART_CONFIG_PAR_ODD,    /* UART_PAR_ODD */
    UART_CONFIG_PAR_ZERO,   /* UART_PAR_ZERO */
    UART_CONFIG_PAR_ONE     /* UART_PAR_ONE */
};

/* Default UART params */
extern const UART_Params UART_defaultParams;

/*
 *  ======== writeSemCallback ========
 *  Simple callback to post a semaphore for the blocking mode.
 */
static void writeSemCallback(UART_Handle handle, void *buffer, size_t count)
{
    UARTTiva_Object *object = (UARTTiva_Object *)handle->object;

    Semaphore_post(Semaphore_handle(&(object->writeSem)));
}

/*
 *  ======== readSemCallback ========
 *  Simple callback to post a semaphore for the blocking mode.
 */
static void readSemCallback(UART_Handle handle, void *buffer, size_t count)
{
    UARTTiva_Object *object = (UARTTiva_Object *)handle->object;

    Semaphore_post(Semaphore_handle(&(object->readSem)));
}

/*
 *  ======== writeData ========
 *  Write and process data to the UART.
 */
static int32_t writeData(UARTTiva_Object        *object,
                         UARTTiva_HWAttrs const *hwAttrs,
                         int32_t size)
{
    /* Send characters until FIFO is full or done. */
    while (size) {
        /* If mode is TEXT process the characters */
        if (object->writeDataMode == UART_DATA_TEXT) {
            if (object->writeCR) {
                if (!UARTCharPutNonBlocking(hwAttrs->baseAddr, '\r')) {
                    break;
                }
                size--;
                object->writeCR = false;

                Log_print2(Diags_USER2, "UART:(%p) Wrote character 0x%x",
                           hwAttrs->baseAddr, '\r');
            }
            else {
                /* Add a return if next character is a newline. */
               if (*(unsigned char *)object->writeBuf == '\n') {
                   size++;
                   object->writeCR = true;
               }

               /* Send the next character and increment counts. */
               if (!UARTCharPutNonBlocking(hwAttrs->baseAddr,
                                         *(unsigned char *)object->writeBuf)) {
                   /* Character was not sent */
                   if (object->writeCR) {
                       size--;
                       object->writeCR = false;
                   }
                   break;
               }

               Log_print2(Diags_USER2, "UART:(%p) Wrote character 0x%x",
                          hwAttrs->baseAddr, *(unsigned char *)object->writeBuf);
               size--;
               object->writeBuf = (unsigned char *)object->writeBuf + 1;
               object->writeCount++;
            }
        }
        else {
            /* Send the next character and increment counts. */
            if (!UARTCharPutNonBlocking(hwAttrs->baseAddr,
                                      *(unsigned char *)object->writeBuf)) {
                /* Character was not sent */
                break;
            }
            Log_print2(Diags_USER2, "UART:(%p) Wrote character 0x%x",
                       hwAttrs->baseAddr, *(unsigned char *)object->writeBuf);
            object->writeBuf = (unsigned char *)object->writeBuf + 1;
            size--;
            object->writeCount++;
        }
    }
    return (size);
}

/*
 *  ======== readData ========
 *  Read and process data from the UART.
 */
static int32_t readData(UARTTiva_Object        *object,
                        UARTTiva_HWAttrs const *hwAttrs,
                        int32_t size)
{
    int32_t readIn;
    UInt32 limSize = size;
    if (limSize > object->readMaxCount) {
    	limSize = object->readMaxCount;
    }
    UInt32 readSize = limSize;
    /* Receive chars until empty or done. */
    while (readSize && (readIn = (int32_t)UARTCharGetNonBlocking(hwAttrs->baseAddr)) != -1) {
        /* If data mode is set to TEXT replace return with a newline. */
        if (object->readDataMode == UART_DATA_TEXT) {
            if ((uint8_t)readIn == '\r') {
                /* Echo character if enabled. */
                if (object->readEcho) {
                    UARTCharPut(hwAttrs->baseAddr, '\r');
                }
                readIn = (int32_t)'\n';
            }
        }

        Log_print2(Diags_USER2, "UART:(%p) Read character 0x%x",
                   hwAttrs->baseAddr, (uint8_t)readIn);

        *((unsigned char *)object->readBuf) = (uint8_t)readIn;
        object->readBuf = (unsigned char *)object->readBuf + 1;
        object->readCount++;
        readSize--;

        /* Echo character if enabled. */
        if (object->readEcho) {
            UARTCharPut(hwAttrs->baseAddr, (uint8_t)readIn);
        }

        /* If read return mode is newline, finish if a newline was received. */
        if (object->readReturnMode == UART_RETURN_NEWLINE && (uint8_t)readIn == '\n') {
            Log_print1(Diags_USER2, "UART:(%p) Newline character received, "
                       "returning", hwAttrs->baseAddr);
            readSize = 0;
            break;
        }
    }
    size = size - (limSize - readSize);
    return (size);
}

/*
 *  ======== UARTTiva_hwiIntFxn ========
 *  Hwi function that processes UART interrupts.
 *
 *  Three UART interrupts are enabled: Transmit FIFO is 7/8 empty,
 *  receive FIFO is 7/8 full and a receive timeout between the time
 *  the last character was received.
 *
 *  writeData and readData are in-lined functions that will read/write data
 *  with processing if enabled.  When the read or write is finished they will
 *  post the semaphore or make the callback and log the transfer.
 *
 *  @param(arg)         The UART_Handle for this Hwi.
 */


static void clearFifo(UARTTiva_HWAttrs const * hwAttrs) {
	while (((int32_t)UARTCharGetNonBlocking(hwAttrs->baseAddr)) != -1);
}

void UARTTiva_reset(UART_Handle handle) {
	UARTTiva_HWAttrs const    *hwAttrs = (UARTTiva_HWAttrs *)handle->hwAttrs;
	while (((int32_t)UARTCharGetNonBlocking(hwAttrs->baseAddr)) != -1);
}

static void UARTTiva_hwiIntFxn(UArg arg)
{
    uint32_t                   status;
    UARTTiva_Object           *object = (UARTTiva_Object   *)((UART_Handle)arg)->object;
    UARTTiva_HWAttrs const    *hwAttrs = (UARTTiva_HWAttrs *)((UART_Handle)arg)->hwAttrs;

    /* Clear interrupts */
    status = UARTIntStatus(hwAttrs->baseAddr, true);
    UARTIntClear(hwAttrs->baseAddr, status);

    Log_print2(Diags_USER2, "UART:(%p) Interrupt with mask 0x%x",
               hwAttrs->baseAddr, status);

    /* Read data if characters are available. */
    if (object->readSize) {
        object->readSize = readData(object, hwAttrs, object->readSize);
        if ( (status == UART_INT_RT) ) {
        	 //   object->readSize = readData(object, hwAttrs, object->readSize);
            	object->readSize = 0;
            //	clearFifo(hwAttrs);
        }
        if (!object->readSize) {
            /* Reset the read buffer so we can pass it back */
            object->readBuf = (unsigned char *)object->readBuf - object->readCount;
            object->readCallback((UART_Handle)arg, object->readBuf,
                                 object->readCount);
            Log_print2(Diags_USER1, "UART:(%p) Read finished, %d bytes read",
                       hwAttrs->baseAddr, object->readCount);
        }
    }
    else
    if ( (status & UART_INT_RT)!=0 ) {
    	clearFifo(hwAttrs);
    }

    /* Write if there are characters to be written. */
    if (object->writeSize) {
        object->writeSize = writeData(object, hwAttrs, object->writeSize);
        if (!object->writeSize) {
            /* Reset the write buffer so we can pass it back */
            object->writeBuf = (unsigned char *)object->writeBuf - object->writeCount;
            object->writeCallback((UART_Handle)arg,
                                  (uint8_t*)object->writeBuf,
                                  object->writeCount);

            Log_print2(Diags_USER1, "UART:(%p) Write finished, %d bytes written",
                       hwAttrs->baseAddr, object->writeCount);
        }
    }
}

/*
 *  ======== UARTTiva_init ========
 */

void UARTTiva_init(UART_Handle handle)
{
    UARTTiva_Object    *object = (UARTTiva_Object *)handle->object;
    object->opened = false;
}

/*
 *  ======== UARTTiva_open ========
 */

static UARTTiva_params defTivaParams;
UART_Handle UARTTiva_open(UART_Handle handle, UART_Params *params)
{
    unsigned int               key;
    Types_FreqHz               freq;
    UARTTiva_Object           *object = (UARTTiva_Object *)handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = (UARTTiva_HWAttrs *)handle->hwAttrs;
    union {
        Hwi_Params             hwiParams;
        Semaphore_Params       semParams;
    } paramsUnion;

    /* If params are NULL use defaults. */
    if (params == NULL) {
        params = (UART_Params *) &UART_defaultParams;
    }

    UARTTiva_params *tivaParams = (UARTTiva_params*) params->custom;
    if (tivaParams == NULL) {
    	defTivaParams.fifolevel = UART_FIFO_RX4_8;
    	defTivaParams.rtSaveRead = false;
    	tivaParams = &defTivaParams;
    }

    /* Check that a callback is set */
    Assert_isTrue((params->readMode != UART_MODE_CALLBACK) ||
                  (params->readCallback != NULL), NULL);
    Assert_isTrue((params->writeMode != UART_MODE_CALLBACK) ||
                  (params->writeCallback != NULL), NULL);

    object->readMode       = params->readMode;
    object->writeMode      = params->writeMode;
    object->readTimeout    = params->readTimeout;
    object->writeTimeout   = params->writeTimeout;
    object->readCallback   = params->readCallback;
    object->writeCallback  = params->writeCallback;
    object->readReturnMode = params->readReturnMode;
    object->readDataMode   = params->readDataMode;
    object->writeDataMode  = params->writeDataMode;
    object->readEcho       = params->readEcho;

    if (tivaParams->rtSaveRead) {
    	    switch(tivaParams->fifolevel) {
    	    case UART_FIFO_RX1_8:
    	    	object->readMaxCount = 1;
    		break;
    	    case UART_FIFO_RX2_8:
    	    	object->readMaxCount = 3;
    		break;
    	    case UART_FIFO_RX4_8:
    	    	object->readMaxCount = 7;
    		break;
    	    case UART_FIFO_RX6_8:
    	    	object->readMaxCount = 11;
    		break;
    	    case UART_FIFO_RX7_8:
    	    	object->readMaxCount = 13;
    		break;
          }
    }else {
    	object->readMaxCount = 0xFFFFFFFF;
    }
    /* Disable preemption while checking if the UART is open. */
    key = Hwi_disable();

    /* Check if the UART is open already with the base addr. */
    if (object->opened == true) {
        Hwi_restore(key);

        Log_warning1("UART:(%p) already in use.", hwAttrs->baseAddr);
        return (NULL);
    }

    object->opened = true;
    Hwi_restore(key);

    /* Set UART variables to defaults. */
    object->writeBuf = NULL;
    object->readBuf = NULL;
    object->writeCount = 0;
    object->readCount = 0;
    object->writeSize = 0;
    object->readSize = 0;
    object->writeCR = false;

    /* Create Hwi object for this UART peripheral. */
    Hwi_Params_init(&(paramsUnion.hwiParams));
    paramsUnion.hwiParams.arg = (UArg)handle;

    Hwi_construct(&(object->hwi), hwAttrs->intNum, UARTTiva_hwiIntFxn,
                  &(paramsUnion.hwiParams), NULL);

    Semaphore_Params_init(&(paramsUnion.semParams));
    paramsUnion.semParams.mode = Semaphore_Mode_BINARY;

    /* If write mode is blocking create a semaphore and set callback. */
    if (object->writeMode == UART_MODE_BLOCKING) {
        Semaphore_construct(&(object->writeSem), 0, &(paramsUnion.semParams));
        object->writeCallback = &writeSemCallback;
    }

    /* If read mode is blocking create a semaphore and set callback. */
    if (object->readMode == UART_MODE_BLOCKING) {
        Semaphore_construct(&(object->readSem), 0, &(paramsUnion.semParams));
        object->readCallback = &readSemCallback;
    }

    /* Enable UART and its interrupt. */
  //  UARTTxIntModeSet(hwAttrs->baseAddr, UART_TXINT_MODE_EOT);
    UARTIntClear(hwAttrs->baseAddr, (UART_INT_TX | UART_INT_RX | UART_INT_RT));
    UARTIntEnable(hwAttrs->baseAddr, (UART_INT_TX | UART_INT_RX | UART_INT_RT));
    UARTEnable(hwAttrs->baseAddr);

    /* Set the FIFO level to 7/8 empty and 7/8 full. */
    UARTFIFOLevelSet(hwAttrs->baseAddr, UART_FIFO_TX4_8, tivaParams->fifolevel);

    BIOS_getCpuFreq(&freq);
    UARTConfigSetExpClk(hwAttrs->baseAddr,
                        freq.lo,
                        params->baudRate,
                        dataLength[params->dataLength] |
                        stopBits[params->stopBits] |
                        parityType[params->parityType]);

    Log_print3(Diags_USER1, "UART:(%p) CPU freq: %d; UART baudrate to %d",
               hwAttrs->baseAddr, freq.lo, params->baudRate);

    Log_print1(Diags_USER1, "UART:(%p) opened", hwAttrs->baseAddr);

    /* Return the handle */
    return (handle);
}

/*
 *  ======== UARTTiva_close ========
 */
void UARTTiva_close(UART_Handle handle)
{
    UARTTiva_Object           *object = (UARTTiva_Object *)handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = (UARTTiva_HWAttrs *)handle->hwAttrs;

    /* Disable UART and interrupts. */
    UARTIntDisable(hwAttrs->baseAddr, UART_INT_TX | UART_INT_RX | UART_INT_RT);
    UARTDisable(hwAttrs->baseAddr);

    /* Destruct the SYS/BIOS objects. */
    Hwi_destruct(&(object->hwi));
    Semaphore_destruct(&(object->writeSem));
    Semaphore_destruct(&(object->readSem));

    object->opened = false;

    Log_print1(Diags_USER1, "UART:(%p) closed", hwAttrs->baseAddr);
}

/*
 *  ======== UARTTiva_control ========
 *  @pre    Function assumes that the handle is not NULL
 */
int UARTTiva_control(UART_Handle handle, unsigned int cmd, void *arg)
{
	/* No implementation yet */
	return (UARTTiva_CMD_UNDEFINED);
}

/*
 *  ======== UARTTiva_write ========
 */
int UARTTiva_write(UART_Handle handle, const void *buffer, size_t size)
{
    unsigned int        key;
    UARTTiva_Object    *object = (UARTTiva_Object *)handle->object;

    /* Check that there is data to write */
    Assert_isTrue(size != 0, NULL);

    /* Disable preemption while checking if the uart is in use. */
    key = Hwi_disable();
    if (object->writeSize) {
        Hwi_restore(key);
        Log_warning1("UART:(%p) Could not write data, uart in use.",
                     ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr);

        return (UART_ERROR);
    }

    /* Save the data to be written and restore interrupts. */
    object->writeBuf = buffer;
    object->writeCount = 0;

    Hwi_restore(key);

    if (!(object->writeSize = writeData(object,
                                        (UARTTiva_HWAttrs *) handle->hwAttrs,
                                        size))) {
        /* Write is finished. */
        Log_print2(Diags_USER1, "UART(%p): Write finished, %d bytes written",
                   ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr,
                   object->writeCount);

        if (object->writeMode == UART_MODE_CALLBACK) {
            /* Reset the write buffer so we can pass it back */
            object->writeBuf = (unsigned char *)object->writeBuf - object->writeCount;
            object->writeCallback(handle, (uint8_t*)object->writeBuf,
                                  object->writeCount);

            /* Return 0 for UART_MODE_CALLBACK */
            return (0);
        }
        return (object->writeCount);
    }

    /* If writeMode is blocking, block and get the status. */
    if (object->writeMode == UART_MODE_BLOCKING) {
        /* Pend on semaphore and wait for Hwi to finish. */
        if (!Semaphore_pend(Semaphore_handle(&(object->writeSem)),
                            object->writeTimeout)) {
            /* Semaphore timed out, make the write empty and log the write. */
            object->writeSize = 0;

            Log_print2(Diags_USER1, "UART:(%p) Write timed out, %d bytes written",
                       ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr,
                       object->writeCount);
        }
        return (object->writeCount);
    }

    return (0);
}

/*
 *  ======== UARTTiva_writePolling ========
 */
int UARTTiva_writePolling(UART_Handle handle, const void *buf, size_t size)
{
    int32_t                    count = 0;
    UARTTiva_Object           *object = (UARTTiva_Object *)handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = (UARTTiva_HWAttrs *)handle->hwAttrs;
    unsigned char             *buffer = (unsigned char *)buf;

    /* Write characters. */
    while (size) {
        if (object->writeDataMode == UART_DATA_TEXT && *buffer == '\n') {
            UARTCharPut(hwAttrs->baseAddr, '\r');
            count++;
        }
        UARTCharPut(hwAttrs->baseAddr, *buffer);

        Log_print2(Diags_USER2, "UART:(%p) Wrote character 0x%x",
                   ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr,
                   *buffer);
        buffer++;
        count++;
        size--;
    }

    Log_print2(Diags_USER1, "UART:(%p) Write polling finished, %d bytes written",
               ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr,
               count);

    return (count);
}

/*
 *  ======== UARTTiva_writeCancel ========
 */
void UARTTiva_writeCancel(UART_Handle handle)
{
    unsigned int        key;
    UARTTiva_Object    *object = (UARTTiva_Object *)handle->object;

    /* Disable interrupts to avoid writing data while changing state. */
    key = Hwi_disable();

    /* Return if there is no write. */
    if (!object->writeSize) {
        Hwi_restore(key);
        return;
    }

    /* Set size = 0 to prevent writing and restore interrupts. */
    object->writeSize = 0;
    Hwi_restore(key);

    /* Reset the write buffer so we can pass it back */
    object->writeBuf = (unsigned char *)object->writeBuf - object->writeCount;
    object->writeCallback(handle,
                          (uint8_t*)object->writeBuf,
                          object->writeCount);

    Log_print2(Diags_USER1, "UART:(%p) Write canceled, %d bytes written",
               ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr,
               object->writeCount);
}

/*
 *  ======== UARTTiva_read ========
 */
int UARTTiva_read(UART_Handle handle, void *buffer, size_t size)
{
    unsigned int        key;
    UARTTiva_Object    *object = (UARTTiva_Object *)handle->object;

    /* Disable preemption while checking if the uart is in use. */
    key = Hwi_disable();
    if (object->readSize) {

        Hwi_restore(key);

        Log_warning1("UART:(%p) Could not read data, uart in use.",
                     ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr);
        return (UART_ERROR);
    }

    /* Save the data to be read and restore interrupts. */
    object->readBuf = buffer;
    object->readCount = 0;

    Hwi_restore(key);
    object->readSize = size;
    object->recieveTimeoutFlag = false;

    if (object->readMode == UART_MODE_CALLBACK)
    if (!(object->readSize = readData(object, (UARTTiva_HWAttrs *) handle->hwAttrs, size))) {
        /* Read is finished. */
        Log_print2(Diags_USER1, "UART:(%p) Read finished, %d bytes read",
                   ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr,
                   object->readCount);

        if (object->readMode == UART_MODE_CALLBACK) {
            /* Reset the read buffer so we can pass it back */
            object->readBuf = (unsigned char *)object->readBuf - object->readCount;
            object->readCallback(handle, object->readBuf, object->readCount);

            /* Return 0 for UART_MODE_CALLBACK */
            return (0);
        }
        return (object->readCount);
    }

    /* If readMode is blocking, block and get the status. */
    if (object->readMode == UART_MODE_BLOCKING) {
        /* Pend on semaphore and wait for Hwi to finish. */
        if (!Semaphore_pend(Semaphore_handle(&(object->readSem)),
                            object->readTimeout)) {
            /* Semaphore timed out, make the read empty and log the read. */
            object->readSize = 0;
            object->recieveTimeoutFlag = true;
            Log_print2(Diags_USER1, "UART:(%p) Read timed out, %d bytes read",
                       ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr,
                       object->readCount);
        }
        return (object->readCount);
    }
    return (0);
}

/*
 *  ======== UARTTiva_readPolling ========
 */
int UARTTiva_readPolling(UART_Handle handle, void *buf, size_t size)
{
    int32_t                    count = 0;
    UARTTiva_Object           *object = (UARTTiva_Object *)handle->object;
    UARTTiva_HWAttrs const    *hwAttrs = (UARTTiva_HWAttrs *)handle->hwAttrs;
    unsigned char             *buffer = (unsigned char *)buf;

    /* Read characters. */
    while (size) {
        *buffer = UARTCharGet(hwAttrs->baseAddr);
        Log_print2(Diags_USER2, "UART:(%p) Read character 0x%x",
                   hwAttrs->baseAddr, *buffer);
        count++;
        size--;

        if (object->readDataMode == UART_DATA_TEXT && *buffer == '\r') {
            /* Echo character if enabled. */
            if (object->readEcho) {
                UARTCharPut(hwAttrs->baseAddr, '\r');
            }
            *buffer = '\n';
        }

        /* Echo character if enabled. */
        if (object->readEcho) {
            UARTCharPut(hwAttrs->baseAddr, *buffer);
        }

        /* If read return mode is newline, finish if a newline was received. */
        if (object->readReturnMode == UART_RETURN_NEWLINE &&
            *buffer == '\n') {
            return (count);
        }

        buffer++;
    }

    Log_print2(Diags_USER1, "UART:(%p) Read polling finished, %d bytes read",
               hwAttrs->baseAddr, count);

    return (count);
}

/*
 *  ======== UARTTiva_readCancel ========
 */
void UARTTiva_readCancel(UART_Handle handle)
{
    unsigned int        key;
    UARTTiva_Object    *object = (UARTTiva_Object *)handle->object;

    /* Disable interrupts to avoid reading data while changing state. */
    key = Hwi_disable();

    /* Return if there is no read. */
    if (!object->readSize) {
        Hwi_restore(key);
        return;
    }

    /* Set size = 0 to prevent reading and restore interrupts. */
    object->readSize = 0;
    Hwi_restore(key);

    /* Reset the read buffer so we can pass it back */
    object->readBuf = (unsigned char *)object->readBuf - object->readCount;
    object->readCallback(handle, object->readBuf, object->readCount);

    Log_print2(Diags_USER1, "UART:(%p) Read canceled, %d bytes read",
               ((UARTTiva_HWAttrs const *)(handle->hwAttrs))->baseAddr,
               object->readCount);
}
