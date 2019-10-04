/*
 * WheelMenu.hpp
 *
 *  Created on: 14 мая 2019 г.
 *      Author: User9
 */

#ifndef PULT_INDICATOR_WHEELMENU_HPP_
#define PULT_INDICATOR_WHEELMENU_HPP_

#include "LCD Classes.h"
#include "../Pult/PultClasses.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "LCDBase.hpp"

#include "Libs/StandartElement/galetniy Perecluxhatel.hpp"
#include "Libs/StandartElement/encoder.hpp"

namespace Screen {


union WheelAnalog {
    uint32_t all;
    __attribute__((__packed__)) struct {
        volatile uint8_t wheelSpeedPan :7;
        volatile uint8_t wheelSpeedTilt :7;
        volatile uint8_t wheelSpeedRoll :7;
        volatile uint8_t analogWheelPanActive :1;
        volatile uint8_t analogWheelTiltActive :1;
        volatile uint8_t analogWheelRollActive :1;
        volatile uint8_t analogPedalActive :1;
        volatile uint8_t NU :7;
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

union WheelDeadZone {
    uint32_t all;
    __attribute__((__packed__)) struct {
        volatile uint16_t panDeadZone :10;
        volatile uint16_t tiltDeadZone :10;
        volatile uint16_t rollDeadZone :10;
        volatile uint8_t NU :2;
        }data;
    };

enum WheelID {
    pan=0,
    tilt,
    dutch
};

enum DispForm {
    digitalWheel=1,
    analogWheel
};

template <uint8_t numberOfCell>
class LCDWheelBase {

protected :

    LCD::LCDBaseNoRotation<numberOfCell> form;

public:

    LCDWheelBase(LCDHeaderSettings & settingsHeader,LCDWorkcEditSettings** setting):form(settingsHeader,setting) {

        }

    bool select;

    virtual void update ()  {}

    virtual void analisColorCell() {}

    virtual void workingForm() {}

    virtual void save() {}

};

enum {stringBuferLongAnalogWheel=25};

#define stepSpeed 0.01
#define speedMax 1.27
#define speedMin 0.0

const char * headerAnalogWheel = "ANALOG WHEEL MENU";

const char * panStaticText = "PAN SPEED : XXXX";
const char * tiltStaticText = "TILT SPEED : XXXX";

#ifdef USAEdition
const char * rollStaticText = "ROLL SPEED : XXXX";
#else
const char * rollStaticText = "DUTCH SPEED : XXXX";
#endif

const char * cellWheelPanOn = "WHEEL PAN : ON";
const char * cellWheelPanOff = "WHEEL PAN : OFF";
const char * cellWheelTiltOn = "WHEEL TILT : ON";
const char * cellWheelTiltOff = "WHEEL TILT : OFF";
#ifdef USAEdition
const char * cellWheelRollOn = "WHEEL ROLL : ON";
const char * cellWheelRollOff = "WHEEL ROLL : OFF";
#else
const char * cellWheelRollOn = "WHEEL DUTCH : ON";
const char * cellWheelRollOff = "WHEEL DUTCH : OFF";
#endif

const char * cellPedalOn = "PEDAL : ON";
const char * cellPedalOff = "PEDAL : OFF";


LCD::LCDWorkcEditSetting panSpeedSetting = {
                                            .text=&headerAnalogWheel,
                                            .maxRisk=1
                                            };

LCD::LCDWorkcEditSetting tiltSpeedSetting = {
                                            .text=&tiltStaticText,
                                            .maxRisk=1
                                            };

LCD::LCDWorkcEditSetting tiltSpeedSetting = {
                                            .text=&rollStaticText,
                                            .maxRisk=1
                                            };

LCD::LCDHeaderSettings LCDAnalogWheel = {
                                         .visibleHeader=true,
                                         .textHeader=headerAnalogWheel,
                                         .visiblePointerDown=false,
                                         .visiblePointerUp=false,
                                         .visiblePointerRight=true,
                                         .visiblePointerLeft=false
                                        };

class  LCDAnalogWheel:public LCDWheelBase<7> {
public:

    LCDAnalogWheel(): LCDWheelBase(HeaderSettings,) {
        }

private:

    WheelAnalog & analogData;


public:
    virtual void update () {
            }


    virtual void save() {
        EE_Working::Write(EE_LC_WHEEL_ANALOG_DATA, analogData.all);
        }



    virtual void analisColorCell () {

                }

    virtual void workingForm() {
        if (Focused) {
            ClearDisp();
            drawForm();
            Focused=false;
            }

        if (select) {

            if (getButtonState(pult_Button_Right)== PRESSED) {
                right ();
                }

            if (getButtonState(pult_Button_Left)== PRESSED) {
                left();
                }

            if (getButtonState(pult_Button_Right)== HOLD) {
                right ();
                }

            if (getButtonState(pult_Button_Left)== HOLD) {
                left();
                }
            }
        }

private:

    void drawForm() {
        cellPointer.ReDraw();
        cellHeader.ReDraw();
        for (uint8_t i=0; i<encoder.getMaxRisk()+1; i++) {
            cell[i]->ReHide();
            }
        cell[encoder.getActualPosition()]->ReDraw();
        }

    void left() {
                }

    void right () {
                }

    void printData () {
                }
    void printPan() {

        }

    void printRoll() {
        #ifdef USAEdition

        #else

        #endif

        }

    void printTilt() {

        }

};

enum {digitalWheelText=5};

enum {nameWheelID=3};

const char * headerDigitalWheel = "DIGITAL WHEEL MENU";
const char * pointerDigital = "<-";

const char * whellPanOff  = "PAN RANGE: OFF";
const char * whellPanSlow = "PAN RANGE: SLOW";
const char * whellPanMiddle = "PAN RANGE: MIDDLE";
const char * whellPanFast  = "PAN RANGE: FAST";
const char * whellPanFull  = "PAN RANGE: FULL";

char * panText [digitalWheelText] = {(char *)whellPanOff, (char *)whellPanSlow, (char *) whellPanMiddle,(char *)whellPanFast, (char *)whellPanFull} ;

const char * whellTiltOff  = "TILT RANGE: OFF";
const char * whellTiltSlow = "TILT RANGE: SLOW";
const char * whellTiltMiddle = "TILT RANGE: MIDDLE";
const char * whellTiltFast  = "TILT RANGE: FAST";
const char * whellTiltFull  = "TILT RANGE: FULL";

char * tiltText[digitalWheelText] = {(char *)whellTiltOff,(char *)whellTiltSlow,(char *)whellTiltMiddle,(char *)whellTiltFast,(char *)whellTiltFull} ;

#ifdef USAEdition
const char * whellRollOff  = "ROLL RANGE: OFF";
const char * whellRollSlow = "ROLL RANGE: SLOW";
const char * whellRollMiddle = "ROLL RANGE: MIDDLE";
const char * whellRollFast  = "ROLL RANGE: FAST";
const char * whellRollFull  = "ROLL RANGE: FULL";

#else
const char * whellRollOff  = "DUTCH RANGE: OFF";
const char * whellRollSlow = "DUTCH RANGE: SLOW";
const char * whellRollMiddle = "DUTCH RANGE: MIDDLE";
const char * whellRollFast  = "DUTCH RANGE: FAST";
const char * whellRollFull  = "DUTCH RANGE: FULL";
#endif

char * rollText[digitalWheelText] = {(char *)whellRollOff,(char *)whellRollSlow,(char *)whellRollMiddle,(char *)whellRollFast,(char *)whellRollFull} ;

const char * festWheelPan  = "PAN WHEEL: FIRST";
const char * festWheelTilt = "PAN WHEEL: SECOND";
const char * festWheelRoll = "PAN WHEEL: THIRD";

char * festText  [nameWheelID]  = {(char *)festWheelPan,  (char *)festWheelTilt,  (char *)festWheelRoll  };

const char * secondWheelPan  = "TILT WHEEL: FIRST";
const char * secondWheelTilt = "TILT WHEEL: SECOND";
const char * secondWheelRoll = "TILT WHEEL: THIRD";

char * secondText[nameWheelID]  = {(char *)secondWheelPan,(char *)secondWheelTilt,(char *)secondWheelRoll};
#ifdef USAEdition
const char * thirdWheelPan  = "ROLL WHEEL: FIRST";
const char * thirdWheelTilt = "ROLL WHEEL: SECOND";
const char * thirdWheelRoll = "ROLL WHEEL: THIRD";
#else
const char * thirdWheelPan  = "DUTCH WHEEL: FIRST";
const char * thirdWheelTilt = "DUTCH WHEEL: SECOND";
const char * thirdWheelRoll = "DUTCH WHEEL: THIRD";
#endif


char * thirdText [nameWheelID]  = {(char *)thirdWheelPan, (char *)thirdWheelTilt, (char *)thirdWheelRoll };

const char * textStartSelect     = "START WHEEL SELECTION";
const char * textPanWheelSelect  = "CHOOSE THE PAN WHEEL";
const char * textTiltWheelSelect = "CHOOSE THE TILT WHEEL";
#ifdef USAEdition
const char * textRollWheelSelect = "CHOOSE THE ROll WHEEL";
#else
const char * textRollWheelSelect = "CHOOSE THE DUTCH WHEEL";
#endif
const char * textEndSelect       = "ASSIGNMENT COMPLETED";

char * selectWheel[6] = {(char*)textStartSelect,(char*)textPanWheelSelect,(char*)textTiltWheelSelect, (char*)textRollWheelSelect,(char*)textEndSelect};

t_Pos_Size_XY positionHeaderDigital = {    .X=30,  .Y=0, .Xsize=289, .Ysize=30  };

t_Pos_Size_XY positionPointerDigital = {    .X=0,  .Y=0, .Xsize=30, .Ysize=30  };


class  LCDDegitalWheel:public LCDWheelBase {
public:

            }

private:

public:
    virtual void update () {
        }

    virtual void workingForm() {

        if (Focused) {
            ClearDisp();
            drawForm();
            Focused=false;
            }

        if (select) {

            if (getButtonState(pult_Button_Right)== PRESSED) {
                right();
                }

            if (getButtonState(pult_Button_Left)== PRESSED) {
                left();
                }

            if (getButtonState(pult_Button_Select)==PRESSED) {
                if (encoder.getActualPosition()==3) {
                    starSetWheel();
                }
                return;
                }
            }
        }

    virtual void save() {
        EE_Working::Write(EE_LC_WHEEL_DIGITAL_DATA, digitalData.all);
        }

private:

    void starSetWheel() {
        startWheelSelect.p_text=(char *)textPanWheelSelect;
        }

    void drawForm() {
         }

    void left() {
         }

    void right() {
        }

    };

class  LCDWheelDeadZone:public LCDWheelBase {
    
    

private:
    
public:
    
    
    };



class LCDWheelMenu: public LCD_Listener {
private:
    LCDWheelBase * activeForm;
    LCDAnalogWheel analogWheelForm;
    LCDDegitalWheel degitalWheelForm;
    WheelAnalog analogData;
    WheelDigital digitalData;



public:

    LCDWheelMenu():activeForm(&analogWheelForm),analogWheelForm(analogData),degitalWheelForm(digitalData) {
        readEEPROM();
        analogWheelForm.update();
        degitalWheelForm.update();
        }

    virtual void Listener() {
        if (Focused) {
            activeForm->Focused=true;
            Focused=false;
            }
        activeForm->workingForm();

        if (!activeForm->select) {
            if (getButtonState(pult_Button_Left)==PRESSED) {
                left ();
                }
            if (getButtonState(pult_Button_Right)==PRESSED) {
                right ();
                }
            }
        if (getButtonState(pult_Button_Dn)== PRESSED) {
            down ();
            }

        if (getButtonState(pult_Button_Up)== PRESSED) {
            up ();
            }
        if (getButtonState(pult_Button_ESC)==PRESSED) {
            exit();
            }
        if (getButtonState(pult_Button_Select)==PRESSED) {
            Select();
            }
        }

    void readEEPROM() {
        uint32_t v[3];
        v[0]=EE_Working::Read(EE_LC_WHEEL_ANALOG_DATA);
        v[1]=EE_Working::Read(EE_LC_WHEEL_DIGITAL_DATA);
        v[2]=EE_Working::Read(EE_LC_DUTCH_WHEEL_SPEED);

        if (v[2]!=0xFFFFFFFF) { //переделываем организацию памяти по завершению в память по адресам
            refactoringEEPROM(v[0],v[1],v[2]);
            }
        else {
        analogData.all = v[0];
        digitalData.all= v[1]; }

        if(v[0]==0xFFFFFFFF)
            {
 //           EE_Working::Write(EE_LC_WHEEL_ANALOG_DATA);
            }

        p_pult->setPanWheelSpeed(((float)analogData.data.wheelSpeedPan)*0.01);
        p_pult->setTiltWheelSpeed(((float)analogData.data.wheelSpeedTilt)*0.01);
        p_pult->setDutchWheelSpeed(((float)analogData.data.wheelSpeedRoll)*0.01);

        }

private:

    void left () {
        }

    void right () {
        }
public:
    void up() {
        }

    void down () {

        }

    void Select() {

        }
private:
    void exit() {

        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        }

    void refactoringEEPROM (uint32_t data1, uint32_t data2,uint32_t data3) {
        //запилить внятный рефакторинг пока сам не понимаю как должно быть
        float values[3];
        values[0]=*((float*)(&data1));
        values[1]=*((float*)(&data2));
        values[2]=*((float*)(&data3));
        EE_Working::Write(EE_LC_DUTCH_WHEEL_SPEED,   0xFFFFFFFF);
        analogData.data.wheelSpeedPan=(uint8_t)(values[0]*100);
        analogData.data.wheelSpeedTilt=(uint8_t)(values[1]*100);
        analogData.data.wheelSpeedRoll=(uint8_t)(values[2]*100);
        analogData.data.analogWheelPanActive=false;
        analogData.data.analogWheelTiltActive=false;
        analogData.data.analogWheelRollActive=false;
        analogData.data.analogPedalActive=false;
        analogData.data.NU=0;
        EE_Working::Write(EE_LC_WHEEL_ANALOG_DATA, analogData.all);
        digitalData.data.digitalWheelFerstID=0;
        digitalData.data.digitalWheelSecondID=1;
        digitalData.data.digitalWheelThirdID=2;
        digitalData.data.digitalWheelPanTransmission=0;
        digitalData.data.digitalWheelTiltTransmission=0;
        digitalData.data.digitalWheelRollTransmission=0;
        digitalData.data.NU=0;
        EE_Working::Write(EE_LC_WHEEL_DIGITAL_DATA, digitalData.all);
        }

};

}


#endif /* PULT_INDICATOR_WHEELMENU_HPP_ */
