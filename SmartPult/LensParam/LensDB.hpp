/*
 * LensDB.hpp
 *
 *  Created on: 16 февр. 2019 г.
 *      Author: User9
 */

#ifndef LENSPARAM_LENSDB_HPP_
#define LENSPARAM_LENSDB_HPP_

#include "../LensParam/LensParam.hpp"
#include "../LensParam/builder.hpp"
#include "../LensParam/writer.hpp"
#include "../LensParam/driver.hpp"

namespace LanseDataBase {




class LanseDB {
public:

    inline void setObj (LanseParam::Objective objIn) {obj=objIn;}
    inline LanseParam::Objective &objektive() {return obj;}

    void store() {
    flash.buildData(obj);
    paket.setSektor(1);// создать переменную и ее записать "1" временно
    paket.creatPaket(0);
    driver.write();
    paket.creatPaket(1);
    driver.write();
    paket.creatPaket(2);
    driver.write();
    paket.creatPaket(3);
    driver.write();
    paket.creatPaket(4);
    driver.write();
    paket.creatPaket(5);
    driver.write();
    paket.creatPaket(6);
    driver.write();

    }
    LanseParam::Objective load(Uint32 objNumber) {

    }

    void delObj(Uint32 objNumber);



private:
LanseParam::Objective obj;
Write::PaketCreating paket;
Builder::Build flash;
Drive::Driver driver;


};

}



#endif /* LENSPARAM_LENSDB_HPP_ */
