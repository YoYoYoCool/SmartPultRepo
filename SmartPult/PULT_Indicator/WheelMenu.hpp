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

#include "Libs/StandartElement/galetniy Perecluxhatel.hpp"
#include "Libs/StandartElement/encoder.hpp"




namespace LCD {





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


enum WheelID {
    pan=0,
    tilt,
    dutch
};



enum DispForm {
    digitalWheel=1,
    analogWheel
};






class LCDWheelBase:public LCD_Listener {

protected :
    LCD_Cell cellHeader;
    LCD_Cell cellPointer;


public:

    LCDWheelBase(int16_t maxRisk, char * headerName, t_Pos_Size_XY headerPosition,
                 char * pointerName, t_Pos_Size_XY pointerPosition ):encoder(maxRisk),
                 cellHeader(headerName,headerPosition),cellPointer(pointerName,pointerPosition)

    {

    }


    virtual void update ()  {}
    bool select;
    StandartElement::Encoder encoder;
    LCD_Cell *cell[7];

    virtual void analisColorCell() {}

    virtual void workingForm() {}

    virtual void save() {}

};

enum {stringBuferLongAnalogWheel=25};

#define stepSpeed 0.01
#define speedMax 1.27
#define speedMin 0.0

const char * headerAnalogWheel = "ANALOG WHEEL MENU";
const char * pointerAnalog = "->";
const char * cellWheelPanOn = "WHEEL PAN : ON";
const char * cellWheelPanOff = "WHEEL PAN : OFF";
const char * cellWheelTiltOn = "WHEEL TILT : ON";
const char * cellWheelTiltOff = "WHEEL TILT : OFF";
const char * cellWheelDutchOn = "WHEEL ROLL : ON";
const char * cellWheelRollOff = "WHEEL ROOL : OFF";
const char * cellPedalOn = "PEDAL : ON";
const char * cellPedalOff = "PEDAL : OFF";

t_Pos_Size_XY positionHeaderAnalog = {    .X=0,  .Y=0, .Xsize=285, .Ysize=30  };

t_Pos_Size_XY positionPointerAnalog = {    .X=287,  .Y=0, .Xsize=30, .Ysize=30  };

class  LCDAnalogWheel:public LCDWheelBase {
public:
    LCDAnalogWheel(WheelAnalog & speedData):analogData(speedData),
        LCDWheelBase(6,(char *)headerAnalogWheel,positionHeaderAnalog,(char *)pointerAnalog,positionPointerAnalog)
        {
        analogData.all=0;
        select=false;
        cellHeader.Active_Style = Style_MenuHeader;
        cellHeader.UnActive_Style = Style_MenuHeader;
        cellPointer.Active_Style = Style_MenuHeader;
        cellPointer.UnActive_Style = Style_MenuHeader;
        t_Pos_Size_XY position={  .X=2,  .Y=32,   .Xsize=317,   .Ysize=27     };
        wheelSpeedPan.Set_Coord(position);
        wheelSpeedPan.p_text=& textPanSpeed[0];
        position.Y+=position.Ysize+2;
        wheelSpeedTilt.Set_Coord(position);
        wheelSpeedTilt.p_text=& textTiltSpeed[0];
        position.Y+=position.Ysize+2;
        wheelSpeedRoll.Set_Coord(position);
        wheelSpeedRoll.p_text=& textRollSpeed[0];
        position.Y+=position.Ysize+2;
        wheelPanEnable.Set_Coord(position);
        wheelPanEnable.p_text=(char*)cellWheelPanOff;
        wheelPanEnable.Active_Style.Cell_Color=ClrLinen;
        wheelPanEnable.UnActive_Style.Cell_Color=ClrLinen;
        p_pult->disablePanAnalogWheel();
        position.Y+=position.Ysize+2;
        wheelTiltEnable.Set_Coord(position);
        wheelTiltEnable.p_text=(char*)cellWheelTiltOff;
        wheelTiltEnable.Active_Style.Cell_Color=ClrLinen;
        wheelTiltEnable.UnActive_Style.Cell_Color=ClrLinen;
        p_pult->disableTiltAnalogWheel();
        position.Y+=position.Ysize+2;
        wheelRollEnable.Set_Coord(position);
        wheelRollEnable.p_text=(char*)cellWheelRollOff;
        wheelRollEnable.Active_Style.Cell_Color=ClrLinen;
        wheelRollEnable.UnActive_Style.Cell_Color=ClrLinen;
        p_pult->disableRollAnalogWheel();
        position.Y+=position.Ysize+2;
        pedalEnable.Set_Coord(position);
        pedalEnable.p_text=(char*)cellPedalOff;
        pedalEnable.Active_Style.Cell_Color=ClrLinen;
        pedalEnable.UnActive_Style.Cell_Color=ClrLinen;
        p_pult->disablePadal();
        cell[0]=&wheelSpeedPan;
        cell[1]=&wheelSpeedTilt;
        cell[2]=&wheelSpeedRoll;
        cell[3]=&wheelPanEnable;
        cell[4]=&wheelTiltEnable;
        cell[5]=&wheelRollEnable;
        cell[6]=&pedalEnable;
        }

private:

    LCD_Cell wheelSpeedPan, wheelSpeedTilt, wheelSpeedRoll,
    wheelPanEnable , wheelTiltEnable, wheelRollEnable, pedalEnable;

    float speedPan;
    float speedTilt;
    float speedRoll;
    char  textPanSpeed[stringBuferLongAnalogWheel];
    char  textTiltSpeed[stringBuferLongAnalogWheel];
    char  textRollSpeed[stringBuferLongAnalogWheel];
    WheelAnalog & analogData;


public:
    virtual void update () {
        speedPan=analogData.data.wheelSpeedPan;
        speedPan*=0.01;
        speedTilt=analogData.data.wheelSpeedTilt;
        speedTilt*=0.01;
        speedRoll=analogData.data.wheelSpeedRoll;
        speedRoll*=0.01;
        printData ();
        if (analogData.data.analogWheelPanActive) {
            wheelPanEnable.p_text=(char*)cellWheelPanOn;
            wheelPanEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            wheelPanEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            p_pult->enablePanAnalogWheel();}
        if (analogData.data.analogWheelTiltActive){
            wheelTiltEnable.p_text=(char*)cellWheelTiltOn;
            wheelTiltEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            wheelTiltEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            p_pult->enableTiltAnalogWheel();}
        if (analogData.data.analogWheelRollActive){
            wheelRollEnable.p_text=(char*)cellWheelDutchOn;
            wheelRollEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            wheelRollEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            p_pult->enableRollAnalogWheel();}
        if (analogData.data.analogPedalActive){
            pedalEnable.p_text=(char*)cellPedalOn;
            pedalEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            pedalEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            p_pult->enablePadal();}
            }


    virtual void save() {
        EE_Working::Write(EE_LC_WHEEL_ANALOG_DATA, analogData.all);
        }



