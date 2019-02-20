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
#include "../PULT_indicator/LCD Classes.h"

namespace Builder {

enum {sizeCrc=4, sizeParametr=64, sizeData=256}; //
enum {sizeSectorFlash=4096};

Uint32 polinom;

struct DataStruct {
    Uint8 dannie[sizeData];
    Uint8 sizeDannie;
};

class DataInput {
public:
    DataInput () {
        data.sizeDannie=0;
        for (Uint8 i=0; i<sizeData; i++) {data.dannie[i]=0;}
    }

    inline void setData (Uint8 dataID,Uint8 dataIn) {   data.dannie[dataID]=dataIn;    }

    inline Uint8 getData (Uint8 dataID) { return data.dannie[dataID];   }

    inline void setSizeDannie (Uint8 size) {data.sizeDannie=size;}

    inline Uint8 getSizeDannie() {return data.sizeDannie;}

private:

    DataStruct data;
};

struct DataWrite {
    Uint8 data[sizeSectorFlash];
    Uint16 sizeData;
};

class DataBuildToWriter {
public:
    DataBuild() {
        for(Uint16 i=0; i<sizeSectorFlash; i++) {_frameWrite.data[i]=0;}
        _frameWrite.sizeData=0;
    }
    inline const DataWrite & frameWrite() {return _frameWrite;}

    inline void addFrameWrite (DataStruct data) {
        Uint8 a=0;
        for (Uint16 i=_frameWrite.sizeData; i<_frameWrite.sizeData+data.sizeDannie; i++) {
            _frameWrite.data[i]=data.dannie[a];
            a++;
            }
        }

    inline void clearFrameWrite () {
        for(Uint16 i=0; i<sizeSectorFlash; i++) {_frameWrite.data[i]=0;}
        _frameWrite.sizeData=0;
        }

private:
    DataWrite _frameWrite;
};

class ToDataBuildToWriter {
public:
private:
    DataBuildToWriter dataWrite;
};

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



class BuildDataObjectiv {
public:

    inline void setData (const LanseParam::Objective & objectiv) {
        LanseParam::Objective obj=objectiv;
        buildName(obj.getName());
//        buildCrcName(crcNameCalculate());
        buildZoomParam (obj.zoom());
        buildIrisParam (obj.iris());
        buildFocusParam (obj.focus());
//        buildCrc(crcParamCalculate());
        }
    inline DataInput & dataName () {return _dataName;}

    inline DataInput & dataZoomPosition () {return _dataZoomPosition;}

    inline DataInput & dataZoomPercent () {return _dataZoomPercent;}

    inline DataInput & dataIrisPosition () {return _dataIrisPosition;}

    inline DataInput & dataIrisPercent () {return _dataIrisPercent;}

    inline DataInput & dataFocusPosition () {return _dataFocusPosition;}

    inline DataInput & dataFocusPercent () {return _dataFocusPercent;}

    inline DataInput & dataTTH () {return _dataTTH;}


private:
        inline void buildName (const LanseParam::ObjectiveName & name ) {

               for (Uint8 i=0;i<name._nameSize; i++ ) {
                   _dataName.setData(i, name.nameObjectiv[i]);
                   }
               _dataName.setData(name._nameSize, (Uint8)'\0');
               for (Uint8 i=name._nameSize+1; i<sizeData; i++) {
                   _dataName.setData(i, 0);
                   }
               _dataName.setSizeDannie(name._nameSize+1);
               }

        inline void buildZoomParam (const LanseParam::Axis & zoomAxis) {
            DataConvert _float;
            axis=zoomAxis;
            for (Uint8 i=0; i<sizeParametr; i++) {
                point=axis.getPoint(i);
                _float.all=point.position;
                _dataZoomPosition.setData(4*i+0,_float.byte.byte0 );
                _dataZoomPosition.setData(4*i+1,_float.byte.byte1 );
                _dataZoomPosition.setData(4*i+2,_float.byte.byte2 );
                _dataZoomPosition.setData(4*i+3,_float.byte.byte3 );
                _float.all=point.percent;
                _dataZoomPercent.setData(4*i+0,_float.byte.byte0 );
                _dataZoomPercent.setData(4*i+1,_float.byte.byte1 );
                _dataZoomPercent.setData(4*i+2,_float.byte.byte2 );
                _dataZoomPercent.setData(4*i+3,_float.byte.byte3 );
                }
            }
            inline void buildIrisParam (const LanseParam::Axis & irisAxis) {
                DataConvert _float;
                axis=irisAxis;
                for (Uint8 i=0; i<sizeParametr; i++) {
                    point=axis.getPoint(i);
                    _float.all=point.position;
                    _dataIrisPosition.setData(4*i+0,_float.byte.byte0 );
                    _dataIrisPosition.setData(4*i+1,_float.byte.byte1 );
                    _dataIrisPosition.setData(4*i+2,_float.byte.byte2 );
                    _dataIrisPosition.setData(4*i+3,_float.byte.byte3 );
                    _float.all=point.percent;
                    _dataIrisPercent.setData(4*i+0,_float.byte.byte0 );
                    _dataIrisPercent.setData(4*i+1,_float.byte.byte1 );
                    _dataIrisPercent.setData(4*i+2,_float.byte.byte2 );
                    _dataIrisPercent.setData(4*i+3,_float.byte.byte3 );
                    }
            }
            inline void buildFocusParam (const LanseParam::Axis & fokusAxis) {
                    DataConvert _float;
                    axis=fokusAxis;
                    for (Uint8 i=0; i<sizeParametr; i++) {
                        point=axis.getPoint(i);
                        _float.all=point.position;
                        _dataFocusPosition.setData(4*i+0,_float.byte.byte0 );
                        _dataFocusPosition.setData(4*i+1,_float.byte.byte1 );
                        _dataFocusPosition.setData(4*i+2,_float.byte.byte2 );
                        _dataFocusPosition.setData(4*i+3,_float.byte.byte3 );
                        _float.all=point.percent;
                        _dataFocusPercent.setData(4*i+0,_float.byte.byte0 );
                        _dataFocusPercent.setData(4*i+1,_float.byte.byte1 );
                        _dataFocusPercent.setData(4*i+2,_float.byte.byte2 );
                        _dataFocusPercent.setData(4*i+3,_float.byte.byte3 );
                        }
                }

        DataInput _dataName;
        DataInput _dataZoomPosition;
        DataInput _dataZoomPercent;
        DataInput _dataIrisPosition;
        DataInput _dataIrisPercent;
        DataInput _dataFocusPosition;
        DataInput _dataFocusPercent;
        DataInput _dataTTH;
        LanseParam::LensePoint point;
        LanseParam::Axis axis;
        LanseParam::Objective obj;

    };

}



#endif /* LENSPARAM_BUILDER_HPP_ */
