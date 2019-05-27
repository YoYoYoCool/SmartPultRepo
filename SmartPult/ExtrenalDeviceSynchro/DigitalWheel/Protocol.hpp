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

	struct __attribute__((packed)) WheelSimplePackTX
	{
		WheelSimplePackHeader 	header;
		WheelSimplePackDataTX 	body;
	};

    struct __attribute__((packed)) WheelSimplePackRXSpeed
    {
        WheelSimplePackHeader   header;
        WheelSimplePackDataRXSpeed   body;
    };

class WheelProtacol {

private:
	    LensDb::LensPackStatic<50> _packOut;
	    LensDb::LensPack & packIn;
	    int32_t speed;

public:
	    WheelProtacol (LensDb::LensPack & pack):  packIn(pack){
	        speed=0;
	    }
	    inline bool creatPaket(WheelsType wheelNumber, ReadCommand comand) {
	        if (wheelNumber>=WHEEL_MAX)
	            return false;

	        if ((comand>=WHEEL_MAX_COMAND)||(comand<=0))
	            return false;

	        WheelSimplePackTX paket ={
	            .header.marker=HEADER_MARKER,
	            .header.wheelType=wheelNumber,
	            .header.packetLen=sizeof(WheelSimplePackTX),
	            .body.readCommand=comand
	            };
	        paket.header.crc = findCrc((uint8_t*)(&(paket.header.marker)), HEADER_LEN-2);

	        paket.body.bodyCrc = findCrc((uint8_t*)(&(paket.body.readCommand)), paket.header.packetLen-HEADER_LEN-2);
/*
	        paket.body.bodyCrc=0;
	        paket.body.readCommand=0;

	        paket.header.crc=0;
	        paket.header.marker=0;
	        paket.header.packetLen=0;
	        paket.header.wheelType=(WheelsType)0;*/

	        ExtrenalDevices::DigitalWheelBuilder<WheelSimplePackTX> builder(_packOut);
	        builder.buildPack(paket);

	        for (uint8_t i =0; i<14; i++ )
	            packIn[i]=0;
	        return true;
	        }


	    inline LensDb::LensPack& pack () {
	        return _packOut;
	        }

	    inline bool createDataSpeed () {

	        WheelSimplePackRXSpeed paketInSpeed;
	        ExtrenalDevices::DigitalWheelParser<WheelSimplePackRXSpeed> parser(packIn);
	        parser.parsePack(paketInSpeed);
	        uint16_t headCRC = findCrc((uint8_t*)(&(paketInSpeed.header.marker)), HEADER_LEN-2);
	        if (headCRC!=paketInSpeed.header.crc) return false;

	        if (paketInSpeed.header.marker!=HEADER_MARKER) return false;

	        if (packIn.getSize()!=paketInSpeed.header.packetLen) return false;

	        uint16_t bodyCRC = findCrc((uint8_t*)(&(paketInSpeed.body.readCommand)), paketInSpeed.header.packetLen-HEADER_LEN-2);

	        if (bodyCRC!=paketInSpeed.body.bodyCrc) return false;
	        speed=paketInSpeed.body.speed;
	        return true;
	        }

        inline int32_t getSpeed () {
            return speed;
            speed=0.0;
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
