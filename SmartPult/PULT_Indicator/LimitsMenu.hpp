/*
 * LimitsMenu.hpp
 *
 *  Created on: 6 сент. 2019 г.
 *      Author: Кирилл Чеверев
 */

#ifndef PULT_INDICATOR_LIMITSMENU_HPP_
#define PULT_INDICATOR_LIMITSMENU_HPP_

#include "LCD Classes.h"
#include "LCDBase.hpp"
#include <locale.h>

namespace LCD {
    namespace Limits {

    LCDHeaderSettings userInfoMenuHeaderSetting = {
                                                   .visibleHeader=true,
                                                   .textHeader="SET LIMITS MENU",
                                                   .visiblePointerDown=false,
                                                   .visiblePointerLeft=false,
                                                   .visiblePointerRight=false,
                                                   .visiblePointerUp=false};

    char  textStaticMenu[16][50] = {           "SET PAN LEFT LIMIT",
                                               "RESET PAN LEFT LIMIT",
                                               "SET PAN RIGHT LIMIT",
                                               "RESET PAN RIGHT LIMIT",
                                               "RESET ALL PAN LIMIT",

    #ifdef USAEdition
                                               "SET TILT UPPER LIMIT",
                                               "RESET TILT UPPER LIMIT",
                                               "SET TILT LOWER LIMIT",
                                               "RESET TILT LOWER LIMIT",
                                               "RESET ALL TILT LIMIT",

                                               "SET ROLL LEFT LIMIT",
                                               "RESET ROLL LEFT LIMIT",
                                               "SET ROLL RIGHT LIMIT",
                                               "RESET ROLL RIGHT LIMIT",
                                               "RESET ALL ROLL LIMIT",
    #else
                                               "SET DUTCH LEFT LIMIT",
                                               "RESET DUTCH LEFT LIMIT",
                                               "SET DUTCH RIGHT LIMIT",
                                               "RESET DUTCH RIGHT LIMIT",
                                               "RESET ALL DUTCH LIMIT",

                                               "SET TILT UPPER LIMIT",
                                               "RESET TILT UPPER LIMIT",
                                               "SET TILT LOWER LIMIT",
                                               "RESET TILT LOWER LIMIT",
                                               "RESET ALL TILT LIMIT",
    #endif
                                               "RESET ALL LIMIT"};

    char * textStatic[16] = {&textStaticMenu[0][0],
                                    &textStaticMenu[1][0],
                                    &textStaticMenu[2][0],
                                    &textStaticMenu[3][0],
                                    &textStaticMenu[4][0],
                                    &textStaticMenu[5][0],
                                    &textStaticMenu[6][0],
                                    &textStaticMenu[7][0],
                                    &textStaticMenu[8][0],
                                    &textStaticMenu[9][0],
                                    &textStaticMenu[10][0],
                                    &textStaticMenu[11][0],
                                    &textStaticMenu[12][0],
                                    &textStaticMenu[13][0],
                                    &textStaticMenu[14][0],
                                    &textStaticMenu[15][0]};

    LCDWorkcEditSettings editStringText1 = {
                                        .maxRisk=2,
                                        .text=&textStatic[0]
    };

    LCDWorkcEditSettings editStringText2 = {
                                        .maxRisk=2,
                                        .text=&textStatic[2]
    };

    LCDWorkcEditSettings editStringText3 = {
                                        .maxRisk=1,
                                        .text=&textStatic[4]
    };

    LCDWorkcEditSettings editStringText4 = {
                                        .maxRisk=2,
                                        .text=&textStatic[5]
    };

    LCDWorkcEditSettings editStringText5 = {
                                        .maxRisk=2,
                                        .text=&textStatic[7]
    };

    LCDWorkcEditSettings editStringText6 = {
                                        .maxRisk=1,
                                        .text=&textStatic[9]
    };

    LCDWorkcEditSettings editStringText7 = {
                                        .maxRisk=2,
                                        .text=&textStatic[10]
    };

    LCDWorkcEditSettings editStringText8 = {
                                            .maxRisk=2,
                                            .text=&textStatic[12]
    };

    LCDWorkcEditSettings editStringText9 = {
                                            .maxRisk=1,
                                            .text=&textStatic[14]
    };

