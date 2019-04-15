/*
 * TurnsViewMenu.hpp
 *
 *  Created on: 9 апр. 2019 г.
 *      Author: Cheverev Kirill
 */

#ifndef PULT_INDICATOR_TURNSVIEWMENU_HPP_
#define PULT_INDICATOR_TURNSVIEWMENU_HPP_

#include "LCD Classes.h"
#include "../Pult/PultClasses.h"
#include "stddef.h"
#include "stdint.h"
#include "stdio.h"
#include "../WatchDogTimer/WatchDog.hpp"

namespace LCD {

enum {
    axisCoord=3,
    errorEdit=3,
    statusJoystic,
    statusLevelCorrect,
    statusMotors,
    infotmEdit,
    cellMax
};
enum {
    timeSleep=320
};
enum {
levelCorrectButtonID=7,
motorOnOffButtonID=10,
joyStickOnOffButtonID=11
};

enum {
    virtualJoyStickOnOffButtonID=0
};


const char * emptyString ='\0';
const char * errorString= "ERROR";
const char * sistemOkString="SISTEM OK";
const char * inicializationString="INICIALIZATION";
const char * sistemHaveErrorString="SISTEM HAVE ERROR";
const char * waitJoyStickStatusString="WAIT JOYSTICK STATUS";
const char * waitLevelCorrectStatusString="WAIT LEVEL CORRECT STATUS";
const char * waitMotorsStatusString="WAIT MOTORS STATUS";
const char * motorsOnString = "MOTORS ON";
const char * motorsOffString ="MOTORS OFF";
const char * levelCorrectOnString = "LEVEL CORRECT ON";
const char * levelCorrectOffString = "LEVEL CORRECT OFF";
const char * joyStrickOnString = "JOYSTIC ON";
const char * joyStrickOffString = "JOYSTIC OFF";
const char * joyStickBlockStartString="JOYSTICK BLOCK START";
const char * joyStickBlockOkString="JOYSTICK BLOCK OK";
const char * joyStickBlockString = "JOYSTICK HARD OFF";
const char * GVBlockStartString="LEVEL CORRECT BLOCK START";
const char * GVBlockOkString="LEVEL CORRECT OK";
const char * GVBlockString="LEVEL CORRECT HARD OFF";
const char * motorBlockStartString="MOTORS BLOCK START";
const char * motorBlockOkString="MOTORS BLOCK OK";
const char * motorHardBlockString="MOTORS HARD ON";
const char * activatedString ="ACTIVATED";
const char * interraptString = "ABORTING";
const char * joyStickUnblokStartString = "JOYSTICK UNBLOCK START";
const char * joyStickUnblokOkString = "JOYSTICK UBLOCK OK";
const char * levelCorrectUnblokStartString = "GV UNBLOCK START";
const char * levelCorrectUnblokOkString = "GV UNBLOCK OK";
const char * motorUnblokStartString = "MOTORS UNBLOCK START";
const char * motorUnblokOkString = "MOTORS UNBLOCK OK";
const char * interruptedString = "ABORTED";




class TurnsViewMenu: public LCD_Menu
{

private:
    float * panVal;
    float * tiltVal;
    float * dutchVal;
    char  textBuffer[3][20];
    PultControlBitsLCD controlBits;
    PultButton * joyStickButtonStatus;
    PultButton * levelCorrectButtonStatus;
    PultButton * motorButtonStatus;
    PultButton * virtualJoyStickOnOffButtonStatus;
    UInt16 gyConFaultBits;
    bool init, fastBlocMotor,
    fastBlockJoystic,fastBlockLevelCorrect,
    startInit, abort,startAbort, kalibrate,startKolibrate;
public:
    TurnsViewMenu (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr):
        LCD_Menu(pNam, Link, Count,0, Menu_Per_Scr),init(false),fastBlocMotor(false),
        fastBlockJoystic(false),fastBlockLevelCorrect(false),startInit(false), abort(false), startAbort(false),
        kalibrate(false),startKolibrate(false)
            {
            joyStickButtonStatus=p_pult->getButton_1_16(joyStickOnOffButtonID);
            levelCorrectButtonStatus=p_pult->getButton_1_16(levelCorrectButtonID);
            motorButtonStatus=p_pult->getButton_1_16(motorOnOffButtonID);
            virtualJoyStickOnOffButtonStatus=p_pult->getButtonVirtual(virtualJoyStickOnOffButtonID);
            panVal=p_pult->getPanDriftFactorUk();
            tiltVal=p_pult->getTiltDriftFactorUk();
            dutchVal=p_pult->getDutchDriftFactorUk();
            t_Pos_Size_XY position={  .X=2,  .Y=32,   .Xsize=157,   .Ysize=27     };
            Cell_1.Set_Coord(position);
            Cell_1.p_text=&textBuffer[0][0];
            //-------------------------------
            position.X+=position.Xsize+2;
            Cell_2.Set_Coord(position);
            Cell_2.p_text=&textBuffer[1][0];
            //-------------------------------
            position.Y+=position.Ysize+2;
            position.X=2;
            Cell_3.Set_Coord(position);
            Cell_3.p_text=&textBuffer[2][0];
            //-------------------------------
            position.X+=position.Xsize+2;
            Cell_4.Set_Coord(position);  //используется для отображения ошибок
            Cell_4.p_text=(char*)emptyString;
            //-------------------------------
            position.X=2;
            position.Y+=position.Ysize+2;
            position.Xsize=315;
            Cell_5.Set_Coord(position); // используется для надписи джойстик вкл/откл
            Cell_5.p_text=(char*)emptyString;
            //-------------------------------
            position.Y+=position.Ysize+2;
            Cell_6.Set_Coord(position); //используется для отображения  ГВ вкл/отк
            Cell_6.p_text=(char*)emptyString;
            //-------------------------------
            position.Y+=position.Ysize+2;
            Cell_7.Set_Coord(position); //используется для отображения цветом моторы вкл/отк
            Cell_7.p_text=(char*)emptyString;
            //-------------------------------
            position.Y+=position.Ysize+2;
            position.Ysize=60;
            Cell_8.Set_Coord(position);//информационное поле описывает состояние
            Cell_8.p_text=(char*)emptyString;
            Cell_8.UnActive_Style.pFont=g_psFontCmsc22;
            }

