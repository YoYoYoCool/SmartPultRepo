/*
 * RNDGenerator.hpp
 *
 *  Created on: 12 мая 2020 г.
 *      Author: Кирилл Чеверев
 */

#ifndef COMMON_LIBS_STANDARTELEMENT_RNDGENERATOR_HPP_
#define COMMON_LIBS_STANDARTELEMENT_RNDGENERATOR_HPP_





namespace Generation {

class Generator {

private:
    int16_t inputValue;
    int16_t oldStepValue;
    float outValue;
    uint8_t IQValue;

    uint16_t calcCRC(uint8_t* pack, size_t longData)
    {
        uint16_t TMP;
        uint16_t crcl;
        uint16_t crch;
        uint16_t crc = 0xffff;

        crcl = crc;
        crch = (crc >> 8);
        for (uint16_t i = 0; i < longData; i++)
        {
            uint16_t dataTmp = pack[i];
            dataTmp = (dataTmp ^ crcl);
            TMP = (dataTmp << 4);
            dataTmp = (TMP ^ dataTmp);
            TMP = (dataTmp >> 5);
            TMP &= 0x07;
            crcl = crch;
            crch = (dataTmp ^ TMP);
            TMP = (dataTmp << 3);
            crcl = (crcl ^ TMP);
            TMP = (dataTmp >> 4);
            TMP &= 0x0F;
            crcl = (crcl ^ TMP);
        }

        crc = (crch << 8) + (crcl & 0x00FF);
        return crc;
    }

public:

    Generator(uint8_t IQValue,int16_t oldStepValue):IQValue(IQValue), oldStepValue(oldStepValue) {

    }

    void setInputValue (uint16_t inputValue) {
        this->inputValue=inputValue;
    }

    float getOutValue() {return this->outValue;}

    void generate () {
        int16_t crcValue=calcCRC((uint8_t)&inputValue,sizeof(inputValue));
        int16_t outValue = crcValue+oldStepValue;
        oldStepValue=crcValue;
        this->outValue=(float)calcCRC((uint8_t)&outValue,sizeof(outValue));
        this->outValue/=this->IQValue;
    }

};

}



#endif /* COMMON_LIBS_STANDARTELEMENT_RNDGENERATOR_HPP_ */
