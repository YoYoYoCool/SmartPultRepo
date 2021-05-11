/*
 * protocolDopReal.hpp
 *
 *  Created on: 19 окт. 2020 г.
 *      Author: Кирилл Чеверев
 */

#ifndef EXTRENALDEVICESYNCHRO_DOPREAL_PROTOCOLDOPREAL_HPP_
#define EXTRENALDEVICESYNCHRO_DOPREAL_PROTOCOLDOPREAL_HPP_



#include "stdint.h"

namespace ProtocolDopReal
{

union DataIntIQ
{
    int32_t all;
    struct {
        uint8_t NU      :   8;
        int32_t data    :   24;
    }data;
};

class ISetDataOutDopReal
    {

    public:

    virtual void setPanAngel(float angel) =0;

    virtual void setTiltAngel(float angel) =0;

    virtual void setRollAngel(float angel) =0;

    virtual void setZoomAngel(float angel) =0;

    virtual void setFocusAngel(float angel) =0;

    virtual void setIrisAngel(float angel) =0;

    virtual void setXValue(float value) =0;

    virtual void setYValue(float value) =0;

    virtual void setZValue(float value) =0;

    };

class IGetCamId
{
public:

virtual uint8_t getCamId() = 0;
};

class IGetDataOutDopReal:public IGetCamId
    {

    public:

    virtual int32_t getPanAngel() =0;

    virtual int32_t getTiltAngel() =0;

    virtual int32_t getRollAngel() =0;

    virtual int32_t getZoomAngel() =0;

    virtual int32_t getFocusAngel() =0;

    virtual int32_t getXValue() =0;

    virtual int32_t getYValue() =0;

    virtual int32_t getZValue() =0;

    };

class ISetCameraId
{
public:
    virtual void setCamId(uint8_t ID) = 0;
};

class ISetUnder_Ower
{
public:
    virtual void setUnder(uint8_t under) = 0;
};

class IDataOutDopReal: public ISetDataOutDopReal,
public ISetCameraId, public ISetUnder_Ower, public IGetDataOutDopReal
{
    };

class DataOutDopReal:public IDataOutDopReal
    {
    private:
        float IQValueDegre;
        float IQValuePifagor;
        volatile DataIntIQ panAxisDegre;
        volatile DataIntIQ tiltAxisDegre;
        volatile DataIntIQ rollAxisDegre;
        volatile DataIntIQ zoomAxisDegre;
        volatile DataIntIQ focusAxisDegre;
        volatile DataIntIQ irisAxisDegre;
        volatile DataIntIQ XValue;
        volatile DataIntIQ YValue;
        volatile DataIntIQ ZValue;

        volatile uint8_t cameraID;
         int8_t under_ower;

    public:

        DataOutDopReal():IQValueDegre((float)(1<<23)),IQValuePifagor((float)(1<<14))
        {

        }

        void setUnder(uint8_t under)
        {
            under_ower = under;
        }

        void setCamId(uint8_t ID)
        {
            cameraID=ID;
        }

        void setPanAngel(float angel)
        {

            panAxisDegre.all =convertDegre(convertAngelHead(angel));
        }

        void setTiltAngel(float angel)
        {
            tiltAxisDegre.all =convertDegre(convertAngelHead(angel));
        }

        void setRollAngel(float angel)
        {
            rollAxisDegre.all =convertDegre(convertAngelHead(angel));
        }

        void setZoomAngel(float angel)
        {
            zoomAxisDegre.all =convertDegre(convertAngelObjective(angel));
        }

        void setFocusAngel(float angel)
        {
            focusAxisDegre.all =convertDegre(convertAngelObjective(angel));
        }

        void setIrisAngel(float angel)
        {
            irisAxisDegre.all =convertDegre(convertAngelObjective(angel));
        }

        void setXValue(float value)
        {
            XValue.all = convertDegre(value);
        }

        void setYValue(float value)
        {
            YValue.all = convertDegre(value);
        }

        void setZValue(float value)
        {
            ZValue.all = convertDegre(value);
        }

        uint8_t getCamId()      {
            return this->cameraID;
        }

        int32_t getPanAngel()
        {
            return this->panAxisDegre.data.data;
        }

