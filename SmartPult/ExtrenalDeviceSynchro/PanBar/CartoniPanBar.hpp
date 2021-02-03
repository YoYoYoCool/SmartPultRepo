/*
 * CatoniPanBar.hpp
 *
 *  Created on: 5 дек. 2017 г.
 *      Author: AhrameshinGN
 */

#ifndef EXTRENALDEVICESYNCHRO_PANBAR_CARTONIPANBAR_HPP_
#define EXTRENALDEVICESYNCHRO_PANBAR_CARTONIPANBAR_HPP_
#define __IEEE_BIG_ENDIAN

#include <Pult/PultClasses.h>
#include <ti/sysbios/BIOS.h>
#include <Board.h>
#include <ti/sysbios/knl/Semaphore.h>
#include "../../GyConCommon/dataTypes.h"
#include "../PultGlobalDefinitions.h"
#include "Libs/StandartElement/KalmanFilter.hpp"
#include "Libs/StandartElement/AttAmpStaticChannel.hpp"
#include "Libs/AdaptiveSystem/AdaptiveFilter.hpp"
//#include "Libs/AdaptiveSystem/Regulatori/PID/channelBase.hpp"
#include "Libs/AdaptiveSystem/ProizvodnajaCalc.hpp"
#include "Libs/AdaptiveSystem/estimatingSistem.hpp"
#include "Libs/math/mymath.hpp"


namespace ExtrenalDevices
{

    enum {kartoniSpeedDelitel=100};

    enum CartoniAxisID
    {
        AXIS_PAN=1,
        AXIS_TILT,
        AXIS_ROLL,
        AXIS_ZOOM=5,
        AXIS_FOCUS,
        AXIS_IRIS,
        AXIS_BUTTONS
    };

    union CartoniAxisData
    {
        volatile UInt8 all[4];
        __attribute__((__packed__)) struct
         {
             volatile UInt32 bit21       :1;
             volatile UInt32 axisID      :6;
             volatile UInt32 NU1         :1;
             volatile UInt32 bits14_20   :7;
             volatile UInt32 NU2         :1;
             volatile UInt32 bits07_13   :7;
             volatile UInt32 NU3         :1;
             volatile UInt32 bits00_06   :7;
         }data;
    };

    union CartoniHeader
    {
        volatile UInt8 all;
        __attribute__((__packed__)) struct
         {
            volatile UInt8 packetNumber:3;
            volatile UInt8 axisCount:4;
            volatile UInt8 upBit:1;
         }data;
    };

    struct CartoniData
    {
        CartoniHeader   header;
        CartoniAxisData axis[7];
    };


    union CartoniButtonsState
    {
        UInt32 all;
        struct
        {
            UInt32 button0  :1;
            UInt32 button1  :1;
            UInt32 button2  :1;
            UInt32 button3  :1;
            UInt32 button4  :1;
            UInt32 button5  :1;
            UInt32 button6  :1;
            UInt32 button7  :1;
            UInt32 button8  :1;
            UInt32 button9  :1;
            UInt32 button10 :1;
            UInt32 button11 :1;
            UInt32 button12 :1;
            UInt32 button13 :1;
            UInt32 button14 :1;
            UInt32 button15 :1;
            UInt32 button16 :1;
            UInt32 button17 :1;
            UInt32 button18 :1;
            UInt32 button19 :1;
            UInt32 button20 :1;
            UInt32 button21 :1;
            UInt32 NU:10;
        };
        struct
        {
            UInt32 low  :16;
            UInt32 high  :16;
        };

    };

    enum CartoniChannelAxisList
    {
        CH_PAN=0,
        CH_DUTCH,
        CH_TILT,
        CH_ZOOM,
        CH_IRIS,
        CH_FOCUS
    };
    struct CartoniAxis
    {
        volatile bool isActive;
        float inputValue;
        float dataOldStep;
        float aproximatingValue;
        float timeOut;
    };

    static float clk = 0.002;

    class CartoniDataConverter
    {
        public:
            CartoniDataConverter(Semaphore_Handle* dataCopySem):
                sem(dataCopySem)
                {
                 resetAllData();
                }

