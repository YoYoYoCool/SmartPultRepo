/*
 * LensDb.cpp
 *
 *  Created on: 26 февр. 2019 г.
 *      Author: KapustinIV
 */

#include "MX66L51235FDriver.hpp"
#include "LensDb.hpp"
#include "LensPackBuilder.hpp"
#include "LensPackParser.hpp"
#include "LensDriver.hpp"
using namespace LensDb;
using namespace Containers;

enum {
    MaxNameSize = 64,
    MaxAxisPoints = 64,
    MaxPackSize = 1024,
};

MX66L51235FDriverSettings driverSettings = {
    .setting0 = 0x1234,
    .setting1 = 0x5678,
    .setting2 = 0x9ABC
};
//MX66L51235FDriver flashDriver(driverSettings);

//LensDb::LensDb<MaxAxisPoints, MaxPackSize> lensDb(flashDriver);

const char* nameStr = "Objective1";
Containers::StringStatic<MaxNameSize> name(nameStr);
LensObjectiveStatic<MaxAxisPoints> objective(name);
//=================Пример использования=================
void example() {
//    lensDb.store(1, objective);
};
