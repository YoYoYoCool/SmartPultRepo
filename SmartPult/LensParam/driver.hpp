/*
 * driver.hpp
 *
 *  Created on: 16 февр. 2019 г.
 *      Author: Cheverev kirill
 */
#include "../LensParam/LensParam.hpp"
#include "../LensParam/writer.hpp"
#include "../drivers/mx66l51235f.h"

#ifndef LENSPARAM_DATAOUT_HPP_
#define LENSPARAM_DATAOUT_HPP_

namespace Drive {

class Driver {

public:

    inline void write () {  /*MX66L51235FPageProgram(1,1,1);*/     }
    inline void read() {    /*MX66L51235FRead(1,1,1);*/     }
    inline void deliteObj (uint32_t sektor) {MX66L51235FSectorErase(sektor); }
    inline void eraseflash () {MX66L51235FChipErase();}

private:



    };

}


#endif /* LENSPARAM_DATAOUT_HPP_ */