        int32_t getTiltAngel()
        {
            return this->tiltAxisDegre.data.data;
        }

        int32_t getRollAngel()
        {
            return this->rollAxisDegre.data.data;
        }

        int32_t getZoomAngel()
        {
            return this->zoomAxisDegre.data.data;
        }

        int32_t getFocusAngel()
        {
            return this->focusAxisDegre.data.data;
        }

        virtual int32_t getXValue()
        {
            return this->XValue.data.data;
        }

        virtual int32_t getYValue()
        {
            return this->YValue.data.data;
        }

        virtual int32_t getZValue()
        {
            return this->ZValue.data.data;
        }

    private:

        int32_t convertDegre(float degre)
        {
         return (int32_t)(degre*this->IQValueDegre);
        }

        int32_t convertPifagorValue(float lin)
        {
            return (int32_t)(lin*this->IQValuePifagor);
        }

        float convertAngelHead(float angel)
        {
            float ang = angel/360.0;
            angel-=360*(int32_t)ang;
            if (angel>180.0) {
                angel-=360.0;
            }
            if (angel<-180.0)
            {
                angel+=360.0;
            }
            return angel;
        }

        float convertAngelObjective (float angel) {
            angel+=1;
            angel*=0.5;
            return angel;
        }
    };

class IGetDataDopRealBase {
protected:
    IGetDataOutDopReal& dataDopRealOrientationData;
public:

    IGetDataDopRealBase(IGetDataOutDopReal& dataDopRealOrientationData):
    dataDopRealOrientationData(dataDopRealOrientationData)
        {

        }

    virtual uint32_t getData() =0;
};

class GetPanDataDopReal: public IGetDataDopRealBase
{
public:

    GetPanDataDopReal(IGetDataOutDopReal& dataDopRealOrientationData):IGetDataDopRealBase(dataDopRealOrientationData)
    {

    }

    uint32_t getData()
    {
        return this->dataDopRealOrientationData.getPanAngel();
    }

};

class GetTiltDataDopReal: public IGetDataDopRealBase
{
public:

    GetTiltDataDopReal(IGetDataOutDopReal& dataDopRealOrientationData):IGetDataDopRealBase(dataDopRealOrientationData)
        {

        }

    uint32_t getData()
    {
        return this->dataDopRealOrientationData.getTiltAngel();
    }

};

class GetRollDataDopReal: public IGetDataDopRealBase
{
public:

    GetRollDataDopReal(IGetDataOutDopReal& dataDopRealOrientationData):IGetDataDopRealBase(dataDopRealOrientationData)
            {

            }

    uint32_t getData()
    {
        return this->dataDopRealOrientationData.getRollAngel();
    }

};

class GetXDataDopReal: public IGetDataDopRealBase
{
public:

    GetXDataDopReal(IGetDataOutDopReal& dataDopRealOrientationData):IGetDataDopRealBase(dataDopRealOrientationData)
            {

            }

    uint32_t getData()
    {
        //return this->dataDopRealOrientationData.getXValue();
        return 0;
    }
};

class GetYDataDopReal: public IGetDataDopRealBase
{
public:

    GetYDataDopReal(IGetDataOutDopReal& dataDopRealOrientationData):IGetDataDopRealBase(dataDopRealOrientationData)
            {

            }

    uint32_t getData()
    {
        //return this->dataDopRealOrientationData.getYValue();;
        return 0;
    }
};

class GetZDataDopReal: public IGetDataDopRealBase
{
public:

    GetZDataDopReal(IGetDataOutDopReal& dataDopRealOrientationData):IGetDataDopRealBase(dataDopRealOrientationData)
            {

            }

    uint32_t getData()
    {
        //return this->dataDopRealOrientationData.getYValue();;
        return 0;
    }
};

class GetZoomDataDopReal: public IGetDataDopRealBase
{
public:

    GetZoomDataDopReal(IGetDataOutDopReal& dataDopRealOrientationData):IGetDataDopRealBase(dataDopRealOrientationData)
            {

            }

    uint32_t getData()
    {
        return this->dataDopRealOrientationData.getZoomAngel();
    }
};

class GetFocusDataDopReal: public IGetDataDopRealBase
{
public:

