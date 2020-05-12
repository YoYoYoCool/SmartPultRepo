/*
 * channelBase.hpp
 *
 *  Created on: 20 но€б. 2019 г.
 *      Author:  ирилл „еверев
 */

#ifndef COMMON_LIBS_REGULATORI_PID_CHANNELBASE_HPP_
#define COMMON_LIBS_REGULATORI_PID_CHANNELBASE_HPP_

#include "Libs/StandartElement/AttAmpStaticChannel.hpp"
#include "Libs/StandartElement/KalmanFilter.hpp"



namespace RegulatorSystem {

    namespace RegulatorChannel {

    class RegulatorChannelBase {

    protected:

        float& error;
        float& signalOut;

    public:

        bool isEnable;

        RegulatorChannelBase(float& error,float &signalOut, bool isEnable):error(error),
        signalOut(signalOut),isEnable(isEnable) {

        }

        virtual void calculate() {

        }

        };

    class RegulatorProportionalChannel: public RegulatorChannelBase {

    private:

        Schematic::AttAmpStaticChannal amplifaer;

    public:

        RegulatorProportionalChannel(Schematic::AttAmpSettings& setting,float& error,float &signalOut,bool isEnable = true):
            amplifaer(setting),
            RegulatorChannelBase(error,signalOut,isEnable){

        }

        virtual void calculate() {   this->signalOut=amplifaer.calkulateOGR(error, 1.0);        }

        };

    struct RegulatorIntegralChannelSetting {
        Schematic::AttAmpSettings amplifaerErrorSetting;
        Schematic::AttAmpSettings amplifaerOutSetting;
    };

    class RegulatorIntegralChannel: public RegulatorChannelBase {

    private:

        Schematic::AttAmpStaticChannal amplifaerError;
        Schematic::AttAmpStaticChannal amplifaerOut;
        float integral;

    public:

        RegulatorIntegralChannel(RegulatorIntegralChannelSetting& setting,float& error,float &signalOut,bool isEnable = true):
            amplifaerError(setting.amplifaerErrorSetting),
            amplifaerOut(setting.amplifaerOutSetting),
            RegulatorChannelBase(error,signalOut,isEnable){

        }

        virtual void calculate() {
            integral+=amplifaerError.calkulateOGR(error, 1.0);
            signalOut=amplifaerOut.calkulateOGR(integral, 1.0);
        }

        };

    struct RegulatorDefferentialChannelSetting {
            Schematic::AttAmpSettings amplifaerErrorInputSetting;
            Schematic::AttAmpSettings amplifaerErrorInput1Setting;
            Schematic::AttAmpSettings amplifaerOutSetting;
            float kalmanKoaf;
        };

    class RegulatorDefferentialChannel: public RegulatorChannelBase {

        Schematic::AttAmpStaticChannal amplifaerError;
        Schematic::AttAmpStaticChannal amplifaerErrorN1;
        Schematic::AttAmpStaticChannal amplifaerOut;
        float errorN1;
        KalmanFilter::KalmanLineUpr filter;


    public:

        RegulatorDefferentialChannel(RegulatorDefferentialChannelSetting& setting, float& error,float &signalOut,bool isEnable = true):
                    amplifaerError(setting.amplifaerErrorInputSetting),
                    amplifaerErrorN1(setting.amplifaerErrorInput1Setting),
                    amplifaerOut(setting.amplifaerOutSetting),
                    RegulatorChannelBase(error,signalOut,isEnable),
                    filter(9.0,0.002){                }

        inline virtual void calculate() {
            float errorNEstimate=amplifaerError.calkulateOGR(error, 1.0);
            float errorN1Estimate=amplifaerError.calkulateOGR(errorN1, 1.0);
            errorN1=error;
            float errorSpeed=errorNEstimate-errorN1Estimate;
            filter.calculate(errorSpeed);
            signalOut=amplifaerOut.calkulateOGR(errorSpeed, 1.0);
            }
        };
    }

    namespace Regulator {

        class RegulatorBase {

        public:

            virtual void calculate () {

            }

            virtual void init() {

            }
        };

        struct RegulatorPIDSetting {
            Schematic::AttAmpSettings& settingProportionalAmplifaer;
            RegulatorChannel::RegulatorIntegralChannelSetting integralSetting;
            RegulatorChannel::RegulatorDefferentialChannelSetting defferentialSetting;
        };

        class RegulatorPID:public RegulatorBase {

        private:
            RegulatorChannel::RegulatorProportionalChannel P;
            RegulatorChannel::RegulatorIntegralChannel I;
            RegulatorChannel::RegulatorDefferentialChannel D;
            RegulatorChannel::RegulatorChannelBase* channels[3];
            float & error;
            float & signalOut;
            float outP;
            float outI;
            float outD;
            float* channelOut[3];

        public:

            RegulatorPID(RegulatorPIDSetting setting, float & error, float & signalOut):
            error(error),signalOut(signalOut),
            P(setting.settingProportionalAmplifaer,error,outP),
            I(setting.integralSetting,error,outI),
            D(setting.defferentialSetting,error,outD)
                {

                }

            virtual void init() {
                channels[0]=&P;
                channels[1]=&I;
                channels[2]=&D;
                channelOut[0]=&outP;
                channelOut[1]=&outI;
                channelOut[2]=&outD;
            }


            virtual void calculate () {
                signalOut=0;
                for (int8_t i=0; i<3;i++) {
                    if (channels[i]->isEnable) {
                        channels[i]->calculate();
                        signalOut+=channelOut[i][0];
                    }
                }
            }
        };
    }
}



#endif /* COMMON_LIBS_REGULATORI_PID_CHANNELBASE_HPP_ */
