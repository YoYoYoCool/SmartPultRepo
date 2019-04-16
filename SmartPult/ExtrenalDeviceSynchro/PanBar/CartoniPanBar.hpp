/*
 * CatoniPanBar.hpp
 *
 *  Created on: 5 дек. 2017 г.
 *      Author: AhrameshinGN
 */

#ifndef EXTRENALDEVICESYNCHRO_PANBAR_CARTONIPANBAR_HPP_
#define EXTRENALDEVICESYNCHRO_PANBAR_CARTONIPANBAR_HPP_
#include <Pult/PultClasses.h>
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include "../../GyConCommon/dataTypes.h"




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
        volatile float value;
    };

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

                if(!crcCalculation(buffer, bufferLen)) {   return; }

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
                                pan.value=axisRendererSpeed(&(data->axis[i]));
                                pan.isActive=true;
                                break;
                            case AXIS_TILT:
                                tilt.value=axisRendererSpeed(&(data->axis[i]));
                                tilt.value*=-1;
                                tilt.isActive=true;
                                break;
                            case AXIS_ROLL:
                                dutch.value=axisRendererSpeed(&(data->axis[i]));
                                dutch.isActive=true;
                                break;
                            case AXIS_ZOOM:
                                zoom.value=axisRendererZoomSpeed(&(data->axis[i]));
                                zoom.isActive=true;
                                break;
                            case AXIS_FOCUS:
                                focus.value=axisRendererAbsolutposition(&(data->axis[i]));
                                focus.isActive=true;
                                break;
                            case AXIS_IRIS:
                                iris.value=axisRendererAbsolutposition(&(data->axis[i]));
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
                            if (buttons.button0==true)     {
                                ret.value=0.0;
                                ret.isActive=true;
                                }
                            else    {
                                ret=pan;    }
                            break;  }
                        case CH_DUTCH:  {
                            if (buttons.button0==true)  {
                                ret.value=0.0;
                                ret.isActive=true;  }
                            else    {
                                ret=dutch;    }
                            break;}
                        case CH_TILT:  {
                            if (buttons.button0==true)  {
                                ret.value=0.0;
                                ret.isActive=true;  }
                            else    {
                                ret=tilt;   }
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
            volatile UInt32 transactionCounter;
            Semaphore_Handle* sem;


            volatile bool invalidAxisID;

            inline void resetAllData()          {
                buttons.all=0;
                pan.isActive=false;
                pan.value=0;
                dutch.isActive=false;
                dutch.value=0;
                tilt.isActive=false;
                tilt.value=0;
                zoom.isActive=false;
                zoom.value=0;
                iris.isActive=false;
                iris.value=0;
                focus.isActive=false;
                focus.value=0;
                invalidAxisID=false;
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
                value =(volatile float)(axis.value);
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

    class CatoniPanBarChannel:public JoyChanel
    {
        public:
            CatoniPanBarChannel(CartoniDataConverter& dc, CartoniChannelAxisList ax, float K,Resistor* speedControl,float deadZone, float T, float maxValue_ ):
                JoyChanel(K, offset,speedControl,deadZone, T),
                channelAxis(ax),
                dataConverter(dc),
                maxValue(maxValue_)
                {

                }

             virtual float getCurrentAdcValue()
                 {
                float rez=filter.calculate(adcValue);
                rez=deadZone(rez, deadZoneValue);
                if (channelAxis==CH_ZOOM) {
                    rez=rez*K;
                    if (rez>0)
                        rez+=0.1;
                    if (rez<0)
                        rez-=0.1;
                    }
                else
//                rez=rez*(getSpeed()+0.5);//задатчик смещения нуля
                if (rez>maxValue) {
                    rez=maxValue;}
                if (rez<-maxValue){
                    rez=-maxValue;}
                if(!this->isEnable) {rez=0.0;}// ne zabit' razkomentirovat'
                if(!axis.isActive)  {rez=0.0;}
                return rez;
                }

            void setData()
                {
                axis=dataConverter.getAxis(channelAxis);
                adcValue=axis.value;
                }

            inline volatile float &  getAxisVal () {
                return axis.value;
                }

        private:
            CartoniChannelAxisList channelAxis;
            CartoniDataConverter& dataConverter;
            CartoniAxis axis;
            const float maxValue;
    };

}



#endif /* EXTRENALDEVICESYNCHRO_PANBAR_CARTONIPANBAR_HPP_ */