    GetFocusDataDopReal(IGetDataOutDopReal& dataDopRealOrientationData):IGetDataDopRealBase(dataDopRealOrientationData)
            {

            }

    uint32_t getData()
    {
        return this->dataDopRealOrientationData.getFocusAngel();
    }
};

enum MessageType
{
    cameraPosition_OrientationData=0xD1
};


class IProtocolDopReal
{
public:
    virtual void setMassegeType (MessageType messageType) =0;
};


class ProtocolDopReal
{
private:

    MessageType messageType;
    IGetCamId& dataCamId;
    GetPanDataDopReal panDataDopReal;
    GetTiltDataDopReal tiltDataDopReal;
    GetRollDataDopReal rollDataDopReal;
    GetXDataDopReal XDataDopReal;
    GetYDataDopReal YDataDopReal;
    GetZDataDopReal ZDataDopReal;
    GetZoomDataDopReal zoomDataDopReal;
    GetFocusDataDopReal focusDataDopReal;
    IGetDataDopRealBase* getAxisData[8];

public:

    ProtocolDopReal(
                    DataOutDopReal& dataDopRealOrientationData
    ):
    dataCamId(dataDopRealOrientationData),
    panDataDopReal(dataDopRealOrientationData),
    tiltDataDopReal(dataDopRealOrientationData),
    rollDataDopReal(dataDopRealOrientationData),
    XDataDopReal(dataDopRealOrientationData),
    YDataDopReal(dataDopRealOrientationData),
    ZDataDopReal(dataDopRealOrientationData),
    zoomDataDopReal(dataDopRealOrientationData),
    focusDataDopReal(dataDopRealOrientationData),
    messageType(cameraPosition_OrientationData)
    {
        getAxisData[0] = &panDataDopReal;
        getAxisData[1] = &tiltDataDopReal;
        getAxisData[2] = &rollDataDopReal;
        getAxisData[3] = &XDataDopReal;
        getAxisData[4] = &YDataDopReal;
        getAxisData[5] = &ZDataDopReal;
        getAxisData[6] = &zoomDataDopReal;
        getAxisData[7] = &focusDataDopReal;
    }

    void creatData(uint8_t* buf, uint16_t* byteCounter)
    {
        switch (messageType)
        {
            case cameraPosition_OrientationData:
            {
                creatCameraPosition_OrientationData(buf, byteCounter);
                break;
            }
        }

    }

private:

    union ConwertAngel
    {
        uint32_t angel;
        struct
        {
          uint8_t low       :8;
          uint8_t medium    :8;
          uint8_t high      :8;
          uint8_t NU        :8;
        }byte;
    };

    void creatCameraPosition_OrientationData(uint8_t* buf, uint16_t* byteCounter)
    {
     buf[0] = (uint8_t)messageType;
     buf[1] = this->dataCamId.getCamId();
     byteCounter[0]=2;
     for (uint8_t i =0; i<8;i++)
     {
         writeAngel(&buf[byteCounter[0]],getAxisData[i]->getData());
         byteCounter[0]+=3;
     }
     buf[byteCounter[0]]=0;
     byteCounter[0]++;
     buf[byteCounter[0]]=0;
     byteCounter[0]++;
     buf[byteCounter[0]]=crcCalc(buf,byteCounter[0]-1);
     byteCounter[0]++;

    }

    uint8_t crcCalc(uint8_t* buf,uint16_t offset)
    {
        uint8_t crc = 0x40;
        for (uint8_t i =0; i<offset+1;i++)
        {
        crc-=buf[i];
        }
        return crc;
    }

    void writeAngel(uint8_t* buf, uint32_t data)
    {
        ConwertAngel angelData;
        angelData.angel = data;
        buf[0] = angelData.byte.high;
        buf[1] = angelData.byte.medium;
        buf[2] = angelData.byte.low;
 /*       buf[0] = angelData.byte.low;
        buf[1] = angelData.byte.medium;
        buf[2] = angelData.byte.high;*/
    }

};
}

#endif /* EXTRENALDEVICESYNCHRO_DOPREAL_PROTOCOLDOPREAL_HPP_ */
