/*
 * StartStopFluid.hpp
 *
 *  Created on: 28 янв. 2021 г.
 *      Author: Кирилл Чеверев
 */

#ifndef PULT_STARTSTOPFLUID_HPP_
#define PULT_STARTSTOPFLUID_HPP_



#include "Libs/math/mymath.hpp"



#define baseValue 0.98/4096.0
#define filterBaseValue 6.28/500.0

class StartStopFluid
{
private:
    float outValue;
    float inValueOldStap;
    float KXStart;
    float KXStop;
    float KYStart;
    float KYStop;
    float startFriq;
    float stopFriq;
    float stopFilterOut,startFilterOut,stopFilterOut2;
    float seturationValue;

    float acc;

    float picDetecter;
    float picDetectedStreem;

    float stopBaseValue;
    float startBaseValue;
    MyMath::Sqrt sqrt;


 /*   float outBuffer[5000];
    uint16_t bufferCount;*/

public:

    StartStopFluid():startFriq(3.0),stopFriq(5.0),
    seturationValue(20.0),
    sqrt(0.005),picDetectedStreem(0.9),stopBaseValue(0.1),startBaseValue(3.0)
        {
        updateKoaf(0.0);
        }


    inline float calulate(float inValue)
       {
        acc = inValue-inValueOldStap;
        acc*=0.002;
        inValueOldStap=inValue;
        if (acc<0.0)
            acc*=-1;
        if (acc>seturationValue)
            acc=seturationValue;
        stopFilterOut2*=KYStop;
        stopFilterOut2+=stopFilterOut*KXStop;
        stopFilterOut*=this->KYStop;
        startFilterOut*=this->KYStart;
        stopFilterOut+=inValue*this->KXStop;
        startFilterOut+=inValue*this->KXStart;
        float plus= maxValue(maxValue(startFilterOut,0),maxValue(stopFilterOut,0));
        float minus = minValue(minValue(startFilterOut,0),minValue(stopFilterOut,0));
        this->outValue = plus+minus;
       }

    inline void updateKoaf(float inValue)
        {
        float TStartValue;
        float TStopValue;
        float baseStop;
        float KAcc = acc/seturationValue;
        if (KAcc>0.99)
            KAcc = 0.99;
        sqrt.calculate(KAcc);
        //sqrt.calculate(KAcc);
        KAcc=1-KAcc;
        baseStop=baseValue*inValue;
        baseStop=1-baseStop;
        TStartValue = startBaseValue;
        TStartValue+=startFriq*baseStop;
        TStopValue = stopBaseValue;
        TStopValue+=stopFriq*baseStop;
        KXStop = (6.28*TStopValue*0.002);
        KXStart = (6.28*TStartValue*0.002);
        KYStart=1.0-KXStart;
        KYStop=1.0-KXStop;
        }

private:

    inline float minValue (float a,float b)
    {
        if (a<b)
            return a;
        return b;
    }

    inline float maxValue (float a,float b)
        {
            if (a>b)
                return a;
            return b;
        }

};

#endif /* PULT_STARTSTOPFLUID_HPP_ */
