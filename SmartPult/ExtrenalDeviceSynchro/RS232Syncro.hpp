/*
 * RS232Syncro.hpp
 *
 *  Created on: 5 дек. 2017 г.
 *      Author: AhrameshinGN
 */

#ifndef EXTRENALDEVICESYNCHRO_RS232SYNCRO_HPP_
#define EXTRENALDEVICESYNCHRO_RS232SYNCRO_HPP_
#include "MotionControl/MotionSynchroModule.hpp"
#include <ExtrenalDeviceSynchro/PanBar/CartoniPanBar.hpp>

namespace ExtrSyncroization
{

    enum ExtrenalDeviceMode
    {
        EXT_DEV_MOTION=0,
        EXT_DEV_PAN_BAR,
    };
    enum dbl
    {
        DATA_BUFFER_LEN=100
    };

    class ExtrenalDevieExchDriver
    {
        public:

            static void exchTask();
            static void init();
            static void selectMode(ExtrenalDeviceMode mode_);
            static MotionSynchroTransmiter motionTransmiter;
            static ExtrenalDevices::CartoniDataConverter dataConverter;

            //Pan bar API
        private:
            static UInt8 dataBuffer[DATA_BUFFER_LEN];
            static ExtrenalDeviceMode mode;
            static UartDriver* uartDriver;
            static void cartoniTask();
            static bool lockTransmitionTask;

    };
}



#endif /* EXTRENALDEVICESYNCHRO_RS232SYNCRO_HPP_ */
