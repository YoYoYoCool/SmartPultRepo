/*
 * MemoryStructur.hpp
 *
 *  Created on: 30 мая 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef PULT_INDICATOR_MEMORYSTRUCTUR_HPP_
#define PULT_INDICATOR_MEMORYSTRUCTUR_HPP_
/*
namespace MemoryStruct {

union StructAxisData {
    uint32_t all;
    __attribute__((__packed__)) struct {
            volatile uint8_t pan          :8;
            volatile uint8_t dutch        :8;
            volatile uint8_t tilt         :8;
            volatile uint8_t zoom         :8;
        }axis;
};

union Configurate {
    uint32_t all;
    __attribute__((__packed__))   struct {
        volatile uint8_t brightness             : 8;
        volatile uint8_t panSense               : 2;
        volatile uint8_t dutchSense             : 2;
        volatile uint8_t tiltSense              : 2;
        volatile uint8_t joyConfig              : 2;
        volatile uint8_t overslangUnderslang    : 1;
        volatile uint8_t ecoMode                : 1;
        volatile uint8_t panFolovingMode        : 1;
        volatile uint8_t NU                     : 7;
        } data;
};

union ZifData {
    uint32_t all;
    __attribute__((__packed__)) struct {
        volatile uint8_t motors1Type            : 3;
        volatile uint8_t motors2Type            : 3;
        volatile uint8_t motors3Type            : 3;
        volatile uint8_t zoomMotorId            : 2;
        volatile uint8_t irisMotorId            : 2;
        volatile uint8_t focusMotorId           : 2;
        volatile uint8_t zoomSense              : 2;
        volatile uint8_t zoomRevers             : 1;
        volatile uint8_t irisRevers             : 1;
        volatile uint8_t focusRevers            : 1;
        volatile int8_t zoomDrift              : 8;
        volatile uint8_t NU                     : 4;
    }data;
};

union WheelAnalog {
    uint32_t all;
__attribute__((__packed__)) struct {
    volatile uint8_t wheelSpeedPan :8;
    volatile uint8_t wheelSpeedTilt :8;
    volatile uint8_t wheelSpeedRoll :8;
    volatile uint8_t analogWheelPanActive :1;
    volatile uint8_t analogWheelTiltActive :1;
    volatile uint8_t analogWheelRollActive :1;
    volatile uint8_t analogPedalActive :1;
    volatile uint8_t NU :4;
    }data;
};

union WheelDigital {
    uint32_t all;
__attribute__((__packed__)) struct {
    volatile uint8_t digitalWheelFerstID:2;
    volatile uint8_t digitalWheelSecondID:2;
    volatile uint8_t digitalWheelThirdID:2;
    volatile uint8_t digitalWheelPanTransmission :3;
    volatile uint8_t digitalWheelTiltTransmission :3;
    volatile uint8_t digitalWheelRollTransmission :3;
    volatile uint32_t NU :17;
    }data;
};

union Sinhro {
    uint32_t all;
    __attribute__((__packed__)) struct {
        volatile uint8_t synchroSourceRS232  : 4;
        volatile uint16_t NULow              : 12;
        volatile uint8_t synchroSourceRS485  : 4;
        volatile uint32_t NUHigh             : 12;
    }data;
};

union LoadParameters {
    uint32_t all;
    __attribute__((__packed__)) struct {
        volatile uint16_t cameraTipe                : 16;
        volatile uint16_t objectiveTipe             : 16;
        }data;
};

union Prerol1 {
    uint32_t all;
    __attribute__((__packed__)) struct {
        volatile uint16_t  master :16;
        volatile uint16_t  slave1 :16;
        }data;
};

union Prerol2 {
    uint32_t all;
    __attribute__((__packed__)) struct {
        volatile uint16_t  slave2 :16;
        volatile uint16_t  slave3 :16;
        }data;
};

typedef  uint32_t MAC;

union Resonans {
    uint32_t all;

    __attribute__((__packed__)) struct {
        volatile uint8_t  type :4;
        volatile uint32_t friq :28;
        }data;
    };

struct EepromDefault
{
    uint32_t min;
    uint32_t def;
    uint32_t max;
};



}

*/

#endif /* PULT_INDICATOR_MEMORYSTRUCTUR_HPP_ */
