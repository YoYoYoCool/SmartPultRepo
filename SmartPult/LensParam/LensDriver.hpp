/*
 * LensDriver.hpp
 *
 *  Created on: 27 февр. 2019 г.
 *      Author: KapustinIV
 */

#ifndef MODULES_LENSDB_LENSDRIVER_HPP_
#define MODULES_LENSDB_LENSDRIVER_HPP_

#include "LensPack.hpp"

namespace LensDb
{

class LensDriver
{
public:
        virtual void write(uint32_t addr, uint8_t* buf, size_t count) = 0;
        virtual void read(uint32_t addr, uint8_t* buf, size_t count) = 0;
        virtual void eraseObjective(uint32_t addr)=0;
        virtual void clearOffset ()=0;
};

} /* namespace LensDb */

#endif /* MODULES_LENSDB_LENSDRIVER_HPP_ */
