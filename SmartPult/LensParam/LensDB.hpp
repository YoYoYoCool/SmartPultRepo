/*
 * LensDB.hpp
 *
 *  Created on: 16 февр. 2019 г.
 *      Author: User9
 */

#ifndef LENSPARAM_LENSDB_HPP_
#define LENSPARAM_LENSDB_HPP_

#include "../LensParam/LensParam.hpp"


namespace LanseDataBase {




class LanseDB {
public:

    inline void setObj (LanseParam::Objective objIn) {obj=objIn;}
    inline LanseParam::Objective &objektive() {return obj;}

    void store() {

    }
    LanseParam::Objective load(Uint32 objNumber) {

    }

    void delObj(Uint32 objNumber);
    inline void setDataObj (const LanseParam::Objective & objectiv) {
        obj=objectiv;
    }
    inline LanseParam::Objective & dataObjectiv () {
        return obj;
    }



private:
LanseParam::Objective obj;




};

}



#endif /* LENSPARAM_LENSDB_HPP_ */
