/*
 * RS232Syncro.cpp
 *
 *  Created on: 5 дек. 2017 г.
 *      Author: AhrameshinGN
 */
#include "RS232Syncro.hpp"


#include <../GyConCommon/dataTypes.h>
#include <../GyConCommon/ExchangeDrivers/UartDriver.h>
#include "Board.h"
#include <ti/sysbios/BIOS.h>
#include <xdc/cfg/global.h>


        void  ExtrenalDevieExchDriver_fastClockInt()
        {
            ExtrSyncroization::ExtrenalDevieExchDriver::dataConverter.clk1MS();
        }

namespace ExtrSyncroization
{

        MotionSynchroTransmiter ExtrenalDevieExchDriver::motionTransmiter;
        ExtrenalDeviceMode ExtrenalDevieExchDriver::mode=EXT_DEV_MOTION;
        ExtrenalDevices::CartoniDataConverter ExtrenalDevieExchDriver::dataConverter((Semaphore_Handle*)(&extrenalDevSem));
        UartDriver* ExtrenalDevieExchDriver::uartDriver;
        Uint8 ExtrenalDevieExchDriver::dataBuffer[DATA_BUFFER_LEN];
        bool ExtrenalDevieExchDriver::lockTransmitionTask=false;
        ProtocolDopReal::DataOutDopReal ExtrenalDevieExchDriver::dataDopReal;
        ProtocolDopReal::ProtocolDopReal protocolDopReal(ExtrenalDevieExchDriver::dataDopReal);
        uint32_t timeOutValue = 16;


        UInt8 testBuf[]={0xA2, 0x03, 0x7F, 0x7E, 0x00, 0x04, 0x00, 0x03, 0x42, 0x0A, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x7F ,0x00 } ;//18 el


        void ExtrenalDevieExchDriver::cartoniTask()
        {
            UInt16 byReceived=uartDriver->read(dataBuffer, DATA_BUFFER_LEN);
            if(byReceived>5)
            {
                dataConverter.convert(dataBuffer, DATA_BUFFER_LEN);
            }
            Task_sleep(1);
        }

        void ExtrenalDevieExchDriver::dopRealTask()
        {
            uint8_t buf[100];
            uint16_t count;
            protocolDopReal.creatData(&buf[0],&count);
            uartDriver->write(buf, count);
 //           Task_sleep(16); // на 20мс
            Task_sleep(timeOutValue);
        }

        void  ExtrenalDevieExchDriver::selectMode(ExtrenalDeviceMode mode_)
        {
#ifdef crane
            if(!(uartDriver->updateUartParams(38400, 30))){lockTransmitionTask=true;}
            else{lockTransmitionTask=false;}
            mode=mode_;
            return;
#endif
            switch(mode_)
            {
                case EXT_DEV_PAN_BAR:
                    if(!(uartDriver->updateUartParams(57600, 30))){lockTransmitionTask=true;}
                    else{lockTransmitionTask=false;}
                    mode=mode_;
                   break;
                case EXT_DEV_MOTION:
                    if(!(uartDriver->updateUartParams(115200, 30))){lockTransmitionTask=true;}
                    else{lockTransmitionTask=false;}
                    mode=mode_;
                    break;
                case EXT_DEV_DOPREAL:
                    if(!(uartDriver->updateUartParams(38400, 30))){lockTransmitionTask=true;}
                    else{lockTransmitionTask=false;}
                    mode=mode_;
                    break;
                default:
                break;
            }
        }

        void ExtrenalDevieExchDriver::init()
        {
            static UartDriver driver(Board_PULT_GYCON_MOTION_SYNC,115200,30);
            uartDriver=&driver;
            motionTransmiter.init(driver);

        }

        void ExtrenalDevieExchDriver::exchTask()
        {

            while(true)
            {
                if(lockTransmitionTask){Task_sleep(1);continue;}

                switch(mode)
                {
                    case EXT_DEV_DOPREAL:
                        dopRealTask();
                        break;
                    case EXT_DEV_PAN_BAR:
                        cartoniTask();
                        break;
                    case EXT_DEV_MOTION:
                    default:
                        motionTransmiter.transmit();
                        break;
                }
            }
        }
    }

//--------------------------------------
//---------------ExtrenalDeviceSyncronization
