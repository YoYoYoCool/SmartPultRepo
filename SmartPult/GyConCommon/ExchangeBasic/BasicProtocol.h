/*
 * Pult.h
 *
 *  Created on: 07 июля 2015 г.
 *      Author: Cno
 */

#ifndef GYCONCOMMON_EXCHANGEBASIC_BASICPROTOCOL_H_
#define GYCONCOMMON_EXCHANGEBASIC_BASICPROTOCOL_H_

#include <../GyConCommon/Exchange/Exchange.h>
#include <../GyConCommon/ExchangeBasic/BasicPack.h>
#include <../GyConCommon/ExchangeDrivers/Rs485Driver.h>
#include <ti/sysbios/knl/Task.h>


//#define COMMANDS_COUNT 6;
//typedef enum PultWriteCommand {
//	PULT_WRITE_CONTROLS = 0,
//	PULT_WRITE_AXIS_SETTINGS = 1,
//	PULT_WRITE_SETTINGS = 2
//} PultWriteCommands;
//
//typedef enum PultAskCommand {
//	PULT_GET_BASIC_DATA = 3,
//	PULT_GET_AXIS_SETTINGS = 4,
//	PULT_GET_SETTINGS = 5
//} PultAskCommands;'

#define BASIC_PROTOCOL_BROADCAST_ADDRESS 0xFF

typedef enum IQ {
	IQ0 = 0, IQ1, IQ2, IQ3, IQ4, IQ5, IQ6, IQ7, IQ8, IQ9,
	     IQ10, IQ11, IQ12, IQ13, IQ14, IQ15, IQ16
} IQ;

typedef struct BasicCmdValue {
	volatile float* const ptr;
	const IQ format;
} BasicValue;

typedef struct BasicCmd {
	BasicCmdValue* const values;
	const UInt8 count;
} BasicCmd;

typedef struct BasicCmdList {
	BasicCmd* const commands;
	const UInt8 count;
} BasicCmdList;

typedef struct BasicProtocolParams {
	BasicProtocolParams(UInt8 address, BasicCmdList* cmdList,
	                    UInt8 uartId, UInt16 recieveTimeout, UInt8 txEnablePin):
		address(address),
		cmdList(cmdList),
		uartId(uartId),
		txEnablePin(txEnablePin),
		recieveTimeout(recieveTimeout) {

	}
	UInt8 address;
	BasicCmdList* const cmdList;
	const UInt8 uartId;
	const UInt16 recieveTimeout;
	const UInt8 txEnablePin;
} BasicProtocolParams;

class BasicProtocol {
public:
	UInt8 address;
	BasicCmdList* const cmdList;
	BasicProtocol(BasicProtocolParams* params) :
			rs485Driver(params->uartId, 115200, params->recieveTimeout, params->txEnablePin),
			header(0xABCD),
			data(0x11),
			pack(&header, &data, &crc, buf, 256),
			exchange(&rs485Driver),
			address(params->address),
			cmdList(params->cmdList),
			dataWriter(&data),
			dataReader(&data){
	}
	virtual bool transaction(UInt8 myAddr, UInt8 theirAddr) {
		return false;
	}
	bool isConnected() {
		return exchange.connected;
	}
protected:
	bool writeData(UInt8 cmdId) {
		if (cmdId >= cmdList->count)
			return false;
		BasicCmd* cmd = &cmdList->commands[cmdId];
		UInt16 i = 0;
		BasicCmdValue* cmdValue = &cmd->values[i];
		dataWriter.reset();
		while (cmdValue->ptr != NULL) {
			dataWriter.write(*cmdValue->ptr, cmdValue->format);
			cmdValue = &cmd->values[++i];
		}
		return true;
	}
	bool readData(Uint8 cmdId) {
		if (cmdId >= cmdList->count)
			return false;
		BasicCmd* cmd = &cmdList->commands[cmdId];
		UInt16 i = 0;
		BasicCmdValue* value = &cmd->values[i];
		dataReader.reset();
		while (value->ptr != NULL) {
			float parseValue = dataReader.parse();
			*value->ptr = parseValue;
			value = &cmd->values[++i];
		}
		return true;
	}

	Rs485Driver rs485Driver;
	BasicHeader header;
	BasicTag8Data data;
	BasicCrc crc;
	UInt8 buf[256];
	Pack_ pack;
	Exchange exchange;
	BasicDataReader dataReader;
	BasicDataWriter dataWriter;
};

class BasicProtocolMaster: public BasicProtocol {
public:
	UInt8 writeCmdId;
	UInt8 askCmdId;
	BasicProtocolMaster(BasicProtocolParams* params):
		BasicProtocol(params) {
	}


	virtual bool transaction(UInt8 myAddr, UInt8 theirAddr) {
		bool res = false;
		header.address = theirAddr;
		header.writeCommand = writeCmdId;
		header.askCommand = askCmdId;
		writeData (writeCmdId);
		exchange.transmit(&pack);
		if (exchange.recieve(&pack)) {
			if (header.address == myAddr) {
				if (readData (askCmdId)) {
					res = true;
				}
			}
		}
		return res;
	}

};

class BasicProtocolSlave: public BasicProtocol {
public:
	BasicProtocolSlave(BasicProtocolParams* params):
		BasicProtocol(params) {
	}

	virtual bool transaction(UInt8 myAddr, UInt8 theirAddr) {
		bool res = false;
		if (recieve(myAddr)) {
			UInt8 writeCmdId = header.writeCommand;
			UInt8 askCmdId = header.askCommand;
			if (readData(writeCmdId)) {
				if (writeData(askCmdId)) {
//					Task_sleep(2);
					header.address = theirAddr;
					exchange.transmit(&pack);
				}
				res = true;
			}
		}
		return res;
	}

	bool recieve(UInt8 myAddr) {
		bool res = false;
		bool recieveSucessOrError = false;
		while(!recieveSucessOrError) {
			if (exchange.recieve(&pack)) {
				if ( (header.address == myAddr) || (header.address == BASIC_PROTOCOL_BROADCAST_ADDRESS) ) {
					UInt8 writeCmdId = header.writeCommand;
					res = readData(writeCmdId);
					recieveSucessOrError = true;
				}
			} else {
				recieveSucessOrError = true;
			}
		}
		return res;
	}

};

#endif /* GYCONCOMMON_EXCHANGEBASIC_BASICPROTOCOL_H_ */
