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
        wheelPan = 	0,
		wheelRoll = 	1,
		wheelTilt = 	2,
		wheelMax
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

	enum HeaderMarker	{	HEADER_MARKER			 =0xAA55 };
	enum HeaderLen		{	HEADER_LEN				 =7		 };
	enum ReadCommandSize{   READ_COMMAND_SIZE        =1      };
	enum HeaderCrcSize  {   HEADER_CRC_SIZE          =2      };
	enum BodyCrcSize    {   BODY_CRC_SIZE            =2      };


	struct __attribute__((packed)) WheelSimplePackHeader
	{
		volatile uint16_t 		marker;
		volatile WheelsType  	wheelType;
		volatile uint16_t 		packetLen;
		volatile uint16_t 		crc;
	};

	struct __attribute__((packed)) WheelSimplePackDataTX
	{
		volatile uint16_t  		bodyCrc;
		volatile uint8_t   		readCommand;

	};
    struct __attribute__((packed)) WheelSimplePackDataRXSpeed
    {
        volatile uint16_t       bodyCrc;
        volatile uint8_t        readCommand;
        volatile uint32_t       speed;
    };

	struct __attribute__((packed)) WheelSimplePackTXSpeed
	{
		WheelSimplePackHeader 	header;
		WheelSimplePackDataTX 	body;
	};

    struct __attribute__((packed)) WheelSimplePackRXSpeed
    {
        WheelSimplePackHeader   header;
        WheelSimplePackDataRXSpeed   body;
    };

    struct SettingsDataIO {
        uint32_t wheelNumber;
        Command comand;
        uint8_t * bufferTX;
        uint8_t * bufferRX;
        uint8_t longDataTX;
        uint8_t longDataRX;
        int32_t * dataInput;
        uint8_t * longDataInput;
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

        inline virtual int8_t creatPaketOut(SettingsDataIO & dataSettings) {

        }

        inline virtual int8_t createDataInput(SettingsDataIO & dataSettings) {

        }

protected:

        inline uint16_t findCrc(uint8_t* pack, size_t crcPos)
        {
            if(crcPos>16)   {       crcPos=16;  }
            uint16_t TMP;
            uint16_t crcl;
            uint16_t crch;
            uint16_t crc = 0xffff;

            crcl = crc;
            crch = (crc >> 8);
            for (uint16_t i = 0; i < crcPos; i++)
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
	        if ((dataSettings.wheelNumber>=wheelMax)||(dataSettings.wheelNumber<0))
	            return invalidWheelNumber;

	        if (dataSettings.comand==wheelSpeedRequest) {
	            dataSettings.longDataTX=sizeof(WheelSimplePackTXSpeed);
	            dataSettings.longDataRX=sizeof(WheelSimplePackRXSpeed);
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
	        if (dataSettings.comand==wheelSpeedRequest) {
	            WheelSimplePackRXSpeed paketInSpeed;
	            ExtrenalDevices::DigitalWheelParser<WheelSimplePackRXSpeed> parser;
	            parser.parsePack(paketInSpeed,dataSettings.bufferRX);

	            uint16_t headCRC = findCrc((uint8_t*)(&(paketInSpeed.header.marker)), HEADER_LEN-2);
	            if (headCRC!=paketInSpeed.header.crc)
	                return invalidHeaderCRC;

	            if (paketInSpeed.header.wheelType!=dataSettings.wheelNumber)
	                return invalidWheelNumber;

	            uint16_t bodyCRC = findCrc((uint8_t*)(&(paketInSpeed.body.readCommand)), paketInSpeed.header.packetLen-HEADER_LEN-2);
	            if (bodyCRC!=paketInSpeed.body.bodyCrc)
	                return invalidBodyCRC;

	            dataSettings.dataInput[0]=paketInSpeed.body.speed;
	            dataSettings.longDataInput[0]=1; // –азмерность данных
	            return createData;
	            }
	        return invalidQuestion;
	        }
        };

struct __attribute__((packed)) WheelPackHeader
    {
        volatile uint16_t       marker;
        volatile uint32_t       wheelName;
        volatile uint16_t       packetLen;
        volatile uint16_t       crc;
    };

struct __attribute__((packed)) WheelPackRead
    {
    volatile uint16_t       bodyCrc;
    volatile uint8_t        readCommand;
    };

struct __attribute__((packed)) WheelPackWrite
    {
    volatile uint16_t       bodyCrc;
    volatile uint8_t        WriteCommand;
    };

struct __attribute__ ((packed)) WheelReadPac {
    WheelPackHeader         header;
    WheelPackRead           body;
    };

struct __attribute__ ((packed)) WheelWritePac {
    WheelPackHeader         header;
    WheelPackWrite          body;
    };

struct __attribute__ ((packed)) WheelSpeedPac {
    volatile uint16_t       bodyCrc;
    volatile uint8_t        readCommand;
    volatile int32_t        speed;
    };

struct __attribute__ ((packed)) WheelIdentificatorPac {
    volatile uint16_t       bodyCrc;
    volatile uint8_t        readCommand;
    volatile int32_t        identificator;
    };

struct __attribute__ ((packed)) WheelFriquencyPac {
    volatile uint16_t       bodyCrc;
    volatile uint8_t        readCommand;
    volatile int8_t         succered;
    };

enum SucceredWrite {
    error=0,
    appli
    };

class WhellProtocolNew: public WheelProtacolBase {

public:
    inline virtual int8_t createDataInput (SettingsDataIO & dataSettings) {

        }

    inline virtual int8_t creatPaketOut(SettingsDataIO & dataSettings) {
        if (dataSettings.comand==wheelSpeedRequest) {
            creatPacSpeedRequesten(dataSettings);
            return createData;}
        if(dataSettings.comand==wheelWriteFrequency) {
            creatPacWriteFreq(dataSettings);
            return createData;}
        if(dataSettings.comand==wheelGetIdentification) {
            creatPacGetIdentificator(dataSettings);
            return createData;}
        return invalidQuestion;
        }

private:

    inline void creatPacWriteFreq(SettingsDataIO & dataSettings) {

        }

    inline void creatPacSpeedRequesten(SettingsDataIO & dataSettings) {
        dataSettings.longDataTX=sizeof(WheelReadPac);
        dataSettings.longDataRX=sizeof(WheelPackHeader);
        dataSettings.longDataRX+=sizeof(WheelSpeedPac);
        WheelReadPac paket ={
                                       .header.marker=HEADER_MARKER,
                                       .header.wheelName=dataSettings.wheelNumber,
                                       .header.packetLen=sizeof(WheelReadPac),
                                       .body.readCommand=dataSettings.comand
                                        };
        paket.header.crc = findCrc((uint8_t*)(&(paket.header.marker)), HEADER_LEN-2);

        paket.body.bodyCrc = findCrc((uint8_t*)(&(paket.body.readCommand)), paket.header.packetLen-HEADER_LEN-2);

        ExtrenalDevices::DigitalWheelBuilder<WheelReadPac> builder;
        builder.buildPack(paket,dataSettings.bufferTX);
        }

    inline void creatPacGetIdentificator(SettingsDataIO & dataSettings) {
        dataSettings.longDataTX=sizeof(WheelReadPac);
        dataSettings.longDataRX=sizeof(WheelPackHeader);
        dataSettings.longDataRX+=sizeof(WheelIdentificatorPac);
        WheelReadPac paket ={
                              .header.marker=HEADER_MARKER,
                              .header.wheelName=dataSettings.wheelNumber,
                              .header.packetLen=sizeof(WheelReadPac),
                              .body.readCommand=dataSettings.comand
        };
        paket.header.crc = findCrc((uint8_t*)(&(paket.header.marker)), HEADER_LEN-2);

        paket.body.bodyCrc = findCrc((uint8_t*)(&(paket.body.readCommand)), paket.header.packetLen-HEADER_LEN-2);

        ExtrenalDevices::DigitalWheelBuilder<WheelReadPac> builder;
        builder.buildPack(paket,dataSettings.bufferTX);
        }
    };

}
#endif /* PROTOCOL_HPP_ */
