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

namespace WheelProtocol {


 enum WheelsType
	{
        WHEEL_PAN = 	0,
		WHEEL_ROLL = 	1,
		WHEEL_TILT = 	2,
		WHEEL_MAX
    };

    enum ReadCommand
	{
    	WHEEL_SPEED_REQUEST=1,
    	WHEEL_SPEED_RESPONSE=2,
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

    struct PaketDataOut {
        WheelsType wheelNumber;
        ReadCommand comand;
        uint8_t * buffer;
        uint8_t longBufOut;
        uint8_t longBufInput;
        };

    struct PaketDataSpeedInput {
        WheelsType wheelNumber;
        ReadCommand comand;
        uint8_t * buffer;
        int32_t * dataInput;
        uint8_t longDataBuf;
        };



    enum {

        invalidQuestion=-3,
        invalidHeaderCRC,
        invalidWheelNumber,
        invalidBodyCRC,
        createData
    };



class WheelProtacol {


public:

	    inline int8_t creatPaketOut(PaketDataOut & dataSettings) {
	        if ((dataSettings.wheelNumber>=WHEEL_MAX)||(dataSettings.wheelNumber<0))
	            return invalidWheelNumber;

	        if (dataSettings.comand==WHEEL_SPEED_REQUEST) {
	            dataSettings.longBufOut=sizeof(WheelSimplePackTXSpeed);
	            dataSettings.longBufInput=sizeof(WheelSimplePackRXSpeed);
	            WheelSimplePackTXSpeed paket ={
	                                           .header.marker=HEADER_MARKER,
	                                           .header.wheelType=dataSettings.wheelNumber,
	                                           .header.packetLen=sizeof(WheelSimplePackTXSpeed),
	                                           .body.readCommand=dataSettings.comand
	                                            };
	            paket.header.crc = findCrc((uint8_t*)(&(paket.header.marker)), HEADER_LEN-2);

	            paket.body.bodyCrc = findCrc((uint8_t*)(&(paket.body.readCommand)), paket.header.packetLen-HEADER_LEN-2);

	            ExtrenalDevices::DigitalWheelBuilder<WheelSimplePackTXSpeed> builder;
	            builder.buildPack(paket,dataSettings.buffer);

	            return createData;
	            }
	        return invalidQuestion;
	        }


	    inline int8_t createDataInput (PaketDataSpeedInput & dataSettings) {
	        if (dataSettings.comand==WHEEL_SPEED_REQUEST) {
	            WheelSimplePackRXSpeed paketInSpeed;
	            ExtrenalDevices::DigitalWheelParser<WheelSimplePackRXSpeed> parser;
	            parser.parsePack(paketInSpeed,dataSettings.buffer);

	            uint16_t headCRC = findCrc((uint8_t*)(&(paketInSpeed.header.marker)), HEADER_LEN-2);
	            if (headCRC!=paketInSpeed.header.crc)
	                return invalidHeaderCRC;

	            if (paketInSpeed.header.wheelType!=dataSettings.wheelNumber)
	                return invalidWheelNumber;

	            uint16_t bodyCRC = findCrc((uint8_t*)(&(paketInSpeed.body.readCommand)), paketInSpeed.header.packetLen-HEADER_LEN-2);
	            if (bodyCRC!=paketInSpeed.body.bodyCrc)
	                return invalidBodyCRC;

	            dataSettings.dataInput[0]=paketInSpeed.body.speed;
	            return createData;
	            }
	        return invalidQuestion;
	        }





private:



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

}
#endif /* PROTOCOL_HPP_ */