    virtual void Draw(byte Active) {
        DrawHeader();
        Tek_Count = Active;
        DrawVert();
        gyConFaultBits= p_pult->getGyConFaultBits();
        if (gyConFaultBits==0) {
            Table_Cell[errorEdit]->p_text=(char*)sistemOkString;
            Table_Cell[errorEdit]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[infotmEdit]->UnActive_Style.pFont=g_psFontCmsc22;
            Table_Cell[infotmEdit]->p_text=(char*)inicializationString;
            Table_Cell[infotmEdit]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            startInit=true;}
        else {
            Table_Cell[errorEdit]->p_text=(char*)errorString;
            Table_Cell[errorEdit]->UnActive_Style.Cell_Color=ClrLinen;
            Table_Cell[infotmEdit]->p_text=(char*)sistemHaveErrorString;
            Table_Cell[infotmEdit]->UnActive_Style.Cell_Color=ClrLinen;
            startInit=false;}

        Table_Cell[statusJoystic]->p_text=(char*)waitJoyStickStatusString;
        Table_Cell[statusJoystic]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;

        Table_Cell[statusLevelCorrect]->p_text=(char*)waitLevelCorrectStatusString;
        Table_Cell[statusLevelCorrect]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;

        Table_Cell[statusMotors]->p_text=(char*)waitMotorsStatusString;
        Table_Cell[statusMotors]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;


        for (uint8_t i=axisCoord;i<cellMax; i++)
        {  Table_Cell[i]->FastDraw(false);       }
    }

    virtual void DrawVert() {
        sprintf(textBuffer[0],"PAN: %2.6f",     panVal[0]);
        sprintf(textBuffer[1],"DUTCH: %2.6f",   dutchVal[0]);
        sprintf(textBuffer[2],"TILT: %2.6f",    tiltVal[0]);
        //todo логика дрейф стопер

        for(UInt8 i=0;i<axisCoord;i++) { Table_Cell[i]->FastDraw(false);}
    }

