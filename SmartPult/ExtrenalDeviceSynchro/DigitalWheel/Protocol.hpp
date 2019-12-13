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
#include <Pult/PultClasses.h>
#include "WheelBuilder.hpp"
#include "WheelParser.hpp"
#include "Libs/Infirmation.hpp"


namespace ProtocolWheel {


 enum WheelsType
	{
        wheelPan = 	0,
		wheelRoll = 	1,
		wheelTilt = 	2,
		wheelMax
    };

    enum CommandRead
	{
    	wheelSpeedRequest=1,
    	wheelSpeedResponse=2,
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


	struct __attribute__((packed)) WheelSimplePackHeaderV1
	{
		 uint16_t 		marker;
		 WheelsType  	wheelType;
		 uint16_t 		packetLen;
		uint16_t 		crc;
	};

	struct __attribute__((packed)) WheelSimplePackDataTXV1
	{
		uint16_t  		bodyCrc;
		uint8_t   		readCommand;
	};

    struct __attribute__((packed)) WheelSimplePackDataRXV1
    {
        uint16_t       bodyCrc;
        uint8_t        readCommand;
        int32_t       speed;
    };

	struct __attribute__((packed)) WheelSimplePackTXSpeed
	{
		WheelSimplePackHeaderV1 	header;
		WheelSimplePackDataTXV1 	body;
	};

    struct __attribute__((packed)) WheelSimplePackRXSpeed
    {
        WheelSimplePackHeaderV1   header;
        WheelSimplePackDataRXV1   body;
    };

    struct SettingsDataIO {
        uint32_t wheelNumber;
        CommandRead comand;
        uint8_t * bufferTX;
        uint8_t * bufferRX;
        uint8_t longDataTX;
        uint8_t longDataRX;
        int32_t * dataInput;
        uint8_t * longDataInput;
        };


    enum {
        invalidWheelId=-5,
        invalidQuestion,
        invalidWriteComand,
        invalidHeaderCRC,
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
	            return invalidWheelId;

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
	                return invalidWheelId;

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

    enum CommandReadV2
        {
        wheelMultiRequestV2,
        wheelSpeedRequestV2,
        wheelWorkTime,
        wheelInformationRequestV2
        };

    enum CommandWriteV2
        {
        wheelNotWriteCommandV2,
        wheelWriteFrequencyV2,
        wheelWriteKspeedV2,
        wheelWriteDeadZoneV2
        };

    enum WheelsTypeV2
       {

           wheelMultiZaprosV2,
           wheelPanV2,
           wheelRollV2,
           wheelTiltV2
       };



    struct __attribute__((packed)) WheelSimplePackHeaderV2
        {
            volatile uint16_t       marker;
            volatile WheelsTypeV2   wheelType;
            volatile uint16_t       packetLen;
            volatile uint16_t       crc;
        };

    struct __attribute__((packed)) WheelSimplePackHeaderV3
            {
                volatile uint16_t       marker;
                volatile uint32_t       wheelID;
                volatile uint16_t       packetLen;
                volatile uint16_t       crc;
            };

    class CommandCreatBaseV2 {

    protected:

        LensDb::LensPack& pack;
        CommandReadV2 & commandRead;
        WheelsTypeV2 &wheelType;
        static WheelSimplePackHeaderV2 header;
        uint8_t & longBody;

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

        template <typename TData>
        void parser(TData & data, uint8_t & pack ) {
            ExtrenalDevices::DigitalWheelParser<TData> parser;
            parser.parsePack(data,&pack);        }

        template <typename TData>
        void builder(TData & data, uint8_t & pack ) {
            ExtrenalDevices::DigitalWheelBuilder<TData> builder;
            builder.buildPack(data,&pack);        }

        template <typename TData>
        void buildHeader() {
            header.wheelType = wheelType;
            header.packetLen = sizeof(WheelSimplePackHeaderV2);
            header.packetLen+= sizeof(TData);
            header.crc = findCrc((uint8_t*)(&(header.marker)), sizeof(WheelSimplePackHeaderV2)-2);
            builder<WheelSimplePackHeaderV2>(header, pack[0]);   }