            void clk1MS()
            {
                transactionCounter++;
                if(transactionCounter>200){transactionCounter=200;}
            }
            void convert(UInt8* buffer, UInt8 bufferLen)
            {

                if(!crcCalculation(buffer, bufferLen)) {

                    return;
                }

                CartoniData* data=(CartoniData*)buffer;
                UInt8 axisCount=data->header.data.axisCount;

                if(axisCount>7) {   axisCount=7;    }

                if(Semaphore_pend( *sem,7))
                {
                    resetAllData();

                    for(UInt8 i=0;i!=axisCount;i++)
                    {
                        switch(data->axis[i].data.axisID)
                        {
                            case AXIS_PAN:
                                pan.dataOldStep=pan.inputValue;
                                pan.inputValue=axisRendererSpeed(&(data->axis[i]));
                                pan.isActive=true;
                                timeOut=metronom;
                                metronom=0;
                                Module::TastinConvert converterPan(pan.inputValue,pan.dataOldStep,pan.aproximatingValue,timeOut,clk);
                                converterPan.calculate();
                                break;
                            case AXIS_TILT:
                                tilt.dataOldStep=tilt.inputValue;
                                tilt.inputValue=axisRendererSpeed(&(data->axis[i]));
                                tilt.inputValue*=-1.0;
                                tilt.isActive=true;
                                Module::TastinConvert converterTilt(tilt.inputValue,tilt.dataOldStep,tilt.aproximatingValue,timeOut,clk);
                                converterTilt.calculate();
                                break;
                            case AXIS_ROLL:
                                dutch.inputValue=axisRendererSpeed(&(data->axis[i]));
                                dutch.isActive=true;
                                break;
                            case AXIS_ZOOM:
                                zoom.inputValue=axisRendererZoomSpeed(&(data->axis[i]));
                                zoom.isActive=true;
                                break;
                            case AXIS_FOCUS:
                                focus.inputValue=axisRendererAbsolutposition(&(data->axis[i]));
                                focus.isActive=true;
                                break;
                            case AXIS_IRIS:
                                iris.inputValue=axisRendererAbsolutposition(&(data->axis[i]));
                                iris.isActive=true;
                                break;
                            case AXIS_BUTTONS:
                                buttons.all=axisRendererButton(&(data->axis[i]));
                                break;
                            default:
                                invalidAxisID=true;
                                break;
                        }
                    }
                    buffer[0]=0;
                    Semaphore_post(*sem);
                }
             }

            inline CartoniAxis getAxis(CartoniChannelAxisList ax)
                {
                CartoniAxis dummyAxis=  {   false, 0   };
                CartoniAxis ret;
                if(Semaphore_pend( *sem,7))    {
                    switch(ax)  {
                        case CH_PAN:    {
                            ret=pan;
                            if (buttons.button0==true)     {
                                ret.isActive=false;
                                }
                            metronom+=0.002;
                            break;  }
                        case CH_DUTCH:  {
                            ret=dutch;
                            if (buttons.button0==true)  {
                                ret.isActive=false;  }
                            break;}
                        case CH_TILT:  {
                            ret=this->tilt;
                            if (buttons.button0==true)  {
                                ret.isActive=false;  }
                            break;}
                        case CH_ZOOM:   {
                            ret=zoom;
                            break;  }
                        case CH_IRIS:   {
                            ret=iris;       break;
                            }
                        case CH_FOCUS:  {
                            ret=focus;      break;
                            }
                        default:    {
                            ret=dummyAxis;  break;
                            }
                        }
                    Semaphore_post(*sem);
                    return ret;
                    }
                return dummyAxis;
            }

        private:
            CartoniButtonsState buttons;
            CartoniAxis pan;
            CartoniAxis dutch;
            CartoniAxis tilt;
            CartoniAxis zoom;
            CartoniAxis iris;
            CartoniAxis focus;
            float metronom;
            float timeOut;
            volatile UInt32 transactionCounter;

            Semaphore_Handle* sem;


            volatile bool invalidAxisID;