    virtual void Listener(){
        if(Focused)
            {
            startInit=true;
            startAbort=false;
            abort=false;
            init=false;
            ClearDisp();
            Draw(Tek_Count);
            Focused = false;
            }
        if (getButtonState(pult_Button_Select) == PRESSED) {
            if (abort) {
                abort=false;
                startInit=true;
                Table_Cell[infotmEdit]->UnActive_Style.pFont=g_psFontCmsc22;
                Table_Cell[infotmEdit]->p_text=(char*)inicializationString;
                Table_Cell[infotmEdit]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
                Table_Cell[infotmEdit]->FastDraw(false);
                }
            }
        if (getButtonState(pult_Button_ESC) == PRESSED)// здесь добавить drift stoper off
            {
            if ((!abort)&&(!startAbort)) {
                startAbort=true;
                return;
                }
            exit();
            return;
            }
        if (getButtonState(pult_Button_ESC) == HOLD)        {
            exit();
            return;
        }

        if (startInit) {
            initLogic();
            return;
            }


        if (startAbort) {
            abortLogic();
            return;
            }

        if (abort) {
            abortWork();
            }




        DrawVert();

        }

private:

    void exit() {
        joyStickButtonStatus->hardRelesased=false;
        virtualJoyStickOnOffButtonStatus->state=RELESASED;
        levelCorrectButtonStatus->hardRelesased=false;
        motorButtonStatus->hardPressed=false;
        init=false;
        startInit=false;
        abort=false;
        startAbort=false;
        p_pult->setDriftStopperMode(false);
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        }

    inline void abortWork() {
        if (joyStickButtonStatus->state==RELESASED) {
            Table_Cell[statusJoystic]->p_text=(char*)joyStrickOffString;
            Table_Cell[statusJoystic]->UnActive_Style.Cell_Color=ClrLinen;
            Table_Cell[statusJoystic]->FastDraw(false);
            }
        else {
            Table_Cell[statusJoystic]->p_text=(char*)joyStrickOnString;
            Table_Cell[statusJoystic]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[statusJoystic]->FastDraw(false);
            }
        if (levelCorrectButtonStatus->state==RELESASED) {
            Table_Cell[statusLevelCorrect]->p_text=(char*)levelCorrectOffString;
            Table_Cell[statusLevelCorrect]->UnActive_Style.Cell_Color=ClrLinen;
            Table_Cell[statusLevelCorrect]->FastDraw(false);
            }
        else {
            Table_Cell[statusLevelCorrect]->p_text=(char*)levelCorrectOnString;
            Table_Cell[statusLevelCorrect]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[statusLevelCorrect]->FastDraw(false);
            }
        if (motorButtonStatus->state==RELESASED) {
            Table_Cell[statusMotors]->p_text=(char*)motorsOffString;
            Table_Cell[statusMotors]->UnActive_Style.Cell_Color=ClrLinen;
            Table_Cell[statusMotors]->FastDraw(false);
            }
        else {
            Table_Cell[statusMotors]->p_text=(char*)motorsOnString;
            Table_Cell[statusMotors]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[statusMotors]->FastDraw(false);
            }
        }

    inline void initLogic() {
        if(!joyStickButtonStatus->hardRelesased) {
            Table_Cell[statusJoystic]->p_text=(char*)joyStickBlockStartString;
            Table_Cell[statusJoystic]->FastDraw(false);
            joyStickButtonStatus->hardRelesased=true;
            virtualJoyStickOnOffButtonStatus->state=PRESSED;
            Task_sleep(timeSleep);

            Table_Cell[statusJoystic]->p_text=(char*)joyStickBlockOkString;
            Table_Cell[statusJoystic]->FastDraw(false);
            Task_sleep(timeSleep);

            Table_Cell[statusJoystic]->p_text=(char*)joyStickBlockString;
            Table_Cell[statusJoystic]->UnActive_Style.Cell_Color=ClrLinen;
            Table_Cell[statusJoystic]->FastDraw(false);
            return;
            }
        if (!levelCorrectButtonStatus->hardRelesased) {
            Table_Cell[statusLevelCorrect]->p_text=(char*)GVBlockStartString;
            Table_Cell[statusLevelCorrect]->FastDraw(false);
            levelCorrectButtonStatus->hardRelesased=true;
            Task_sleep(statusLevelCorrect);

            Table_Cell[statusLevelCorrect]->p_text=(char*)GVBlockOkString;
            Table_Cell[statusLevelCorrect]->FastDraw(false);
            Task_sleep(timeSleep);

            Table_Cell[statusLevelCorrect]->p_text=(char*)GVBlockString;
            Table_Cell[statusLevelCorrect]->UnActive_Style.Cell_Color=ClrLinen;
            Table_Cell[statusLevelCorrect]->FastDraw(false);
            return;
            }
        if (!motorButtonStatus->hardPressed) {
            Table_Cell[statusMotors]->p_text=(char*)motorBlockStartString;
            Table_Cell[statusMotors]->FastDraw(false);
            motorButtonStatus->hardPressed=true;
            Task_sleep(timeSleep);
            Table_Cell[statusMotors]->p_text=(char*)motorBlockOkString;
            Table_Cell[statusMotors]->FastDraw(false);
            Task_sleep(timeSleep);
            Table_Cell[statusMotors]->p_text=(char*)motorHardBlockString;
            Table_Cell[statusMotors]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[statusMotors]->FastDraw(false);
            return;
            }
        if (!init) {
            Table_Cell[infotmEdit]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Cell_8.UnActive_Style.pFont=g_psFontCmsc22;
            Table_Cell[infotmEdit]->p_text=(char*)activatedString;
            p_pult->setDriftStopperMode(true);
            Table_Cell[infotmEdit]->FastDraw(false);
            init=true;
            return;
            }
        startInit=false;
        }