    LCDWorkcEditSettings editStringText10 = {
                                            .maxRisk=1,
                                            .text=&textStatic[15]
    };

    LCDWorkcEditSettings * editStrings[10] = {&editStringText1,
                                            &editStringText2,
                                            &editStringText3,
                                            &editStringText4,
                                            &editStringText5,
                                            &editStringText6,
                                            &editStringText7,
                                            &editStringText8,
                                            &editStringText9,
                                            &editStringText10};

        class FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {          }
        };

        class SetPanLeftLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->setLeftPanLimit();
                mainMenu->panLimiterStateFlag+=TL_DOWN;}
        };

        class SetPanRightLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->setRightPanLimit();
                mainMenu->panLimiterStateFlag+=TL_UP;}
        };

        class ResetPanLeftLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->resetLeftPanLimit();
                mainMenu->panLimiterStateFlag-=TL_DOWN;}
        };

        class ResetPanRightLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->resetRightPanLimit();
                mainMenu->panLimiterStateFlag-=TL_UP;}
        };

        class ResetPanLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->resetLeftPanLimit();
                p_pult->resetRightPanLimit();
                mainMenu->panLimiterStateFlag=TL_RESET;}
        };

        class SetRollLeftLimit: public FunctionBase {
        public:

        virtual void function(LCD_Main * mainMenu) {
            p_pult->setLeftRollLimit();
            mainMenu->rollLimiterStateFlag+=TL_UP;}
        };

        class SetRollRightLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->setRightRollLimit();
                mainMenu->rollLimiterStateFlag+=TL_DOWN;}
        };

        class ResetRollLeftLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->resetLeftRollLimit();
                mainMenu->rollLimiterStateFlag-=TL_UP;}
        };

        class ResetRollRightLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->resetRightRollLimit();
                mainMenu->rollLimiterStateFlag-=TL_DOWN;}
        };

        class ResetRollLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->resetLeftRollLimit();
                p_pult->resetRightRollLimit();
                mainMenu->rollLimiterStateFlag=TL_RESET;}
        };

        class SetUpperTiltLimit: public FunctionBase {
        public:

        virtual void function(LCD_Main * mainMenu) {
            p_pult->setTiltUpLimit();
            mainMenu->tiltLimiterStateFlag+=TL_UP;}
        };

        class SetLowerTiltLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->setTiltDnLimit();
                mainMenu->tiltLimiterStateFlag+=TL_DOWN;}
        };

        class ResetUpperTiltLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->resetTiltUpLimit();
                mainMenu->tiltLimiterStateFlag-=TL_UP;}
        };

        class ResetLowerTiltLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->resetTiltDnLimit();
                mainMenu->tiltLimiterStateFlag-=TL_DOWN;}
        };

        class ResetTiltLimit: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->resetTiltUpLimit();
                p_pult->resetTiltDnLimit();
                mainMenu->tiltLimiterStateFlag=TL_RESET;}
        };

        class ResetLimits: public FunctionBase {
        public:

            virtual void function(LCD_Main * mainMenu) {
                p_pult->resetLeftPanLimit();
                p_pult->resetRightPanLimit();
                p_pult->resetLeftRollLimit();
                p_pult->resetRightRollLimit();
                p_pult->resetTiltUpLimit();
                p_pult->resetTiltDnLimit();
                mainMenu->panLimiterStateFlag = TL_RESET;
                mainMenu->rollLimiterStateFlag=TL_RESET;
                mainMenu->tiltLimiterStateFlag=TL_RESET;}
        };

        enum {numberCell = 10, setColor};
        class EditWorckBase {

            protected:
            uint32_t color[setColor];



            public:

            EditWorckBase() {
                color[0]=ClrMediumOrchid;
                color[1]=ClrDarkSlateBlue;
                }

            virtual void function(LCDStatic & form) {

            }
        };


        class EditWorckNormal: public EditWorckBase {

        public:

            EditWorckNormal():EditWorckBase() {

            }

            virtual void function(LCDStatic & form) {
                form.stepRight(false);
                form.setBackGroundColor(color[form.getCellEncoderPosition()],form.encoderVertical.getActualPosition());
                form.drawCell(form.encoderVertical.getActualPosition());
            }
        };

        class EditWorckResetAllAxisLimit: public EditWorckBase {

        public:

            EditWorckResetAllAxisLimit():EditWorckBase() {

            }

            virtual void function(LCDStatic &  form) {
                if (form.getCellEncoderPosition(2)!=0) {
                    form.stepLeft(false,2);
                    form.setBackGroundColor(color[0],form.encoderVertical.getActualPosition()-2);
                    form.drawCell(form.encoderVertical.getActualPosition()-2,false);}
                if (form.getCellEncoderPosition(1)!=0) {
                    form.stepLeft(false,1);
                    form.setBackGroundColor(color[0],form.encoderVertical.getActualPosition()-1);
                    form.drawCell(form.encoderVertical.getActualPosition()-1,false);}
            }
        };

        class EditWorckResetAllHeadlLimit: public EditWorckBase {

        public:

            EditWorckResetAllHeadlLimit():EditWorckBase() {

            }

            virtual void function(LCDStatic & form) {
                if (form.getCellEncoderPosition(9)!=0) {
                    form.stepLeft(false,9);
                    form.setBackGroundColor(color[0],form.encoderVertical.getActualPosition()-9);
                    form.drawCell(form.encoderVertical.getActualPosition()-9,false);}
                if (form.getCellEncoderPosition(8)!=0) {
                    form.stepLeft(false,8);
                    form.setBackGroundColor(color[0],form.encoderVertical.getActualPosition()-8);
                    form.drawCell(form.encoderVertical.getActualPosition()-8,false);}
                if (form.getCellEncoderPosition(6)!=0) {
                    form.stepLeft(false,6);
                    form.setBackGroundColor(color[0],form.encoderVertical.getActualPosition()-6);
                    form.drawCell(form.encoderVertical.getActualPosition()-6,false);}
                if (form.getCellEncoderPosition(5)!=0) {
                    form.stepLeft(false,5);
                    form.setBackGroundColor(color[0],form.encoderVertical.getActualPosition()-5);
                    form.drawCell(form.encoderVertical.getActualPosition()-5,false);}
                if (form.getCellEncoderPosition(3)!=0) {
                    form.stepLeft(false,3);
                    form.setBackGroundColor(color[0],form.encoderVertical.getActualPosition()-3);
                    form.drawCell(form.encoderVertical.getActualPosition()-3,false);}
                if (form.getCellEncoderPosition(2)!=0) {
                    form.stepLeft(false,2);
                    form.setBackGroundColor(color[0],form.encoderVertical.getActualPosition()-2);
                    form.drawCell(form.encoderVertical.getActualPosition()-2,false);}
            }
        };

        class LimitMenu:public LCD_Listener {

        private:

            enum { stepEdit=1, setColor=2};

            #define colorLimitNotActive     ClrYellow
            #define colorLimitActive        ClrDarkBlue

            FunctionBase * functionHead[16];
            FunctionBase ** functionsHead[10];
            EditWorckBase * functionEdit[16];
            EditWorckBase ** functionsEdit[10];
            LCDBaseNoRotation<numberCell> form;
            uint32_t color[setColor];
            LCD_Main ** mainMenu;

            SetPanLeftLimit     setPanLeftLimit;
            SetPanRightLimit    setPanRightLimit;
            ResetPanLeftLimit   resetPanLeftLimit;
            ResetPanRightLimit  resetPanRightLimit;
            ResetPanLimit       resetPanLimit;
            SetRollLeftLimit    setRollLeftLimit;
            SetRollRightLimit   setRollRightLimit;
            ResetRollLeftLimit  resetRollLeftLimit;
            ResetRollRightLimit resetRollRightLimit;
            ResetRollLimit      resetRollLimit;
            SetUpperTiltLimit   setUpperTiltLimit;
            SetLowerTiltLimit   setLowerTiltLimit;
            ResetUpperTiltLimit resetUpperTiltLimit;
            ResetLowerTiltLimit resetLowerTiltLimit;
            ResetTiltLimit      resetTiltLimit;
            ResetLimits         resetLimits;

            EditWorckNormal normalWorckEdit;
            EditWorckResetAllAxisLimit editResetAxisLimt;
            EditWorckResetAllHeadlLimit editResetLimt;


        public:

            LimitMenu(LCD_Main ** mainMenu):form(userInfoMenuHeaderSetting,&editStrings[0],stepEdit), mainMenu(mainMenu){
                color[0]=ClrMediumOrchid;
                color[1]=ClrDarkSlateBlue;
                for (uint8_t i=0; i<numberCell; i++){
                form.setBackGroundColor(color[0],i);}
                for (uint8_t i=0; i <16;i++) {
                    functionEdit[i]=&normalWorckEdit;}

                functionHead[0]=&setPanRightLimit;
                functionHead[1]=&resetPanRightLimit;
                functionHead[2]=&setPanLeftLimit;
                functionHead[3]=&resetPanLeftLimit;
                functionHead[4]=&resetPanLimit;

#ifdef USAEdition
                functionHead[5]=&setUpperTiltLimit;
                functionHead[6]=&resetUpperTiltLimit;
                functionHead[7]=&setLowerTiltLimit;
                functionHead[8]=&resetLowerTiltLimit;
                functionHead[9]=&resetTiltLimit;
                functionHead[10]=&setRollLeftLimit;
                functionHead[11]=&resetRollLeftLimit;
                functionHead[12]=&setRollRightLimit;
                functionHead[13]=&resetRollRightLimit;
                functionHead[14]=&resetRollLimit;
#else
                functionHead[5]=&setRollLeftLimit;
                functionHead[6]=&resetRollLeftLimit;
                functionHead[7]=&setRollRightLimit;
                functionHead[8]=&resetRollRightLimit;
                functionHead[9]=&resetRollLimit;
                functionHead[10]=&setUpperTiltLimit;
                functionHead[11]=&resetUpperTiltLimit;
                functionHead[12]=&setLowerTiltLimit;
                functionHead[13]=&resetLowerTiltLimit;
                functionHead[14]=&resetTiltLimit;
#endif
                functionEdit[4]=&editResetAxisLimt;
                functionEdit[9]=&editResetAxisLimt;
                functionEdit[14]=&editResetAxisLimt;
                functionEdit[15]=&editResetLimt;
                functionHead[15]=&resetLimits;

                functionsHead[0]=&functionHead[0];
                functionsHead[1]=&functionHead[2];
                functionsHead[2]=&functionHead[4];
                functionsHead[3]=&functionHead[5];
                functionsHead[4]=&functionHead[7];
                functionsHead[5]=&functionHead[9];
                functionsHead[6]=&functionHead[10];
                functionsHead[7]=&functionHead[12];
                functionsHead[8]=&functionHead[14];
                functionsHead[9]=&functionHead[15];

                functionsEdit[0]=&functionEdit[0];
                functionsEdit[1]=&functionEdit[2];
                functionsEdit[2]=&functionEdit[4];
                functionsEdit[3]=&functionEdit[5];
                functionsEdit[4]=&functionEdit[7];
                functionsEdit[5]=&functionEdit[9];
                functionsEdit[6]=&functionEdit[10];
                functionsEdit[7]=&functionEdit[12];
                functionsEdit[8]=&functionEdit[14];
                functionsEdit[9]=&functionEdit[15];

                }

            virtual void Listener() {
                if (this->Focused) {
                    ClearDisp();
                    form.drawForm();
                    Focused=false;
                }

                if (getButtonState(pult_Button_ESC) == PRESSED)        {    exit();      }

                if (getButtonState(pult_Button_Up) == PRESSED)        {    form.stepUp();      }

                if (getButtonState(pult_Button_Dn) == PRESSED)        {    form.stepDown();      }

                if (getButtonState(pult_Button_Select) == PRESSED)        {
                    functionsHead[form.encoderVertical.getActualPosition()][form.getCellEncoderPosition()]->function(mainMenu[0]);
                    functionsEdit[form.encoderVertical.getActualPosition()][form.getCellEncoderPosition()]->function(form);
                    }
                }

        private:
            void exit() {
                    pDispl = pDispl->Parent;
                    pDispl->Focused = true;
                }

        };
    }
}



#endif /* PULT_INDICATOR_LIMITSMENU_HPP_ */
