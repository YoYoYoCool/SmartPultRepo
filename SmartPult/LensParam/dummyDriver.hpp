/*
 * dummyDriver.hpp
 *
 *  Created on: 2 мар. 2019 г.
 *      Author: User9
 */

#ifndef LENSPARAM_DUMMYDRIVER_HPP_
#define LENSPARAM_DUMMYDRIVER_HPP_

#include "LensPack.hpp"
#include "LensDriver.hpp"

namespace LensDb
{
struct DummyDriverSettings {
    uint32_t setting0;
    uint32_t setting1;
    uint32_t setting2;
};


class DummyDriver : public LensDriver {
private:
    uint8_t pack[300];
    uint16_t offset;
public:

    DummyDriver(DummyDriverSettings& settings){}

    virtual void write(uint32_t addr, uint8_t* buf, size_t count) {

        for (Uint16 i=0; i<count;i++) {
            pack[i]=buf[i];
        }
        uint32_t a;// штоб было куда ткнуть точку останова
        a++;
        a--;
    }

    virtual void read (uint32_t addr, uint8_t* buf,size_t count) {

        for (Uint16 i=0; i<count;i++) {
                buf[i]=pack[offset];
                offset++;
                }
                uint32_t a;
                a++;
    }

    virtual void eraseObjective(uint32_t addr) {

    }

    virtual void clearOffset () {
        offset=0;
        }
};
}



#endif /* LENSPARAM_DUMMYDRIVER_HPP_ */
