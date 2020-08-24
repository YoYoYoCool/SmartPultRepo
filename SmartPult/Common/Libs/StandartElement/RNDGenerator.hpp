/*
 * RNDGenerator.hpp
 *
 *  Created on: 12 мая 2020 г.
 *      Author: Кирилл Чеверев
 */

#ifndef COMMON_LIBS_STANDARTELEMENT_RNDGENERATOR_HPP_
#define COMMON_LIBS_STANDARTELEMENT_RNDGENERATOR_HPP_

#include "PF.hpp"



namespace Generation {

class Generator {

private:
     uint32_t counter;
     int16_t polinom;
     int16_t genVal;
     float outValue;
     float outVal;
    Filter::PolosovoyFilter filter;


public:

    Generator(int16_t polinom, float friqLeft, float friqRight, float& fCLK):polinom(polinom),
    filter(friqLeft,friqRight,fCLK)
{    }

    float getOutValue() {
        return this->outValue;}

    inline void generate () {
        counter++;
        int16_t CRC=0xffff;
        for (int32_t i=0; i<4;i++) {
            uint32_t cel = counter;
            cel>>=8*i;
            CRC^=cel;
            for (uint32_t i1=0;i1<8;i1++) {
                if ((CRC&0x8000)==0x8000) {
                CRC^=polinom;
                CRC<<=1;}
                else {
                    CRC<<=1;
                }
            }
        }
        genVal= CRC;
    }

    inline void calcFilter() {
        filter.setInputValue(genVal);
        filter.calculateFilter();
        outValue=filter.getOutValue();
    }


};

}



#endif /* COMMON_LIBS_STANDARTELEMENT_RNDGENERATOR_HPP_ */
