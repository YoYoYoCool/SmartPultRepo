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
    int32_t* dataInput;
    uint8_t* longData;
    bool validatData;
    };

enum {
    invalidCreatPaket=-5,
    invalidDriverTx,
    invalidDriverRx,
    invalidCreatData,
    exchengeOk=1
};


class DigitalWheelManager {


private:
    WheelProtocol::WheelProtacol & protokol;
    Rs485Driver2 & driver;
    LensDb::LensPackStatic<30> packInput;
    LensDb::LensPackStatic<30> packOutput;
    int32_t dataInput;
    int16_t counter;
    bool conect;
    WheelProtocol::PaketDataOut dataTx;
    WheelProtocol::PaketDataSpeedInput dataRx;
    DataOut & dataOut;


public:

    DigitalWheelManager (Rs485Driver2 & driver, WheelProtocol::WheelProtacol & protokol,
                         WheelProtocol::WheelsType wheelId, DataOut & dataOut) :
    driver(driver),
    protokol(protokol),
    dataOut(dataOut){
        counter=0;
        dataTx.buffer=&packOutput[0];
        dataTx.wheelNumber=wheelId;
        dataTx.longBufOut=0;
        dataRx.buffer=&packInput[0];
        dataRx.dataInput=&dataInput;
        dataRx.wheelNumber=wheelId;
        dataOut.dataInput=dataRx.dataInput;
        dataOut.longData=&dataRx.longDataBuf;
        dataOut.validatData=false;
        }

    inline int8_t exchenge (WheelProtocol::ReadCommand comandID  ) {
        dataTx.comand=comandID;
        dataRx.comand=comandID;
        int8_t exchengeData = protokol.creatPaketOut(dataTx);
        if (exchengeData!=WheelProtocol::createData) {
            resetData();
            return invalidCreatPaket;}
        if (driver.write(dataTx.buffer,dataTx.longBufOut)<dataTx.longBufOut) {
            resetData();
            return invalidDriverTx;}
        Task_sleep(1);
        if(driver.read(dataRx.buffer,dataTx.longBufInput)<dataTx.longBufInput) {
            resetData();
            return invalidDriverRx;}
        exchengeData = protokol.createDataInput(dataRx);
        if (exchengeData!=WheelProtocol::createData)  {
            resetData();
            return invalidCreatPaket;   }
        dataOut.validatData=true;
        return exchengeOk;

        }

    inline DataOut getData() {
        return dataOut;
        }

    inline bool getConnect () { return conect; }

private:
    inline void  resetData () {
        dataOut.dataInput[0]=0;
        dataOut.longData=0;
        dataOut.validatData=false;
    }


    inline void TXD () {
        }

    inline void RXD () {

        }


};




}

#endif /* EXTRENALDEVICESYNCHRO_DIGITALWHEEL_EXCHANGEWHEELMANAGER_HPP_ */
