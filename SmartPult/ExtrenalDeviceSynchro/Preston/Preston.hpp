/*
 * Preston.hpp
 *
 *  Created on: 15 θών. 2019 γ.
 *      Author: user9
 */

#ifndef PULT_PRESTON_HPP_
#define PULT_PRESTON_HPP_




#include <Pult/PultClasses.h>
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "PULT_Indicator/ServoPwm.h"

namespace ExtrenalDevices {

    struct PrestonSetting {
        bool on;
        float speed;
    };


    class Preston {

    private:
        Pwm *prestonDriver;
        PrestonBoard * enableDriver;
        bool enable;

    public:
        Preston(PrestonBoard * enableDriver): enableDriver(enableDriver) , enable(true){

        }

        void setDriver(Pwm * prestonDriver) {
            this->prestonDriver=prestonDriver;
            }

        void setEnableDriver(PrestonBoard * enableDriver) {
            this->enableDriver=enableDriver;
            }

        void update(float speed) {
            if (enable) {
                if (speed>1)
                    speed=1;
                if(speed<-1)
                    speed=-1;
                speed+=1;
                speed*=0.5;
                if (speed>0.923)
                    speed=0.923;
                if (speed<0.077)
                    speed=0.077;
                }
            else {
                speed=0.5;
                }
            enableDriver->setOnState(enable);
            prestonDriver->setDuty(speed);
            }

        void setEnable (bool enable ) { this->enable=enable; }

        inline void xorEnable () {
            if (enable) {
                enable=false;
                return;
                }
            enable=true;
            }

        inline bool getEnable () {
            return enable;
            }

    };

}

#endif /* PULT_PRESTON_HPP_ */