    inline void abortLogic () {
        if (init) {
            Table_Cell[infotmEdit]->UnActive_Style.Cell_Color=ClrLinen;
            Cell_8.UnActive_Style.pFont=g_psFontCmsc22;
            Table_Cell[infotmEdit]->p_text=(char*)interraptString;
            p_pult->setDriftStopperMode(false);
            Table_Cell[infotmEdit]->FastDraw(false);
            init=false;
            Task_sleep(timeSleep);
            return;
            }
        if (joyStickButtonStatus->hardRelesased) {
            Table_Cell[statusJoystic]->p_text=(char*)joyStickUnblokStartString;
            Table_Cell[statusJoystic]->FastDraw(false);
            joyStickButtonStatus->hardRelesased=false;
            virtualJoyStickOnOffButtonStatus->state=RELESASED;
            Task_sleep(timeSleep);

            Table_Cell[statusJoystic]->p_text=(char*)joyStickUnblokOkString;
            Table_Cell[statusJoystic]->FastDraw(false);
            Task_sleep(timeSleep);

            Table_Cell[statusJoystic]->p_text=(char*)waitJoyStickStatusString;
            Table_Cell[statusJoystic]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[statusJoystic]->FastDraw(false);
            return;
            }
        if (levelCorrectButtonStatus->hardRelesased) {
            Table_Cell[statusLevelCorrect]->p_text=(char*)levelCorrectUnblokStartString;
            Table_Cell[statusLevelCorrect]->FastDraw(false);
            levelCorrectButtonStatus->hardRelesased=false;
            Task_sleep(timeSleep);

            Table_Cell[statusLevelCorrect]->p_text=(char*)levelCorrectUnblokOkString;
            Table_Cell[statusLevelCorrect]->FastDraw(false);
            Task_sleep(timeSleep);

            Table_Cell[statusLevelCorrect]->p_text=(char*)waitLevelCorrectStatusString;
            Table_Cell[statusLevelCorrect]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[statusLevelCorrect]->FastDraw(false);
            return;
            }
        if (motorButtonStatus->hardPressed) {
            Table_Cell[statusMotors]->p_text=(char*)motorUnblokStartString;
            Table_Cell[statusMotors]->FastDraw(false);
            motorButtonStatus->hardPressed=false;
            Task_sleep(timeSleep);

            Table_Cell[statusMotors]->p_text=(char*)motorUnblokOkString;
            Table_Cell[statusMotors]->FastDraw(false);
            Task_sleep(timeSleep);

            Table_Cell[statusMotors]->p_text=(char*)waitMotorsStatusString;
            Table_Cell[statusMotors]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[statusMotors]->FastDraw(false);
            return;
            }
        Table_Cell[infotmEdit]->p_text=(char*)interruptedString;
        Table_Cell[infotmEdit]->FastDraw(false);
        abort=true;
        startAbort=false;
        }
    };
}

#endif /* PULT_INDICATOR_TURNSVIEWMENU_HPP_ */