    virtual void analisColorCell () {
        if (cell[encoder.getActualPosition()]=&wheelPanEnable) {

            }
        if (cell[encoder.getActualPosition()]=&wheelTiltEnable) {

            }
        if (cell[encoder.getActualPosition()]=&wheelRollEnable) {

            }
        if (cell[encoder.getActualPosition()]=&pedalEnable) {

            }
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
        if (cell[encoder.getActualPosition()]==&wheelSpeedPan) {
            speedPan-=stepSpeed;
            if (speedPan<speedMin) {     speedPan=speedMin; }
            p_pult->setPanWheelSpeed(speedPan);
            sprintf(&textPanSpeed[0],"WHEEL SPEED PAN:" "%1.2f", speedPan);
            wheelSpeedPan.ReDraw();
            float speed = speedPan;
            speed*=100;
            analogData.data.wheelSpeedPan=(uint8_t)speed;
            return;
            }
        if (cell[encoder.getActualPosition()]==&wheelSpeedTilt) {
            speedTilt-=stepSpeed;
            if (speedTilt<speedMin)
                speedTilt=speedMin;
            p_pult->setTiltWheelSpeed(speedTilt);
            sprintf(&textTiltSpeed[0],"WHEEL SPEED TILT:" "%1.2f", speedTilt);
            wheelSpeedTilt.ReDraw();
            float speed = speedTilt;
            speed*=100;
            analogData.data.wheelSpeedTilt=(uint8_t)speed;
            }
        if (cell[encoder.getActualPosition()]==&wheelSpeedRoll) {
            speedRoll-=stepSpeed;
            if (speedRoll<speedMin)
                speedRoll=speedMin;
            p_pult->setDutchWheelSpeed(speedRoll);
            sprintf(&textRollSpeed[0],"WHEEL SPEED ROLL:" "%1.2f", speedRoll);
            wheelSpeedRoll.ReDraw();
            float speed = speedRoll;
            speed*=100;
            analogData.data.wheelSpeedRoll=(uint8_t)speed;
            }
        if (cell[encoder.getActualPosition()]==&wheelPanEnable) {
            analogData.data.analogWheelPanActive=false;
            p_pult->disablePanAnalogWheel();
            wheelPanEnable.Active_Style.Cell_Color=ClrLinen;
            wheelPanEnable.UnActive_Style.Cell_Color=ClrLinen;
            wheelPanEnable.p_text=(char*)cellWheelPanOff;
            wheelPanEnable.ReDraw();
            }
        if (cell[encoder.getActualPosition()]==&wheelTiltEnable) {
            analogData.data.analogWheelTiltActive=false;
            p_pult->disableTiltAnalogWheel();
            wheelTiltEnable.Active_Style.Cell_Color=ClrLinen;
            wheelTiltEnable.UnActive_Style.Cell_Color=ClrLinen;
            wheelTiltEnable.p_text=(char*)cellWheelTiltOff;
            wheelTiltEnable.ReDraw();
            }
        if (cell[encoder.getActualPosition()]==&wheelRollEnable) {
            analogData.data.analogWheelRollActive=false;
            p_pult->disableRollAnalogWheel();
            wheelRollEnable.Active_Style.Cell_Color=ClrLinen;
            wheelRollEnable.UnActive_Style.Cell_Color=ClrLinen;
            wheelRollEnable.p_text=(char*)cellWheelRollOff;
            wheelRollEnable.ReDraw();
            }
        if (cell[encoder.getActualPosition()]==&pedalEnable) {
            analogData.data.analogPedalActive=false;
            p_pult->disablePadal();
            pedalEnable.Active_Style.Cell_Color=ClrLinen;
            pedalEnable.UnActive_Style.Cell_Color=ClrLinen;
            pedalEnable.p_text=(char*)cellPedalOff;
            pedalEnable.ReDraw();
            }
        }

    void right () {
        if (cell[encoder.getActualPosition()]==&wheelSpeedPan) {
            speedPan+=stepSpeed;
            if (speedPan>speedMax)
                speedPan=speedMax;
            p_pult->setPanWheelSpeed(speedPan);
            sprintf(&textPanSpeed[0],"WHEEL SPEED PAN:" "%1.2f", speedPan);
            wheelSpeedPan.ReDraw();
            float speed = speedPan;
            speed*=100;
            analogData.data.wheelSpeedPan=(uint8_t)speed;
            }
        if (cell[encoder.getActualPosition()]==&wheelSpeedTilt) {
            speedTilt+=stepSpeed;
            if (speedTilt>speedMax)
                speedTilt=speedMax;
            p_pult->setTiltWheelSpeed(speedTilt);
            sprintf(&textTiltSpeed[0],"WHEEL SPEED TILT:" "%1.2f", speedTilt);
            wheelSpeedTilt.ReDraw();
            float speed = speedTilt;
            speed*=100;
            analogData.data.wheelSpeedTilt=(uint8_t)speed;
            }
        if (cell[encoder.getActualPosition()]==&wheelSpeedRoll) {
            speedRoll+=stepSpeed;
            if (speedRoll>speedMax)
                speedRoll=speedMax;
            p_pult->setDutchWheelSpeed(speedRoll);
            sprintf(&textRollSpeed[0],"WHEEL SPEED ROLL:" "%1.2f", speedRoll);
            wheelSpeedRoll.ReDraw();
            float speed = speedRoll;
            speed*=100;
            analogData.data.wheelSpeedRoll=(uint8_t)speed;
            }
        if (cell[encoder.getActualPosition()]==&wheelPanEnable) {
            analogData.data.analogWheelPanActive=true;
            p_pult->enablePanAnalogWheel();
            wheelPanEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            wheelPanEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            wheelPanEnable.p_text=(char*)cellWheelPanOn;
            wheelPanEnable.ReDraw();
            }
        if (cell[encoder.getActualPosition()]==&wheelTiltEnable) {
            analogData.data.analogWheelTiltActive=true;
            p_pult->enableTiltAnalogWheel();
            wheelTiltEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            wheelTiltEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            wheelTiltEnable.p_text=(char*)cellWheelTiltOn;
            wheelTiltEnable.ReDraw();
            }
        if (cell[encoder.getActualPosition()]==&wheelRollEnable) {
            analogData.data.analogWheelRollActive=true;
            analogData.data.analogPedalActive=false;
            p_pult->enableRollAnalogWheel();
            p_pult->disablePadal();
            wheelRollEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            wheelRollEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            pedalEnable.Active_Style.Cell_Color=ClrLinen;
            pedalEnable.UnActive_Style.Cell_Color=ClrLinen;
            wheelRollEnable.p_text=(char*)cellWheelDutchOn;
            pedalEnable.p_text=(char*)cellPedalOff;
            pedalEnable.ReHide();
            wheelRollEnable.ReDraw();
            }
        if (cell[encoder.getActualPosition()]==&pedalEnable) {
            analogData.data.analogPedalActive=true;
            analogData.data.analogWheelRollActive=false;
            p_pult->enablePadal();
            p_pult->disableRollAnalogWheel();
            wheelRollEnable.Active_Style.Cell_Color=ClrLinen;
            wheelRollEnable.UnActive_Style.Cell_Color=ClrLinen;
            pedalEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            pedalEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            pedalEnable.p_text=(char*)cellPedalOn;
            wheelRollEnable.p_text=(char*)cellWheelRollOff;
            wheelRollEnable.ReHide();
            pedalEnable.ReDraw();
            }
        }

