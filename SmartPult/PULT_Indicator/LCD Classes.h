

#ifndef _LCD_CLASSES_H_
#define _LCD_CLASSES_H_

#include "PultGlobalDefinitions.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "grlib/grlib.h"
#include <Pult/Pult.h>
#include <SPI/SPI_Work.h>
#include "PWM_Work.h"
#include "drivers/kentec320x240x16_ssd2119.h"
#include "../GyConCommon/dataTypes.h"
#include "../MotionControl/MotionSynchroModule.hpp"
#include "MemoryControl.hpp"



extern Pult* p_pult;

//==== � ������ ROM.h ���, �����, ����, � � ����� ���!
#if defined(TARGET_IS_SNOWFLAKE_RA0)
#define ROM_GPIOPinTypeLCD                                                    \
        ((void (*)(uint32_t ui32Port,                                         \
                   uint8_t ui8Pins))ROM_GPIOTABLE[56])
#endif
//====

//typedef uint8_t byte;

typedef union PultControlBitsLCD {
    UInt16 all;
    struct {
        volatile UInt16 onOffMotors:1;
        volatile UInt16 levelCorrect:1;//
        volatile UInt16 fastLevelCorrect:1;//
        volatile UInt16 levelSetup:1;//
        volatile UInt16 gvCalibration:1;
        volatile UInt16 gvAcc:1;
        volatile UInt16 setUpTiltLimits:1;
        volatile UInt16 setDwTiltLimits:1;
        volatile UInt16 resetUpTiltLimits:1;
        volatile UInt16 resetDwTiltLimits:1;
        volatile UInt16 overslangUnderslang:1;
        volatile UInt16 goToAngels:1;
        volatile UInt16 joysticOn:1;
        volatile UInt16 panSuspensionResonatorDisable:1;
        volatile UInt16 driftStopperOn:1;

    }bit;
}PultControlBitsLCD;

typedef union GyConStateBitsLCD {
   UInt16 all;
   struct {
       UInt16 panDusFault:1;
       UInt16 dutchDusFault:1;
       UInt16 tiltDusFault:1;
       UInt16 gvFault:1;
       UInt16 encodersFault:1;
       UInt16 pultFault:1;
       UInt16 GyConProgrammVersion:5;
       UInt16 bit11:1;
       UInt16 stateBits:4;
    } faultBits;
}GyConStateBitsLCD;

PultControlBitsLCD controlBits;
GyConStateBitsLCD gyConFaultBits;

typedef struct
{
    int16_t X; //��������� ���������� X
    int16_t Y; //��������� ���������� Y
    int16_t Xsize; //������ �� X
    int16_t Ysize; //������ �� Y
}
t_Pos_Size_XY;

#define Cell_Active 1
#define Cell_UnActive 0

typedef struct {
    bool Border; //���� ��� ��� �����
    uint32_t Border_Color; //���� �����
    uint32_t Cell_Color; //���� ����
    uint32_t Font_Color; //���� ������
    const tFont* pFont; //������ ������
    bool Text_Centered; //����� �� ������
}
tCell_Style;

extern tCell_Style Style_MenuUnActive; //����� ���������� ������ ����
extern tCell_Style Style_MenuActive; //����� �������� ������ ����
extern tCell_Style Style_MenuSelected; //�����  ��������� ������ ����
extern tCell_Style Style_MenuHeader; //�����  ������ ��������� ����
extern tCell_Style Style_Error; //����� ������ ������

class LCD_Listener {
public:
    char* pName;
    UInt32 Param;
    LCD_Listener* Parent; //������ �� ��������(��� ������)
    bool Focused; //���� �� ��� ������������ � ������� ������
    PultButtonStates getButtonState(PultButtons pultButton);
    virtual void Listener() {}; //����� ������������� ����� ��������� ������� ��� ������� ��������� ������ � ������
};

typedef struct {
    char* Name; //��� ������ ����
    LCD_Listener* pPointSub; //��������� �� �������
}
tMenu_Link;

extern LCD_Listener* pDispl;

class ILCD_Cell {
public:

    virtual void Set_Coord(t_Pos_Size_XY & pos) =0;

    virtual void SetText(char* ptext) =0;

