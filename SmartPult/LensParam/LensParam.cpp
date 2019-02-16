/*
 * LensParam.cpp
 *
 *  Created on: 15 февр. 2019 г.
 *      Author: Cheverev kirill
 */

#include <LensParam/LensParam.hpp>

using namespace LanseParam;

void lenseAxisParametr::setObectivName (nameObektivParam inputValue) { obektiv= inputValue; }

nameObektivParam lenseAxisParametr::getObectivName() { return obektiv;}

void lenseAxisParametr::setAxisParametr(Uint8 axisName, float procent, float position,Uint8 point) {
switch (axisName)
        {
case zoomAxis:
    zoomParam.position[point]=position;
    zoomParam.procent[point]=procent;
    return;
case irisAxis:
    irisParam.position[point]=position;
    irisParam.procent[point]=procent;
    return;
case focusAxis:
    focusParam.position[point]=position;
    focusParam.procent[point]=procent;
    return;
    }
}

void lenseAxisParametr::setMaxPoint(Uint8 axisName, Uint8 point) {
    switch (axisName)
            {
    case zoomAxis:
        zoomParam.maxPoint=point;
        return;
    case irisAxis:
        irisParam.maxPoint=point;
        return;
    case focusAxis:
        focusParam.maxPoint=point;
        return;
        }
}



paramLenseAxis lenseAxisParametr::getAxisParametr(Uint8 axisName) {
switch (axisName)
    {
    case zoomAxis:
        return zoomParam;
    case irisAxis:
        return irisParam;
    case focusAxis:
        return focusParam;
    }
}
