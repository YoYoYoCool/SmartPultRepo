/*
 * Measure Sistems types.hpp
 *
 *  Created on: 29 мар. 2019 г.
 *      Author: User9
 */

#ifndef PULT_INDICATOR_PULTDBWORK_MEASURE_SISTEM_TYPES_HPP_
#define PULT_INDICATOR_PULTDBWORK_MEASURE_SISTEM_TYPES_HPP_


#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "PULT_Indicator/LCD Classes.h"

namespace LCD {

class MeasureSistemTipes: public LCD_Menu {

    public:
    MeasureSistemTipes(char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr) :
            LCD_Menu(pNam, Link, Count, Orient,  Menu_Per_Scr) {

        }
    virtual void Listener() {
        if (this->Focused) {
            ClearDisp();
            Draw(Tek_Count);
            Focused = false;
            }
        if (getButtonState(pult_Button_Select) == PRESSED)
            {

            }
        if (getButtonState(pult_Button_Dn) == PRESSED)  Plus();

        if (getButtonState(pult_Button_Up) == PRESSED)  Minus();

        if (getButtonState(pult_Button_ESC) == PRESSED)
            {
            pDispl = pDispl->Parent;
            pDispl->Focused = true;
            return;
            }
        }
    };

}



#endif /* PULT_INDICATOR_PULTDBWORK_MEASURE_SISTEM_TYPES_HPP_ */