    virtual void setCellColor (uint32_t color) =0;

    virtual void ReDraw()=0;

    virtual void ReHide()=0;
};

class LCD_Cell: public LCD_Listener, public ILCD_Cell {
private:
    tCell_Style Tek_Style;
    tRectangle pRect;
    void SetRect(); //����������: ������ ����� ���������� ��������� ��������������
    void PreDraw(); //����������: ������� ������ � ������ �����
    bool Drawed, Hided; //������� ��� ��� ����������
public:
    t_Pos_Size_XY p_Pos_Size_XY; //������� � ������������ ������
    tCell_Style Active_Style, UnActive_Style;
    char* p_text; //�������� �����
    bool Rounded; //� �����

    LCD_Cell() {
    Tek_Style = Style_MenuActive;
    Active_Style = Style_MenuActive;
    UnActive_Style = Style_MenuUnActive;
    Rounded = false;
    Drawed = false;
    Hided = false;}
    LCD_Cell (char *text, t_Pos_Size_XY  position) : p_text(text),p_Pos_Size_XY(position) { // ������ ��� ���� ����� ��� � ������ ����������� ������ ����������
        Tek_Style = Style_MenuActive;
        Active_Style = Style_MenuActive;
        UnActive_Style = Style_MenuUnActive;
        this->Rounded=false;
        this->Drawed=false;
        this->Hided=false;
    }
    LCD_Cell(char* pNam, UInt32 X=1,UInt32 Y=1,UInt32 Xsize=10,UInt32 Ysize=10) // ������ �����������
    {
        pName = pNam;
        p_Pos_Size_XY.X = X-1;  p_Pos_Size_XY.Xsize = Xsize;
        p_Pos_Size_XY.Y = Y-1;  p_Pos_Size_XY.Ysize = Ysize;

        Tek_Style = Style_MenuActive;
        Active_Style = Style_MenuActive;
        UnActive_Style = Style_MenuUnActive;
        Rounded = false;
        Drawed = false;
        Hided = false;
    }
    void Set_Coord(UInt32 X,UInt32 Y,UInt32 Xsize,UInt32 Ysize);
    void FastDraw(UInt32 X,UInt32 Y,UInt32 Xsize,UInt32 Ysize, char* ptext, byte Active); //������ ������ � ������������������ �����������
    inline void FastDraw (bool active) {
        if (active)  ReDraw();
        else    ReHide();
        }
    void setBoardVisible (bool visible) {
        Tek_Style.Border=visible;
        }

    virtual void setSizeText (const tFont* fontSize) { Tek_Style.pFont=fontSize;}

    virtual void Set_Coord (t_Pos_Size_XY& pos) {p_Pos_Size_XY=pos;}

    virtual void SetText(char* ptext) {p_text = ptext;} //��������� ����� ��� ����

    virtual void setCellColor (uint32_t color) {
        Active_Style.Cell_Color= color;
        UnActive_Style.Cell_Color = color;
    }

    void Draw(); //������ ������ � ����� Active_Style
    void Hide(); //������ ������ � ����� UnActive_Style
    virtual void ReDraw(){Drawed = false; Draw();} //������ ������ � ����� Active_Style
    virtual void ReHide(){  Hided = false; Hide();} //������ ������ � ����� UnActive_Style
    void Clean(){PreDraw();} //������� ������ (������ ������)
    virtual void Listener(); //����� ������������� ����� ��������� ������� ��� ������� ��������� ������ � ������
    void DrawTecStile();

};

class TikerCell: public LCD_Cell
{
public:
    TikerCell(char* pNam, UInt32 X=1,UInt32 Y=1,UInt32 Xsize=10,UInt32 Ysize=10);
    void tick();

private:
    UInt8 stringCount;
    Int8* strings[4];
    UInt8 currentString;

    UInt8 tickerDeviderMaximum;
    UInt8 tickerDeviderCounter;

    UInt8 tempLine[26];
    UInt8 stringPointerIterator;


    void renderSplitString(UInt8 ind);
    inline UInt8 nextString(UInt8 current);
    inline void setStringCount(UInt8 count){if(count<=4){stringCount=count;}}
    inline void updateFromPult();


};