    void printData () {
        sprintf(&textPanSpeed[0],"WHEEL SPEED PAN:" "%1.2f", speedPan);
        sprintf(&textTiltSpeed[0],"WHEEL SPEED TILT:" "%1.2f", speedTilt);
        sprintf(&textRollSpeed[0],"WHEEL SPEED ROLL:" "%1.2f", speedRoll);
        }

};

enum {digitalWheelText=5};

enum {nameWheelID=3};

const char * headerDigitalWheel = "DIGITAL WHEEL MENU";
const char * pointerDigital = "<-";

const char * whellPanOff = "TRANSMISSION PAN: OFF";
const char * whellPanSqrt = "TRANSMISSION PAN: FIRST";
const char * whellPanLine = "TRANSMISSION PAN: SECOND";
const char * whellPanSqr = "TRANSMISSION PAN: THIRD";
const char * whellPanCub = "TRANSMISSION PAN: CUB";

char * panText [digitalWheelText] = {(char *)whellPanOff, (char *)whellPanSqrt, (char *) whellPanLine,(char *)whellPanSqr, (char *)whellPanCub} ;

const char * whellTiltOff = "TRANSMISSION TILT: OFF";
const char * whellTiltSqrt = "TRANSMISSION TILT: FIRST";
const char * whellTiltLine = "TRANSMISSION TILT: SECOND";
const char * whellTiltSqr = "TRANSMISSION TILT: THIRD";
const char * whellTiltCub = "TRANSMISSION TILT: CUB";

char * tiltText[digitalWheelText] = {(char *)whellTiltOff,(char *)whellTiltSqrt,(char *)whellTiltLine,(char *)whellTiltSqr,(char *)whellTiltCub} ;

const char * whellRollOff = "TRANSMISSION ROLL: OFF";
const char * whellRollSqrt = "TRANSMISSION ROLL: FIRST";
const char * whellRollLine = "TRANSMISSION ROLL: SECOND";
const char * whellRollSqr = "TRANSMISSION ROLL: THIRD";
const char * whellRollCub = "TRANSMISSION ROLL: CUB";

char * rollText[digitalWheelText] = {(char *)whellRollOff,(char *)whellRollSqrt,(char *)whellRollLine,(char *)whellRollSqr,(char *)whellRollCub} ;

const char * festWheelPan  = "PAN WHEEL: FIRST";
const char * festWheelTilt = "PAN WHEEL: SECOND";
const char * festWheelRoll = "PAN WHEEL: THIRD";

char * festText  [nameWheelID]  = {(char *)festWheelPan,  (char *)festWheelTilt,  (char *)festWheelRoll  };

const char * secondWheelPan  = "TILT WHEEL: FIRST";
const char * secondWheelTilt = "TILT WHEEL: SECOND";
const char * secondWheelRoll = "TILT WHEEL: THIRD";

char * secondText[nameWheelID]  = {(char *)secondWheelPan,(char *)secondWheelTilt,(char *)secondWheelRoll};

const char * thirdWheelPan  = "ROLL WHEEL: FIRST";
const char * thirdWheelTilt = "ROLL WHEEL: SECOND";
const char * thirdWheelRoll = "ROLL WHEEL: THIRD";

char * thirdText [nameWheelID]  = {(char *)thirdWheelPan, (char *)thirdWheelTilt, (char *)thirdWheelRoll };

const char * textStartSelect     = "START WHEEL SELECTION";
const char * textPanWheelSelect  = "CHOOSE THE PAN WHEEL";
const char * textTiltWheelSelect = "CHOOSE THE TILT WHEEL";
const char * textRollWheelSelect = "CHOOSE THE ROll WHEEL";
const char * textEndSelect       = "ASSIGNMENT COMPLETED";

char * selectWheel[6] = {(char*)textStartSelect,(char*)textPanWheelSelect,(char*)textTiltWheelSelect, (char*)textRollWheelSelect,(char*)textEndSelect};

t_Pos_Size_XY positionHeaderDigital = {    .X=30,  .Y=0, .Xsize=289, .Ysize=30  };

t_Pos_Size_XY positionPointerDigital = {    .X=0,  .Y=0, .Xsize=30, .Ysize=30  };


class  LCDDegitalWheel:public LCDWheelBase {
public:
    LCDDegitalWheel( WheelDigital & digitalData):
        digitalData(digitalData),
        LCDWheelBase(3,(char *)headerDigitalWheel,positionHeaderDigital,(char *)pointerDigital,positionPointerDigital) {
            select=false;

            cellHeader.Active_Style = Style_MenuHeader;
            cellHeader.UnActive_Style = Style_MenuHeader;
            cellPointer.Active_Style = Style_MenuHeader;
            cellPointer.UnActive_Style = Style_MenuHeader;

            t_Pos_Size_XY position={  .X=2,  .Y=32,   .Xsize=315,   .Ysize=27     };
            wheelPanTransmission.Set_Coord(position);
            wheelPanTransmission.p_text=(char *)whellPanOff;
            wheelPanTransmission.Active_Style.Cell_Color=ClrLinen;
            wheelPanTransmission.UnActive_Style.Cell_Color=ClrLinen;

            position.Y+=position.Ysize;
            wheelTiltTransmission.Set_Coord(position);
            wheelTiltTransmission.p_text=(char *)whellTiltOff;
            wheelTiltTransmission.Active_Style.Cell_Color=ClrLinen;
            wheelTiltTransmission.UnActive_Style.Cell_Color=ClrLinen;

            position.Y+=position.Ysize;
            wheelRollTransmission.Set_Coord(position);
            wheelRollTransmission.p_text=(char *)whellRollOff;
            wheelRollTransmission.Active_Style.Cell_Color=ClrLinen;
            wheelRollTransmission.UnActive_Style.Cell_Color=ClrLinen;

            p_pult->disableAllDigitalWheel();
            position.Y+=position.Ysize;
            startWheelSelect.Set_Coord(position);
            startWheelSelect.p_text=(char *)textStartSelect;

            position.Y+=position.Ysize;
            ferstIDWheel.Set_Coord(position);
            ferstIDWheel.p_text=(char *)festWheelPan;

            position.Y+=position.Ysize;
            secondIDWheel.Set_Coord(position);
            secondIDWheel.p_text=(char *)secondWheelTilt;

            position.Y+=position.Ysize;
            thirdIDWheel.Set_Coord(position);
            thirdIDWheel.p_text=(char *)thirdWheelRoll;

            cell[0]=&wheelPanTransmission;
            cell[1]=&wheelTiltTransmission;
            cell[2]=&wheelRollTransmission;
            cell[3]=&startWheelSelect;
            cell[4]=&ferstIDWheel;
            cell[5]=&secondIDWheel;
            cell[6]=&thirdIDWheel;


            }

private:

    LCD_Cell wheelPanTransmission, wheelTiltTransmission, wheelRollTransmission;
    LCD_Cell ferstIDWheel, secondIDWheel,thirdIDWheel;
    LCD_Cell startWheelSelect;
    WheelDigital & digitalData;

public:
    virtual void update () {
        p_pult->digitalWheelPanSetFunction((int8_t)digitalData.data.digitalWheelPanTransmission,
                                           (int8_t)digitalData.data.digitalWheelTiltTransmission,
                                           (int8_t)digitalData.data.digitalWheelRollTransmission);
        if (digitalData.data.digitalWheelPanTransmission!=0) {
            p_pult->digitalWhellPanEnable();
            wheelPanTransmission.p_text=panText[(int8_t)digitalData.data.digitalWheelPanTransmission];
            wheelPanTransmission.Active_Style.Cell_Color=ClrMediumOrchid;
            wheelPanTransmission.UnActive_Style.Cell_Color=ClrMediumOrchid;
            }

        if (digitalData.data.digitalWheelTiltTransmission!=0) {
            p_pult->digitalWhellTiltEnable();
            wheelTiltTransmission.p_text=tiltText[(int8_t)digitalData.data.digitalWheelTiltTransmission];
            wheelTiltTransmission.Active_Style.Cell_Color=ClrMediumOrchid;
            wheelTiltTransmission.UnActive_Style.Cell_Color=ClrMediumOrchid;
            }

        if (digitalData.data.digitalWheelRollTransmission!=0) {
            p_pult->digitalWhellRollEnable();
            wheelRollTransmission.p_text=rollText[(int8_t)digitalData.data.digitalWheelRollTransmission];
            wheelRollTransmission.Active_Style.Cell_Color=ClrMediumOrchid;
            wheelRollTransmission.UnActive_Style.Cell_Color=ClrMediumOrchid;
            }

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
        cellPointer.ReDraw();
        cellHeader.ReDraw();
        for (uint8_t i=0; i<encoder.getMaxRisk()+1; i++) {
            cell[i]->ReHide();
            }
        cell[encoder.getActualPosition()]->ReDraw();
        ferstIDWheel.ReHide();
        secondIDWheel.ReHide();
        thirdIDWheel.ReHide();
        }

