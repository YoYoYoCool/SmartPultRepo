/*
 * dummyDriver.hpp
 *
 *  Created on: 2 мар. 2019 г.
 *      Author: User9
 */

#ifndef LENSPARAM_DUMMYDRIVER_HPP_
#define LENSPARAM_DUMMYDRIVER_HPP_

#include "LensPack.hpp"

namespace LensDb
{
struct DummyDriverSettings {
    uint32_t setting0;
    uint32_t setting1;
    uint32_t setting2;
};


class DummyDriver : public LensDriver {
private:
    LensPackStatic<4096> pack;
public:

    DummyDriver(DummyDriverSettings& settings){}

    virtual void write(uint32_t addr, uint8_t* buf, size_t count) {

    }

    virtual void read (uint32_t addr, uint8_t* buf, size_t count) {

    }

    virtual void eraseObjective(uint32_t addr) {

    }


};
}



#endif /* LENSPARAM_DUMMYDRIVER_HPP_ */
