/*
 * MX66L51235FDriver.hpp
 *
 *  Created on: 27 февр. 2019 г.
 *      Author: KapustinIV
 */

#ifndef MODULES_LENSDB_MX66L51235FDRIVER_HPP_
#define MODULES_LENSDB_MX66L51235FDRIVER_HPP_

#include "LensDriver.hpp"
#include "drivers/mx66l51235f.h"

namespace LensDb
{

enum {maxSizePageProgram=255,
     sizePage=256};


struct MX66L51235FDriverSettings {
    uint32_t setting0;
    uint32_t setting1;
    uint32_t setting2;
};

class MX66L51235FDriver : public LensDriver
{
public:
    MX66L51235FDriver(MX66L51235FDriverSettings& settings) {
    }
    virtual void write(uint32_t addr, uint8_t* buf, size_t count) {
        uint16_t countWrite=count/sizePage; //считаем колличество пакетов на передачу в флэшку
        uint16_t longAndPage=count-(sizePage*count); //считаем длинну последнего пакета
        uint16_t bufAddrAndPage=sizePage*count;//считаем стартовый адрес последнего пакета
        for (uint16_t index=0; index<countWrite; index++){
            MX66L51235FPageProgram(addr,&buf[index*sizePage],maxSizePageProgram);
            addr+=maxSizePageProgram;}
        MX66L51235FPageProgram(addr,&buf[bufAddrAndPage],longAndPage);
    }
    virtual void read(uint32_t addr, uint8_t* buf, size_t count) {
        MX66L51235FRead(addr, buf,count);
    }
    virtual void eraseObjective (uint32_t addr) {
        MX66L51235FSectorErase(addr);
        }
    virtual void clearOffset () {}
private:


};

} /* namespace LensDb */

#endif /* MODULES_LENSDB_MX66L51235FDRIVER_HPP_ */