typedef enum {
    GV_Acc=0,
    Motor_Stop=1,
    Pan_Stop=2,
    Dutch_Stop=3,
    Tilt_Stop=4,
    Main_Counter=5,
    Main_Counter_R=6,
    Main_Counter_L=7,
    Error=8,
    Pan_Set=9,
    Dutch_Set=10,
    Tilt_Set=11,
    Horiz_Angle=12
} Field;

enum
{
    TL_RESET=0,
    TL_UP,
    TL_DOWN,
    TL_UP_DOWN,

};
class LCD_Main: public LCD_Listener{
private:
    char profileString[10];
    char voltageString[10];
    char counterTimeString[20];
    char motionPlayModeString[8];
    char motionMixModeString[10];
    char limitTimeString[20];
    char servCounterStr[5];
    char motionStateString[10];
    char motionTrackNumberString[10];

    UInt8 motionLastState;
    UInt8 motionLastTrackNumber;
    MotionPlayMode motionPlayModeState;
    MotionMixMode motionMixModeState;
    UInt32 limitTimeLastState;
    UInt32 counterTimeLastState;

    volatile PanAxisDirection currentPanDirection;
    volatile float currentValue;
    volatile bool inMotionMode;
    volatile UInt8 selectModeCounter;

    void renderDateString(char* buffer, UInt8 bufLen,UInt32 seconds);
    void updateMotionState();
    void updateMotionTrackNumber();
    void updateLimitTime();
    void updateCounterTime();
    void updateMotionPlayModeStr();
    void updateMotionMixModeStr();
    void selectMotionMode();
    void deselectMotionMode();

public:
    byte GV_Acc, Motor_Stop, Pan_Stop, Dutch_Stop, Tilt_Stop;
    byte Error;
    char* Error_Descr;
    Int16 Main_Counter;
    UInt16 lastVoltage;

    LCD_Cell Cell_P_Name, Cell_P_Inf, Cell_D_Name, Cell_D_Inf, Cell_T_Name, Cell_T_Inf;
#ifdef joyPult
    LCD_Cell Cell_Z_Name, Cell_Z_Inf;
#endif
    LCD_Cell Cell_Main_Counter, Cell_Main_Counter_Right, Cell_Main_Counter_Left;
    LCD_Cell Cell_Joyst_State, Cell_Motor_State, Cell_GV_Acc/*, CellTiltLimitedState*/;
    LCD_Cell motionLimitTime, motionCurrentTime,motionTrackNumber, motionState, motionPlayMode,motionMixMode;

    uint8_t  panLimiterStateFlag, rollLimiterStateFlag, tiltLimiterStateFlag;
    //Error line
    TikerCell ErrorLine;
    LCD_Cell tiltLimiterState, panLimiterState,rollLimiterState;
    LCD_Cell headVoltage;
    LCD_Cell currentProfile;

    LCD_Main(char* pName);
    void Draw(); //���������
    void ReDraw(); //�����������
    //void Sign(Cell_Name field); //�������������� ����
    bool explore_Koeff_Butt(); //������������ ������ �������������
    void Serv_Counter(float value,bool isForseUpdate); //����������� � ���������� �������
    virtual void Listener(); //����� ������������� ����� ��������� ������� ��� ������� ��������� ������ � ������
    void testLeds();
    void explore_Presets(); //������������ ������� �� ����
    void sendSetupValuesByPult();
    void updateTiltLimiterCell();
    void updatecurrentProfileCell();
    void updateVoltageString(bool);
    void upToMotionMode();
    void switchMotionMode();
};


class LCD_Menu: public LCD_Listener{
protected:
    t_Pos_Size_XY p_Pos_Size_XY; //� ������ ��������� ��� ����� ������ � �������
public:
    byte Space; //���������� ����� ���������
    byte Start, Counter_Cell, Tek_Count, old_position_number; //��������� ������� ����������, ����� ������� � ����� ��������
    byte Orientation; //���������� (0 - ������, 1 - ������� � ������)
    byte Menu_On_Screen; //������� ������ ������ � �����
    LCD_Cell Cell_Header, Cell_1, Cell_2, Cell_3, Cell_4, Cell_5,
    Cell_6, Cell_7, Cell_8, Cell_9, Cell_10;
    LCD_Cell* Table_Cell[10]; //����� ������ �� ������
    tMenu_Link* Menu_Link;
    LCD_Menu(char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr);

