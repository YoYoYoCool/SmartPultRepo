/*
 * Objective Select.hpp
 *
 *  Created on: 28 мар. 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef PULT_INDICATOR_PULTDBWORK_OBJECTIVE_SELECT_HPP_
#define PULT_INDICATOR_PULTDBWORK_OBJECTIVE_SELECT_HPP_

#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "PULT_Indicator/LCD Classes.h"
#include "LensParam/LensDb.hpp"

namespace LCD {


class LCDMenuObjectiveSelect:public LCD_Menu {

private:
//    LensDb::LensDb& lensDataBaseManager;
    uint32_t objectivId;
    uint32_t maxObjectivSaveDB;

public:
 /*   LCDMenuObjectiveSelect(LensDb::LensDb& lensDataBaseManager,char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr) :
        //lensDataBaseManager(lensDataBaseManager),
        LCD_Menu(pNam, Link, Count, Orient,  Menu_Per_Scr), {

        }*/
    LCDMenuObjectiveSelect(char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr) :
        LCD_Menu(pNam, Link, Count, Orient,  Menu_Per_Scr),
        objectivId(0){

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

#endif /* PULT_INDICATOR_PULTDBWORK_OBJECTIVE_SELECT_HPP_ */
