/*
 * UserInfoMenu.hpp
 *
 *  Created on: 2 сент. 2019 г.
 *      Author: Кирилл Чеверев
 */

#ifndef PULT_INDICATOR_USERINFOMENU_HPP_
#define PULT_INDICATOR_USERINFOMENU_HPP_





#include "LCD Classes.h"
#include "LCDBase.hpp"

namespace LCD {

namespace InfoMenu {

union versionProgramUnion {
    uint32_t all;
    struct {
        uint16_t low        : 16;
        uint8_t central     : 8;
        uint8_t high        : 8;
    }patch;
};

union DataIzgotovlenia {
    uint32_t all;
    struct {
        uint8_t day  :8;
        uint8_t month :8;
        uint16_t yers :16;
    }data;
};

struct ParametrStruct {
    versionProgramUnion numberVersion;
    char simbol;
    DataIzgotovlenia data;
    uint32_t factoryNumber;
    uint32_t runTime;
    };

LCDHeaderSettings userInfoMenuHeaderSetting = {
                                               .visibleHeader=true,
                                               .textHeader="USER INFO MENU",
                                               .visiblePointerDown=false,
                                               .visiblePointerLeft=false,
                                               .visiblePointerRight=false,
                                               .visiblePointerUp=false};

char  textStaticUserInfoMenu[20][50] = { //  "Pult info ",
                                           "Run time: ",

#ifdef joyPult
#ifdef USAEdition
                                           "Version: v1.03.03USA",
#else
                                           "Pult Version: v1.03.03EU",
#endif

#ifdef Garanin
                                           "Version: v1.03.03PBW",
#endif
#endif

#ifdef WhellSmartPult
                                           "Version: v1.03.03WP",
#endif
//                                           "Prod. date:",
//                                           "Factory № ",
//                                           "Smart Head Info",
//                                           "Run time:",
//                                           "Production date:",
                                           "Smart Version: ",
                                           "Factory № ",
                                           "FIZ Box Info :",
                                           "Run time:",
                                           "Production date:",
                                           "Version",
                                           "Factory № ",
                                           "Radio Chanal Info:",
                                           "Run time:",
                                           "Production date:",
                                           "Version",
                                           "Factory № ",};


char * textStaticUserInfo[20] = {&textStaticUserInfoMenu[0][0],
                                &textStaticUserInfoMenu[1][0],
                                &textStaticUserInfoMenu[2][0],
                                &textStaticUserInfoMenu[3][0],
                                &textStaticUserInfoMenu[4][0],
                                &textStaticUserInfoMenu[5][0],
                                &textStaticUserInfoMenu[6][0],
                                &textStaticUserInfoMenu[7][0],
                                &textStaticUserInfoMenu[8][0],
                                &textStaticUserInfoMenu[9][0],
                                &textStaticUserInfoMenu[10][0],
                                &textStaticUserInfoMenu[11][0],
                                &textStaticUserInfoMenu[12][0],
                                &textStaticUserInfoMenu[13][0],
                                &textStaticUserInfoMenu[14][0],
                                &textStaticUserInfoMenu[15][0],
                                &textStaticUserInfoMenu[16][0],
                                &textStaticUserInfoMenu[17][0],
                                &textStaticUserInfoMenu[18][0],
                                &textStaticUserInfoMenu[19][0]};

LCDWorkcEditSettings editStringText1 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[0]
};

LCDWorkcEditSettings editStringText2 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[1]
};

LCDWorkcEditSettings editStringText3 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[2]
};

LCDWorkcEditSettings editStringText4 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[3]
};

LCDWorkcEditSettings editStringText5 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[4]
};

LCDWorkcEditSettings editStringText6 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[5]
};

LCDWorkcEditSettings editStringText7 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[6]
};

LCDWorkcEditSettings editStringText8 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[7]
};

LCDWorkcEditSettings editStringText9 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[8]
};

LCDWorkcEditSettings editStringText10 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[9]
};

LCDWorkcEditSettings editStringText11 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[10]
};

LCDWorkcEditSettings editStringText12 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[11]
};

LCDWorkcEditSettings editStringText13 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[12]
};

LCDWorkcEditSettings editStringText14 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[13]
};

LCDWorkcEditSettings editStringText15 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[14]
};

LCDWorkcEditSettings editStringText16 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[15]
};

LCDWorkcEditSettings editStringText17 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[16]
};

LCDWorkcEditSettings editStringText18 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[17]
};

LCDWorkcEditSettings editStringText19 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[18]
};

LCDWorkcEditSettings editStringText20 = {
                                    .maxRisk=1,
                                    .text=&textStaticUserInfo[19]
};