    LCD_Menu();
    void Plus();void Minus();void Right();void Left(); //���������
    void Select(); //�����
    virtual void Draw(byte Active); //������ ������� � ���������

    virtual void DrawVert(); //��������� ������������� ����
    void DrawHoriz(); //��������� ��������������� ����
    void DrawHeader() {
        if(Orientation==0) //���� ���������� 1*6
        {   Cell_Header.FastDraw(0,0,319,30, pName, Cell_UnActive); }
        else //���������� 2*3
        {   Cell_Header.FastDraw(0,0,319,30, pName, Cell_UnActive); }
    }
    void readEEPROM();


    virtual void Listener(); //����� ������������� ����� ��������� ������� ��� ������� ��������� ������ � ������
};

enum {
    ecoModSistem=0,
    panTorqueSistem,
    dutchTorqueSistem,
    tiltTorqueSistem,
    zoomTorqueSistem
};

enum {
    ecoMod=1,
    panTorque,
    dutchTorque,
    tiltTorque,
    zoomTorque
};

class SetMaxTorque: public LCD_Menu{
private:
    void drawEco() {
        if (eco)
            sprintf(bufferNames[ecoModSistem],"ECO MOD ON");
        else
            sprintf(bufferNames[ecoModSistem],"ECO MOD OFF");
    }
    void setEco() {
        if (eco) eco=false;
        else eco=true;
        values[ecoModSistem]=(UInt32)eco;
        drawEco();
        p_pult->setEcoMode(eco);
        }
protected:
    char bufferNames[5][15];
    volatile UInt32 values[4];
    bool eco;

    virtual inline void saveInEprom();

public:
    SetMaxTorque(char* pNam);
    virtual void Draw(byte Active);
    virtual void DrawVert();
    virtual void Listener();


    virtual void upValue(UInt8 id);
    virtual void downValue(UInt8 id);

    virtual void action();
    virtual void updateFromEeprom();
};

class SetMotionPrerol: public LCD_Menu{
protected:
    char bufferNames[4][15];
    volatile UInt32 values[4];

    virtual inline void saveInEprom();

public:
    SetMotionPrerol(char* pNam);
    virtual void Draw(byte Active);
    virtual void DrawVert();
    virtual void Listener();

    virtual void upValue(UInt8 id);
    virtual void downValue(UInt8 id);

    virtual void action();
    virtual void updateFromEeprom();
};


//-----------------------------------------------

enum MaxSectorCount
{
    FOLOWING_MODE_MAX_SECTOR=15
};


class FolowingModeMenu: public LCD_Menu{
protected:
    char bufferNames[4][15];
    FolowingModeConfigData data;

    virtual inline void saveInEprom();

public:
    FolowingModeMenu(char* pNam);
    virtual void Draw(byte Active);
    virtual void DrawVert();
    virtual void Listener();

    virtual void upValue(UInt8 id);
    virtual void downValue(UInt8 id);

    virtual void action();
    virtual void updateFromEeprom();
};
//-----------------------------------------------

class SetZoomDrift: public LCD_Menu{
protected:
    char bufferName[15];
    volatile Int32 value;

    virtual inline void saveInEprom();

public:
    SetZoomDrift(char* pNam);
    virtual void Draw(byte Active);
    virtual void DrawVert();
    virtual void Listener();

    virtual void upValue(UInt8 id);
    virtual void downValue(UInt8 id);

    virtual void action();
    virtual void updateFromEeprom();
};

class HourMeterMenu: public LCD_Menu{
protected:
    char bufferName[2][15];
    volatile UInt32 rawValue;
    UInt32 calcHash(UInt32 val);
public:
    HourMeterMenu(char* pNam);
    virtual void Draw(byte Active);
    virtual void DrawVert();
    virtual void Listener();
    void setRawValue(UInt32 rawV);
};

