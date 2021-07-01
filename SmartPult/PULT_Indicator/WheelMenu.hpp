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
        volatile uint8_t analogPedalActive :2;
        volatile uint8_t NU :6;
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
        volatile uint8_t digitalPedalChannel :2;
        volatile uint8_t shakerBoxChannel :1;
        volatile uint32_t NU :14;
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
#ifdef USAEdition
const char * cellWheelRollOn = "WHEEL ROLL : ON";
const char * cellWheelRollOff = "WHEEL ROLL : OFF";
#else
const char * cellWheelRollOn = "WHEEL DUTCH: ON";
const char * cellWheelRollOff = "WHEEL DUTCH: OFF";
#endif

#ifdef USAEdition
const char * cellPedalOnRoll = "PEDAL : ON ROLL";
#else
const char * cellPedalOnRoll = "PEDAL : ON DUTCH";
#endif
const char * cellPedalOnZoom = "PEDAL : ON ZOOM";
const char * cellPedalOff = "PEDAL : OFF";
t_Pos_Size_XY positionHeaderAnalog = {    .X=0,  .Y=0, .Xsize=285, .Ysize=30  };

t_Pos_Size_XY positionPointerAnalog = {    .X=287,  .Y=0, .Xsize=30, .Ysize=30  };

class  LCDAnalogWheel:public LCDWheelBase {
public:
    LCDAnalogWheel(WheelAnalog & speedData):analogData(speedData),
        LCDWheelBase(6,(char *)headerAnalogWheel,positionHeaderAnalog,
        (char *)pointerAnalog,positionPointerAnalog),
        encoderPedal(2,0)
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
    StandartElement::Encoder encoderPedal;


public:
    virtual void update ()
    {
        speedPan=analogData.data.wheelSpeedPan;
        speedPan*=0.01;
        speedTilt=analogData.data.wheelSpeedTilt;
        speedTilt*=0.01;
        speedRoll=analogData.data.wheelSpeedRoll;
        speedRoll*=0.01;
        printData();
        if (analogData.data.analogWheelPanActive)
            {
            wheelPanEnable.p_text=(char*)cellWheelPanOn;
            wheelPanEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            wheelPanEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            p_pult->enablePanAnalogWheel();
            }
        if (analogData.data.analogWheelTiltActive)
            {
            wheelTiltEnable.p_text=(char*)cellWheelTiltOn;
            wheelTiltEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            wheelTiltEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            p_pult->enableTiltAnalogWheel();
            }
        if (analogData.data.analogWheelRollActive)
            {
            wheelRollEnable.p_text=(char*)cellWheelRollOn;
            wheelRollEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            wheelRollEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            p_pult->enableRollAnalogWheel();
            }
        if (analogData.data.analogPedalActive==1)
            {
            pedalEnable.p_text=(char*)cellPedalOnRoll;
            pedalEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            pedalEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            p_pult->enableDutchPadal();
            }
        if (analogData.data.analogPedalActive==2)
            {
            pedalEnable.p_text=(char*)cellPedalOnZoom;
            pedalEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            pedalEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            p_pult->enableZoomPadal();
            }
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
            float speed = speedPan;
            speed*=100;
            printPan();
            analogData.data.wheelSpeedPan=(uint8_t)speed;
            }
        if (cell[encoder.getActualPosition()]==&wheelSpeedTilt) {
            speedTilt-=stepSpeed;
            if (speedTilt<speedMin)
                speedTilt=speedMin;
            p_pult->setTiltWheelSpeed(speedTilt);
            float speed = speedTilt;
            speed*=100;
            printTilt();
            analogData.data.wheelSpeedTilt=(uint8_t)speed;
            }
        if (cell[encoder.getActualPosition()]==&wheelSpeedRoll) {
            speedRoll-=stepSpeed;
            if (speedRoll<speedMin)
                speedRoll=speedMin;
            p_pult->setDutchWheelSpeed(speedRoll);
            float speed = speedRoll;
            speed*=100;
            printRoll();
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
            encoderPedal.decrement();
            analogData.data.analogPedalActive=encoderPedal.getActualPosition();
            switch (encoderPedal.getActualPosition())
            {
            case 0:
                p_pult->disablePadal();
                pedalEnable.Active_Style.Cell_Color=ClrLinen;
                pedalEnable.UnActive_Style.Cell_Color=ClrLinen;
                pedalEnable.p_text=(char*)cellPedalOff;
                break;
            case 1:
                p_pult->enableDutchPadal();
                pedalEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
                pedalEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
                pedalEnable.p_text=(char*)cellPedalOnRoll;
                break;
            case 2:
                p_pult->enableZoomPadal();
                pedalEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
                pedalEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
                pedalEnable.p_text=(char*)cellPedalOnZoom;
                break;
            }
            pedalEnable.ReDraw();
            }
        }

    void right () {
        if (cell[encoder.getActualPosition()]==&wheelSpeedPan) {
            speedPan+=stepSpeed;
            if (speedPan>speedMax)
                speedPan=speedMax;
            p_pult->setPanWheelSpeed(speedPan);
            float speed = speedPan;
            speed*=100;
            printPan();
            analogData.data.wheelSpeedPan=(uint8_t)speed;
            }
        if (cell[encoder.getActualPosition()]==&wheelSpeedTilt) {
            speedTilt+=stepSpeed;
            if (speedTilt>speedMax)
                speedTilt=speedMax;
            p_pult->setTiltWheelSpeed(speedTilt);
            float speed = speedTilt;
            speed*=100;
            printTilt();
            analogData.data.wheelSpeedTilt=(uint8_t)speed;
            }
        if (cell[encoder.getActualPosition()]==&wheelSpeedRoll) {
            speedRoll+=stepSpeed;
            if (speedRoll>speedMax)
                speedRoll=speedMax;
            p_pult->setDutchWheelSpeed(speedRoll);
            float speed = speedRoll;
            speed*=100;
            printRoll();
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
            encoderPedal.setActualPosition(0);
            analogData.data.analogPedalActive=encoderPedal.getActualPosition();
            wheelRollEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
            wheelRollEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            pedalEnable.Active_Style.Cell_Color=ClrLinen;
            pedalEnable.UnActive_Style.Cell_Color=ClrLinen;
            wheelRollEnable.p_text=(char*)cellWheelRollOn;
            pedalEnable.p_text=(char*)cellPedalOff;
            pedalEnable.ReHide();
            wheelRollEnable.ReDraw();
            }
        if (cell[encoder.getActualPosition()]==&pedalEnable) {
            analogData.data.analogWheelRollActive=false;
            p_pult->disableRollAnalogWheel();
            wheelRollEnable.Active_Style.Cell_Color=ClrLinen;
            wheelRollEnable.UnActive_Style.Cell_Color=ClrLinen;
            encoderPedal.increment();
            analogData.data.analogPedalActive=encoderPedal.getActualPosition();
            switch (encoderPedal.getActualPosition())
            {
            case 0:
                p_pult->disablePadal();
                pedalEnable.Active_Style.Cell_Color=ClrLinen;
                pedalEnable.UnActive_Style.Cell_Color=ClrLinen;
                pedalEnable.p_text=(char*)cellPedalOff;
                break;
            case 1:
                p_pult->enableDutchPadal();
                pedalEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
                pedalEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
                pedalEnable.p_text=(char*)cellPedalOnRoll;
                break;
            case 2:
                p_pult->enableZoomPadal();
                pedalEnable.Active_Style.Cell_Color=ClrDarkSlateBlue;
                pedalEnable.UnActive_Style.Cell_Color=ClrDarkSlateBlue;
                pedalEnable.p_text=(char*)cellPedalOnZoom;
                break;
            }
            wheelRollEnable.ReHide();
            pedalEnable.ReDraw();
            }

        }

    void printData () {
        sprintf(&textPanSpeed[0],"PAN SPEED: " "%1.2f", speedPan);
        sprintf(&textTiltSpeed[0],"TILT SPEED: " "%1.2f", speedTilt);
        #ifdef USAEdition
        sprintf(&textRollSpeed[0],"ROLL SPEED: " "%1.2f", speedRoll);
        #else
        sprintf(&textRollSpeed[0],"DUTCH SPEED: " "%1.2f", speedRoll);
        #endif
        }
    void printPan() {
        sprintf(&textPanSpeed[0],"PAN SPEED: " "%1.2f", speedPan);
        wheelSpeedPan.ReDraw();
        }

    void printRoll() {
        #ifdef USAEdition
        sprintf(&textRollSpeed[0],"ROLL SPEED: " "%1.2f", speedRoll);
        #else
        sprintf(&textRollSpeed[0],"DUTCH SPEED: " "%1.2f", speedRoll);
        #endif
        wheelSpeedRoll.ReDraw();
        }

    void printTilt() {
        sprintf(&textTiltSpeed[0],"TILT SPEED: " "%1.2f", speedTilt);
        wheelSpeedTilt.ReDraw();
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

const char * textDigitalPedalDisable     = "DIGITAL PEDAL: DISABLE";
#ifdef USAEdition
const char * textDigitalPedalRoll  = "DIGITAL PEDAL: ROLL";
#else
const char * textDigitalPedalRoll  = "DIGITAL PEDAL: DUTCH";
#endif
const char * textDigitalPedalZoom = "DIGITAL PEDAL: ZOOM";
char * pedalText[3] = {
                         (char*)textDigitalPedalDisable,
                         (char*)textDigitalPedalRoll,
                         (char*)textDigitalPedalZoom
};

const char * shakerBoxDisable  = "SHAKER BOX: DISABLE";
const char * shakerBoxEnable = "SHAKER BOX: ENABLE";


char * shakerBoxText  [2]  = {
                              (char *)shakerBoxDisable,
                              (char *)shakerBoxEnable  };

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



t_Pos_Size_XY positionHeaderDigital = {    .X=30,  .Y=0, .Xsize=289, .Ysize=30  };

t_Pos_Size_XY positionPointerDigital = {    .X=0,  .Y=0, .Xsize=30, .Ysize=30  };


class  LCDDegitalWheel:public LCDWheelBase {
public:
    LCDDegitalWheel( WheelDigital & digitalData):
        digitalData(digitalData),
        LCDWheelBase(4,(char *)headerDigitalWheel,positionHeaderDigital,(char *)pointerDigital,positionPointerDigital) {
            select=false;
            digitalData.all=0;
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
            digitalPedal.Set_Coord(position);
            digitalPedal.p_text=(char *)textDigitalPedalDisable;
            digitalPedal.Active_Style.Cell_Color=ClrLinen;
            digitalPedal.UnActive_Style.Cell_Color=ClrLinen;

            position.Y+=position.Ysize;
            shakerBox.Set_Coord(position);
            shakerBox.p_text=(char *)shakerBoxDisable;
            shakerBox.Active_Style.Cell_Color=ClrLinen;
            shakerBox.UnActive_Style.Cell_Color=ClrLinen;

            position.Y+=position.Ysize;
            secondIDWheel.Set_Coord(position);
            secondIDWheel.p_text=(char *)secondWheelTilt;

            position.Y+=position.Ysize;
            thirdIDWheel.Set_Coord(position);
            thirdIDWheel.p_text=(char *)thirdWheelRoll;

            cell[0]=&wheelPanTransmission;
            cell[1]=&wheelTiltTransmission;
            cell[2]=&wheelRollTransmission;
            cell[3]=&digitalPedal;
            cell[4]=&shakerBox;
            cell[5]=&secondIDWheel;
            cell[6]=&thirdIDWheel;
            }

private:

    LCD_Cell wheelPanTransmission, wheelTiltTransmission, wheelRollTransmission,digitalPedal,shakerBox;
    LCD_Cell  secondIDWheel,thirdIDWheel;

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
        else {
            p_pult->digitalWhellPanDisable();
        }

        if (digitalData.data.digitalWheelTiltTransmission!=0) {
            p_pult->digitalWhellTiltEnable();
            wheelTiltTransmission.p_text=tiltText[(int8_t)digitalData.data.digitalWheelTiltTransmission];
            wheelTiltTransmission.Active_Style.Cell_Color=ClrMediumOrchid;
            wheelTiltTransmission.UnActive_Style.Cell_Color=ClrMediumOrchid;
            }
        else {
            p_pult->digitalWhellTiltDisable();
            }

        if (digitalData.data.digitalWheelRollTransmission!=0) {
            p_pult->digitalWhellRollEnable();
            wheelRollTransmission.p_text=rollText[(int8_t)digitalData.data.digitalWheelRollTransmission];
            wheelRollTransmission.Active_Style.Cell_Color=ClrMediumOrchid;
            wheelRollTransmission.UnActive_Style.Cell_Color=ClrMediumOrchid;
            }
        else {
            p_pult->digitalWhellRollDisable();
                }
        if (digitalData.data.digitalPedalChannel==1)
            {
            p_pult->enableDigitalRollPedal();
            digitalPedal.p_text=pedalText[1];
            digitalPedal.Active_Style.Cell_Color=ClrMediumOrchid;
            digitalPedal.UnActive_Style.Cell_Color=ClrMediumOrchid;
            }
        else if (digitalData.data.digitalPedalChannel==2)
            {
            p_pult->enableDigitalZoomPedal();
            digitalPedal.p_text=pedalText[2];
            digitalPedal.Active_Style.Cell_Color=ClrMediumOrchid;
            digitalPedal.UnActive_Style.Cell_Color=ClrMediumOrchid;
            }
        else
            {
            p_pult->disableDigitalPedal();
            }
        if (digitalData.data.shakerBoxChannel)
            {
            p_pult->enableDigitalZoomPedal();
            digitalPedal.p_text=pedalText[2];
            digitalPedal.Active_Style.Cell_Color=ClrMediumOrchid;
            digitalPedal.UnActive_Style.Cell_Color=ClrMediumOrchid;
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
            }
        }

    virtual void save() {
        EE_Working::Write(EE_LC_WHEEL_DIGITAL_DATA, digitalData.all);
        }

private:

    void starSetWheel() {
       // startWheelSelect.p_text=(char *)textPanWheelSelect;
        }

    void drawForm() {
        cellPointer.ReDraw();
        cellHeader.ReDraw();
        for (uint8_t i=0; i<encoder.getMaxRisk()+1; i++) {
            cell[i]->ReHide();
            }
        cell[encoder.getActualPosition()]->ReDraw();

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
        if (cell[encoder.getActualPosition()]==&digitalPedal) {
            digitalData.data.digitalPedalChannel--;
            if (digitalData.data.digitalPedalChannel>2)
                digitalData.data.digitalPedalChannel=2;
            digitalPedal.p_text=pedalText[digitalData.data.digitalPedalChannel];
            switch (digitalData.data.digitalPedalChannel)
                {
            case 0:
                digitalPedal.Active_Style.Cell_Color=ClrLinen;
                digitalPedal.UnActive_Style.Cell_Color=ClrLinen;
                p_pult->disableDigitalPedal();
                break;
            case 1:
                digitalPedal.Active_Style.Cell_Color=ClrMediumOrchid;
                digitalPedal.UnActive_Style.Cell_Color=ClrMediumOrchid;
                p_pult->enableDigitalRollPedal();
                break;
            case 2:
                digitalPedal.Active_Style.Cell_Color=ClrMediumOrchid;
                digitalPedal.UnActive_Style.Cell_Color=ClrMediumOrchid;
                p_pult->enableDigitalZoomPedal();
                break;
                }
            digitalPedal.ReDraw();
            return;
            }

        if (cell[encoder.getActualPosition()]==&shakerBox) {
            digitalData.data.shakerBoxChannel--;
            shakerBox.p_text=shakerBoxText[digitalData.data.shakerBoxChannel];
            switch (digitalData.data.shakerBoxChannel)
            {
            case 0:
                shakerBox.Active_Style.Cell_Color=ClrLinen;
                shakerBox.UnActive_Style.Cell_Color=ClrLinen;
                p_pult->disableShakerBox();
                break;
            case 1:
                shakerBox.Active_Style.Cell_Color=ClrMediumOrchid;
                shakerBox.UnActive_Style.Cell_Color=ClrMediumOrchid;
                p_pult->enableShakerBox();
                break;
            }
            shakerBox.ReDraw();
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
        if (cell[encoder.getActualPosition()]==&digitalPedal) {
            digitalData.data.digitalPedalChannel++;
            if (digitalData.data.digitalPedalChannel>2)
                digitalData.data.digitalPedalChannel=0;
            digitalPedal.p_text=pedalText[digitalData.data.digitalPedalChannel];
            switch (digitalData.data.digitalPedalChannel)
                {
            case 0:
                digitalPedal.Active_Style.Cell_Color=ClrLinen;
                digitalPedal.UnActive_Style.Cell_Color=ClrLinen;
                p_pult->disableDigitalPedal();
                break;
            case 1:
                digitalPedal.Active_Style.Cell_Color=ClrMediumOrchid;
                digitalPedal.UnActive_Style.Cell_Color=ClrMediumOrchid;
                p_pult->enableDigitalRollPedal();
                break;
            case 2:
                digitalPedal.Active_Style.Cell_Color=ClrMediumOrchid;
                digitalPedal.UnActive_Style.Cell_Color=ClrMediumOrchid;
                p_pult->enableDigitalZoomPedal();
                break;
                }
            digitalPedal.ReDraw();
            return;
            }

        if (cell[encoder.getActualPosition()]==&shakerBox) {
            digitalData.data.shakerBoxChannel++;
            shakerBox.p_text=shakerBoxText[digitalData.data.shakerBoxChannel];
            switch (digitalData.data.shakerBoxChannel)
            {
            case 0:
                shakerBox.Active_Style.Cell_Color=ClrLinen;
                shakerBox.UnActive_Style.Cell_Color=ClrLinen;
                p_pult->disableShakerBox();
                break;
            case 1:
                shakerBox.Active_Style.Cell_Color=ClrMediumOrchid;
                shakerBox.UnActive_Style.Cell_Color=ClrMediumOrchid;
                p_pult->enableShakerBox();
                break;
            }
            shakerBox.ReDraw();
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
        activeForm->encoder.decrement();
        activeForm->cell[activeForm->encoder.getActualPosition()]->ReDraw();
        }

    void down () {
        activeForm->cell[activeForm->encoder.getActualPosition()]->ReHide();
        activeForm->encoder.increment();
        activeForm->cell[activeForm->encoder.getActualPosition()]->ReDraw();
        }

    void Select() {
        for (int16_t i=0; i<activeForm->encoder.getMaxRisk()+1; i++) {
            activeForm->cell[i]->Active_Style.Border_Color=ClrSkyBlue;
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
