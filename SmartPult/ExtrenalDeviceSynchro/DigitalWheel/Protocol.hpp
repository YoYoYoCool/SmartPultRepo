/*
 * Protocol.hpp
 *
 *  Created on: 9 апр. 2019 г.
 * скорость 115200 8бит 1 стоп без париттета
 *      Author: AhrameshinGN , Cheverev Kirill
 */

#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_


#include "LensParam/LensPack.hpp"
#include "WheelBuilder.hpp"
#include "WheelParser.hpp"

namespace ProtocolWheel {


 enum WheelsType
    {
        wheelPan =  0,
        wheelRoll =     1,
        wheelTilt =     2,
        pedal   =   3,
        shakerBox = 4,
        panBar = 31
    };

    enum Command
    {
        wheelSpeedRequest=1,
        WHEEL_SPEED_RESPONSE=2,
        wheelWriteFrequency,
        wheelGetIdentification,
        WHEEL_MAX_COMAND
    };

    enum {
        longDataSpeed=14
    };// длинна ответного пакета

    enum HeaderMarker   {   HEADER_MARKER            =0xAA55 };
    enum HeaderLen      {   HEADER_LEN               =7      };
    enum ReadCommandSize{   READ_COMMAND_SIZE        =1      };
    enum HeaderCrcSize  {   HEADER_CRC_SIZE          =2      };
    enum BodyCrcSize    {   BODY_CRC_SIZE            =2      };


    struct __attribute__((packed)) WheelSimplePackHeader
    {
        volatile uint16_t       marker;
        volatile WheelsType     wheelType;
        volatile uint16_t       packetLen;
        volatile uint16_t       crc;
    };

    struct __attribute__((packed)) WheelSimplePackDataTX
    {
        volatile uint16_t       bodyCrc;
        volatile uint8_t        readCommand;

    };
    struct __attribute__((packed)) WheelSimplePackDataRXSpeed
    {
        volatile uint16_t       bodyCrc;
        volatile uint8_t        readCommand;
        volatile int32_t       speed[10];
    };

    struct __attribute__((packed)) WheelSimplePackTXSpeed
    {
        WheelSimplePackHeader   header;
        WheelSimplePackDataTX   body;
    };

    struct __attribute__((packed)) WheelSimplePackRXSpeed
    {
        WheelSimplePackHeader   header;
        WheelSimplePackDataRXSpeed   body;
    };

    struct SettingsDataIO {
        int32_t wheelNumber;
        Command comand;
        uint8_t * bufferTX;
        uint8_t * bufferRX;
        uint8_t longDataTX;
        uint8_t longDataRX;
        int32_t * dataInput;
        uint8_t longDataInput;
        };

    enum {
        invalidQuestion=-3,
        invalidHeaderCRC,
        invalidWheelNumber,
        invalidBodyCRC,
        createData
    };

class WheelProtacolBase {
public:

        inline virtual int8_t creatPaketOut(SettingsDataIO & dataSettings) =0;


        inline virtual int8_t createDataInput(SettingsDataIO & dataSettings) =0;

protected:

        inline uint16_t findCrc(uint8_t* pack, size_t crcPos)
        {
            uint16_t TMP;
            uint16_t crcl;
            uint16_t crch;
            uint16_t crc = 0xffff;

            crcl = crc;
            crch = (crc >> 8);
            for (uint16_t i = 0; i != crcPos; i++)
            {
                uint16_t dataTmp = pack[i];
                dataTmp = (dataTmp ^ crcl);
                TMP = (dataTmp << 4);
                dataTmp = (TMP ^ dataTmp);
                TMP = (dataTmp >> 5);
                TMP &= 0x07;
                crcl = crch;
                crch = (dataTmp ^ TMP);
                TMP = (dataTmp << 3);
                crcl = (crcl ^ TMP);
                TMP = (dataTmp >> 4);
                TMP &= 0x0F;
                crcl = (crcl ^ TMP);
            }

            crc = (crch << 8) + (crcl & 0x00FF);
            return crc;
        }

    };

class WheelProtocol: public WheelProtacolBase {


public:

        inline virtual int8_t creatPaketOut(SettingsDataIO & dataSettings) {
            if (dataSettings.wheelNumber<0)
                return invalidWheelNumber;

            if (dataSettings.comand==wheelSpeedRequest) {
                dataSettings.longDataTX=sizeof(WheelSimplePackTXSpeed);
                dataSettings.longDataRX=sizeof(WheelSimplePackRXSpeed);
                if (dataSettings.wheelNumber<shakerBox)
                {
                    dataSettings.longDataRX-=((10-1)*4);
                    dataSettings.longDataInput = 1;
                }
                if ((dataSettings.wheelNumber==panBar))
                {
                    dataSettings.longDataRX-=((10-7)*4);
                    dataSettings.longDataInput = 7;
                }
                WheelSimplePackTXSpeed paket ={
                                               .header.marker=HEADER_MARKER,
                                               .header.wheelType=(WheelsType)dataSettings.wheelNumber,
                                               .header.packetLen=sizeof(WheelSimplePackTXSpeed),
                                               .body.readCommand=dataSettings.comand
                                                };
                paket.header.crc = findCrc((uint8_t*)(&(paket.header.marker)), HEADER_LEN-2);

                paket.body.bodyCrc = findCrc((uint8_t*)(&(paket.body.readCommand)), paket.header.packetLen-HEADER_LEN-2);

                ExtrenalDevices::DigitalWheelBuilder<WheelSimplePackTXSpeed> builder;
                builder.buildPack(paket,dataSettings.bufferTX);

                return createData;
                }
            return invalidQuestion;
            }


        inline virtual int8_t createDataInput (SettingsDataIO & dataSettings) {

                WheelSimplePackRXSpeed paketIn;
                ExtrenalDevices::DigitalWheelParser<WheelSimplePackRXSpeed> parser;

                memcpy((uint8_t*)&paketIn,dataSettings.bufferRX,sizeof(WheelSimplePackRXSpeed));
 //               parser.parsePack(paketIn,dataSettings.bufferRX);

                uint16_t headCRC = findCrc((uint8_t*)&paketIn.header.marker, HEADER_LEN-2);
                if (headCRC!=paketIn.header.crc)
                    return invalidHeaderCRC;

                if (paketIn.header.wheelType!=dataSettings.wheelNumber)
                    return invalidWheelNumber;

                uint16_t bodyCRC = findCrc((uint8_t*)&paketIn.body.readCommand, (paketIn.header.packetLen-HEADER_LEN)-2);
                if (bodyCRC!=paketIn.body.bodyCrc)
                    return invalidBodyCRC;
                for (uint8_t i =0; i<dataSettings.longDataInput;i++)
                {
                    dataSettings.dataInput[i]=paketIn.body.speed[i];
                }
                return createData;

            return invalidQuestion;
            }
        };





}
#endif /* PROTOCOL_HPP_ */