class SetStartFluid: public SetMaxTorque{
protected:
    inline void saveInEprom();

public:
    SetStartFluid(char* pNam);
    void upValue(UInt8 id);
    void downValue(UInt8 id);
    void action();
    void updateFromEeprom();
};


class SetJoyDeadZone: public LCD_Menu{
protected:
    char bufferNames[4][15];
    volatile UInt32 values[4];

    inline void saveInEprom();

public:
    SetJoyDeadZone(char* pNam);
    virtual void Draw(byte Active);
    virtual void DrawVert();
    virtual void Listener();

    void upValue(UInt8 id);
    void downValue(UInt8 id);

    void action();
    void updateFromEeprom();
};




class CheckMenu: public LCD_Listener{
protected:
    t_Pos_Size_XY p_Pos_Size_XY;
    char buffer[21];
    void render(char* name, bool flag1, bool flag2);
    void renderM(char* name, bool flag1, UInt8 counter);
public:
    LCD_Cell c0,c1,c2,c3,c4,c5,c6,c7,c8,c9,c10,c11,c12,c13,c14,c15;
    LCD_Cell connCell;
    LCD_Cell conDelay;
    LCD_Cell* cells[16]; //����� ������ �� ������
    CheckMenu();
    virtual void Draw();
    virtual void Listener();
};


class SelectMenu: public LCD_Menu
{
protected:
    const UInt32 currentEepromAddress;
public:
    SelectMenu (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress);
    virtual void Draw(byte Active);
    virtual void DrawVert();
    virtual void updateFromEEPROM();
    virtual void saveInEEPROM();
    virtual void action();
    virtual void Listener();
};

class SelectMenuSpiderSelect:public SelectMenu
{
    volatile float editValue;
    const SuspensionResonansChannel channel;
    const UInt32 eepromEditValueAddress;
public:
    SelectMenuSpiderSelect (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress,UInt32 eepromEditAddr, SuspensionResonansChannel ch);
    virtual void action();
    virtual void Listener();
    virtual void DrawVert();
    virtual void updateFromEEPROM();
};

class SelectMenuZoomSense:public SelectMenu
{
public:
    SelectMenuZoomSense (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress);
    virtual void action();
};

class SelectMenuCameraStart:public SelectMenu
{
public:
    SelectMenuCameraStart (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress);
    virtual void action();
};
class SelectMenuOverslang:public SelectMenu
{
public:
    SelectMenuOverslang (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress);
    virtual void action();
};

class SelectMenuSynchronization:public SelectMenu
{
private:
    uint8_t camId;

public:
    SelectMenuSynchronization (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress);
    virtual void action();
    virtual void updateFromEEPROM();

};

class ProfileSelectMenu: public LCD_Menu
{
public:
    ProfileSelectMenu (char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr);
    virtual void Listener();
private:

};

class SwitchAxsesMenu: public LCD_Menu
{
public:
    SwitchAxsesMenu (char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr);
    void updateFromEEPROM();
    virtual void Listener();
};

class SwitchMotorAction: public LCD_Menu
{
protected:
    volatile UInt8 currentVariable;
    volatile UInt8 load[10];
public:
    virtual void Draw(byte Active); //������ ������� � ���������
    virtual void DrawVert(); //��������� ������������� ����
    SwitchMotorAction (char* pNam,tMenu_Link* Link,UInt8 cellCount);
    virtual void updateFromEEPROM();
    virtual void Listener();

    virtual void setCurrentVariable(UInt8 v);
    virtual void setValue(UInt8 motorValue);
    virtual void pultUpdate();
    virtual void saveInEEPROM();

};

class LCD_Menu_lcZIF: public LCD_Menu
{
protected:
     tMenu_Link* subtext;
     UInt8 subtextCount;
public:
    virtual void Draw(byte Active); //������ ������� � ���������
    virtual void DrawVert(); //��������� ������������� ����
    LCD_Menu_lcZIF(char* pNam, tMenu_Link* Link, byte Count, byte Orient,
    byte Menu_Per_Scr, tMenu_Link* subtext_, UInt8 subtextCount_ );
};

class LCD_Menu_lcMotrors: public LCD_Menu_lcZIF
{

public:
    virtual void DrawVert(); //��������� ������������� ����
     LCD_Menu_lcMotrors(char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr, tMenu_Link* subtext_, UInt8 subtextCount_ );
};

