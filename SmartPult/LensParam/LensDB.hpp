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
    LensPackStatic<MaxPackSize> pack;
    LensPackBuilder<MaxPackSize> packBuilder ;
    LensDriver& driver;
    LensPackParser<MaxPackSize> packParser;
public:
    LensDb(LensDriver& driver) : driver(driver), packBuilder(pack), packParser(pack){    }
    inline bool store(size_t objectiveId, LensObjective& lensObjective) {
        packBuilder.buildPack(lensObjective);
        driver.write(objectiveId, &pack[0], pack.getSize());
        return validateDb(objectiveId);
        }

    inline bool load(size_t objectiveId, LensObjective& lensObjective) {
        driver.read(objectiveId, &pack[0],headerSize);
        driver.read(objectiveId, &pack[headerSize],(packParser.parsePackSize()));
        return packParser.parseObjective(lensObjective);
        }
    inline void defrogmentationLensDb () {}
private:

    inline bool validateDb (size_t id) {
        return true;
    }


};

} /* namespace LensDb */

#endif /* MODULES_LENSDB_LENSDB_HPP_ */
