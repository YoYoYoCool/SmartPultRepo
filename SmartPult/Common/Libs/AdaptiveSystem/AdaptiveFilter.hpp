/*
 * AdaptiveFilter.hpp
 *
 *  Created on: 19 но€б. 2019 г.
 *      Author:  ирилл „еверев
 */

#ifndef COMMON_LIBS_ADAPTIVESYSTEM_ADAPTIVEFILTER_HPP_
#define COMMON_LIBS_ADAPTIVESYSTEM_ADAPTIVEFILTER_HPP_

#include "Libs/StandartElement/KalmanFilter.hpp"
#include "Libs/math/mymath.hpp"


namespace AdaptiveSistem {

namespace AdaptiveFilterKalman{



    class AdaptiveFilterBase {

    protected:

        float & controlAction;
        KalmanFilter::KalmanBase& filter;
        float koafBaseReturn;
        float koafSeturation;
        float koafAproximaton;



    public:

        AdaptiveFilterBase(KalmanFilter::KalmanBase& filter,
                           float & controlAction,
                           float & friqBaseReturn,
                           float & friqSeturation):
            filter(filter),
            koafBaseReturn((friqBaseReturn*2*3.14)/500),
            controlAction(controlAction),
            koafSeturation((friqSeturation*2*3.14)/500),
            koafAproximaton(((koafSeturation-koafBaseReturn)*2*3.14)/500)
                {

                }

        inline virtual void  calculate() {}


    };


    class AdaptiveFilterPanBarFromAcceleration: public AdaptiveFilterBase{

    private:

        float & seturationAcceleration;
        MyMath::MyMathBase & funktion;
        float koaf;


    public:

        AdaptiveFilterPanBarFromAcceleration(KalmanFilter::KalmanBase& filter,
                                             MyMath::MyMathBase & funktion,
                                 float & controlAction,
                                 float & koafBaseReturn,
                                 float & seturationKoaf,
                                 float & seturationAcceleration):
            AdaptiveFilterBase(filter,controlAction,koafBaseReturn,seturationKoaf),
            funktion(funktion),
            seturationAcceleration(seturationAcceleration){

        }

       inline virtual void  calculate() {
            filter.calculate();
            filter.updateKoafficient(this->calculatKalmanKoaf());

        }

    private:

       inline float calculatKalmanKoaf()
       {
           float koaf = controlAction/seturationAcceleration;
           if (koaf<0.0)
               koaf*=-1;
           if (koaf>0.99)
               koaf=0.99;
           koaf*=koaf;
//           funktion.calculate(koaf);
           koaf*=this->koafAproximaton;
           koaf+=koafBaseReturn;
           if (koaf < koafBaseReturn)
               return koafBaseReturn;
           if (koaf > koafSeturation)
               return koafSeturation;
           return koaf;
       }



    };



}
}



#endif /* COMMON_LIBS_ADAPTIVESYSTEM_ADAPTIVEFILTER_HPP_ */
