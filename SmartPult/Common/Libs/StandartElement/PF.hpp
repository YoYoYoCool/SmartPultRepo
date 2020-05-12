/*
 * PF.hpp
 *
 *  Created on: 12 мая 2020 г.
 *      Author: Кирилл Чеверев
 */

#ifndef COMMON_LIBS_STANDARTELEMENT_PF_HPP_
#define COMMON_LIBS_STANDARTELEMENT_PF_HPP_

#include "KalmanFilter.hpp"



namespace Filter
    {

    class FilterBase {

    protected:
         float inputValue;
         float outValue;

    public :

         void setInputValue(float inputValue) { this->inputValue=inputValue;}

         float getOutValue () {    return outValue;       }

    };

    class IPolosovoyFilter {

    public:
        virtual void setFriquensyLeftFilter (float value) =0;



        virtual void setFriquensyRightFilter (float value) =0;




    };

    class PolosovoyFilter: public  FilterBase , public IPolosovoyFilter
    {

    private:
        KalmanFilter::KalmanLineUprFriq LPFLeft;
        KalmanFilter::KalmanLineUprFriq LPFRight;
        float outLeftFilter;
        float fvchValue;


    public:
        PolosovoyFilter(float  friqLeft,float friqRight, float& fCLK):
            LPFLeft(friqLeft,inputValue,outLeftFilter),
        LPFRight(friqRight,fCLK,fvchValue,outValue) {

        }

        void calculateFilter() {
            LPFLeft.calculate();
            fvchValue=this->inputValue-this->outLeftFilter;
            LPFRight.calculate();
        }

        virtual void setFriquensyLeftFilter (float value) {
            LPFLeft.updateKoafficient(value);
        }

        virtual void setFriquensyRightFilter (float value) {
            LPFRight.updateKoafficient(value);
        }

    };


    }

#endif /* COMMON_LIBS_STANDARTELEMENT_PF_HPP_ */
