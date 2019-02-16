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


namespace LanseParam {

enum AxisList {
    zoomAxis=0,
    irisAxis,
    focusAxis
};

struct paramLenseAxis {
    volatile float position[64];
    volatile float procent[64];
    volatile Uint8 maxPoint;
};

struct nameObektivParam {
    char* nameObectiv;
    Uint8 longName;
};

class lenseAxisParametr {
public:
    void setName (char *name);
    nameObektivParam getObectivName();
    void setAxisParametr(Uint8 axisName, float procent, float position,Uint8 point);
    void setAxisMaxPoint(Uint8 axisName, Uint8 point);
    paramLenseAxis getAxisParametr(Uint8 axisName);
    void setObectivName(nameObektivParam inputValue);
    void setMaxPoint(Uint8 tochka, Uint8 axisName);


private:
    paramLenseAxis zoomParam;
    paramLenseAxis irisParam;
    paramLenseAxis focusParam;
    nameObektivParam obektiv;

    };


}


#endif /* LENSPARAM_LENSPARAM_HPP_ */
