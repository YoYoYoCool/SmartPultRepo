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

    struct PanBarFilterSetting {
        float minSpeedFromCalculate;
        float maxSpidQuad;
        float koafMinBase;
        };

    class PanBarFilter {
    private:

        KalmanFilter::KalmanBase& filter;
        PanBarFilterSetting & setting;

    public:

        PanBarFilter(KalmanFilter::KalmanBase& filter,PanBarFilterSetting & setting): filter(filter),
        setting(setting)    {        }

        float calculate(float inputData) {
            float rez = filter.calculate(inputData);
            filter.updateKoafficient(calculatKalmanKoaf(rez));
            return rez;
            }

    private:

        float calculatKalmanKoaf(float inputData) {
            if ((inputData<-setting.minSpeedFromCalculate)||(inputData>setting.minSpeedFromCalculate)){
            int32_t analizData = (int32_t)inputData;
            analizData*=analizData;
            float koaf=(float)analizData;
            koaf/=setting.maxSpidQuad;
            if (koaf<0)
            koaf*=-1;
            return koaf;}
            else
            return setting.koafMinBase;
            }
        };
    }
}

#endif /* EXTRENALDEVICESYNCHRO_PANBAR_KALMANFILTERFROMPANBAR_HPP_ */