class SwitchMotorTypeMenu: public  SwitchMotorAction
{
public:
    SwitchMotorTypeMenu(char* pNam,tMenu_Link* Link,UInt8 cellCount);
    void updateFromEEPROM();
    void setValue(UInt8 motorValue);
    void pultUpdate();
    void saveInEEPROM();
};

class SecretMenuView:public LCD_Menu
{
public:
    SecretMenuView(char* pNam,  byte Count,byte Menu_Per_Scr);
    virtual void Listener();
    virtual void DrawVert();
    virtual void Draw(byte Active) ;
    virtual inline void updateDataFromPult();

protected:
    char bufferBig[30];
    char bufSmall[10];
    float values[10];
    char* names[10];
    UInt8 counter;
    UInt8 maxCount;


    inline void renderCellString(char* head, float var1, float var2, bool isTwoValueStyle);
};

class SecretMenu:public SecretMenuView
{
public:
    SecretMenu(char* pNam,  byte Count,byte Menu_Per_Scr);
    virtual void Listener();
    virtual inline void updateDataFromPult();
    inline void readFromEEPROM();

protected:

    UInt8 inputState;
    float oldValue;
    float newValue;
    inline void upValue(bool faster);
    inline void downValue(bool faster);
    float stepValue;

};



class LCD_Set_Koeff: public LCD_Listener{
private:
public:
    t_Pos_Size_XY p_Pos_Size_XY; //� ������ ��������� ��� ����� ������ � �������
    byte Space; //���������� ����� ���������
    bool Selected; //���������, ��� ������ �������, � ���-����� �������� �� ������
    tCell_Style Active_Style, UnActive_Style, Select_Style;
    byte Start, Counter_Cell, Tek_Count; //��������� ������� ����������, ����� ������� � ����� ��������
    byte Menu_On_Screen; //������� ������ ������ � �����
    LCD_Cell Cell_Header, Cell_1, Cell_2, Cell_3, Cell_4, Cell_5, Cell_6, Cell_7, Cell_8, Cell_9, Cell_10;
    LCD_Cell* Table_Cell[10]; //����� ������ �� ������
    char buff_Cell[10][15]; //����� ������� �����
    UInt16 PredValue;
    LCD_Set_Koeff(char* pNam, byte Count, byte Menu_Per_Scr);
    void Plus(UInt16 HowMatch);void Minus(UInt16 HowMatch);void Right();void Left(); //���������
    void Select(); //�����
    void sign_Pult(UInt8 num); //�������� ������ �� ��������� ���������
    void SetText(byte num); //����� ������ �����, �� ������ ����� ������������ �������
    void Draw(byte Active); //������ ������� � ���������
    void DrawSelected(byte Active); //������ ������ �������� ������
    void DrawHeader();
    virtual void Listener(); //����� ������������� ����� ��������� ������� ��� ������� ��������� ������ � ������
    void Read_EE_State(); // ��� ����� ��������� �������� �������� �� ������
};


typedef struct {
    char Field[20]; //������������ ���������
    char Data_On[20]; //������������ �������� ���
    char Data_Off[20]; //������������ �������� ����
}
t_Data_For_Bit_Menu;


class LCD_Koeff: public LCD_Listener{
private:
public:
    UInt16 Value_Koeff;
    LCD_Cell Cell_Koeff, Cell_String; //��������� ������������ � ������ �������� ������������
    char* p_text; //�������� �����

    void SetText(char* ptext); //��������� ����� ��� ����
    LCD_Koeff(char* pName, char* ptext, UInt16 text_length);
    void Draw(); //���������
    virtual void Listener(); //����� ������������� ����� ��������� ������� ��� ������� ��������� ������ � ������
    void readFromEEPROM();
};


typedef enum{
    ButtOff = 0,
    ButtOn = 1,
    ButtHold = 2
} Butt_State;

