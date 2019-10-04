/*
 * kalmanFilterStage.hpp
 *
 *  Created on: 1 окт. 2019 г.
 *      Author: Кирилл Чеверев
 */

#ifndef COMMON_LIBS_STANDARTELEMENT_KALMANFILTERSTAGE_HPP_
#define COMMON_LIBS_STANDARTELEMENT_KALMANFILTERSTAGE_HPP_




#include "KalmanFilter.hpp"
#include "galetniy Perecluxhatel.hpp"

    namespace KalmanFilter {

    template <uint8_t channel>
    class KalmanFilterLine {

    private:

        KalmanLine filterKalman[channel];
        KalmanLineUpr filterKalmanUpr[channel];
        KalmanBase * filter[channel];
        KalmanBase * filterSG;
        float rez[channel];
        KalmanLine sgFilterKalman;
        KalmanLineUpr sgFilterKalmanUpr;
        float porogUp[channel];
        float porogDn[channel];
        StandartElement::GaletniyPerecluxhatel perecluchatel;
        uint8_t counterStepUp;
        uint8_t counterStepDn;
        uint8_t maxCounterStep;
        uint8_t exponent;

    public:

        KalmanFilterLine( uint32_t exponent, uint8_t maxCounterStep=10):perecluchatel(0,8),
                counterStepUp(0), counterStepDn(0),maxCounterStep(maxCounterStep),exponent(exponent) {

                    }

        void settings (float ** sigmaEta,float ** sigmaPsi) {
            for (uint8_t i=0; i<channel; i++) {
                filterKalman[i].set(sigmaEta[i][0],sigmaPsi[i][0]);
                filter[i]=&filterKalman[i];
                porogUp[i] = i+1;
                porogDn[i] = i;
                for (uint32_t j=1;j<exponent;j++) {
                    porogUp[i]*=i+1;
                    porogDn[i]*= i;}
            }
            filterSG=&sgFilterKalman;
            sgFilterKalman.set(sigmaEta[channel][0],sigmaPsi[channel][0]);
            }

        void settings (float **kalmanKoaf) {
                    for (uint8_t i=0; i<channel; i++) {
                        filterKalmanUpr[i].set(kalmanKoaf[i][0]);
                        filter[i]=&filterKalmanUpr[i];
                        porogUp[i] = i+1;
                        porogDn[i] = i;
                        for (uint32_t j=1;j<exponent;j++) {
                            porogUp[i]*=i+1;
                            porogDn[i]*= i;}
                    }
                    filterSG=&sgFilterKalmanUpr;
                    sgFilterKalmanUpr.set(kalmanKoaf[channel][0]);
                    }

        float calculate (float inputData) {
            for (uint8_t i =0; i<channel; i++) {
            rez[i] = filter[i]->calculate(inputData);   }
            checkForCrossingOverMax();
            checkForCrossingOverMin();
            return filterSG->calculate(rez[perecluchatel.getActualPosition()]);
        }



    private:

        void checkForCrossingOverMax() {
            int8_t i =perecluchatel.getActualPosition();
            if ((rez[i]<-porogUp[i])||(rez[i]>porogUp[i])){
                counterStepUp++;
                if (counterStepUp>maxCounterStep) {
                    counterStepUp=0;
                    perecluchatel.stepRight();}
            }
            else {counterStepUp=0;}
        }

        void checkForCrossingOverMin() {
            int8_t i =perecluchatel.getActualPosition();
            if (rez[i]==0.0){
                counterStepDn++;
                if (counterStepDn>maxCounterStep) {
                    for (uint8_t i=0; i<channel; i++) {
                        rez[i]=0.0;}
                    perecluchatel.setNeedPosition(0);
                    counterStepDn=0;
                    }
                }
            else { counterStepDn=0;  }
        }

    };
}


#endif /* COMMON_LIBS_STANDARTELEMENT_KALMANFILTERSTAGE_HPP_ */
