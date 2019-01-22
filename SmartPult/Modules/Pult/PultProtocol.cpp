/*
 * PultProtocol.cpp
 *
 *  Created on: 24 дек. 2014 г.
 *      Author: CNO
 */

#include <Modules/Pult/PultProtocol.h>
#include <ti/sysbios/BIOS.h>

PultProtocol::PultProtocol(ExchangeDriver* driver):
driver(driver),
byteArray(txBytes, 128) {

}

void PultProtocol::transmit(ExchangeDataList* dataList) {
    UInt16 id = 0;
    UInt16 offset = 0;
    ExchangeData* data;
    byteArray.clear();
    txBytes[0] = 0xA4;
    offset++; offset++;
    data = dataList->items[id];
    while (data!=NULL) {
     	data->addTo(txBytes, &offset);
    	data = dataList->items[++id];
    };
    txBytes[1] = offset;
    byteArray.size = offset;
    crc16.insertCrc(&byteArray, offset);
    offset+=2;
    driver->write(txBytes, offset);
}