    void left() {
        if (cell[encoder.getActualPosition()]==&wheelPanTransmission) {
            p_pult->digitalWheelPanStepLeft();
            digitalData.data.digitalWheelPanTransmission=p_pult->digitalWheelPanGetFunction();
            if (p_pult->digitalWheelPanGetFunction()==0) {
                p_pult->digitalWhellPanDisable();
                wheelPanTransmission.Active_Style.Cell_Color=ClrLinen;
                wheelPanTransmission.UnActive_Style.Cell_Color=ClrLinen;
                }
            wheelPanTransmission.p_text=panText[p_pult->digitalWheelPanGetFunction()];
            wheelPanTransmission.ReDraw();
            return;
            }

        if (cell[encoder.getActualPosition()]==&wheelTiltTransmission) {
            p_pult->digitalWheelTiltStepLeft();
            digitalData.data.digitalWheelTiltTransmission=p_pult->digitalWheelTiltGetFunction();
            if (p_pult->digitalWheelTiltGetFunction()==0) {
                p_pult->digitalWhellTiltDisable();
                wheelTiltTransmission.Active_Style.Cell_Color=ClrLinen;
                wheelTiltTransmission.UnActive_Style.Cell_Color=ClrLinen;
                }
            wheelTiltTransmission.p_text=tiltText[p_pult->digitalWheelTiltGetFunction()];
            wheelTiltTransmission.ReDraw();
            return;
            }

        if (cell[encoder.getActualPosition()]==&wheelRollTransmission) {
            p_pult->digitalWheelRollStepLeft();
            digitalData.data.digitalWheelRollTransmission=p_pult->digitalWheelRollGetFunction();
            if (p_pult->digitalWheelRollGetFunction()==0) {
                p_pult->digitalWhellRollDisable();
                wheelRollTransmission.Active_Style.Cell_Color=ClrLinen;
                wheelRollTransmission.UnActive_Style.Cell_Color=ClrLinen;
                }
            wheelRollTransmission.p_text=rollText[p_pult->digitalWheelRollGetFunction()];
            wheelRollTransmission.ReDraw();
            return;
            }
        }

