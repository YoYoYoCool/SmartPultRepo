/*
 * shakerInternalChannel.hpp
 *
 *  Created on: 12 ��� 2020 �.
 *      Author: ������ �������
 */

#ifndef INTERNALDEVICE_SHAKERINTERNALCHANNEL_HPP_
#define INTERNALDEVICE_SHAKERINTERNALCHANNEL_HPP_

#include "Libs/StandartElement/PF.hpp"
#include "Libs/StandartElement/RNDGenerator.hpp"
#include "Libs/Containers/RingBuffer.hpp"
#include <Pult/PultClasses.h>
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"



class IShakerBlocking {

public:

    virtual void setBlokingMode(bool mode)=0;

};

class IShaker:public IShakerBlocking {

public:

    virtual IResistor& _resistor() =0;

};

enum ChannalName {
    PANSHAKERCHANNEL, TILTSHAKERCHANNEL,ROLLSHAKERCHANNEL,ZOOMSHAKERCHANNEL
};

class ShakerChannel:public JoyChannelIF, public IShaker {

private:
    float inputValue;
    Containers::RingBuffer<float,30> buffer;
    float outValue, sum;
    VirtualMenuResistor speedResistor;
    bool bloking, isEnable;
    ChannalName name;
    float IQValue;

public:

    ShakerChannel(uint8_t IQValue,
                  ChannalName name):
                  isEnable(false),
                  bloking(true), name(name),IQValue(1<<IQValue)
                   {

    }

    virtual IResistor& _resistor(){
        return speedResistor;
    }



    virtual void setBlokingMode(bool mode) {
        bloking= mode;    }

    void setInputeValue(float inputValue)  {
        this->inputValue = inputValue;

    }

    inline virtual float getCurrentAdcValue() {
        outValue=inputValue;
        outValue*=speedResistor.value;
        outValue/=IQValue;
        if (outValue>230.0)
            outValue=230.0;
        if (outValue<-230.0)
            outValue=-230.0;
        return outValue;
    }

    virtual float getCalibrationOffsetValue()   {return 0.0;}

    virtual bool isEnable_(){
        if (bloking)
            return false;
        return this->isEnable;}

    virtual void enable()   {     isEnable=true;   }

    virtual void disable()  {     isEnable=false;   }

};



#endif /* INTERNALDEVICE_SHAKERINTERNALCHANNEL_HPP_ */