            inline void resetAllData()          {
      /*          buttons.all=0;
                pan.isActive=false;
                dutch.isActive=false;
                tilt.isActive=false;
                zoom.isActive=false;
                iris.isActive=false;
                focus.isActive=false;
                invalidAxisID=false;*/
                transactionCounter=0;        }

            inline float axisRendererSpeed(CartoniAxisData* d)
                {
                int32_t data=0;
                data|=(d->data.bit21&0x1 )      <<31;
                if (data<0) {data|=0x7FE00000;}
                data|=(d->data.bits14_20&0x7F)  <<14;
                data|=(d->data.bits07_13&0x7F)  <<7;
                data|=(d->data.bits00_06&0x7F);
                float dat=(float)data;
                dat/=kartoniSpeedDelitel;
                return dat;
                }
            inline Int32 axisRendererZoomSpeed(CartoniAxisData* d)
                {
                Int32 data=0;
                data|=(d->data.bits14_20&0x40 )      <<25;
                if (data<0) {data|=0x7FF80000;}
                data|=(d->data.bits14_20&0x3F)  <<14;
                data|=(d->data.bits07_13&0x7F)  <<7;
                data|=(d->data.bits00_06&0x7F);
                return data;
                }
            inline Int32 axisRendererAbsolutposition(CartoniAxisData* d)
                {
                Int32 data=0;
                data|=(d->data.bits14_20&0x40 )      <<25;
                if (data<0) {data|=0x7FF80000;}
                data|=(d->data.bits14_20&0x3F)  <<14;
                data|=(d->data.bits07_13&0x7F)  <<7;
                data|=(d->data.bits00_06&0x7F);
                return data;
                }
            inline Int32 axisRendererButton(CartoniAxisData* d)
                {
                Int32 data=0;
                data|=(d->data.bit21&0x01 )      <<21;
                data|=(d->data.bits14_20&0x3F)  <<14;
                data|=(d->data.bits07_13&0x7F)  <<7;
                data|=(d->data.bits00_06&0x7F);
                return data;
                }

            inline bool crcCalculation(UInt8* buffer, UInt8 bufferLen)
                {
                CartoniData* d = (CartoniData*) buffer;
                UInt8 axisCount = d->header.data.axisCount;
                if (axisCount > 7)                  {   return false;   }
                if (d->header.data.upBit == 0)      {   return false;   }

                UInt8 totalBytes = (axisCount * 4) + 2;
                if (totalBytes > bufferLen)         {   return false;   }

                UInt8 CRC = 0;
                for (UInt8 i = 0; i != (totalBytes - 2); i++)
                {
                    CRC ^= (buffer[i] & 0x7F);
                }

                if (CRC != buffer[totalBytes - 1])   {   return false;   }
                return true;

                }
            };




    class CatoniPanBarResistor:public Resistor
    {
        public:
            CatoniPanBarResistor(CartoniDataConverter& dc, CartoniChannelAxisList ax,const float K_, const float maxValue_ ):
                Resistor(K_,0),
                channelAxis(ax),
                dataConverter(dc),
                maxValue(maxValue_)
            {

            }
            virtual void calculate(float adcValue)
            {
                value =(float)(axis.inputValue);
                value *=K;
                if (value>maxValue)
                    value=maxValue;
                if(value<-maxValue)
                    value=-maxValue;
            }

            bool setData()
            {
               axis=dataConverter.getAxis(channelAxis);
               return axis.isActive;
            }

            inline bool isActive()
            {
                return axis.isActive;
            }

        private:
            CartoniChannelAxisList channelAxis;
            CartoniDataConverter& dataConverter;
            CartoniAxis axis;
            const float maxValue;
//            const float K;
    };

static Schematic::AttAmpSettings settingsAmplifaer = {
    .amplifierSettings.Vcc=230.0,
    .amplifierSettings.Vee=-230.0,
    .amplifierSettings.stableRatio=1.0,
    .amplifierSettings.resistenceMax=3722,
    .amplifierSettings.adjustableRatio=19,
    .attenuatorSettings.Vcc=230.0,
    .attenuatorSettings.Vee=-230.0,
    .attenuatorSettings.deadZone=0.02,
    .attenuatorSettings.attenuationK=0.2
};

static float friqError = 40.0;

static float fCLK = 500.0;

static float koafBaseReturn = 5.0; // Hg
static float koafSeturation = 9.0; //  Hg
static float seturationAcceleration = 15; //grad/sec^2