class Button_Listener{
private:
public:
    Uint16 Counter_Button[PULT_BUTTONS_COUNT];
    PultButtonStates State[PULT_BUTTONS_COUNT];
    bool Fix[PULT_BUTTONS_COUNT];
    Button_Listener();
    void Button_Analizer();
    void Button_Pressed(PultButtons Butt);
    void Button_Released(PultButtons Butt);
    PultButtonStates Get_Button(PultButtons Butt);
};

typedef struct {
    byte num; //����� �����
    float X;
    float Y;
}
tLine;

class LCD_Equalizer: public LCD_Listener{
private:
    UInt8 curve[230];
    UInt32 LineOff, LineOn, LineActive, LineSelect;
    const PultEqualaserType type;
    List<Point*>* referencePoints;
    void updateReferencePoints();
    inline void invalidateCurve();
    volatile float currentSpeedValue;
    volatile float currentJoyPozition;
    void updateSpeedValue();
public:
    EEPROM_Filds EE_Start_Adr;
    byte Line_Counter, Tek_Num_Line; //���������� �����
    bool Selected; //����������, ��� ����� �������, � ����-����� �������� ��
    tCell_Style Active_Style, UnActive_Style, Select_Style;
    byte Menu_On_Screen; //������� ������ ������ � �����
    LCD_Cell Cell_Header, Cell_X, Cell_Y;
    char buff_Cell[2][10]; //����� ������� ����� X � Y
    tLine lines[15];
    tLine PredLine;
    LCD_Equalizer(char* pNam, byte lineCounter, EEPROM_Filds ee_start_adr,PultEqualaserType type_);
    void Plus();void Minus();void Right();void Left(); //���������
    void Select(); //�����
    //void sign_Pult(UInt8 num); //�������� ������ �� ��������� ���������
    void Set_X_Y(byte num); //����� ������ �����, �� ������ ����� ������������ �������
    void Set_Y(byte num) {}; //����� ������ �����, �� ������ ����� ������������ �������
    void DrawLine(tLine Line, UInt32 Color); //������ ������� � ���������
    void Draw(); //
    void DrawHeader();
    virtual void Listener(); //����� ������������� ����� ��������� ������� ��� ������� ��������� ������ � ������
    void Read_EE_State(); // ��� ����� ��������� �������� �������� �� ������
    void Write_EE_State(tLine Line); // ��� ����� ������� �������� �������� � ������
    void DrawCurve(UInt32 Color);

};



#define Color_Fon_Displ ClrMediumOrchid //ClrBlueViolet// ���� ���� ClrDarkBlue
#define Color_Menu_Header ClrDarkGoldenrod // ClrChocolate
#define Color_Menu_Fon_Active ClrMediumOrchid // ClrChocolate
#define Color_Menu_Fon_InActive ClrMediumOrchid //ClrDarkSalmon
#define Color_Menu_Text_Active ClrWhite
#define Color_Menu_Text_InActive ClrBlack

//extern void Disp_All_Color();
//extern void Disp_All_Char();

extern void itoa(long unsigned int value, char* result, int base);
extern void u16_TO_buffer(char* buffer, uint16_t digits, uint8_t base);

extern void ClearDisp(); //������ �������� ������
extern void Init_Styles(); //������� ����� ������

extern void pultIndikatorInit();
extern void pultIndikator_Task(Pult* point_pult);
extern void debug_button_init();
extern void Button_Analiz();

extern void LCD_PinInit();
extern void MainListener();

extern void CellPaint(UInt32 num_str, char* text, UInt32 text_length, uint32_t color);
extern void CellPaint_XY(UInt32 num_str, UInt32 num_col, char* text, UInt32 text_length, uint32_t color);
void loadEepromValueFromPult();

typedef struct
{
//TODO ���������� �� const char � �������� ��� ��� �������� ������ � flash, � �� ������ ��� � ��������
//����� ��������� ��������� �� flash
    char Name[20];
    UInt32 Value;
}
tColor;
/*
void ClearDisp()
{
    //������� ������ �������� ������
    tRectangle pRect;

    pRect.i16XMin = 0;
    pRect.i16YMin = 0;
    pRect.i16XMax = 319;
    pRect.i16YMax = 239;
    DpyRectFill(&g_sKentec320x240x16_SSD2119,&pRect,Color_Fon_Displ);
}*/


#endif /* _LCD_CLASSES_H_ */