LCDWorkcEditSettings * editString[20] = {&editStringText1,
                                        &editStringText2,
                                        &editStringText3,
                                        &editStringText4,
                                        &editStringText5,
                                        &editStringText6,
                                        &editStringText7,
                                        &editStringText8,
                                        &editStringText9,
                                        &editStringText10,
                                        &editStringText11,
                                        &editStringText12,
                                        &editStringText13,
                                        &editStringText14,
                                        &editStringText15,
                                        &editStringText16,
                                        &editStringText17,
                                        &editStringText18,
                                        &editStringText19,
                                        &editStringText20};

struct RunTime {
    uint32_t hour;
    char simbolFerst;
    uint8_t min;
    char simbolSecond;
    uint8_t second;
};

//#define WriteData

enum {PultRunTime=0, PultVersion, HeadVersion};

class UserInfoMenu:public LCD_Listener {

private:
    enum {numberCell = 3, ysize=15};

    LCDBaseNoRotation<numberCell> form;
    ParametrStruct smartVersion;
    ParametrStruct FIZVersion;
    ParametrStruct radioChannal;
    uint32_t timeOldInput;
    uint32_t runTime;
    uint32_t runTimeOld;
    RunTime clock;
    uint32_t timeToSave;

public:

    UserInfoMenu(): form(userInfoMenuHeaderSetting,&editString[0]) {
        runTime=EE_Working::ReadCalibrationData(EE_CAL_HOURS);
        form.setFontSizeAll(g_psFontCmsc18);
#ifdef WriteData
        DataIzgotovlenia data = { .data.day = 25, .data.month = 12, .data.yers = 2025 };
        EE_Working::WriteCalibrationData(EE_CAL_DATA_PRODUCTION, data.all);
        sprintf(&editString[3]->text[0][11],"%1d :%1d :%1d",data.data.day,data.data.month,data.data.yers);
#else
/*        DataIzgotovlenia data;
        data.all = EE_Working::ReadCalibrationData(EE_CAL_DATA_PRODUCTION);
        sprintf(&editString[3]->text[0][11],"%1d :%1d :%1d",data.data.day,data.data.month,data.data.yers);*/
#endif
        if (runTime == 0xFFFFFFFF) {
            EE_Working::WriteCalibrationData(EE_CAL_HOURS, 0);
            }
        for (uint8_t i =0;i<numberCell;i++) {
            form.borderVisible(i,false);}
        /*t_Pos_Size_XY pos= { .X=2, .Y = 31, .Xsize=318, .Ysize=20};
        form.setCoordSell(0,pos);
        pos.X=1; pos.Y = 51; pos.Xsize=180; pos.Ysize=ysize;
        form.setCoordSell(1,pos);
        pos.X=181; pos.Y = 51; pos.Xsize=137; pos.Ysize=ysize;
        form.setCoordSell(2,pos);
        pos.X=1; pos.Y = 71; pos.Xsize=184; pos.Ysize=ysize;
        form.setCoordSell(3,pos);
        pos.X=186; pos.Y = 71; pos.Xsize=130; pos.Ysize=ysize;
        form.setCoordSell(4,pos);*/
        runTimeOld=runTime;
        timeOldInput=0;
        timeToSave=runTime+600;
        }

    virtual void Listener () {
        if (this->Focused) {
            ClearDisp();
            GyConStateBitsLCD versionSmart;
            versionSmart.all= p_pult->getGyConFaultBits();
            sprintf(&editString[HeadVersion]->text[0][15],"%1d",versionSmart.faultBits.GyConProgrammVersion+16);
            form.drawNoActiveFormTecStile();
            Focused=false;
            }
        if (runTimeOld != runTime) {
            form.drawCellTecStile(PultRunTime);
            runTimeOld = runTime;}
        if (getButtonState(pult_Button_ESC) == PRESSED)        {
            exit();
            }
        }

    inline void calcelateRuntime() {
        uint32_t timeInput = (uint32_t)p_pult->getTimeToStart();
        uint32_t deltaVremeni=timeInput-timeOldInput;
        runTime+=deltaVremeni;
        uint32_t time = runTime;
        uint32_t hour= time /3600;
        time= time- (hour*3600);
        uint8_t min = time / 60;
        time = time - (min*60);
        uint8_t second = time;
        printTime(hour,min,second);
        if (runTime>=timeToSave) {
            EE_Working::WriteCalibrationData(EE_CAL_HOURS, runTime);
            timeToSave+=600;    }
        timeOldInput=timeInput;
        }

    inline void setSmartVersionProgram(ParametrStruct version) {

        }

    inline void setFIZVersionProgram(ParametrStruct version) {

        }

    inline void setRadioChannalVersionProgram(ParametrStruct version) {

        }

private:

    void exit() {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
    }

   inline void printTime (uint32_t hour, uint8_t min, uint8_t second) {
        if (runTime%2)
            sprintf(&editString[PultRunTime]->text[0][10],"%1d :%1d :%1d",hour,min,second);
       else
            sprintf(&editString[PultRunTime]->text[0][10],"%1d %1d %1d",hour,min,second);
       }
    };
}
}




#endif /* PULT_INDICATOR_USERINFOMENU_HPP_ */