    class CatoniPanBarChannel:public JoyChanel
    {

    private:
        CartoniChannelAxisList channelAxis;
        CartoniDataConverter& dataConverter;
        CartoniAxis axis;
        float speedOut;
        const float maxValue;
        #ifdef Garanin
        int32_t * speed;
        #else
        float speedInput;
        float speedFilter;
//        float controlAction;
        float errorSpeed;
        float speedEstimate;

        float acceleration;

        KalmanFilter::KalmanLineUpr filterInpute;
        KalmanFilter::KalmanLineUprFriq filterErrorModule;
        AdaptiveSistem::AdaptiveFilterKalman::AdaptiveFilterPanBarFromAcceleration filterInputAdaptive;
        MyMath::Sqrt function;
        Module::ComplexSystems::EstimateSignal estimateSpeed;
        Module::CalculateError errorModuleEstimate;
        Module::Integrator integratorEstimate;
        Module::Proizvodnaja accelerationModule;
        Module::Aproximation aproximationModule;

    #endif

        public:
            CatoniPanBarChannel(CartoniDataConverter& dc,
                                CartoniChannelAxisList ax,
                                float K,
                                Resistor* speedControl,
                                float deadZone,
                                float T,
                                float maxValue_
                                ):
                JoyChanel(K, offset,speedControl,deadZone, T),
                channelAxis(ax),
                dataConverter(dc),
#ifndef Garanin

                aproximationModule(axis.inputValue,speedInput,axis.dataOldStep,axis.aproximatingValue),
                filterErrorModule(friqError,fCLK,errorSpeed,errorSpeed),
                errorModuleEstimate(filterErrorModule,speedInput,speedEstimate,errorSpeed),
                integratorEstimate(errorSpeed,speedEstimate),
                estimateSpeed(errorModuleEstimate,integratorEstimate),
                filterInpute(koafBaseReturn,speedEstimate,speedOut),
                function(0.005),
                filterInputAdaptive(filterInpute,function,acceleration,koafBaseReturn,koafSeturation,seturationAcceleration),
                accelerationModule(speedOut,acceleration,clk),

#endif
                maxValue(maxValue_)     {                }

             virtual float getCurrentAdcValue()
                 {

                if (channelAxis==CH_ZOOM) {
#ifndef Garanin
                    speedOut=adcValue;
                    speedOut*=K;
#endif
                    }
                else {
                #ifndef Garanin
//              speedOut=adcValue;
//                calculateSpeedAxis();
                speedOut=axis.inputValue;
                #endif
                }
/*              float speedK = speedControl->adcValue/1024;
                speedOut*=speedK;*/
                if (speedOut>maxValue) {
                    speedOut=maxValue;}
                if (speedOut<-maxValue){
                    speedOut=-maxValue;}
                if(!this->isEnable) {speedOut=0.0;}// ne zabit' razkomentirovat'
                if(!axis.isActive)  {speedOut=0.0;}
                return speedOut;
                }


            void setData()
                {
                #ifdef Garanin
                axis=dataConverter.getAxis(channelAxis);
                if (axis.isActive) {
                    speed[0]=(int32_t)(axis.inputValue*1000);
                    }
                else
                    speed[0]=0;
                #else
                axis=dataConverter.getAxis(channelAxis);
                aproximationModule.calculate();
                #endif
                }

            inline volatile float &  getAxisVal () {
                return axis.inputValue;
                }
            #ifdef Garanin
            inline void setSpeed (int32_t * speed) {
                this->speed=speed;
                }
            #endif


#ifndef Garanin

        private:

            inline void calculateSpeedAxis() {
                estimateSpeed.calculate();
                speedEstimate-=(acceleration*0.002)*2;
                filterInputAdaptive.calculate();
                accelerationModule.calculate();
            }
#endif
    };
}



#endif /* EXTRENALDEVICESYNCHRO_PANBAR_CARTONIPANBAR_HPP_ */
