/*
 * builder.hpp
 *
 *  Created on: 16 февр. 2019 г.
 *      Cheverev kirill
 */

#ifndef LENSPARAM_BUILDER_HPP_
#define LENSPARAM_BUILDER_HPP_
#include "../LensParam/LensDB.hpp"
#include "../LensParam/LensParam.hpp"
#include "../LensParam/writer.hpp"
#include "../PULT_indicator/LCD Classes.h"

namespace Builder {

enum {sizeCrc=4, sizeParametr=64, sizeData=256}; //

Uint32 polinom;

union DataConvert {



    float all;
    struct {
        Uint8 byte0 :8;
        Uint8 byte1 :8;
        Uint8 byte2 :8;
        Uint8 byte3 :8;
    }byte;
};
union Crc {
    Uint32 all;
    struct {
        Uint8 byte0 :8;
        Uint8 byte1 :8;
        Uint8 byte2 :8;
        Uint8 byte3 :8;
    }byte;
};
struct Data {
    Uint8 nameObjektiv[sizeData];
    Uint8 crcName[sizeCrc];
    Uint8 zoomPosition[sizeData];
    Uint8 zoomPercent[sizeData];
    Uint8 irisPosition[sizeData];
    Uint8 irisPercent[sizeData];
    Uint8 focusPosition[sizeData];
    Uint8 focusPercent[sizeData];
    Uint8 crc[sizeCrc];
};

class Build {
public:
    inline  Uint32 buildData(const LanseParam::Objective & objectiv) {
        LanseParam::Objective obj=objectiv;
        buildName(obj.getName());
        buildCrcName(crcNameCalculate());
        buildZoomParam (obj.zoom());
        buildIrisParam (obj.iris());
        buildFocusParam (obj.focus());
        buildCrc(crcParamCalculate());

        }
    inline void setData ();
    inline const Data& getData () {return data;}

private:

    inline void buildName (const LanseParam::ObjectiveName & name ) {
        for (Uint8 i=0;i<name._nameSize; i++ ) {
            data.nameObjektiv[i]=(Uint8)name.nameObjectiv[i];
            }
        data.nameObjektiv[name._nameSize]=(Uint8)'\0';
        for (Uint8 i=name._nameSize+1; i<sizeData; i++) {
            data.nameObjektiv[i]=0;
            }
        }
    inline void buildZoomParam (const LanseParam::Axis & zoomAxis) {
        DataConvert _float;
        axis=zoomAxis;
        for (Uint8 i=0; i<sizeParametr; i++) {
            point=axis.getPoint(i);
            _float.all=point.position;
            data.zoomPosition[4*i+0]=_float.byte.byte0;
            data.zoomPosition[4*i+1]=_float.byte.byte1;
            data.zoomPosition[4*i+2]=_float.byte.byte2;
            data.zoomPosition[4*i+3]=_float.byte.byte3;
            _float.all=point.percent;
            data.zoomPercent[4*i+0]=_float.byte.byte0;
            data.zoomPercent[4*i+1]=_float.byte.byte1;
            data.zoomPercent[4*i+2]=_float.byte.byte2;
            data.zoomPercent[4*i+3]=_float.byte.byte3;
            }
        }

    inline void buildIrisParam (const LanseParam::Axis & irisAxis) {
        DataConvert _float;
        axis=irisAxis;
        for (Uint8 i=0; i<sizeParametr; i++) {
            point=axis.getPoint(i);
            _float.all=point.position;
            data.irisPosition[4*i+0]=_float.byte.byte0;
            data.irisPosition[4*i+1]=_float.byte.byte1;
            data.irisPosition[4*i+2]=_float.byte.byte2;
            data.irisPosition[4*i+3]=_float.byte.byte3;
            _float.all=point.percent;
            data.irisPercent[4*i+0]=_float.byte.byte0;
            data.irisPercent[4*i+1]=_float.byte.byte1;
            data.irisPercent[4*i+2]=_float.byte.byte2;
            data.irisPercent[4*i+3]=_float.byte.byte3;
            }
        }

    inline void buildFocusParam (const LanseParam::Axis & focusAxis) {
        DataConvert _float;
        axis=focusAxis;
        for (Uint8 i=0; i<sizeParametr; i++) {
            point=axis.getPoint(i);
            _float.all=point.position;
            data.focusPosition[4*i+0]=_float.byte.byte0;
            data.focusPosition[4*i+1]=_float.byte.byte1;
            data.focusPosition[4*i+2]=_float.byte.byte2;
            data.focusPosition[4*i+3]=_float.byte.byte3;
            _float.all=point.percent;
            data.focusPercent[4*i+0]=_float.byte.byte0;
            data.focusPercent[4*i+1]=_float.byte.byte1;
            data.focusPercent[4*i+2]=_float.byte.byte2;
            data.focusPercent[4*i+3]=_float.byte.byte3;
            }
        }
    inline void buildCrcName (Crc crc) {
        data.crcName[0]=crc.byte.byte0;
        data.crcName[1]=crc.byte.byte1;
        data.crcName[2]=crc.byte.byte2;
        data.crcName[3]=crc.byte.byte3;
        }
    inline void buildCrc(Crc crc) {
        data.crc[0]=crc.byte.byte0;
        data.crc[1]=crc.byte.byte1;
        data.crc[2]=crc.byte.byte2;
        data.crc[3]=crc.byte.byte3;
        }
    inline Crc crcNameCalculate () {
        Crc crc;
        return crc;
    }
    inline Crc crcParamCalculate () {
        Crc crc;
        return crc;
    }
    Data data;
    LanseParam::LensePoint point;
    LanseParam::Axis axis;
    LanseParam::Objective obj;

    };

}



#endif /* LENSPARAM_BUILDER_HPP_ */