    void right() {
        if (cell[encoder.getActualPosition()]==&wheelPanTransmission) {
            if (p_pult->digitalWheelPanGetFunction()==0) {
                p_pult->digitalWhellPanEnable();
                wheelPanTransmission.Active_Style.Cell_Color=ClrMediumOrchid;
                wheelPanTransmission.UnActive_Style.Cell_Color=ClrMediumOrchid;
                }
            p_pult->digitalWheelPanStepRight();
            digitalData.data.digitalWheelPanTransmission=p_pult->digitalWheelPanGetFunction();
            wheelPanTransmission.p_text=panText[p_pult->digitalWheelPanGetFunction()];
            wheelPanTransmission.ReDraw();
            return;
            }

        if (cell[encoder.getActualPosition()]==&wheelTiltTransmission) {
            if (p_pult->digitalWheelTiltGetFunction()==0) {
                p_pult->digitalWhellTiltEnable();
                wheelTiltTransmission.Active_Style.Cell_Color=ClrMediumOrchid;
                wheelTiltTransmission.UnActive_Style.Cell_Color=ClrMediumOrchid;
                }
            p_pult->digitalWheelTiltStepRight();
            digitalData.data.digitalWheelTiltTransmission=p_pult->digitalWheelTiltGetFunction();
            wheelTiltTransmission.p_text=tiltText[p_pult->digitalWheelTiltGetFunction()];
            wheelTiltTransmission.ReDraw();
            return;
            }

        if (cell[encoder.getActualPosition()]==&wheelRollTransmission) {
            if (p_pult->digitalWheelRollGetFunction()==0) {
                p_pult->digitalWhellRollEnable();
                wheelRollTransmission.Active_Style.Cell_Color=ClrMediumOrchid;
                wheelRollTransmission.UnActive_Style.Cell_Color=ClrMediumOrchid;
                }
            p_pult->digitalWheelRollStepRight();
            digitalData.data.digitalWheelRollTransmission=p_pult->digitalWheelRollGetFunction();
            wheelRollTransmission.p_text=rollText[p_pult->digitalWheelRollGetFunction()];
            wheelRollTransmission.ReDraw();
            return;
            }
        }

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
        ReadEEPROM();
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

    void ReadEEPROM() {
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
        if (activeForm==&analogWheelForm)
            return;
        activeForm=&analogWheelForm;
        activeForm->Focused=true;
        }

    void right () {
        if (activeForm==&degitalWheelForm)
            return;
        activeForm=&degitalWheelForm;
        activeForm->Focused=true;
        }
public:
    void up() {
        int16_t i = activeForm->encoder.getActualPosition();
        activeForm->cell[activeForm->encoder.getActualPosition()]->ReHide();
        activeForm->encoder.stepLeft();
        activeForm->cell[activeForm->encoder.getActualPosition()]->ReDraw();
        }

    void down () {
        activeForm->cell[activeForm->encoder.getActualPosition()]->ReHide();
        activeForm->encoder.stepRight();
        activeForm->cell[activeForm->encoder.getActualPosition()]->ReDraw();
        }

    void Select() {
        for (int16_t i=0; i<activeForm->encoder.getMaxRisk()+1; i++) {
            activeForm->cell[i]->Active_Style.Border_Color=ClrSkyBlue;
            activeForm->cell[i]->ReHide();
            }
        activeForm->cell[activeForm->encoder.getActualPosition()]->ReDraw();
        activeForm->select=true;
        }
private:
    void exit() {
        if (activeForm->select) {
            int16_t a= activeForm->encoder.getMaxRisk()+1;
            for (int16_t i=0; i<a; i++) {
                activeForm->cell[i]->Active_Style.Border_Color=ClrWhite;
                activeForm->cell[i]->UnActive_Style.Border_Color=ClrBlack;
                activeForm->cell[i]->ReHide();
                }
            activeForm->cell[activeForm->encoder.getActualPosition()]->ReDraw();
            activeForm->save();
            activeForm->select=false;
            return;
            }

        activeForm->encoder.setActualPosition(0);
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
