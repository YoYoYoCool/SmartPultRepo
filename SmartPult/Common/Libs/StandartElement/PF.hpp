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

         virtual void setInputValue(float inputValue) { this->inputValue=inputValue;}

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
        KalmanFilter::KalmanLineUprFriq LPFLeft1;
        KalmanFilter::KalmanLineUprFriq LPFRight1;
        float outLeftFilter1,outRightFilter1;


    public:
        PolosovoyFilter(float  friqLeft,float friqRight, float& fCLK):
        LPFLeft1(friqLeft,fCLK,inputValue,outLeftFilter1),
        LPFRight1(friqRight,fCLK,outRightFilter1,outValue)

        {

        }

        void calculateFilter() {
            LPFLeft1.calculate();
            outRightFilter1=this->inputValue;
            outRightFilter1-=outLeftFilter1;
            LPFRight1.calculate();
        }

        virtual void setInputValue(float inputValue) { this->inputValue=inputValue;}

        inline void setFriquensyLeftFilter (float value) {
            LPFLeft1.updateKoafficient(value);
        }

        inline void setFriquensyRightFilter (float value) {
            LPFRight1.updateKoafficient(value);
        }

    };


    }

#endif /* COMMON_LIBS_STANDARTELEMENT_PF_HPP_ */