        template <typename TData>
        int8_t createPackBodyRX(TData &body){
            parser<TData>(body, pack[0]);
            uint8_t * question = (uint8_t*) &body;
            uint16_t * crc = (uint16_t*) &body;
            if (crc[0]!=findCrc(&question[2], sizeof(TData)-2))
                return invalidBodyCRC;
            if (question[2]!=(uint8_t)this->commandRead)
                return invalidQuestion;
            return createData;
        }

    public:

        CommandCreatBaseV2(LensDb::LensPack& pack,
                           CommandReadV2 & commandRead,
                           WheelsTypeV2 &wheelType,
                           uint8_t & longBody):
            pack(pack),commandRead(commandRead),
            wheelType(wheelType),longBody(longBody){
                                                  header.marker = (uint16_t)HEADER_MARKER;
            }

        virtual void buildPack () {

            }

        int8_t parserHeaderPack () {
            parser<WheelSimplePackHeaderV2>(header, pack[0]);
            if (header.crc!=findCrc((uint8_t*)(&(header.marker)), sizeof(WheelSimplePackHeaderV2)))
                return invalidHeaderCRC;
            if (header.wheelType!=wheelType)
                return invalidWheelId;
            longBody = header.packetLen - sizeof(WheelSimplePackHeaderV2);
            return createData;
            }

        virtual int8_t parserBodyPack () {
            return createData;
        }

        virtual void updateInputData() {

        }

    };

    struct __attribute__((packed)) WheelSimplePackMultiZaprosRXV1
    {
        volatile uint16_t       bodyCrc;
        volatile uint8_t        readComand;
        volatile uint8_t        writeComand;
        volatile float          dataWrite;
    };

    class CommandCreatWheelMultiZaprosV2:public CommandCreatBaseV2 {

    public:

        CommandCreatWheelMultiZaprosV2(LensDb::LensPack& pack,
                                       CommandReadV2 & commandRead,WheelsTypeV2 &wheelType,uint8_t & longBody):
                                       CommandCreatBaseV2(pack,commandRead,wheelType,longBody) {}

        virtual void buildPack () {
            header.wheelType = wheelType;
            header.packetLen = sizeof(WheelSimplePackHeaderV2);
            header.crc = findCrc((uint8_t*)(&(header.marker)), sizeof(WheelSimplePackHeaderV2)-2);
            builder<WheelSimplePackHeaderV2>(header, pack[0]);
            this->longBody=header.packetLen;
            }

    };

    class CommandCreatWheelNotWriteCommandV2:public CommandCreatBaseV2 {

    public:

        CommandCreatWheelNotWriteCommandV2(LensDb::LensPack& pack,
                                                            CommandReadV2 & commandRead,
                                                            WheelsTypeV2 &wheelType,
                                                            uint8_t & longBody):
                                                            CommandCreatBaseV2(pack,commandRead,wheelType,longBody)
                                                            {}

        virtual void buildPack () {
            buildHeader<WheelSimplePackDataTXV1>();
            WheelSimplePackDataTXV1 bodyTx;
            bodyTx.readCommand = this->commandRead;
            bodyTx.bodyCrc = findCrc((uint8_t*)(&(bodyTx.readCommand)), sizeof(WheelSimplePackDataTXV1)-2);
            builder<WheelSimplePackDataTXV1>(bodyTx, pack[sizeof(WheelSimplePackHeaderV2)]);
            this->longBody=header.packetLen;
            }



    };

    class JetDataNotWrite: public CommandCreatWheelNotWriteCommandV2 {

    private:

        int32_t ** Content;
        WheelSimplePackDataRXV1 bodyRx;

    public:

        JetDataNotWrite(LensDb::LensPack& pack,
                 CommandReadV2 & commandRead,
                 WheelsTypeV2 &wheelType,
                 uint8_t & longBody,int32_t ** Content):
                 CommandCreatWheelNotWriteCommandV2(pack,commandRead,wheelType,longBody),
                 Content(Content)
                    {}


        virtual int8_t parserBodyPack () {   return createPackBodyRX<WheelSimplePackDataRXV1>(bodyRx);        }

        virtual void updateInputData() {     Content[0][0] = bodyRx.speed;        }

    };




    struct __attribute__((__packed__)) WheelSimplePackInformationRXV1 {
        volatile uint16_t       bodyCrc;
        volatile uint8_t        readCommand;
        volatile InformationStruct::Information    informationWheel;
        };

    class CommandCreatWheelInformationRequestV2:public CommandCreatWheelNotWriteCommandV2 {

    private:

      InformationStruct::Information ** wheelInformation;
      WheelSimplePackInformationRXV1 bodyRx;

    public:

        CommandCreatWheelInformationRequestV2(LensDb::LensPack& pack,
                                              CommandReadV2 & commandRead,
                                              WheelsTypeV2 &wheelType,
                                              uint8_t & longBody,
                                              InformationStruct::Information ** wheelInformation):
                                              CommandCreatWheelNotWriteCommandV2(pack,commandRead,wheelType, longBody),
                                              wheelInformation(wheelInformation) {}

        virtual int8_t parserBodyPack () {   return createPackBodyRX<WheelSimplePackInformationRXV1>(bodyRx);        }

        virtual void updateInputData() {
            wheelInformation[0]->dataOfManufacturing.all = bodyRx.informationWheel.dataOfManufacturing.all;
            wheelInformation[0]->number = bodyRx.informationWheel.number;
            wheelInformation[0]->version.all = bodyRx.informationWheel.version.all;
        }

    };

    struct __attribute__((packed)) WheelSimplePackReadSpeedDataRXV2
    {
        volatile uint16_t       bodyCrc;
        volatile uint8_t        readCommand;
        volatile int32_t        speed;
        volatile uint8_t        buttonState;
    };

    struct __attribute__((packed)) WheelSimplePackDataTXV2
    {
        volatile uint16_t       bodyCrc;
        volatile uint8_t        readComand;
        volatile uint8_t        writeComand;
        volatile float          dataWrite;
    };

    class CommandCreatRequestAndWriteCommandV2:public CommandCreatBaseV2 {

    private:

        CommandWriteV2 & commandWrite;
        float ** writeContent;
        int32_t ** contentRead;
        int8_t ** buttonState;
        WheelSimplePackReadSpeedDataRXV2 bodyRx;

    public:

        CommandCreatRequestAndWriteCommandV2(LensDb::LensPack& pack,CommandReadV2 & commandRead,CommandWriteV2 & commandWrite,
                                             WheelsTypeV2 &wheelType,uint8_t & longBody,int32_t ** contentRead,
                                             float ** writeContent,int8_t ** buttonState):
                                                         CommandCreatBaseV2(pack,commandRead,wheelType,longBody),
                                                         commandWrite(commandWrite),
                                                         writeContent(writeContent),
                                                         contentRead(contentRead),
                                                         buttonState(buttonState){}

        virtual void buildPack () {
            buildHeader<WheelSimplePackDataTXV2>();
            WheelSimplePackDataTXV2 bodyTx;
            bodyTx.readComand = this->commandRead;
            bodyTx.writeComand = this->commandWrite;
            bodyTx.dataWrite = writeContent[0][0];
            bodyTx.bodyCrc = findCrc((uint8_t*)(&(bodyTx.readComand)), sizeof(WheelSimplePackDataTXV2)-2);
            builder<WheelSimplePackDataTXV2>(bodyTx, pack[sizeof(WheelSimplePackHeaderV2)]);
            }

        virtual int8_t parserBodyPack () {   return createPackBodyRX<WheelSimplePackReadSpeedDataRXV2>(bodyRx);        }

