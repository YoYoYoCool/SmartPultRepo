/*
 * Protocol.hpp
 *
 *  Created on: 9 апр. 2019 г.
 * скорость 115200 8бит 1 стоп без париттета
 *      Author: AhrameshinGN
 */

#ifndef PROTOCOL_HPP_
#define PROTOCOL_HPP_

namespace wheelProtocol {


 enum WheelsType
	{
        WHEEL_PAN = 	0,
		WHEEL_DUTCH = 	1,
		WHEEL_TILT = 	2
    };

    enum ReadCommand
	{
    	WHEEL_SPEED_REQUEST=1,
    	WHEEL_SPEED_RESPONSE=2
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

	struct __attribute__((packed)) WheelSimplePackData
	{
		volatile uint16_t  		bodyCrc;
		volatile uint8_t   		readCommand;
		volatile uint32_t*  	content;
	};

	struct __attribute__((packed)) WheelSimplePackFrame
	{
		WheelSimplePackHeader 	header;
		WheelSimplePackData 	body;
	};


/*	inline uint16_t findCrc(uint8_t* pack, size_t crcPos)
	    {
	    if(crcPos>MAX_BUFFER_LEN)	{		crcPos=MAX_BUFFER_LEN;	}
	    uint16_t TMP, crcl, crch, crc = 0xffff;

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
	    }*/

}
#endif /* PROTOCOL_HPP_ */
