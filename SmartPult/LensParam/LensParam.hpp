/*
 * LensParam.hpp
 *
 *  Created on: 15 февр. 2019 г.
 *      Author: Cheverev kirill
 */

#ifndef LENSPARAM_LENSPARAM_HPP_
#define LENSPARAM_LENSPARAM_HPP_

#include <Pult/PultClasses.h>
#include <ti/sysbios/BIOS.h>
#include "stddef.h"
#include "stdint.h"
#include "DebugAssert.hpp"
#include "Lib/Lib.h"

namespace LanseParam {

enum {
    MaxAxisPoints = 64
};

enum {
    MaxSize = 64
};

struct LensePoint {
    float position;
    float percent;
};


struct ObjectiveName {
    char* nameObjectiv;
    Uint8 _nameSize;
};

class Axis {


public:
    inline void setPoint(uint32_t pointId, LensePoint& point) {
        debugAssert(pointId<MaxAxisPoints);
        points[pointId] = point;
    }
    inline LensePoint getPoint(uint32_t pointId) {
        debugAssert(pointId<MaxAxisPoints);
        return points[pointId];
    }
    inline void setSize(Uint8 size ) {sizePoint=size;}

    inline Uint8 getSize() { return sizePoint; }

private:
    LensePoint points[MaxAxisPoints];
    Uint8 sizePoint;
};

class Objective  {

public:
    inline Axis& zoom() {return _zoom;};
    inline Axis& iris() {return _iris;};
    inline Axis& focus() {return _focus;};
    inline ObjectiveName& getName() {        return name;    }
    inline void setName(char* nameObj) {
       name.nameObjectiv=nameObj;
       name._nameSize=strLen(nameObj,MaxSize);     }
    inline void setZoom (Uint8 pointId, LensePoint& point){ _zoom.setPoint(pointId, point);}
    inline void setIris (Uint8 pointId, LensePoint& point){ _iris.setPoint(pointId, point);}
    inline void setFocus (Uint8 pointId, LensePoint& point){ _focus.setPoint(pointId, point);}

private:
    ObjectiveName name;
    Axis _zoom;
    Axis _iris;
    Axis _focus;
};




}
#endif /* LENSPARAM_LENSPARAM_HPP_ */
