/*
 * LCDPultTest.hpp
 *
 *  Created on: 1 июл. 2019 г.
 *      Author: Kirill
 */

#ifndef PULT_INDICATOR_LCDPULTTEST_HPP_
#define PULT_INDICATOR_LCDPULTTEST_HPP_




#include "LCD Classes.h"
#include "../Pult/PultClasses.h"
#include "Libs/StandartElement/galetniy Perecluxhatel.hpp"
#include "Libs/StandartElement/encoder.hpp"
#include "LCDBase.hpp"

namespace LCD {

const char* menuName = "ZIF REVERS MENU";

LCDHeaderSettings settingsHeader = {
                            .visibleHeader=true,
                            .visiblePointerLeft=false,
                            .visiblePointerRight=false,
                            .visiblePointerUp=false,
                            .visiblePointerDown=false,
                            .textHeader= menuName
    };

const char * startStringTest[] = {"Начать тест кнопок",
                                  "Начать тест потенометров"};

const char * stringButtonTest[] = {   "Тумблеры в положене ОТКЛ",               //1
                                      "Нажмите клавишу ОК",                     //2
                                      "Нажмите клавишу Left",                   //3
                                      "Нажмите клавишу Right",                  //4
                                      "Нажмите клавишу Up",                     //5
                                      "Нажмите клавишу Down",                   //6
                                      "Нажмите клавишу Play/Pause",             //7
                                      "Нажмите клавишу Stop",                   //8
                                      "Нажмите клавишу Delete",                 //9
                                      "Нажмите клавишу Revers play",            //10
                                      "Нажмите клавишу ESC",                    //11
                                      "Нажмите клавишу CAMERA START",           //12
                                      "Нажмите клавишу PK",                     //13
                                      "Нажмите клавишу PI",                     //14
            #ifndef USAEdition
                                      "Нажмите клавишу DK",                     //15
                                      "Нажмите клавишу DI",                     //16
            #else
                                      "Нажмите клавишу RK",                     //15
                                      "Нажмите клавишу RI",                     //16
            #endif
                                      "Нажмите клавишу TK",                     //17
                                      "Нажмите клавишу TI",                     //18
                                      "Нажмите клавишу Pan Sens",               //19
            #ifndef USAEdition
                                      "Нажмите клавишу Dutch Sens",             //20
            #else
                                      "Нажмите клавишу Roll Sens",              //20
            #endif
                                      "Нажмите клавишу Tilt Sens",              //21
                                      "Нажмите клавишу GV Calibration",         //22
            #ifdef Garanin
                                      "Нажмите клавишу Fast Level Correct",     //23
            #else
                                      "Нажмите клавишу Shaker",                 //23
            #endif
                                      "Нажмите клавишу GV/ACC",                 //24
                                      "Тумблер PAN REVERS в ON",                //25
            #ifndef USAEdition
                                      "Тумблер DUTCH REVERS в ON",              //26
            #else
                                      "Тумблер ROLL REVERS в ON",               //26
            #endif
                                      "Тумблер TILT REVERS в ON",               //27
                                      "Тумблер MOTORS в ON",                    //28
                                      "Тумблер JOYSTICK в ON",                  //29
                                      "Тумблер LEVEL SETUP в ON",               //30
                                      "Тумблер Backlight в ON"                  //31
                                        };

const char * stringResistorsTest[] = {   "PAN SPEED in MIN",            //1
                                         "PAN SPEED in MAX",            //2
                                         "PAN FLUID in MIN",            //3
                                         "PAN FLUID in MAX",            //4
                                         "PAN DRIFT in MIN",            //5
                                         "PAN DRIFT in MAX",            //6
            #ifndef USAEdition
                                         "DUTCH SPEED in MIN",          //7
                                         "DUTCH SPEED in MAX",          //8
                                         "DUTCH FLUID in MIN",          //9
                                         "DUTCH FLUID in MAX",          //10
                                         "DUTCH DRIFT in MIN",          //11
                                         "DUTCH DRIFT in MAX",          //12
            #else
                                         "ROLL SPEED in MIN",           //7
                                         "ROLL SPEED in MAX",           //8
                                         "ROLL FLUID in MIN",           //9
                                         "ROLL FLUID in MAX",           //10
                                         "ROLL DRIFT in MIN",           //11
                                         "ROLL DRIFT in MAX",           //12
            #endif
                                         "TILT SPEED in MIN",           //13
                                         "TILT SPEED in MAX",           //14
                                         "TILT FLUID in MIN",           //15
                                         "TILT FLUID in MAX",           //16
                                         "TILT DRIFT in MIN",           //17
                                         "TILT DRIFT in MAX",           //18
                                         "ZOOM SPEED in MIN",           //19
                                         "ZOOM SPEED in MAX",           //20
                                         "SPEED LEVEL CORRECT in MIN",  //21
                                         "SPEED LEVEL CORRECT in MAX",  //22
                                         "IRIS in MIN",                 //23
                                         "IRIS in MAX",                 //24
                                         "FOCUS in MIN",                //25
                                         "FOCUS in MAX",                //26
                                         "PAN JOY MAX LEFT",            //27
                                         "PAN JOY MAX RIGHT",           //28
                                         "TILT JOY MAX BACKWARD",       //29
                                         "TILT JOY MAX FORWARD",        //30
#ifndef USAEdition
                                         "DUTCH JOY MAX LEFT",          //31
                                         "DUTCH JOY MAX RIGHT",         //32
#else
                                         "ROLL JOY MAX LEFT",           //31
                                         "ROLL JOY MAX RIGHT",          //32
#endif
                                         "ZOOM MAX BACKWARD",           //33
                                         "ZOOM MAX FORWARD"             //34
                                        };

const char * korrectData[]   = { "Данные корректны",
                                 "Данные некорректны"};

    LCDWorkcEditSettings startStringSettings = {
        .text=&startStringTest[0],
        .maxRisk=2-1
        };

    LCDWorkcEditSettings stringButtonTest = {
        .text=&stringButtonTest[0],
        .maxRisk=2-1
        };

    LCDWorkcEditSettings stringResistorsTest = {
        .text=&stringButtonTest[0],
        .maxRisk=2-1
        };

    LCDWorkcEditSettings korrectData = {
        .text=&stringButtonTest[0],
        .maxRisk=2-1
        };


enum {
    cellNumber=3
    }
class PultScrineTest {

private:
    LCDBaseNoRotation<cellNumber> form;

public:

    PultScrineTest() {

        }

private:

    inline void buttonTest () {
        
        }
        
    inline void potenciometrTest () {
            
        }
    
    
    };
}



#endif /* PULT_INDICATOR_LCDPULTTEST_HPP_ */
