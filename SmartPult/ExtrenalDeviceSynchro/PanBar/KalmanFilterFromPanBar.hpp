/*
 * KalmanFilterFromPanBar.hpp
 *
 *  Created on: 3 окт. 2019 г.
 *      Author: Кирилл Чеверев
 */

#ifndef EXTRENALDEVICESYNCHRO_PANBAR_KALMANFILTERFROMPANBAR_HPP_
#define EXTRENALDEVICESYNCHRO_PANBAR_KALMANFILTERFROMPANBAR_HPP_





#include "Libs/StandartElement/KalmanFilter.hpp"
#include "Libs/StandartElement/galetniy Perecluxhatel.hpp"

namespace ExtrenalDevices {
    namespace FilterPanBar {

    enum {channalFilter = 7};

    __attribute__((__packed__)) struct FilterStaticSetting {
        int8_t counterStepUp;
        int8_t counterStepDn;
        int8_t counterStepZero;
        int8_t maxCounterStep;
        int8_t exponent;
        int8_t stepPorogUp;
        int8_t stepPorogDn;
    };

    class PanBarFilter {
    private:

        KalmanFilter::KalmanMultiChannel<channalFilter> filter;
        float porogUp[channalFilter];
        float porogDn[channalFilter];
        KalmanFilter::KalmanBase * filterOut;
        KalmanFilter::KalmanBase * filterDynamics;
        StandartElement::GaletniyPerecluxhatel perecluchatel;
        FilterStaticSetting* setting;

    public:

        PanBarFilter(FilterStaticSetting* setting,
                     KalmanFilter::KalmanBase * filterOut,
                     KalmanFilter::KalmanBase * filterDynamics): setting(setting),
                     filterOut(filterOut),
                     filterDynamics(filterDynamics){

        }

        PanBarFilter(KalmanFilter::KalmanBase * filterOut,
                     KalmanFilter::KalmanBase * filterDynamics):
                     filterOut(filterOut),
                     filterDynamics(filterDynamics){

        }

        PanBarFilter() {}

        void set (KalmanFilter::KalmanBase ** filter) {
            this->filter.setFilter(filter);
            for (uint32_t j=1;j<setting->exponent;j++) {
                porogUp[j]*=j+setting->stepPorogUp;
                porogDn[j] = j+setting->stepPorogDn;
                }

            }

        float calculate(float *inputData) {
            filter.calculate(inputData, perecluchatel.getActualPosition());
            checkForCrossingOverMax();
            checkForCrossingOverMin();
            checkForZero();
            filterDynamics->calculate(inputData);
            filterOut->calculate(checkDinamic());
        }

    private:

        float checkDinamic() {
            float i = filter.getData(perecluchatel.getActualPosition());
            float j = filterDynamics->getData();
            if ((i!=0.0)||(j!=0)) {
                return filter.getData(perecluchatel.getActualPosition());/*
                if (i<0) {
                    if (filter.getData(perecluchatel.getActualPosition())>filterDynamics->getData())
                        return filter.getData(perecluchatel.getActualPosition());
                    else
                        return filterDynamics->getData();
                }
                if (i>0){
                    if (filter.getData(perecluchatel.getActualPosition())<filterDynamics->getData())
                        return filter.getData(perecluchatel.getActualPosition());
                    else
                        return filterDynamics->getData();
                }*/
            }
            else {
                return filter.getData(perecluchatel.getActualPosition());
            }
        }

        void checkForCrossingOverMax() {
            int8_t i =perecluchatel.getActualPosition();
            if ((filter.getData(i)<-porogUp[i])||(filter.getData(i)>porogUp[i])){
                setting->counterStepUp++;
                if (setting->counterStepUp>setting->maxCounterStep) {
                    setting->counterStepUp=0;
                    filter.setData(filter.getData(perecluchatel.getActualPosition()),perecluchatel.getActualPosition()+1);
                    perecluchatel.stepRight();}
                    }
            else {setting->counterStepUp=0;}
            }

        void checkForCrossingOverMin() {
            int8_t i =perecluchatel.getActualPosition();
            if ((filter.getData(i)<-porogUp[i])||(filter.getData(i)>porogUp[i])){
                setting->counterStepDn++;
                if (setting->counterStepDn>setting->maxCounterStep) {
                    setting->counterStepDn=0;
                    filter.setData(filter.getData(perecluchatel.getActualPosition()),perecluchatel.getActualPosition()-1);
                    perecluchatel.stepLeft();}
            }
            else {setting->counterStepDn=0;}
        }

        void checkForZero() {
            int8_t i =perecluchatel.getActualPosition();
            if (filter.getData(i)==0.0){
                setting->counterStepZero++;
                if (setting->counterStepZero>setting->maxCounterStep) {
                    for (uint8_t i=0; i<channalFilter; i++) {
                        filter.setData(0.0,i);}
                    perecluchatel.setNeedPosition(0);
                    setting->counterStepZero=0;
                }
            }
            else { setting->counterStepZero=0;  }
        }


        };
    }
}


#endif /* EXTRENALDEVICESYNCHRO_PANBAR_KALMANFILTERFROMPANBAR_HPP_ */
