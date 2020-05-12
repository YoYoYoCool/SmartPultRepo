/*
 * math.hpp
 *
 *  Created on: 2 дек. 2019 г.
 *      Author: Кирилл Чеверев
 */

#ifndef COMMON_LIBS_MATH_MYMATH_HPP_
#define COMMON_LIBS_MATH_MYMATH_HPP_




namespace MyMath {

    class MyMathBase {

        public:

        virtual void calculate (float & inputValue) =0;

    };

    class yx:public MyMathBase {



    };


    class Sqrt: public MyMathBase {
    private:

        float precision;

    public:

        Sqrt(float precision):
            precision(precision){}

        virtual void calculate (float & inputValue) {
            float xi=1.0;
            float  xin=0.0;
            float dataOut=0.0;
            if (inputValue==0.0)
                return;
            while (1) {
                dataOut=inputValue;
                dataOut/=xi;
                dataOut+=xi;
                dataOut/=2;
                float a = xi-dataOut;
                xi=dataOut;
                if (a<0.0)
                    a*=-1;
                if (a<precision)
                break;  }
            inputValue=dataOut;
            }

    };

}



#endif /* COMMON_LIBS_MATH_MATH_HPP_ */
