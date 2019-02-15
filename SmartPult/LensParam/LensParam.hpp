/*
 * LensParam.hpp
 *
 *  Created on: 15 февр. 2019 г.
 *      Author: User9
 */

#ifndef LENSPARAM_LENSPARAM_HPP_
#define LENSPARAM_LENSPARAM_HPP_

#include <Pult/PultClasses.h>
#include <ti/sysbios/BIOS.h>

namespace LanseParam {

enum AxisList {
    zoomAxis=0,
    irisAxis,
    fokusAxis
};

struct paramLenseAxis {

    float position[64];
    float procent[64];
    };


class lenseAxisParametr {
public:
    void setName (char *name);
    Uint32 getNameObectiv();
    void axisSet(Uint8 axisName, float Procent, float Position,Uint8 tochka);
    float axisGet(Uint8 axisName);

private:
    paramLenseAxis zoomParam;
    paramLenseAxis irisParam;
    paramLenseAxis focusParam;
    char nameObectiv[100];
};

class lensExcheng: public lenseAxisParametr   {
public:


private:
    Uint16 crcCalc;
    Uint16 crcRead;


};
}

#endif /* LENSPARAM_LENSPARAM_HPP_ */
