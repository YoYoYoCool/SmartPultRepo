/*
 * encoder.hpp
 *
 *  Created on: 23 мая 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef COMMON_LIBS_STANDARTELEMENT_ENCODER_HPP_
#define COMMON_LIBS_STANDARTELEMENT_ENCODER_HPP_




namespace StandartElement {

class Encoder {

private:
    int16_t maxRisk;
    int16_t count;

public:

    Encoder (int16_t maxRisk,int16_t count=0): count(count), maxRisk(maxRisk) {

        }

    Encoder (): count(0), maxRisk(32767) {

        }

    void stepLeft() {
        count--;
        if (count<0)
            count=maxRisk;
        }

    void stepRight() {
        count++;
        if (count>maxRisk)
            count=0;
        }

    inline int16_t  getActualPosition() {       return count;      }

    inline void setActualPosition(int16_t ActualPosition) {count=ActualPosition;}

    inline int16_t getMaxRisk () {  return maxRisk; }


};

}
#endif /* COMMON_LIBS_STANDARTELEMENT_ENCODER_HPP_ */
