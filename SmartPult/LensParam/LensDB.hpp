/*
 * LensDb.hpp
 *
 *  Created on: 26 февр. 2019 г.
 *      Author: KapustinIV
 */

#ifndef MODULES_LENSDB_LENSDB_HPP_
#define MODULES_LENSDB_LENSDB_HPP_

#include "LensData.hpp"
#include "Libs/Containers/StringStatic.hpp"
#include "LensPackBuilder.hpp"
#include "LensPackParser.hpp"
#include "LensDriver.hpp"
namespace LensDb {


template<size_t MaxAxisPoints, size_t MaxPackSize>
class LensDb {
private:
    LensPackBuilder<MaxAxisPoints, MaxPackSize> packBuilder;
    LensDriver& driver;
    LensPackParser packParser;
public:
    LensDb(LensDriver& driver) : driver(driver) {
    }
    inline bool store(size_t id, LensObjective& lensObjective) {
        LensPack& pack = packBuilder.buildPack(lensObjective);
        driver.write(calcAddr(id), &pack[0], pack.getSize());
        return validateDb(calcAddr(id));
        }

    inline bool load(size_t id, LensObjective& lensObjective) {
        LensPackStatic<MaxPackSize> pack;
        driver.read(calcAddr(id), &pack[0], maxPackSize);
        if (!packParser.validate(&pack[0])) return false;
        packParser.objectiveBuild(&pack[0], lensObjective)
        return true;
        }
    inline void defrogmentaciaLensDataBase () {}
private:

    inline bool validateDb (size_t id) {
        return true;
    }

    inline uint32_t calcAddr(uint32_t id) {
        id-=1;
        return id<<12;
    }
};

} /* namespace LensDb */

#endif /* MODULES_LENSDB_LENSDB_HPP_ */
