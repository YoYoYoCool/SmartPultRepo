/*
 * ExchangeWheelManager.hpp
 *
 *  Created on: 16 апр. 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef EXTRENALDEVICESYNCHRO_DIGITALWHEEL_EXCHANGEWHEELMANAGER_HPP_
#define EXTRENALDEVICESYNCHRO_DIGITALWHEEL_EXCHANGEWHEELMANAGER_HPP_


#include <../GyConCommon/ExchangeDrivers/Rs485Driver.h>
#include <../GyConCommon/ExchangeBasic/BasicProtocol.h>
#include "Protocol.hpp"
#include "LensParam/LensPack.hpp"

namespace ExtrenalDevices {

enum {maxCounter=1000,
};



struct DataOut {
    int32_t* dataInput;  // указывает на данные куда их складывать
    uint8_t longData;
    bool validatData;
    bool connect;
    };

enum {
    invalidCreatPaket=-5,
    invalidDriverTx,
    invalidDriverRx,
    invalidCreatData,
    exchengeOk=1
};

union WheelExchengeError {
    uint32_t all;
    struct {
        int8_t  createPackError :8;
        int8_t  driverTxError :8;
        int8_t  driverRxError :8;
        int8_t  createDataError :8;
        };
    };



class DigitalWheelManager {


private:
    ProtocolWheel::WheelProtocol & protokol;
    Rs485Driver2 & driver;
    LensDb::LensPack& packRx;
    LensDb::LensPack& packTx;
    ProtocolWheel::SettingsDataIO settingsDataIO;


public:


    DigitalWheelManager (Rs485Driver2 & driver,
                         ProtocolWheel::WheelProtocol & protokol,
                         LensDb::LensPack& packRx,
                         LensDb::LensPack& packTx):
        driver(driver),
        protokol(protokol),
        packRx(packRx),
        packTx(packTx)
            {
            setUp();
            }



    inline int8_t exchenge (ProtocolWheel::WheelsType wheedId,
                            ProtocolWheel::Command comandID,
                            DataOut & dataOut) {
        WheelExchengeError errors;
        errors.all=0;
        clearPack(packRx,30);
        clearPack(packTx,30);
        settingsDataIO.wheelNumber=wheedId;
        settingsDataIO.comand=comandID;
        settingsDataIO.dataInput=dataOut.dataInput;
        settingsDataIO.longDataInput=&dataOut.longData;
        int8_t exchengeData = protokol.creatPaketOut(settingsDataIO);
        if (exchengeData!=ProtocolWheel::createData) {
            resetData(dataOut);
            return invalidCreatPaket;}
        if (driver.write(settingsDataIO.bufferTX,settingsDataIO.longDataTX)<settingsDataIO.longDataTX) {
            resetData(dataOut);
            return invalidDriverTx;}
        Task_sleep(1);
        if(driver.read(settingsDataIO.bufferRX,settingsDataIO.longDataRX)<settingsDataIO.longDataRX) {
            resetData(dataOut);
            return invalidDriverRx;}
        exchengeData = protokol.createDataInput(settingsDataIO);
        if (exchengeData!=ProtocolWheel::createData)  {
            resetData(dataOut);
            return invalidCreatPaket;   }
        dataOut.validatData=true;
        return exchengeOk;
        }


private:

    inline void setUp() {
        settingsDataIO.bufferRX=&packRx[0];
        settingsDataIO.bufferTX=&packTx[0];
        }


    inline void  resetData (DataOut & dataOut) {
        dataOut.dataInput[0]=0;
        dataOut.longData=0;
        dataOut.validatData=false;
    }

    inline void clearPack (LensDb::LensPack & pack, size_t size) {
        for (uint8_t i =0; i<size; i++) {
            pack[i]=0;
            }
        }



    inline void TXD () {
        }

    inline void RXD () {

        }


};




}

#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_EXCHANGEWHEELMANAGER_HPP_ */