        virtual void updateInputData() {
            contentRead[0][0]=bodyRx.speed;
            buttonState[0][0] = bodyRx.buttonState;
        }

    };


    //--------------------------------------------------------------------

    enum UsedProtocol{
        multiZaprosProtocol,
        readNotWriteProtacol,
        readAndWriteProtocol,
        readInformationProtocol,
        maxProtocol
    };

    class WheelProtacolBaseV2 {
        public:

            WheelProtacolBaseV2(LensDb::LensPack& pack):pack(pack) {}

                inline virtual int8_t creatPaketOut() {

                    return 0;
                }

                inline virtual int8_t createDataInput() {
                    return 0;
                }

                inline uint8_t getLongPackTx () {   return   longData;      }

                inline uint8_t getLongPackRx () {   return   longData;      }

        protected:

                LensDb::LensPack& pack;
                uint8_t longData;

            };

        class WheelProtocolV2: public WheelProtacolBaseV2 {

        private:

            WheelsTypeV2 &wheelType;
            CommandReadV2 & commandRead;
            CommandWriteV2 & commandWrite;
            CommandCreatBaseV2 * creator[4];
            CommandCreatWheelMultiZaprosV2 multiZapros;
            JetDataNotWrite getDataNotWrite;
            CommandCreatWheelInformationRequestV2 getInformation;
            CommandCreatRequestAndWriteCommandV2 getDataAndWrite;
            UsedProtocol usedProtocol;


        public:

            WheelProtocolV2(LensDb::LensPack& pack, WheelsTypeV2 &wheelType, CommandReadV2 & commandRead,
                            CommandWriteV2 & commandWrite, int32_t ** contentRead, float ** contentWrite,
                            InformationStruct::Information ** wheelInformation, int8_t **buttonState):
                            WheelProtacolBaseV2(pack), wheelType(wheelType), commandRead(commandRead),
                            commandWrite(commandWrite),multiZapros(pack,commandRead,wheelType,this->longData),
                            getDataNotWrite(pack,commandRead,wheelType,this->longData,contentRead),
                            getInformation(pack,commandRead,wheelType,this->longData,wheelInformation),
                            getDataAndWrite(pack,commandRead,commandWrite,wheelType,this->longData,contentRead,contentWrite,buttonState)
                            {
                            creator[(uint8_t)multiZaprosProtocol]=&multiZapros;
                            creator[(uint8_t)readNotWriteProtacol]=&getDataNotWrite;
                            creator[(uint8_t)readAndWriteProtocol]=&getDataAndWrite;
                            creator[(uint8_t)readInformationProtocol]=&getDataAndWrite;
                            }

                inline virtual int8_t creatPaketOut() {
                    if ((wheelType<wheelMultiZaprosV2)||(wheelType>wheelTiltV2))                        return invalidWheelId;

                    if ((commandRead<wheelMultiRequestV2)||(commandRead>wheelInformationRequestV2))    return invalidQuestion;

                    if ((commandWrite<wheelNotWriteCommandV2)||(commandWrite>wheelWriteDeadZoneV2))     return invalidWriteComand;

                    creator[analisedUsedProtocol()]->parserHeaderPack();
                    creator[analisedUsedProtocol()]->buildPack();

                    }


                inline virtual int8_t createDataInput () {

                    return invalidQuestion;
                    }


        private:

                uint8_t analisedUsedProtocol() {
                    if (commandRead==wheelMultiRequestV2)
                        return (uint8_t)multiZaprosProtocol;
                    if (commandRead==wheelInformationRequestV2)
                        return readInformationProtocol;
                    if ((commandRead>wheelMultiRequestV2)&&(commandWrite==wheelNotWriteCommandV2))
                        return readNotWriteProtacol;
                    if ((commandRead>wheelMultiRequestV2)&&(commandWrite>wheelNotWriteCommandV2))
                        return readAndWriteProtocol;
                }

                };

}
#endif /* PROTOCOL_HPP_ */
