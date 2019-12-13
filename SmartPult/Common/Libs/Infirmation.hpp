/*
 * Infirmation.hpp
 *
 *  Created on: 12 дек. 2019 г.
 *      Author: Кирилл Чеверев
 */

#ifndef COMMON_LIBS_INFIRMATION_HPP_
#define COMMON_LIBS_INFIRMATION_HPP_




namespace InformationStruct {

union Version {
    uint16_t all;
    __attribute__((__packed__)) struct {
        uint8_t boardVersion    :   4;
        uint8_t cpuVersion      :   4;
        uint8_t programVersion  :   8;
    }data;
};

union DataOfManufacture{
        uint32_t all;
        __attribute__((__packed__)) struct {
            uint8_t day             :   8;
            uint8_t month           :   8;
            uint16_t yers           :   16;
        }data;
    };

struct __attribute__((__packed__)) Information {
    volatile Version version;
    volatile DataOfManufacture dataOfManufacturing;
    volatile uint32_t number;
};

}


#endif /* COMMON_LIBS_INFIRMATION_HPP_ */
