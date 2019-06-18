/*
 * backlight.hpp
 *
 *  Created on: 16 июн. 2019 г.
 *      Author: user9
 */

#ifndef PULT_BACKLIGHT_HPP_
#define PULT_BACKLIGHT_HPP_

#include "PULT_Indicator/ServoPwm.h"
#include <Pult/PultClasses.h>


class Backlight {

private:
    Pwm *backlightDriver;
    float brightness; // в процентах
    PultButton* backlightButton;

public:

    Backlight(Pwm * backlightDriver,PultButton* button): backlightDriver(backlightDriver),
    backlightButton(button),
    brightness(99) {

    }

    Backlight() {
        brightness=99.0;
    }


    void setBrightness (float brightness) {     this->brightness=brightness;    }

    void update() {

        if (backlightButton->isPressed()) {
            backlightDriver->setDuty(brightness*0.01);
            }
        else  {
            backlightDriver->setDuty(0.0);
            }
        }

    void setDriver(Pwm * backlightDriver) {
        this->backlightDriver=backlightDriver;
        }

    void setButton(PultButton* backlightButton) {
        this->backlightButton=backlightButton;
        }

};



#endif /* PULT_BACKLIGHT_HPP_ */
