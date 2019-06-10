/*
 * AmplifierStage.hpp
 *
 *  Created on: 27 мая 2019 г.
 *      Author: CheverevKirill
 */

#ifndef COMMON_LIBS_STANDARTELEMENT_AMPLIFIERSTAGE_HPP_
#define COMMON_LIBS_STANDARTELEMENT_AMPLIFIERSTAGE_HPP_


namespace StandartElement {

struct AmplifierSettings{
    float resistenceMax;
    float stableRatio;
    float adjustableRatio;
    float Vcc;
    float Vee;
};



class AmplifierStage {

private:

    AmplifierSettings settings;

public:

    AmplifierStage()  {    }

    AmplifierStage(AmplifierSettings settings) :
    settings(settings) {    }

    inline void setting (AmplifierSettings  settings) {
        this->settings=settings;
        }

    inline float gain (float data, const float resistance)  {
        float amplifaerK=resistance;
        amplifaerK/=settings.resistenceMax;
        if (amplifaerK>1.0) amplifaerK=1.0;
        amplifaerK*=settings.adjustableRatio;
        amplifaerK+=settings.stableRatio;
        data*=amplifaerK;
        if (data>settings.Vcc) {
            return settings.Vcc;}
        if (data<settings.Vee) {
            return settings.Vee; }
        return data;
        }

};



}


#endif /* COMMON_LIBS_STANDARTELEMENT_AMPLIFIERSTAGE_HPP_ */
