/*
 * ZIFRevers.hpp
 *
 *  Created on: 11 θών. 2019 γ.
 *      Author: User9
 */

#ifndef PULT_INDICATOR_ZIFREVERS_HPP_
#define PULT_INDICATOR_ZIFREVERS_HPP_





#include "LCD Classes.h"
#include "../Pult/PultClasses.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "LCDBase.hpp"


namespace LCD {


enum {
     maxText=2,
     maxString=3
     };

const char* zifRevers = "ZIF REVERS MENU";

LCDHeaderSettings settingsHeader = {
                            .visibleHeader=true,
                            .visiblePointerLeft=false,
                            .visiblePointerRight=false,
                            .visiblePointerUp=false,
                            .visiblePointerDown=false,
                            .textHeader= zifRevers
    };

    const char * zoomReversOn   = "ZOOM REVERS ON";
    const char * zoomReversOff  = "ZOOM REVERS OFF";
    const char * irisReversOn   = "IRIS REVERS ON";
    const char * irisReversOff  = "IRIS REVERS OFF";
    const char * focusReversOn   = "FOCUS REVERS ON";
    const char * focusReversOff  = "FOCUS REVERS OFF";

    char * zoomText[maxText] = { (char*)zoomReversOn,
                                       (char*)zoomReversOff  };
    char * irisText[maxText] = { (char*)irisReversOn,
                                       (char*)irisReversOff  };
    char * focusText[maxText] = { (char*)focusReversOn,
                                        (char*)focusReversOff  };

    LCDWorkcEditSettings zoomSetting = {    .maxRisk=maxText,
                                            .text=&zoomText[0]    };

    LCDWorkcEditSettings focusSetting = {    .maxRisk=maxText,
                                            .text=&focusText[0]    };

    LCDWorkcEditSettings irisSetting = {    .maxRisk=maxText,
                                            .text=&irisText[0]    };

    LCDWorkcEditSettings * ZIFformSettings[maxString] = {&zoomSetting,&focusSetting,&irisSetting};



class ZIFRevers: public LCD_Listener {

private:


    union memoryZIFRevers {
        uint32_t all;
        struct  {
            uint8_t zoomRevers  :1;
            uint8_t irisRevers  :1;
            uint8_t focusRevers :1;
            uint32_t NU         :29;
            }data;
        };


    LCDBaseNoRotation <maxString> form;
    memoryZIFRevers reversData;
    uint32_t color[maxText];

public:

    ZIFRevers ():form(settingsHeader,&ZIFformSettings[0]) {
        readEEPROM();
        }

    virtual void Listener() {
        if (this->Focused) {
            ClearDisp();
            form.drawForm();
            this->Focused=false;
        }

        if (getButtonState(pult_Button_ESC)==PRESSED) {
            exit();
        }

        if (getButtonState(pult_Button_Dn)== PRESSED) {
            form.stepDown();
        }

        if (getButtonState(pult_Button_Up)== PRESSED) {
            form.stepUp();
        }

        if (getButtonState(pult_Button_Left)==PRESSED) {
            form.stepLeft();
            updateRevers();
        }

        if (getButtonState(pult_Button_Right)==PRESSED) {
            form.stepRight();
            updateRevers();
        }

        if (getButtonState(pult_Button_Select)==PRESSED) {

        }

    }

    void readEEPROM() {
        reversData.all=EE_Working::Read(EE_LC_ZIF_REVERS);
        if (reversData.all==0xFFFFFFFF) {
            reversData.all=0x7;
            EE_Working::Write(EE_LC_ZIF_REVERS, reversData.all);
            }
        int8_t val[maxString];
        val[0]=reversData.data.zoomRevers;
        val[1]=reversData.data.focusRevers;
        val[2]=reversData.data.irisRevers;
        for (int8_t i=0; i<maxString; i++)
            form.setGorizontalEncoder(i,val[i]);
        p_pult->setZIFRevers((bool)reversData.data.zoomRevers,
                             (bool)reversData.data.focusRevers,
                             (bool)reversData.data.irisRevers);
        }

private:

    void updateRevers() {
        if (form.encoderVertical.getActualPosition()==0) {
            reversData.data.zoomRevers=(uint8_t)form.getCellEncoderPosition();
            }
        if (form.encoderVertical.getActualPosition()==1) {
            reversData.data.focusRevers=(uint8_t)form.getCellEncoderPosition();
            }
        if (form.encoderVertical.getActualPosition()==2) {
            reversData.data.irisRevers=(uint8_t)form.getCellEncoderPosition();
            }
        p_pult->setZIFRevers((bool)reversData.data.zoomRevers,
                             (bool)reversData.data.focusRevers,
                             (bool)reversData.data.irisRevers);
        saveInEEPROM();
        }

    void exit() {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        }

    inline void saveInEEPROM (){
        EE_Working::Write(EE_LC_ZIF_REVERS, reversData.all);
    }





};



}



#endif /* PULT_INDICATOR_ZIFREVERS_HPP_ */
