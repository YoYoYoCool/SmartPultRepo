

#include "../Board.h"

#include "LCD Classes.h"
#include "drivers/kentec320x240x16_ssd2119.h"
#include "driverlib/rom.h"
#include "driverlib/eeprom.h"
#include <xdc/runtime/System.h>
#include "../Pult/Aproximators/AproximatorIF.hpp"
#include "../WatchDogTimer/WatchDog.hpp"
#include "Image.hpp"
#include "../ExtrenalDeviceSynchro/RS232Syncro.hpp"
#include "MotionControl/LogicController/MotionLogicController.hpp"
#include "PultDBWork/Objective Select.hpp"
#include "PultDBWork/Measure Sistem types.hpp"
#include "UserInfoMenu.hpp"
#include "LimitsMenu.hpp"
//#include "tColor.h"
//#include "LCD Lens.hpp"


#include "TurnsViewMenu.hpp"
#include "WheelMenu.hpp"
#include "ZIFRevers.hpp"
#include "ShakerForm.hpp"


#define VERTICAL_LEN 200
Pult* p_pult;
PultButton* btn;

extern WatchDog watchDogTimer;

//extern tColor FullColorTable[140]; //����� �������������

tCell_Style Style_MenuUnActive; //����� ���������� ������ ����
tCell_Style Style_MenuActive; //����� �������� ������ ����
tCell_Style Style_MenuSelected; // ��������� ������ ����
tCell_Style Style_MenuHeader; //�����  ������ ��������� ����
tCell_Style Style_Error; //����� ������ ������

tContext sContext;
static volatile UInt32 HourMeter_rawValue=0;


//#define thisname


//������������ � ����� ������ �������� ����
tMenu_Link Main_Menu_Link[6]={
        {"COEFF SETUP",NULL},
        {"SETUP",NULL},
        {"AXIS LIMITER",NULL},
        {"MOTION CTRL",NULL},
        {"EQUALIZER",NULL},
        {"LENS CONTROL",NULL}

};
enum {CoeffSetupMainMenu=1,
    SetupMainMenu,
    TiltLimiterMainMenu,
    MotionCtrlMainMenu,
    EqualizerMainMenu,
    LensControlMainMenu}; //� ��������� ����� � ������� ���� �� ������� ������ � ��������� ����������

enum {CoeffSetupMainMenuSistem=0,
    SetupMainMenuSistem,
    TiltLimiterMainMenuSistem,
    MotionCtrlMainMenuSistem,
    EqualizerMainMenuSistem,
    LensControlMainMenuSistem};

tMenu_Link Txt_MenuTiltLimit[5]={
        {"Set Upper Limit",NULL},
        {"Set Lower Limit",NULL},
        {"Reset Upper Limit",NULL},
        {"Reset Lower Limit",NULL},
        {"Reset All Limits",NULL}
};

enum {SetUpperLimitMenuTiltLimit=1,
    SetLowerLimitMenuTiltLimit,
    ResetUpperLimitMenuTiltLimit,
    ResetLowerLimmitMenuTiltLimit,
    ResetAllLimitMenuTiltLimit};

enum {SetUpperLimitMenuTiltLimitSistem=0,
    SetLowerLimitMenuTiltLimitSistem,
    ResetUpperLimitMenuTiltLimitSistem,
    ResetLowerLimitMenuTiltLimitSistem,
    ResetAllLimitMenuTiltLimitSistem};

//������������ ������ ���� PAN DUTCH TILT ����� ������� �����������

#ifdef joyPult

    tMenu_Link PDTMenu_link[4]={
        {"PAN",NULL},
    #ifdef USAEdition
        {"ROLL",NULL},
    #else
        {"DUTCH",NULL},
    #endif
        {"TILT",NULL},
        {"ZOOM",NULL}
        };

    enum {PanPDTMenu=1,
        DutchPDTMenu,
        TiltPDTMenu,
        ZoomPDTMenu};
    enum {PanPDTMenuSistem=0,
        DutchPDTMenuSistem,
        TiltPDTMenuSistem,
        ZoomPDTMenuSistem};
#else
    tMenu_Link PDTMenu_link[3]={
        {"PAN",NULL},
    #ifdef USAEdition
        {"ROLL",NULL},
    #else
        {"DUTCH",NULL},
    #endif
        {"TILT",NULL}
    };
    enum {PanPDTMenu=1,
        DutchPDTMenu,
        TiltPDTMenu
        };
    enum {PanPDTMenuSistem=0,
        DutchPDTMenuSistem,
        TiltPDTMenuSistem
        };
#endif


    tMenu_Link shakerMenuText[2]={
                {"RND SHAKER", NULL},
                {"SIN SHAKER", NULL},
         //       {"SHAKER BOX", NULL}
            };


tMenu_Link secretMenuText[3]={
        {"Set",NULL},
        {"View",NULL},
        {"Check",NULL},
    };

enum {SetSecretMenu=1,
    VievSecretMenu,
    CheckSecretMenu};

enum {SetSecretMenuSistem=0,
    VievSecretMenuSistem,
    CheckSecretMenuSistem};

#ifdef joyPult

    tMenu_Link setupMenuText[8]={
            {"BRIGHTNESS",NULL},
            {"USER PROFILES",NULL},
            {"JOYSTICK PLACEMENT",NULL},
            {"UNDER/OVERSLUNG",NULL},
            {"WHEELS MENU",NULL},
            {"JOYSTICK CALIBRATION",NULL},
            {"TUNING",NULL},
            {"FOLLOWING MODE",NULL}
            };

    enum {BrightnessSetupMenu=1,
        UserProfilesSetupMenu,
        JoystickPlacementSetupMenu,
        UnderOverslungSetupMenu,
        WhellSpeedSetupMenu,
        JoystickCalibrationSetupMenu,
        TuningSetupMenu,
        FolowingModeSetupMenu};

    enum {BrightnessSetupMenuSistem=0,
        UserProfilesSetupMenuSistem,
        JoystickPlacementSetupMenu_sistem,
        UnderOverslungSetupMenu_sistem,
        WhellSpeedSetupMenuSistem,
        JoystickCalibrationSetupMenuSistem,
        TuningSetupMenuSistem,
        FolowingModeSetupMenuSistem};
#else

    tMenu_Link setupMenuText[6]={
            {"BRIGHTNESS",NULL},
            {"USER PROFILES",NULL},
            {"UNDER/OVERSLUNG",NULL},
            {"WHEELS MENU",NULL},
            {"TUNING",NULL},
            {"FOLOWING MODE",NULL}
            };

    enum {BrightnessSetupMenu=1,
        UserProfilesSetupMenu,
        UnderOverslungSetupMenu,
        WhellSpeedSetupMenu,
        TuningSetupMenu,
        FolowingModeSetupMenu};

    enum {BrightnessSetupMenuSistem=0,
        UserProfilesSetupMenuSistem,
        UnderOverslungSetupMenu_sistem,
        WhellSpeedSetupMenuSistem,
        TuningSetupMenuSistem,
        FolowingModeSetupMenuSistem};
#endif

#ifdef joyPult

    tMenu_Link tuningMenuText[8]={
            {"SUSPENSION RESONANCE",NULL},
            {"MAX TORQUE",NULL},
            {"JOYSTICK DEADZONE",NULL},
            {"USER INFO MENU",NULL},
            {"PREROL",NULL},
            {"SYNCHRO SOURCE",NULL},
            {"DRIFT STOPPER",NULL},
            {"SHAKER",NULL}
    };
    enum {SuspensionResonansTuningMenu=1,
        MaxTorqueTuningMenu,
        JoysticDeadZoneTuningMenu,
        TotalRuntimeTuningMenu,
        PrerolTuningMenu,
        SynchroSourceTuningMenu,
        DriftStopperTuningMenu,
        InternalSakerTuningMenu};

    enum {SuspensionResonansTuningMenuSistem=0,
        MaxTorqueTuningMenuSistem,
        JoysticDeadZoneTuningMenuSistem,
        TotalRuntimeTuningMenuSistem,
        PrerolTuningMenuSistem,
        SynchroSourceTuningMenuSistem,
        DriftStopperTuningMenuSistem,
        InternalSakerTuningMenuSistem};

#else

    tMenu_Link tuningMenuText[7]={
            {"SUSPENTION RESONANCE",NULL},
            {"MAX TORQUE",NULL},
            {"USER INFO MENU",NULL},
            {"PREROL",NULL},
            {"SYNCHRO SOURCE",NULL},
            {"DRIFT STOPPER",NULL},
            {"SHAKER",NULL}
    };
    enum {SuspensionResonansTuningMenu=1,
        MaxTorqueTuningMenu,
        TotalRuntimeTuningMenu,
        PrerolTuningMenu,
        SynchroSourceTuningMenu,
        DriftStopperTuningMenu};

    enum {SuspensionResonansTuningMenuSistem=0,
        MaxTorqueTuningMenuSistem,
        TotalRuntimeTuningMenuSistem,
        PrerolTuningMenuSistem,
        SynchroSourceTuningMenuSistem,
        DriftStopperTuningMenuSistem};
#endif

tMenu_Link suspensionResonanceSelectMenuText[2]={
        {"TILT",NULL},
        {"PAN",NULL}
};

enum {TiltResonanceSuspensionResonance=1,
    PanResonanceSuspensionResonance};

enum {TiltResonanceSuspensionResonanceSistem=0,
    PanResonanceSuspensionResonanceSistem};

tMenu_Link profileMenuText[6]={
        {"PROFILE 1", NULL},
        {"PROFILE 2", NULL},
        {"PROFILE 3", NULL},
        {"PROFILE 4", NULL},
        {"PROFILE 5", NULL},
        {"RST CURRENT PROFILE", NULL},
};
enum {ProfileProfileMenu=1,
    RstCurrentProfileProfileMenu=6};

enum {ProfileProfileMenuSistem=0,
    RstCurrentProfileProfileMenuSistem=5};


tMenu_Link switchAxesText[2]={
        {"RIGHT SIDE/HAND", NULL},
        {"LEFT SIDE/HAND", NULL},
};
enum {RigthSideHandSwitchAxesText=1,
    LeftSideHandSwitchAxesText};
enum {RigthSideHandSwitchAxesTextSistem=0,
    LeftSideHandSwitchAxesTextSistem};

#ifdef joyPult

#ifdef joyPultRussian
tMenu_Link lensControlSetup[7]={
        {"MOTOR MAPPING", NULL},
        {"MOTOR MODELS", NULL},
        {"CAMERA MODEL", NULL},
        {"ZOOM SENSITIVITY", NULL},
        {"LENS CALIBRATE", NULL},
        {"ZOOM DRIFT", NULL},
        {"EXTERNAL LENS CONTROL", NULL}
};
enum {MotorMappingLensControlSetup=1,
    MotorModelLensControlSetup,
    CameraModelLensControlSetup,
    ZoomSensLensControlSetup,
    LensCalibrateLensControlSetup,
    ZoomDriftLensControlSetup,
    externalLensControl};

enum {MotorMappingLensControlSetupSistem=0,
    MotorModelLensControlSetupSistem,
    CameraModelLensControlSetupSistem,
    ZoomSensLensControlSetupSistem,
    LensCalibrateLensControlSetupSistem,
    ZoomDriftLensControlSetupSistem,
    externalLensControlSistem};
#else
tMenu_Link lensControlSetup[8]={
        {"MOTOR MAPPING", NULL},
        {"MOTOR MODELS", NULL},
        {"CAMERA MODEL", NULL},
        {"ZOOM SENSITIVITY", NULL},
        {"LENS CALIBRATE", NULL},
        {"ZOOM DRIFT", NULL},
        {"ZIF REVERS", NULL},
        {"EXTERNAL LENS CONTROL", NULL}
};
enum {MotorMappingLensControlSetup=1,
    MotorModelLensControlSetup,
    CameraModelLensControlSetup,
    ZoomSensLensControlSetup,
    LensCalibrateLensControlSetup,
    ZoomDriftLensControlSetup,
    ZIFReversSetup,
    externalLensControl};

enum {MotorMappingLensControlSetupSistem=0,
    MotorModelLensControlSetupSistem,
    CameraModelLensControlSetupSistem,
    ZoomSensLensControlSetupSistem,
    LensCalibrateLensControlSetupSistem,
    ZoomDriftLensControlSetupSistem,
    ZIFReversSistem,
    externalLensControlSistem};
#endif

#else

tMenu_Link lensControlSetup[4]={
        {"MOTOR MAPPING", NULL},
        {"MOTOR MODELS", NULL},
        {"CAMERA MODEL", NULL},
        {"LENS CALIBRATE", NULL},
    };
enum {MotorMappingLensControlSetup=1,
    MotorModelLensControlSetup,
    CameraModelLensControlSetup,
    LensCalibrateLensControlSetup};

enum {MotorMappingLensControlSetupSistem=0,
    MotorModelLensControlSetupSistem,
    CameraModelLensControlSetupSistem,
    LensCalibrateLensControlSetupSistem};


#endif

tMenu_Link lensControlZIF[3]={
        {"ZOOM", NULL},
        {"IRIS", NULL},
        {"FOCUS",NULL},
    };
enum {ZoomLensControl=1,
    IrisLensControl,
    FocusLensControl};

enum {ZoomLensControlSistem=0,
    IrisLensControlSistem,
    FocusLensControlSistem};

tMenu_Link lensControlMotorsText[3]={
        {"MOTOR 1", NULL},
        {"MOTOR 2", NULL},
        {"MOTOR 3",NULL}
};
enum {MotorLensControlMotors1=1,
    MotorLensControlMotors2,
    MotorLensControlMotors3};

enum {MotorLensControlMotors1Sistem=0,
    MotorLensControlMotors2Sistem,
    MotorLensControlMotors3Sistem};

tMenu_Link lensControlMotorsTypesText[2]={
        {"HEDEN M21VE", NULL},
        {"HEDEN M26VE", NULL}

};

enum {HEDENM21veMotorsTypes=1,
    HEDENM26veMotorsTypes};

enum {HEDENM21veMotorsTypesSistem=0,
    HEDENM26veMotorsTypesSistem};

tMenu_Link lensControlCameraStartText[8]={
        {"ARRI ALEXA", NULL},
        {"RED ONE", NULL},
        {"RED SCARLET", NULL},
        {"RED EPIC", NULL},
        {"CANON LENS", NULL},
        {"FUJINON LENS", NULL},
        {"FRONT", NULL},
        {"LEVEL", NULL}

};

enum {ArriAlexaLensControlCamera=1,
    RedOneLensControlCamera,
    RedScarletLensControlCamera,
    RedEpicLensControlCamera,
    CanonLensLensControlCamera,
    FujinonLensLensControlCamera,
    FrontLensControlCamera,
    LevelLensControlCamera};

enum {ArriAlexaLensControlCameraSistem=0,
    RedOneLensControlCameraSistem,
    RedScarletLensControlCameraSistem,
    RedEpicLensControlCameraSistem,
    CanonLensLensControlCameraSistem,
    FujinonLensLensControlCameraSistem,
    FrontLensControlCameraSistem,
    LevelLensControlCameraSistem};

tMenu_Link lensControlZoomSenseText[4]={
        {"LINEAR", NULL},
        {"CUBIC", NULL},
        {"QUADRATIC", NULL},
        {"EQUALIZER", NULL},
};
enum {LinLensControlZoomSense=1,
    CubLensControlZoomSense,
    QuadLensControlZoomSense,
    EquLensControlZoomSense};

enum {LinLensControlZoomSenseSistem=0,
    CubLensControlZoomSenseSistem,
    QuadLensControlZoomSenseSistem,
    EquLensControlZoomSenseSistem};

tMenu_Link overslangOnOffText[2]={
        {"OVERSLUNG", NULL},
        {"UNDERSLUNG", NULL}
};

enum {OverslungOnOff=1,
    underslungOnOff};

enum {OverslungOnOffSistem=0,
    underslungOnOffSistem};

tMenu_Link tiltSpiderSelectText[2]={
        {"DEFAULT", NULL},
        {"EDIT", NULL}
};

enum {DefaultTiltSpiderSelect=1,
    EditTiltSpiderSelect};
enum {DefaultTiltSpiderSelectSistem=0,
    EditTiltSpiderSelectSistem};

tMenu_Link panSpiderSelectText[3]={
        {"DEFAULT", NULL},
        {"EDIT", NULL},
        {"DISABLE", NULL}
};
enum {DefaultPanSpiderSelect=1,
    EditPanSpiderSelect,
    DisablePanSpiderSelect};
enum {DefaultPanSpiderSelectSistem=0,
    EditPanSpiderSelectSistem,
    DisablePanSpiderSelectSistem};

char textDopReal[20] = "DOP REAL CAMID:";

tMenu_Link syncroSourceText[3]={
        {"SYNCHRO", NULL},
        {"PANBAR", NULL},
        {&textDopReal[0], NULL}
};
enum {SynchroSyncroSource=1,
    PanbarSyncroSource};
enum {SynchroSyncroSourcesSistem=0,
    PanbarSyncroSourceSistem};

tMenu_Link axisTurnsText[4]={
        {"PAN  : ", NULL},
        {"DUTCH: ", NULL},
        {"TILT : ", NULL},
        {"AUTO ", NULL}

};

//�������� �� ��� �������� �����

LCD_Menu* pMain_Menu;
LCD_Menu* pSub_Menu1;
LCD_Set_Koeff* pSet_Koeff;
LCD_Koeff* pBright_Set;
LCD_Menu* pTiltLimit_Menu;
LCD_Menu* p_PDT_Menu_Eq;
LCD_Equalizer* pEqualizerPan;
LCD_Equalizer* pEqualizerDutch;
LCD_Equalizer* pEqualizerTilt;
LCD_Equalizer* pEqualizerZoom;
LCD_Cell* pCell_Motion;
LCD::Limits::LimitMenu *limitsMenuPointer;
SecretMenu* secretMenuPointer;
SecretMenuView* secretMenuViewPointer;
LCD_Menu* secretMenuSelectorPointer;
LCD_Menu* setupMenuPointer;
LCD_Menu* tuningMenuPointer;
LCD_Menu* suspResonanceSelectPointer;
LCD_Menu* startFluidSelectPointer;
ProfileSelectMenu* profileMenuPointer;
SwitchAxsesMenu* switchAxesPointer;
LCD_Menu* lensControlMenuPointer;
LCD_Menu* lensControlObjectiveMenuPointer;
LCD_Menu* lensControlObjectiveMenySelectPointer;
LCD_Menu* lensControlZifSetupPointer;
LCD_Menu* lensControlMotorsSetupPointer;
LCD_Menu* lensCreatMenuPointer;
SelectMenu* lensControlCameraStartPointer;
SelectMenu* lensControlZoomSensePointer;
SetZoomDrift*   lensControlZoomDriftPointer;
SelectMenu* setupOverslangActivatePointer;
SelectMenu* syncroSorcePointer;
SwitchMotorTypeMenu* lensControlMotorTypeSelectPointer;
SwitchMotorAction* lensControlMotorActionSelectPointer;
LCD_Main* mainScreenPointer;
HourMeterMenu* hourMeterMenuPointer;
LCD::ZIFRevers * zifReversMenuPointer;
LCD::LCDWheelMenu* wheelMenuPointer;
LCD::InfoMenu::UserInfoMenu* userIfoMenuPointer;
SelectMenuSpiderSelect* tiltSpiderSelectMenuPointer;
SelectMenuSpiderSelect* panSpiderSelectMenuPointer;
SetMaxTorque* setMaxTorqueMenuPointer;
//SetStartFluid* setStartFluidPointer;
SetJoyDeadZone* setJoyDeadZoneMenuPointer;
SetMotionPrerol* setMotionPrerolPointer;
FolowingModeMenu* folowingModePointer;
LCD::TurnsViewMenu* axisTurnsViewMenuPointer;
LCD::Form::ShakerRNDMenu* shakerRNDMenuPointer;

//EE_Working EE_Work;

TrackLedState tracksLed[6]=
{
        {TRACK_LED_DN,MOTION_LED1},
        {TRACK_LED_DN,MOTION_LED2},
        {TRACK_LED_DN,MOTION_LED3},
        {TRACK_LED_DN,MOTION_LED4},
        {TRACK_LED_DN,MOTION_LED5},
        {TRACK_LED_DN,MOTION_LED6}
};
MotionButtonControl motionLogicController(&tracksLed[0], LED9, *p_pult);

//��������� �� ���������� ���� �������� ���� (�������������)
LCD_Listener* pDispl;


void pultIndikator_Task(Pult* point_pult)
{
    motionLogicController.setMotionSynchroModule(&(ExtrSyncroization::ExtrenalDevieExchDriver::motionTransmiter));
    watchDogTimer.registerKey(WD_KEY3);
//  EE_Working::cleanEEPROM();
    EE_Working::refactoringEEPROM();
    EE_Working::getProfile();
    p_pult = point_pult;


    //������ ������� �������� ����
    //������������
    LCD_Set_Koeff Set_Koeff(Main_Menu_Link[0].Name, 6, 6);
    pSet_Koeff = &Set_Koeff;
    Main_Menu_Link[0].pPointSub = pSet_Koeff;

    //�������
    LCD_Koeff Bright_Set((char*)"","BRIGHTNESS", 11);
    pBright_Set = &Bright_Set;
//  Main_Menu_Link[num++].pPointSub = pBright_Set;

    //������������ �������
    LCD_Menu TiltLimit_Menu(Main_Menu_Link[2].Name, Txt_MenuTiltLimit, 5, 0, 5);
    pTiltLimit_Menu = &TiltLimit_Menu;
    LCD::Limits::LimitMenu limitsMenu(&mainScreenPointer);
    limitsMenuPointer=&limitsMenu;
//  Main_Menu_Link[2].pPointSub = pTiltLimit_Menu;
    Main_Menu_Link[2].pPointSub = limitsMenuPointer;

    SecretMenu secretMenu("Secret Menu",10,5 );
    secretMenu.updateDataFromPult();
    secretMenuPointer=&secretMenu;

    SecretMenuView secretMenuView("Secret Menu View",10,5 );
    secretMenuView.updateDataFromPult();
    secretMenuViewPointer=&secretMenuView;

    CheckMenu checkMenu;

    LCD_Menu secretMenuSelector("SELECT",  secretMenuText, 3, 0, 3);
    secretMenuSelectorPointer = &secretMenuSelector;
    secretMenuText[0].pPointSub=secretMenuPointer;
    secretMenuText[1].pPointSub=secretMenuViewPointer;
    secretMenuText[2].pPointSub=&checkMenu;

    SwitchAxsesMenu switchAxesMenu("JOYSTICK PLACEMENT", switchAxesText, 2, 0, 2);
    switchAxesPointer = &switchAxesMenu;

    ProfileSelectMenu profileMenu("USER PROFILES",  profileMenuText, 6, 0, 6);
    profileMenuPointer = &profileMenu;

    SelectMenuOverslang setupOverslangActivate("UNDERSLUNG/OVERSLUNG", overslangOnOffText, 2, 2,EE_LC_OVERSLANG_ON_OFF);
    setupOverslangActivatePointer=&setupOverslangActivate;
    setupOverslangActivatePointer->updateFromEEPROM();

    LCD::TurnsViewMenu axisTurnsViewMenu("DRIFT STOPPER", axisTurnsText, 7, 7);
    axisTurnsViewMenuPointer=&axisTurnsViewMenu;

    LCD::LCDWheelMenu wheelMenu;
    wheelMenuPointer=&wheelMenu;

    SelectMenuSpiderSelect tiltSpiderSelectMenu("SUSPENSION RESONANCE", tiltSpiderSelectText, 2, 2,EE_LC_TILT_SUSPENSION_RES_TYPE,EE_LC_TILT_SUSPENSION_EDIT_FREQ,SUSPENSION_RESONANCE_CHANNEL_TILT);
    tiltSpiderSelectMenuPointer=&tiltSpiderSelectMenu;
    tiltSpiderSelectMenuPointer->updateFromEEPROM();

    SelectMenuSpiderSelect panSpiderSelectMenu("SUSPENSION RESONANCE", panSpiderSelectText, 3, 3,EE_LC_PAN_SUSPENSION_RES_TYPE,EE_LC_PAN_SUSPENSION_EDIT_FREQ,SUSPENSION_RESONANCE_CHANNEL_PAN);
    panSpiderSelectMenuPointer=&panSpiderSelectMenu;
    panSpiderSelectMenuPointer->updateFromEEPROM();

    SetMaxTorque setMaxTorqueMenu("MAX TORQUE");
    setMaxTorqueMenuPointer=&setMaxTorqueMenu;
    setMaxTorqueMenu.updateFromEeprom();

    /*SetStartFluid setStartFluidMenu("FRONT RAMP");
    setStartFluidPointer=&setStartFluidMenu;
    setStartFluidMenu.updateFromEeprom();*/
#ifdef joyPult
    SetJoyDeadZone setJoyDeadZoneMenu("DEADZONE");
    setJoyDeadZoneMenuPointer=&setJoyDeadZoneMenu;
    setJoyDeadZoneMenu.updateFromEeprom();
#endif

    LCD_Menu suspResonanceSelectMenu("SUSPENSION RESONANCE",  suspensionResonanceSelectMenuText, 2, 0, 2    );
    suspResonanceSelectPointer=&suspResonanceSelectMenu;
    suspensionResonanceSelectMenuText[0].pPointSub=&tiltSpiderSelectMenu;
    suspensionResonanceSelectMenuText[1].pPointSub=&panSpiderSelectMenu;

    HourMeterMenu  hourMeterMenu("TOTAL RUNTIME");
    hourMeterMenuPointer=&hourMeterMenu;
    LCD::InfoMenu::UserInfoMenu userIfoMenu;
    userIfoMenuPointer=&userIfoMenu;
    HourMeter_rawValue=EE_Working::ReadCalibrationData(EE_CAL_HOURS);
    if(HourMeter_rawValue==0xFFFFFFFF)
    {
        HourMeter_rawValue=0;
        EE_Working::WriteCalibrationData(EE_CAL_HOURS, HourMeter_rawValue);
    }

    SetMotionPrerol motionPrerolMenu("PREROL");
    setMotionPrerolPointer=&motionPrerolMenu;
    motionPrerolMenu.updateFromEeprom();

    SelectMenuSynchronization syncroSouce("SYNCHRO SOURCE", syncroSourceText, 3, 3,EE_LC_MOTION_SYNCRO_SOURCE);
    syncroSorcePointer=&syncroSouce;
    syncroSorcePointer->updateFromEEPROM();

    LCD::Form::ShakerRNDMenu shakerRNDMenu;
    shakerRNDMenuPointer = &shakerRNDMenu;


#ifdef joyPult
    LCD_Menu tuningMenu("TUNING",  tuningMenuText, 8, 0, 8);
    tuningMenuPointer=&tuningMenu;
    tuningMenuText[0].pPointSub=&suspResonanceSelectMenu;
    tuningMenuText[1].pPointSub=&setMaxTorqueMenu;
    tuningMenuText[2].pPointSub=&setJoyDeadZoneMenu;
    tuningMenuText[3].pPointSub=&userIfoMenu;
    tuningMenuText[4].pPointSub=&motionPrerolMenu;
    tuningMenuText[5].pPointSub=&syncroSouce;
    tuningMenuText[6].pPointSub=&axisTurnsViewMenu;
    tuningMenuText[7].pPointSub=&shakerRNDMenu;
#else
    LCD_Menu tuningMenu("TUNING",  tuningMenuText, 7, 0, 7  );
    tuningMenuPointer=&tuningMenu;
    tuningMenuText[0].pPointSub=&suspResonanceSelectMenu;
    tuningMenuText[1].pPointSub=&setMaxTorqueMenu;
    tuningMenuText[2].pPointSub=&userIfoMenu;
    tuningMenuText[3].pPointSub=&motionPrerolMenu;
    tuningMenuText[4].pPointSub=&syncroSouce;
    tuningMenuText[5].pPointSub=&axisTurnsViewMenu;
    tuningMenuText[6].pPointSub=&shakerRNDMenu;
#endif

    FolowingModeMenu folowingModeMenu("FOLLOWING MODE");
    folowingModePointer=&folowingModeMenu;
    folowingModeMenu.updateFromEeprom();
#ifdef joyPult
    LCD_Menu setupMenu("SETUP",  setupMenuText, 8, 0, 8 );

    Main_Menu_Link[1].pPointSub = &setupMenu;
    setupMenuText[0].pPointSub=pBright_Set;
    setupMenuText[1].pPointSub=&profileMenu;
    setupMenuText[2].pPointSub=&switchAxesMenu;
    setupMenuText[3].pPointSub=&setupOverslangActivate;
    setupMenuText[4].pPointSub=&wheelMenu;
    setupMenuText[6].pPointSub=&tuningMenu;
    setupMenuText[7].pPointSub=&folowingModeMenu;
#else
    LCD_Menu setupMenu("SETUP",  setupMenuText, 6, 0, 6 );
    Main_Menu_Link[1].pPointSub = &setupMenu;
    setupMenuText[0].pPointSub=pBright_Set;
    setupMenuText[1].pPointSub=&profileMenu;
    setupMenuText[2].pPointSub=&setupOverslangActivate;
    setupMenuText[3].pPointSub=&wheelMenu;
    setupMenuText[4].pPointSub=&tuningMenu;
    setupMenuText[5].pPointSub=&folowingModeMenu;
#endif
    setupMenuPointer=&setupMenu;
    //Motion
    LCD_Cell Cell_Motion((char*)"Insert module",30,90,250,60);
    Cell_Motion.Active_Style.Cell_Color = ClrLinen;
    Cell_Motion.Active_Style.Font_Color = ClrWhite;
    Cell_Motion.Active_Style.Border_Color = ClrWhite;
    Cell_Motion.Active_Style.pFont = g_psFontCmsc32;
    Cell_Motion.SetText((char*)"Insert module");
    pCell_Motion = &Cell_Motion;
    Main_Menu_Link[3].pPointSub = pCell_Motion;


    //����������
    //���� ����������� (3 ��)
    LCD_Equalizer EqualizerPan((char*)"Pan",7, EE_PK_Line_0,PanJoystickEqualaser); //"EQUALIZER"
    pEqualizerPan = &EqualizerPan;
#ifdef USAEdition
    LCD_Equalizer EqualizerDutch((char*)"Roll",7, EE_DK_Line_0,DutchJoystickEqualaser); //"EQUALIZER"
#else
    LCD_Equalizer EqualizerDutch((char*)"Dutch",7, EE_DK_Line_0,DutchJoystickEqualaser); //"EQUALIZER"
#endif
    pEqualizerDutch = &EqualizerDutch;
    LCD_Equalizer EqualizerTilt((char*)"Tilt",7, EE_TK_Line_0,TiltJoystickEqualaser); //"EQUALIZER"
    pEqualizerTilt = &EqualizerTilt;
#ifdef  joyPult
    LCD_Equalizer EqualizerZoom((char*)"Zoom",7, EE_ZK_Line_0,ZoomJoystickEqualaser); //"EQUALIZER"
    pEqualizerZoom = &EqualizerZoom;
    //������ ������� ������ ��� �����������
    LCD_Menu PDT_Menu_Eq((char*)"Select Axis", PDTMenu_link,4,0,4);
    p_PDT_Menu_Eq = &PDT_Menu_Eq;
    Main_Menu_Link[4].pPointSub = p_PDT_Menu_Eq;
#else
    LCD_Menu PDT_Menu_Eq((char*)"Select Axis", PDTMenu_link,3,0,3);
    p_PDT_Menu_Eq = &PDT_Menu_Eq;
    Main_Menu_Link[4].pPointSub = p_PDT_Menu_Eq;
#endif
    //todo
    //"LENS CONTROL"
#ifdef  joyPult
    #ifdef joyPultRussian
        LCD_Menu lensControlMenu("LENS CONTROL",lensControlSetup ,7,0,7);
    #else
        LCD_Menu lensControlMenu("LENS CONTROL",lensControlSetup ,8,0,8);
        LCD::ZIFRevers  zifReversMenu;
        zifReversMenuPointer=&zifReversMenu;
        lensControlSetup[ZIFReversSistem].pPointSub=&zifReversMenu;
    #endif
    lensControlMenu.readEEPROM();
    lensControlMenuPointer=&lensControlMenu;

    SelectMenuZoomSense lensControlZoomSense("ZOOM SENSE", lensControlZoomSenseText, 4, 4,EE_LC_ZOOM_SENSE);
    lensControlZoomSense.updateFromEEPROM();
    lensControlZoomSensePointer=&lensControlZoomSense;

    SetZoomDrift lensControlZoomDrift("ZOOM DRIFT");
    lensControlZoomDrift.updateFromEeprom();
    lensControlZoomDriftPointer=&lensControlZoomDrift;

    lensControlSetup[ZoomSensLensControlSetupSistem].pPointSub=&lensControlZoomSense;
    lensControlSetup[ZoomDriftLensControlSetupSistem].pPointSub=&lensControlZoomDrift;

#else
    LCD_Menu lensControlMenu("LENS CONTROL",lensControlSetup ,4,0,4);
    lensControlMenu.readEEPROM();
    lensControlMenuPointer=&lensControlMenu;

#endif

    LCD_Menu_lcZIF lensControlZifSetup("MOTOR MAPPING",lensControlZIF ,3,0,3,lensControlMotorsText,3);
    lensControlZifSetupPointer=&lensControlZifSetup;

    LCD_Menu_lcMotrors lensControlMotorsSetup("MOTOR MODELS",lensControlMotorsText ,3,0,3,lensControlMotorsTypesText,2);
    lensControlMotorsSetupPointer=&lensControlMotorsSetup;

    SelectMenuCameraStart lensControlCameraStart("CAMERA MODEL", lensControlCameraStartText, 8, 8, EE_LC_CAMERA_START);
    lensControlCameraStart.updateFromEEPROM();
    lensControlCameraStartPointer=&lensControlCameraStart;

    lensControlSetup[MotorMappingLensControlSetupSistem].pPointSub=&lensControlZifSetup;
    lensControlSetup[MotorModelLensControlSetupSistem].pPointSub=&lensControlMotorsSetup;
    lensControlSetup[CameraModelLensControlSetupSistem].pPointSub=&lensControlCameraStart;

    SwitchMotorTypeMenu lensControlMotorTypeSelect("SELECT TYPE",lensControlMotorsTypesText,2);
    lensControlMotorTypeSelectPointer=&lensControlMotorTypeSelect;

    SwitchMotorAction lensControlMotorActionSelect("SELECT MOTOR",lensControlMotorsText,3);
    lensControlMotorActionSelectPointer=&lensControlMotorActionSelect;

    lensControlZIF[ZoomLensControlSistem].pPointSub=&lensControlMotorActionSelect;
    lensControlZIF[IrisLensControlSistem].pPointSub=&lensControlMotorActionSelect;
    lensControlZIF[FocusLensControlSistem].pPointSub=&lensControlMotorActionSelect;

    lensControlMotorsText[MotorLensControlMotors1Sistem].pPointSub=&lensControlMotorTypeSelect;
    lensControlMotorsText [MotorLensControlMotors2Sistem].pPointSub=&lensControlMotorTypeSelect;
    lensControlMotorsText[MotorLensControlMotors3Sistem].pPointSub=&lensControlMotorTypeSelect;

    Main_Menu_Link[5].pPointSub =&lensControlMenu;

    LCD_Menu Main_Menu((char*)"MAIN MENU", Main_Menu_Link,6,1,6);
    pMain_Menu = &Main_Menu;

    UInt32 vax=0;

    watchDogTimer.useKey(WD_KEY3);

    for(UInt16 x=0;x!=320;x++)
        for(UInt16 y=0;y!=240;y++)  {
            vax=0;
            vax|=buLogo[3*(y*320+x)+2];
            vax=vax<<8;
            vax|=buLogo[3*(y*320+x)+1];
            vax=vax<<8;
            vax|=buLogo[3*(y*320+x)+0];
            DpyPixelDraw(&g_sKentec320x240x16_SSD2119, x, y, vax) ;
            watchDogTimer.useKey(WD_KEY3);
            }

    for(UInt32 i=0;i!=200;i++)  {
        Task_sleep(10);
        watchDogTimer.useKey(WD_KEY3);
    }

    LCD_Main Main_Screen((char*)"Main_Screen");
/*  pDispl = &userIfoMenu;
    pDispl->Focused=true;*/

    pDispl = &Main_Screen;
    mainScreenPointer=&Main_Screen;

    Main_Screen.Draw();
    Main_Screen.Focused = false;

    volatile UInt32 lastTimeToStart=0;
    volatile UInt32 newTTS=0;
    volatile UInt32 rawValue=0;

    while(1) {
        watchDogTimer.useKey(WD_KEY3);
        Task_sleep(10);

        pDispl->Listener();
        motionLogicController.clocking();
        userIfoMenu.calcelateRuntime();

        }
    }

PultButtonStates LCD_Listener::getButtonState(PultButtons pultButton)
{
    btn = p_pult->getButton(pultButton);
    if (btn->state == HOLD)
    {
        btn->buttonFix = false;
        return HOLD;
    }
    if ((btn->state == PRESSED)&&(btn->buttonFix))
    {
        GPIO_write(Debug_Pin_PN5, Board_PIN_LOW);
        btn->buttonFix = false;
        return PRESSED;
    }
    return RELESASED;
}

LCD_Main::LCD_Main(char* pName):
Cell_Main_Counter((char*) "", 1,1,10,10),
Cell_Main_Counter_Right((char*) "", 1,1,10,10),
Cell_Main_Counter_Left((char*) "", 1,1,10,10),
Cell_P_Name((char*) "", 1,1,10,10),
Cell_P_Inf((char*) "", 1,1,10,10),
Cell_D_Name((char*) "", 1,1,10,10),
Cell_D_Inf((char*) "", 1,1,10,10),
Cell_T_Name((char*) "", 1,1,10,10),
Cell_T_Inf((char*) "", 1,1,10,10),
#ifdef joyPult
Cell_Z_Name((char*) "", 1,1,10,10),
Cell_Z_Inf((char*) "", 1,1,10,10),
#endif
Cell_Joyst_State((char*) "", 1,1,10,10),
Cell_Motor_State((char*) "", 1,1,10,10),
Cell_GV_Acc((char*) "", 1,1,10,10),
ErrorLine((char*) "", 1,1,10,10),
tiltLimiterState((char*) "", 1,1,10,10),
panLimiterState((char*) "", 1,1,10,10),
rollLimiterState((char*) "", 1,1,10,10),
headVoltage((char*) "", 1,1,10,10),
currentProfile((char*) "", 1,1,10,10),
motionLimitTime((char*) "", 1,1,10,10),
motionCurrentTime((char*) "", 1,1,10,10),
motionTrackNumber((char*) "", 1,1,10,10),
motionState((char*) "", 1,1,10,10),
motionPlayMode((char*) "", 1,1,10,10),
motionMixMode((char*) "", 1,1,10,10)
{
    panLimiterStateFlag=TL_RESET;
    rollLimiterStateFlag=TL_RESET;
    tiltLimiterStateFlag=TL_RESET;
    currentPanDirection=PAN_AXIS_STOP;
    currentValue=0.0;
    inMotionMode=false;
    motionLastState=0xFF;
    motionLastTrackNumber=0xFF;
    motionMixModeState=MOTION_MIX_COUNT;
    selectModeCounter=0;
    limitTimeLastState=0xFFFFFFFF;
    counterTimeLastState=0xFFFFFFFF;

    counterTimeString[1]=0;
    limitTimeString[1]=0;
    motionStateString[1]=0;
    motionTrackNumberString[1]=0;
    motionPlayModeString[1]=0;
    motionMixModeString[1]=0;
    lastVoltage=0xFFFF;
    //Draw();
}
void LCD_Main::renderDateString(char* buffer, UInt8 bufLen,UInt32 seconds)
{
    UInt32 hour=0;
    UInt8 min=0;

    hour=(UInt32)(seconds/3600);
    seconds-=(hour*3600);

    min=(UInt8)(seconds/60);
    seconds-=(min*60);
    if(hour>=1000){hour=999;}
    if(min>=60){min=59;}
    if(seconds>=60){seconds=59;}
    sprintf(buffer,"%3d:%2d:%2d",hour,min,seconds);
    buffer[bufLen-1]=0;
}
void LCD_Main::updateLimitTime()
{
    UInt32 sec=0;
    UInt32 timeCorrection=1;

    timeCorrection=(UInt32)(1000/p_pult->getMotionModeTransferDelay());
    if(timeCorrection<=0) {   timeCorrection=1; }

    if(p_pult->getMotionSubsystem()->isReady())
    {
        sec=(UInt32)(p_pult->getMotionSubsystem()->getFreeSpace()/timeCorrection);
        strncpy(limitTimeString,"Free:   ",8);
    }
    if(p_pult->getMotionSubsystem()->isRec())
    {
        sec=((UInt32)(p_pult->getMotionSubsystem()->getCurrentTrack()->getMaxCount()/timeCorrection))   -
            ((UInt32)(p_pult->getMotionSubsystem()->getCurrentTrack()->iterator     /timeCorrection));
        strncpy(limitTimeString,"Free:   ",8);
    }
    if(p_pult->getMotionSubsystem()->isPlay()||p_pult->getMotionSubsystem()->isPaused())
    {
        sec=(UInt32)(p_pult->getMotionSubsystem()->getCurrentTrack()->getMaxCount()/timeCorrection);
        strncpy(limitTimeString,"Len:    ",8);
    }

    if(limitTimeLastState!=sec)
    {
        renderDateString(&limitTimeString[8],10,sec);
        limitTimeLastState=sec;
        motionLimitTime.ReDraw();
    }
}

void LCD_Main::updateMotionMixModeStr()
{
    MotionMixMode tmp=p_pult->getMotionSubsystem()->getMixMode();
    switch(tmp)
    {
        case MOTION_MIX_TRACK_ONLY:
            strncpy(motionMixModeString, "T/O ",5);
            break;
        case MOTION_MIX_PDTZ:
            strncpy(motionMixModeString, "Mix S",6);
            break;
        case MOTION_MIX_ALL:
            strncpy(motionMixModeString, "Mix A",6);
            break;
        default:
            strncpy(motionMixModeString, "T/O ",5);
            tmp=MOTION_MIX_TRACK_ONLY;
            break;
    }
    motionMixModeString[9]=0;

    if(tmp!=motionMixModeState)
    {
        motionMixModeState=tmp;
        motionMixMode.ReDraw();
    }
}
void LCD_Main::updateMotionPlayModeStr()
{
    MotionPlayMode pm=p_pult->getMotionSubsystem()->getPlayMode();
    strncpy(motionPlayModeString, " ",2);
    switch(pm)
    {
        case MOTION_PLAY_MODE_ONCE:
//          strncpy(motionPlayModeString, "Once ",6);
            break;
        case MOTION_PLAY_MODE_CIRCLE:
//          strncpy(motionPlayModeString, "Repit",6);
            break;
        case MOTION_PLAY_MODE_PP:
//          strncpy(motionPlayModeString, "PP",3);
            break;
        default:
//          strncpy(motionPlayModeString, "Once ",6);
            pm=MOTION_PLAY_MODE_ONCE;
            break;
    }
    motionPlayModeString[7]=0;

    if(pm!=motionPlayModeState)
    {
        motionPlayModeState=pm;
        motionPlayMode.ReDraw();
    }

}
void LCD_Main::updateCounterTime()
{
    UInt32 sec=0;
    UInt32 timeCorrectionTrack=1;
    UInt32 timeCorrectionSystem=1;

    timeCorrectionSystem=(UInt32)(1000/p_pult->getMotionModeTransferDelay());
    if(timeCorrectionSystem<=0){timeCorrectionSystem=1;}

    if(p_pult->getMotionSubsystem()->isReady())
    {
        if(  !((UInt32)(p_pult->getMotionSubsystem()->getTrackTransferDelay(&timeCorrectionTrack))) )
        {
            timeCorrectionTrack=1;
        }
        if(timeCorrectionTrack<=0){ timeCorrectionTrack=1;   }
        timeCorrectionTrack=1000/timeCorrectionTrack;
        if(timeCorrectionTrack<=0){ timeCorrectionTrack=1;   }


        sec=(UInt32)(p_pult->getMotionSubsystem()->getCurrentTrack()->meta.getPointCount()/timeCorrectionTrack);
        strncpy(counterTimeString,"Track:  ",8);
    }
    if(p_pult->getMotionSubsystem()->isRec())
    {
        sec=(UInt32)(p_pult->getMotionSubsystem()->getCurrentTrack()->iterator/timeCorrectionSystem);
        strncpy(counterTimeString,"Cur:    ",8);
    }
    if(p_pult->getMotionSubsystem()->isPlay()||p_pult->getMotionSubsystem()->isPaused())
    {
        sec=(UInt32)(p_pult->getMotionSubsystem()->getCurrentTrack()->iterator/timeCorrectionSystem);
        strncpy(counterTimeString,"Cur:    ",8);
    }

    if(counterTimeLastState!=sec)
    {
        renderDateString(&counterTimeString[8],10,sec);
        counterTimeLastState=sec;
        motionCurrentTime.ReDraw();
    }
}
void LCD_Main::updateMotionState()
{
    UInt8 curState=0xFF;
    switch(motionLogicController.getState())
    {

        case MOTION_MENU_READY_TO_REC:
            strncpy(motionStateString,"READY",6);
            curState=0;
            break;
        case MOTION_MENU_REC:
            strncpy(motionStateString,"REC",4);
            curState=1;
            break;
        case MOTION_MENU_PLAY:
            strncpy(motionStateString,"PLAY",5);
            curState=2;
            break;
        case MOTION_MENU_REVERS_PLAY:
            strncpy(motionStateString,"R.PLAY",7);
            curState=3;
            break;
        case MOTION_MENU_REVERS_PLAY_PAUSE:
        case MOTION_MENU_PLAY_PAUSE:
            strncpy(motionStateString,"PAUSE",6);
            curState=4;
            break;
        case MOTION_MENU_WAIT_READY_TO_PLAY:
            strncpy(motionStateString,"ANGLES",7);
            curState=5;
            break;
        case MOTION_MENU_READY_TO_PLAY:
            strncpy(motionStateString,"READY",6);
            curState=6;
            break;
        case MOTION_MENU_PREROL:
            strncpy(motionStateString,"PREROL",7);
            curState=8;
            break;
        case MOTION_MENU_READY:
        default:
            strncpy(motionStateString,"WAITING",8);
            curState=7;
            break;
    }
    motionStateString[9]=0;


    if(curState!=motionLastState)
    {
        counterTimeLastState=0xFFFFFFFF; //Update time strings
        limitTimeLastState=0xFFFFFFFF;
        motionLastState=curState;
        motionState.ReDraw();
    }

}
void LCD_Main::updateMotionTrackNumber()
{
    UInt8 curTrackNum=p_pult->getMotionSubsystem()->getCurrentTrackID();
    sprintf(motionTrackNumberString,"Track:%d",curTrackNum+1);
    motionTrackNumberString[9]=0;
    if(curTrackNum!=motionLastTrackNumber)
    {
        motionLastTrackNumber=curTrackNum;
        motionTrackNumber.ReDraw();
    }

}

inline void checkEEpromCalibrationValue(byte b)
{
    if(EE_Working::ReadCalibrationData(b)==0xFFFF)
    {
        EE_Working::WriteCalibrationData(b,1860);
    }
}

void LCD_Main::sendSetupValuesByPult()
{
    p_pult->setJoystickPresetId(PanJoystickPreset,  EE_Working::Read(PAN_FUNC_VALUE     ));
    p_pult->setJoystickPresetId(DutchJoystickPreset,EE_Working::Read(DATCH_FUNC_VALUE   ));
    p_pult->setJoystickPresetId(TiltJoystickPreset, EE_Working::Read(TILT_FUNC_VALUE    ));
    p_pult->setJoystickPresetId(ZoomJoystickPreset, EE_Working::Read(EE_LC_ZOOM_SENSE   ));


    switch(EE_Working::Read(EE_LC_CAMERA_START))
    {
        case 0:
        case 1:
        case 3:
        case 4:
        case 5:
        case 6:p_pult->setCameraStartOptions(CAMERA_START_FRONT);break;

        case 2:
        case 7:p_pult->setCameraStartOptions(CAMERA_START_LEVEL);break;

        default:p_pult->setCameraStartOptions(CAMERA_START_FRONT);break;

    }

    checkEEpromCalibrationValue(EE_CAL_OFFSET_MAIN_PAN);
    checkEEpromCalibrationValue(EE_CAL_OFFSET_MAIN_DUTCH);
    checkEEpromCalibrationValue(EE_CAL_OFFSET_MAIN_TILT);
    checkEEpromCalibrationValue(EE_CAL_OFFSET_MAIN_ZOOM);
    checkEEpromCalibrationValue(EE_CAL_OFFSET_EXT_PAN);
    checkEEpromCalibrationValue(EE_CAL_OFFSET_EXT_DUCTH);
    checkEEpromCalibrationValue(EE_CAL_OFFSET_EXT_TILT);
    checkEEpromCalibrationValue(EE_CAL_OFFSET_EXT2_DUCTH);

    p_pult->setCalibrationOffset(CALIB_OFFSET_MAIN_PAN,     EE_Working::ReadCalibrationData(EE_CAL_OFFSET_MAIN_PAN));
    p_pult->setCalibrationOffset(CALIB_OFFSET_MAIN_DUTCH,   EE_Working::ReadCalibrationData(EE_CAL_OFFSET_MAIN_DUTCH));
    p_pult->setCalibrationOffset(CALIB_OFFSET_MAIN_TILT,    EE_Working::ReadCalibrationData(EE_CAL_OFFSET_MAIN_TILT));
    p_pult->setCalibrationOffset(CALIB_OFFSET_MAIN_ZOOM,    EE_Working::ReadCalibrationData(EE_CAL_OFFSET_MAIN_ZOOM));
    p_pult->setCalibrationOffset(CALIB_OFFSET_EXT_PAN,      EE_Working::ReadCalibrationData(EE_CAL_OFFSET_EXT_PAN));
    p_pult->setCalibrationOffset(CALIB_OFFSET_EXT_DUTCH,    EE_Working::ReadCalibrationData(EE_CAL_OFFSET_EXT_DUCTH));
    p_pult->setCalibrationOffset(CALIB_OFFSET_EXT_TILT,     EE_Working::ReadCalibrationData(EE_CAL_OFFSET_EXT_TILT));
    p_pult->setCalibrationOffset(CALIB_OFFSET_EXT2_DUTCH,   EE_Working::ReadCalibrationData(EE_CAL_OFFSET_EXT2_DUCTH));
}
void LCD_Main::Draw() //���������
{

    sendSetupValuesByPult();
    ClearDisp(); //�������� ������
    if(!inMotionMode)
    {

        p_pult->resetCentralButtons();
        // ======== ����� ==============
        //������� ����������
        Cell_Main_Counter.Active_Style.Cell_Color = ClrDarkSlateBlue;
        Cell_Main_Counter.Active_Style.pFont = g_psFontCmsc48;
        Cell_Main_Counter.Rounded = true;
        Cell_Main_Counter.FastDraw(126,90,70,90, (char*)"0.0", Cell_Active);
        //������� ��������� ������ (>)
        Cell_Main_Counter_Right.Active_Style.Cell_Color = ClrDarkSlateBlue;
        Cell_Main_Counter_Right.Active_Style.Cell_Color = Color_Menu_Fon_InActive;
        Cell_Main_Counter_Right.Active_Style.pFont = g_psFontCmsc48;
        Cell_Main_Counter_Right.Active_Style.Border = false;
        Cell_Main_Counter_Right.UnActive_Style.Font_Color = Cell_Main_Counter_Right.UnActive_Style.Cell_Color;
        Cell_Main_Counter_Right.UnActive_Style.Border = false;
        Cell_Main_Counter_Right.FastDraw(205,105,50,50, (char*)">", Cell_UnActive);
        //������� ��������� ����� (<)
        Cell_Main_Counter_Left.Active_Style = Cell_Main_Counter_Right.Active_Style;
        Cell_Main_Counter_Left.UnActive_Style = Cell_Main_Counter_Right.UnActive_Style;
        Cell_Main_Counter_Left.FastDraw(65,105,50,50, (char*)"<", Cell_UnActive);

        Cell_Joyst_State.Active_Style = Cell_Main_Counter_Left.Active_Style;
        Cell_Joyst_State.UnActive_Style = Cell_Main_Counter_Left.UnActive_Style;
        Cell_Joyst_State.Active_Style.pFont = g_psFontCmsc22;
//      Cell_Joyst_State.Active_Style.Cell_Color = ClrGoldenrod;
        Cell_Joyst_State.Active_Style.Cell_Color = Style_MenuActive.Cell_Color;
        Cell_Joyst_State.UnActive_Style.pFont = g_psFontCmsc22;
#ifdef joyPult
        Cell_Joyst_State.FastDraw(60,30,200,30, (char*)"Joysticks OFF", Cell_UnActive);
#else
        Cell_Joyst_State.FastDraw(60,30,200,30, (char*)"All Wheels OFF", Cell_UnActive);
#endif

        Cell_Motor_State.Active_Style = Cell_Joyst_State.Active_Style;
        Cell_Motor_State.UnActive_Style = Cell_Joyst_State.UnActive_Style;
        Cell_Motor_State.FastDraw(60,60,200,30, (char*)"Motors OFF", Cell_UnActive);

        Cell_GV_Acc.Active_Style = Cell_Joyst_State.Active_Style;
        Cell_GV_Acc.UnActive_Style = Cell_Joyst_State.UnActive_Style;
        Cell_GV_Acc.FastDraw(250,105,60,30, (char*)"ACC", Cell_Active);

        ErrorLine.Active_Style=Cell_Joyst_State.Active_Style;
        ErrorLine.Active_Style.Cell_Color= ClrLinen;
        ErrorLine.Active_Style.Text_Centered=false;
        ErrorLine.UnActive_Style= Cell_Joyst_State.UnActive_Style;
        ErrorLine.FastDraw(2,208,316,30, (char*)"", Cell_UnActive);

        panLimiterState.Active_Style = Cell_Joyst_State.Active_Style;
        panLimiterState.UnActive_Style = Cell_Joyst_State.UnActive_Style;
        panLimiterState.Active_Style.Text_Centered=false;
        panLimiterState.FastDraw(5,55,60,30, (char*)"", Cell_Active);

        tiltLimiterState.Active_Style = Cell_Joyst_State.Active_Style;
        tiltLimiterState.UnActive_Style = Cell_Joyst_State.UnActive_Style;
        tiltLimiterState.Active_Style.Text_Centered=false;
        tiltLimiterState.FastDraw(5,85,60,30, (char*)"", Cell_Active);

        rollLimiterState.Active_Style = Cell_Joyst_State.Active_Style;
        rollLimiterState.UnActive_Style = Cell_Joyst_State.UnActive_Style;
        rollLimiterState.Active_Style.Text_Centered=false;
        rollLimiterState.FastDraw(5,155,60,30, (char*)"", Cell_Active);

        headVoltage.Active_Style = Cell_Joyst_State.Active_Style;
        headVoltage.UnActive_Style = Cell_Joyst_State.UnActive_Style;
        headVoltage.Active_Style.Text_Centered=false;
        headVoltage.FastDraw(5,180,80,30, voltageString, Cell_Active);

        updateTiltLimiterCell();



        currentProfile.Active_Style = Cell_Joyst_State.Active_Style;
        currentProfile.UnActive_Style = Cell_Joyst_State.UnActive_Style;
        updatecurrentProfileCell();
        currentProfile.FastDraw(255,30,60,30, profileString, Cell_Active);



        // ======== END ����� ==============
    }
    else
    {
        p_pult->resetCentralButtons();
        // ======== ����� ==============
        //������� ����������
        Cell_Main_Counter.Active_Style.Cell_Color = ClrDarkSlateBlue;
        Cell_Main_Counter.Active_Style.pFont = g_psFontCmsc26;
        Cell_Main_Counter.Rounded = true;
        Cell_Main_Counter.FastDraw(52,35,27,45, (char*)"0.0", Cell_Active);
        //������� ��������� ������ (>)
        Cell_Main_Counter_Right.Active_Style.Cell_Color = ClrDarkSlateBlue;
        Cell_Main_Counter_Right.Active_Style.Cell_Color = Color_Menu_Fon_InActive;
        Cell_Main_Counter_Right.Active_Style.pFont = g_psFontCmsc24;
        Cell_Main_Counter_Right.Active_Style.Border = false;
        Cell_Main_Counter_Right.UnActive_Style.Font_Color = Cell_Main_Counter_Right.UnActive_Style.Cell_Color;
        Cell_Main_Counter_Right.UnActive_Style.Border = false;
        Cell_Main_Counter_Right.FastDraw(85,45,25,25, (char*)">", Cell_UnActive);
        //������� ��������� ����� (<)
        Cell_Main_Counter_Left.Active_Style = Cell_Main_Counter_Right.Active_Style;
        Cell_Main_Counter_Left.UnActive_Style = Cell_Main_Counter_Right.UnActive_Style;
        Cell_Main_Counter_Left.FastDraw(15,45,25,25, (char*)"<", Cell_UnActive);

        Cell_Joyst_State.Active_Style = Cell_Main_Counter_Left.Active_Style;
        Cell_Joyst_State.UnActive_Style = Cell_Main_Counter_Left.UnActive_Style;
        Cell_Joyst_State.Active_Style.Text_Centered=false;
        Cell_Joyst_State.UnActive_Style.Text_Centered=false;

        Cell_Joyst_State.Active_Style.pFont = g_psFontCmsc18;
//      Cell_Joyst_State.Active_Style.Cell_Color = ClrGoldenrod;//Style_MenuActive.Cell_Color;
        Cell_Joyst_State.Active_Style.Cell_Color = Style_MenuActive.Cell_Color;
        Cell_Joyst_State.UnActive_Style.pFont = g_psFontCmsc18;
#ifdef joyPult
        Cell_Joyst_State.FastDraw(5,85,130,20, (char*)"Joysticks OFF", Cell_UnActive);
#else
        Cell_Joyst_State.FastDraw(5,85,130,20, (char*)"All Wheels OFF", Cell_UnActive);
#endif
        Cell_Motor_State.Active_Style = Cell_Joyst_State.Active_Style;
        Cell_Motor_State.UnActive_Style = Cell_Joyst_State.UnActive_Style;
        Cell_Motor_State.FastDraw(5,110,130,20, (char*)"Motors OFF", Cell_UnActive);


        ErrorLine.Active_Style=Cell_Joyst_State.Active_Style;
        ErrorLine.Active_Style.pFont=g_psFontCmsc22;
        ErrorLine.UnActive_Style.pFont=g_psFontCmsc22;
        ErrorLine.Active_Style.Cell_Color= ClrLinen;
//      ErrorLine.Active_Style.Text_Centered=false;
        ErrorLine.UnActive_Style= Cell_Joyst_State.UnActive_Style;
        ErrorLine.FastDraw(2,208,316,30, (char*)"", Cell_UnActive);

        panLimiterState.Active_Style = Cell_Joyst_State.Active_Style;
        panLimiterState.UnActive_Style = Cell_Joyst_State.UnActive_Style;
//      panLimiterState.Active_Style.Text_Centered=false;
        panLimiterState.FastDraw(5,85,60,30, (char*)"", Cell_Active);

        tiltLimiterState.Active_Style = Cell_Joyst_State.Active_Style;
        tiltLimiterState.UnActive_Style = Cell_Joyst_State.UnActive_Style;
//      tiltLimiterState.Active_Style.Text_Centered=false;
        tiltLimiterState.FastDraw(5,110,60,30, (char*)"", Cell_Active);

        rollLimiterState.Active_Style = Cell_Joyst_State.Active_Style;
        rollLimiterState.UnActive_Style = Cell_Joyst_State.UnActive_Style;
//      rollLimiterState.Active_Style.Text_Centered=false;
        rollLimiterState.FastDraw(5,135,60,30, (char*)"", Cell_Active);

        Cell_GV_Acc.Active_Style = Cell_Joyst_State.Active_Style;
        Cell_GV_Acc.UnActive_Style = Cell_Joyst_State.UnActive_Style;
        Cell_GV_Acc.FastDraw(5,160,130,20, (char*)"ACC", Cell_Active);

        headVoltage.Active_Style = Cell_Joyst_State.Active_Style;
        headVoltage.UnActive_Style = Cell_Joyst_State.UnActive_Style;
//      headVoltage.Active_Style.Text_Centered=false;
        headVoltage.FastDraw(5,185,60,20, voltageString, Cell_Active);

        updateTiltLimiterCell();

        currentProfile.Active_Style = Cell_Joyst_State.Active_Style;
        currentProfile.UnActive_Style = Cell_Joyst_State.UnActive_Style;
        currentProfile.Active_Style.pFont=g_psFontCmsc22;
        currentProfile.UnActive_Style.pFont=g_psFontCmsc22;
        //currentProfile.Active_Style.Text_Centered=false;
        updatecurrentProfileCell();
        currentProfile.FastDraw(255,30,60,30, profileString, Cell_Active);

        motionLimitTime.Active_Style=Cell_Joyst_State.Active_Style;
        motionLimitTime.UnActive_Style=Cell_Joyst_State.UnActive_Style;

        motionCurrentTime.Active_Style=Cell_Joyst_State.Active_Style;
        motionCurrentTime.UnActive_Style=Cell_Joyst_State.UnActive_Style;

        motionTrackNumber.Active_Style=Cell_Joyst_State.Active_Style;
        motionTrackNumber.UnActive_Style=Cell_Joyst_State.UnActive_Style;

        motionState.Active_Style=Cell_Joyst_State.Active_Style;
        motionState.UnActive_Style=Cell_Joyst_State.UnActive_Style;

        motionPlayMode.Active_Style=Cell_Joyst_State.Active_Style;
        motionPlayMode.UnActive_Style=Cell_Joyst_State.UnActive_Style;

        motionMixMode.Active_Style=Cell_Joyst_State.Active_Style;
        motionMixMode.UnActive_Style=Cell_Joyst_State.UnActive_Style;

        motionLimitTime.Active_Style.pFont=g_psFontCmsc20;
        motionLimitTime.UnActive_Style.pFont=g_psFontCmsc20;
        motionCurrentTime.Active_Style.pFont=g_psFontCmsc20;
        motionCurrentTime.UnActive_Style.pFont=g_psFontCmsc20;
        motionTrackNumber.Active_Style.pFont=g_psFontCmsc20;
        motionTrackNumber.UnActive_Style.pFont=g_psFontCmsc20;
        motionState.Active_Style.pFont=g_psFontCmsc20;
        motionState.UnActive_Style.pFont=g_psFontCmsc20;
        motionPlayMode.Active_Style.pFont=g_psFontCmsc18;
        motionPlayMode.UnActive_Style.pFont=g_psFontCmsc18;
        motionMixMode.Active_Style.pFont=g_psFontCmsc18;
        motionMixMode.UnActive_Style.pFont=g_psFontCmsc18;

        motionTrackNumber.FastDraw  (140,80,105,30,motionTrackNumberString, Cell_Active);
        motionState.FastDraw        (140,110,105,30,motionStateString, Cell_Active);
        motionLimitTime.FastDraw    (140,140,175,30,limitTimeString, Cell_Active);
        motionCurrentTime.FastDraw  (140,170,175,30,counterTimeString, Cell_Active);
        motionPlayMode.FastDraw     (245,80,70,30,motionPlayModeString, Cell_Active);
        motionMixMode.FastDraw      (245,110,70,30,motionMixModeString, Cell_Active);

        // ======== END ����� ==============
    }

    // ======== ������ ==============void FastDraw(UInt32 X,UInt32 Y,UInt32 Xsize,UInt32 Ysize, char* ptext, byte Active);
//todo ������ ��� ����
#ifdef joyPult
    UInt16 PosX = 2, Length1 = 22, Length2 = 59, Otkat = 2;
#else
    UInt16 PosX = 1, Length1 = 22, Length2 = 86, Otkat = 2;
#endif
    Cell_P_Name.Active_Style = Style_MenuHeader;
    Cell_P_Name.FastDraw(PosX,1,Length1,30, (char*)"P", Cell_Active);

    Cell_P_Inf.Active_Style = Style_MenuHeader;
    Cell_P_Inf.UnActive_Style = Style_Error;
    Cell_P_Inf.FastDraw(PosX + (Length1-Otkat),1,Length2,30, (char*)"PS1", Cell_Active);


#ifdef USAEdition
    Cell_T_Name.Active_Style = Style_MenuHeader;
    Cell_T_Name.FastDraw(PosX + 1*(Length1-Otkat) + Length2,1,Length1,30, (char*)"T", Cell_Active);

    Cell_T_Inf.Active_Style = Style_MenuHeader;
    Cell_T_Inf.UnActive_Style = Style_Error;
    Cell_T_Inf.FastDraw(PosX + 2*(Length1-Otkat) + Length2,1,Length2,30, (char*)"TS1", Cell_Active);

    Cell_D_Name.Active_Style = Style_MenuHeader;
    Cell_D_Name.FastDraw(PosX + 2*(Length1-Otkat) + 2*Length2,1,Length1,30, (char*)"R", Cell_Active);

    Cell_D_Inf.Active_Style = Style_MenuHeader;
    Cell_D_Inf.UnActive_Style = Style_Error;
    Cell_D_Inf.FastDraw(PosX + 3*(Length1-Otkat) + 2*Length2,1,Length2,30, (char*)"DS1", Cell_Active);

    #else
    Cell_D_Name.Active_Style = Style_MenuHeader;
    Cell_D_Name.FastDraw(PosX + (Length1-Otkat) + Length2,1,Length1,30, (char*)"D", Cell_Active);

    Cell_D_Inf.Active_Style = Style_MenuHeader;
    Cell_D_Inf.UnActive_Style = Style_Error;
    Cell_D_Inf.FastDraw(PosX + 2*(Length1-Otkat) + Length2,1,Length2,30, (char*)"DS1", Cell_Active);

    Cell_T_Name.Active_Style = Style_MenuHeader;
    Cell_T_Name.FastDraw(PosX + 2*(Length1-Otkat) + 2*Length2,1,Length1,30, (char*)"T", Cell_Active);

    Cell_T_Inf.Active_Style = Style_MenuHeader;
    Cell_T_Inf.UnActive_Style = Style_Error;
    Cell_T_Inf.FastDraw(PosX + 3*(Length1-Otkat) + 2*Length2,1,Length2,30, (char*)"TS1", Cell_Active);
    #endif


#ifdef joyPult
    Cell_Z_Name.Active_Style = Style_MenuHeader;
    Cell_Z_Name.FastDraw(PosX + 3*(Length1-Otkat) + 3*Length2,1,Length1,30, (char*)"Z", Cell_Active);

    Cell_Z_Inf.Active_Style = Style_MenuHeader;
    Cell_Z_Inf.UnActive_Style = Style_Error;
    Cell_Z_Inf.FastDraw(PosX + 4*(Length1-Otkat) + 3*Length2,1,Length2,30, (char*)"ZS1", Cell_Active);
#endif
    // ======== END ������ ==============

    Serv_Counter(p_pult->getPanTurns(), true);


}

void LCD_Main::ReDraw() //�����������
{
    ClearDisp(); //�������� ������

    // ======== ����� ==============
    //������� ����������

    currentValue = p_pult->getPanTurns()-0.2;
    currentPanDirection=PAN_AXIS_STOP;
    Serv_Counter(p_pult->getPanTurns(), true); //������ ������� ����� � ����������
    //Cell_Main_Counter.ReDraw();

    if(p_pult->isJoySticksEnabled()) //�������� ����������
        Cell_Joyst_State.ReHide();
    else
        Cell_Joyst_State.ReDraw();

    if(p_pult->isMotorsEnabled()) //��������� �������
        Cell_Motor_State.ReHide();
    else
        Cell_Motor_State.ReDraw();


    Cell_GV_Acc.ReDraw();
    // ======== END ����� ==============

    // ======== ������ ==============
    Cell_P_Name.ReDraw();
    Cell_P_Inf.ReDraw();
    Cell_D_Name.ReDraw();
    Cell_D_Inf.ReDraw();
    Cell_T_Name.ReDraw();
    Cell_T_Inf.ReDraw();
#ifdef joyPult
    Cell_Z_Name.ReDraw();
    Cell_Z_Inf.ReDraw();
#endif
    // ======== END ������ ==============
    updateTiltLimiterCell();
    updatecurrentProfileCell();
    currentProfile.ReDraw();

    updateVoltageString(true);

    if(inMotionMode)
    {
        renderDateString(limitTimeString,10,4000);
        renderDateString(counterTimeString,10,4100);

        motionLastState=0xFF;
        motionLastTrackNumber=0xFF;
        limitTimeLastState=0;
        counterTimeLastState=0;
        motionPlayModeState=MOTION_PLAY_MODE_ONCE;
        motionMixModeState=MOTION_MIX_TRACK_ONLY;

        motionLimitTime.ReDraw();
        motionCurrentTime.ReDraw();
        motionLimitTime.ReDraw();
        motionCurrentTime.ReDraw();
        motionPlayMode.ReDraw();
        motionMixMode.ReDraw();
    }

}
void LCD_Main::updatecurrentProfileCell()
{
    sprintf(profileString,"P%d",EE_Working::getProfileID()+1);
    profileString[9]=0;
}
void LCD_Main::updateVoltageString(bool manual)
{
#ifdef panWheelDebug
    sprintf(voltageString,"V: %4.1f",p_pult->getSpeedPasha());
    voltageString[9]=0;
    headVoltage.ReDraw();
#endif

#ifdef rollWheelDebug
    sprintf(voltageString,"V: %4.1f",p_pult->getSpeedPasha());
    voltageString[9]=0;
    headVoltage.ReDraw();
#endif

#ifdef tiltWheelDebug
    sprintf(voltageString,"V: %4.1f",p_pult->getSpeedPasha());
    voltageString[9]=0;
    headVoltage.ReDraw();
#endif

#ifndef debugWheel

    UInt16 t=(UInt16)(p_pult->getHeadVoltage()*10);

    if((lastVoltage!=t)||manual)
    {
        lastVoltage=t;
        sprintf(voltageString,"V: %.1f",(((float)(t))/10));
        voltageString[9]=0;
        headVoltage.ReDraw();
        return;
    }
#endif
}

void LCD_Main::updateTiltLimiterCell()
{
    switch(panLimiterStateFlag)
    {
        case TL_UP:
            panLimiterState.p_text="P LF";
            panLimiterState.ReDraw();
            break;
        case TL_DOWN:
            panLimiterState.p_text="P RT";
            panLimiterState.ReDraw();
            break;
        case TL_UP_DOWN:
            panLimiterState.p_text="P ALL";
            panLimiterState.ReDraw();
            break;
        case TL_RESET:
        default:
            panLimiterState.Hide();
            break;
    }
    switch(tiltLimiterStateFlag)
    {
        case TL_UP:
            tiltLimiterState.p_text="T UP";
            tiltLimiterState.ReDraw();
            break;
        case TL_DOWN:
            tiltLimiterState.p_text="T DOWN";
            tiltLimiterState.ReDraw();
            break;
        case TL_UP_DOWN:
            tiltLimiterState.p_text="T ALL";
            tiltLimiterState.ReDraw();
            break;
        case TL_RESET:
        default:
            tiltLimiterState.Hide();
            break;
    }
#ifdef USAEdition
    switch(rollLimiterStateFlag)
    {
        case TL_UP:
            rollLimiterState.p_text="R LF";
            rollLimiterState.ReDraw();
            break;
        case TL_DOWN:
            rollLimiterState.p_text="R RT";
            rollLimiterState.ReDraw();
            break;
        case TL_UP_DOWN:
            rollLimiterState.p_text="R ALL";
            rollLimiterState.ReDraw();
            break;
        case TL_RESET:
        default:
            rollLimiterState.Hide();
            break;
    }
#else
    switch(rollLimiterStateFlag)
        {
            case TL_UP:
                rollLimiterState.p_text="D LT";
                rollLimiterState.ReDraw();
                break;
            case TL_DOWN:
                rollLimiterState.p_text="D RT";
                rollLimiterState.ReDraw();
                break;
            case TL_UP_DOWN:
                rollLimiterState.p_text="D ALL";
                rollLimiterState.ReDraw();
                break;
            case TL_RESET:
            default:
                rollLimiterState.Hide();
                break;
        }
#endif

}

void LCD_Main::upToMotionMode()
{
    motionLogicController.deactivate();
    return;
}
void LCD_Main::selectMotionMode()
{
    motionLogicController.activate();
    inMotionMode=true;
    this->Draw();
}
void LCD_Main::deselectMotionMode()
{
    motionLogicController.deactivate();
    inMotionMode=false;
    this->Draw();
}
void LCD_Main::switchMotionMode()
{
    if(inMotionMode)
    {
        deselectMotionMode();
        return;
    }
    else
    {
        selectMotionMode();
        return;
    }
}
void LCD_Main::Listener()
{
    if(selectModeCounter!=0){selectModeCounter--;}
    if((p_pult->getButton(pult_Button_Up)->state==HOLD)&&(p_pult->getButton(pult_Button_Dn)->state==HOLD)&&(selectModeCounter==0))
    {
        if(!inMotionMode)
        {
            selectMotionMode();
            selectModeCounter=0x60;
        }
        else
        {
            deselectMotionMode();
            selectModeCounter=0x60;
        }
    }

    if(Focused)
    {
        ClearDisp();
        ReDraw();
        Focused = false;
    }

    if (getButtonState(pult_Button_Select) == PRESSED) //������ � ������� ����
    {
        pDispl = pMain_Menu;
        pDispl->Focused = true;
        pDispl->Parent = this;
        return;
    }

    if(p_pult->isJoySticksEnabled())    {   Cell_Joyst_State.Hide();}
    else                                {   Cell_Joyst_State.Draw();}

    if(p_pult->isMotorsEnabled())       {   Cell_Motor_State.Hide();}
    else                                {   Cell_Motor_State.Draw();}

    Serv_Counter(p_pult->getPanTurns(), false); //������ ������� ����� � ����������

    explore_Presets(); //������ ��������� ��������
    ErrorLine.tick(); //�������� ������
    char* promName = p_pult->getGvAcc(); //������ �� ���, GV ��� ACC
    if(Cell_GV_Acc.p_text != promName)
    {
        Cell_GV_Acc.p_text = promName;
        Cell_GV_Acc.SetText(promName);
        Cell_GV_Acc.ReDraw();
    }

     updateVoltageString(false);
//todo motion modes deactivate
    if(inMotionMode)
    {
            if(p_pult->getButton(pult_Button_Left)->isClicked())
            {
                p_pult->getMotionSubsystem()->swicthPlayMode();
            }
            if(p_pult->getButton(pult_Button_Right)->isClicked())
            {
                p_pult->getMotionSubsystem()->switchMixMode();
            }
            updateMotionState();
            updateMotionTrackNumber();
            updateLimitTime();
            updateCounterTime();
            updateMotionPlayModeStr();
            updateMotionMixModeStr();


    }
    else
    {

        if(explore_Koeff_Butt()) return;
    }

}

void LCD_Main::testLeds()
{
    static int count=0;
    char buff_String[7];

    SPI_Reset_Pin((SPI_LED_Pins)count);
    count++;
    if(count>31) count=1;
    SPI_Set_Pin((SPI_LED_Pins)count);
    u16_TO_buffer(&buff_String[3],count,2);
    buff_String[0] = 'L'; buff_String[1] = 'E'; buff_String[2] = 'D';
    buff_String[5] = 0;
    Cell_D_Inf.SetText(&buff_String[0]);
    Cell_D_Inf.ReDraw();

}



void LCD_Main::explore_Presets()
{
    Uint8 temp;
    char* promName = (char*)p_pult->getJoystickPresetName(PanJoystickPreset);
    if(Cell_P_Inf.p_text != promName)
    {
        temp=p_pult->getJoystickPresetId(PanJoystickPreset);
        if(temp!=EE_Working::Read(PAN_FUNC_VALUE))
        {
            if(EE_Working::Write(PAN_FUNC_VALUE,temp)!=0)
            {
                        //EEPROM EXCEPTION
            }
        }
        Cell_P_Inf.p_text = promName;
        Cell_P_Inf.SetText(promName);
        Cell_P_Inf.ReDraw();
    }
    promName = (char*)p_pult->getJoystickPresetName(DutchJoystickPreset);
    if(Cell_D_Inf.p_text != promName)
    {
        temp=p_pult->getJoystickPresetId(DutchJoystickPreset);
        if(temp!=EE_Working::Read(DATCH_FUNC_VALUE))
        {
            if(EE_Working::Write(DATCH_FUNC_VALUE,temp)!=0)
            {
                        //EEPROM EXCEPTION
            }
        }
        Cell_D_Inf.p_text = promName;
        Cell_D_Inf.SetText(promName);
        Cell_D_Inf.ReDraw();
    }
    promName = (char*)p_pult->getJoystickPresetName(TiltJoystickPreset);
    if(Cell_T_Inf.p_text != promName)
    {
        temp=p_pult->getJoystickPresetId(TiltJoystickPreset);
        if(temp!=EE_Working::Read(TILT_FUNC_VALUE))
        {
            if(EE_Working::Write(TILT_FUNC_VALUE,temp)!=0)
            {
                        //EEPROM EXCEPTION
            }
        }
        Cell_T_Inf.p_text = promName;
        Cell_T_Inf.SetText(promName);
        Cell_T_Inf.ReDraw();
    }
#ifdef joyPult
    promName = (char*)p_pult->getJoystickPresetName(ZoomJoystickPreset);
    if(Cell_Z_Inf.p_text != promName)
    {
        temp=p_pult->getJoystickPresetId(ZoomJoystickPreset);
        if(temp!=EE_Working::Read(EE_LC_ZOOM_SENSE))
        {
            if(EE_Working::Write(EE_LC_ZOOM_SENSE,temp)!=0)
            {
                        //EEPROM EXCEPTION
            }
        }
        Cell_Z_Inf.p_text = promName;
        Cell_Z_Inf.SetText(promName);
        Cell_Z_Inf.ReDraw();
    }
#endif
}

bool LCD_Main::explore_Koeff_Butt()
{
    bool is_butt = false;
    UInt32 promParam;

    if (getButtonState(pult_Button_PK) == PRESSED)
    {   promParam = EE_PK+1; is_butt = true;    }

    if (getButtonState(pult_Button_PI) == PRESSED)
    {   promParam = EE_PI+1; is_butt = true;    }

    if (getButtonState(pult_Button_DK) == PRESSED)
    {   promParam = EE_DK+1; is_butt = true;    }

    if (getButtonState(pult_Button_DI) == PRESSED)
    {   promParam = EE_DI+1; is_butt = true;    }

    if (getButtonState(pult_Button_TK) == PRESSED)
    {   promParam = EE_TK+1; is_butt = true;    }

    if (getButtonState(pult_Button_TI) == PRESSED)
    {   promParam = EE_TI+1; is_butt = true;    }

    if(is_butt)
    {
        pDispl = pSet_Koeff;
        pDispl->Param = promParam;
        pDispl->Focused = true;
        pDispl->Parent = this;
        Task_sleep(100);
        return true;
    }

    return false;
}

void LCD_Main::Serv_Counter(float value, bool isForseUpdate)
{
    UInt8 tmp[2];
    UInt8 vModule=0;
    if(value<=-0.1){vModule=1;} //-
    if(value>=0.1){vModule=2;} //+

    if(value<0){value=-value;}

    tmp[0]=(UInt8)(value);
    tmp[1]=(UInt8)((value-tmp[0])*10);

    if(isForseUpdate||(currentPanDirection!=p_pult->getPanAxisDirection())||((value>=(currentValue+0.03))||(value<=(currentValue-0.03))))
    {
        currentValue=value;

        Cell_Main_Counter.Active_Style.Cell_Color = ClrDarkSlateBlue;
        if(tmp[0]>=3){ Cell_Main_Counter.Active_Style.Cell_Color = ClrLime;}
        if(tmp[0]>=4){ Cell_Main_Counter.Active_Style.Cell_Color = ClrLinen;}
        snprintf(servCounterStr,5,"%d.%d",tmp[0],tmp[1]);
        servCounterStr[4]=0;
        Cell_Main_Counter.SetText(servCounterStr);
        Cell_Main_Counter.ReDraw();

        currentPanDirection=p_pult->getPanAxisDirection();
        Cell_Main_Counter_Right.Active_Style.Cell_Color = Color_Menu_Fon_InActive;
        Cell_Main_Counter_Left.Active_Style.Cell_Color = Color_Menu_Fon_InActive;
        Cell_Main_Counter_Left.Active_Style.Font_Color=ClrWhite;
        Cell_Main_Counter_Right.Active_Style.Font_Color=ClrWhite;
        switch(currentPanDirection)
        {
            case PAN_AXIS_UP:
                Cell_Main_Counter_Left.Active_Style.Font_Color = ClrLinen;
                switch(vModule)
                {
                    case 1:Cell_Main_Counter_Right.ReDraw();break;
                    case 2:
                    case 0:
                    default:Cell_Main_Counter_Right.Hide();break;
                }
                Cell_Main_Counter_Left.ReDraw();
                break;

            case PAN_AXIS_DOWN:
                Cell_Main_Counter_Right.Active_Style.Font_Color = ClrLinen;
                switch(vModule)
                {
                    case 2:Cell_Main_Counter_Left.ReDraw();break;
                    case 0:
                    case 1:
                    default:Cell_Main_Counter_Left.Hide();break;
                }
                Cell_Main_Counter_Right.ReDraw();
                break;
            case PAN_AXIS_STOP:
            default:
                switch(vModule)
                {
                    case 2:Cell_Main_Counter_Left.ReDraw();Cell_Main_Counter_Right.Hide();break;
                    case 1:Cell_Main_Counter_Right.ReDraw();Cell_Main_Counter_Left.Hide();break;
                    case 0:
                    default:Cell_Main_Counter_Left.Hide();Cell_Main_Counter_Right.Hide();break;
                }
                break;
            }
    }

}

//--------------------------------------------------------------------------------------
void LCD_Cell::SetRect()
{
    pRect.i16XMin = p_Pos_Size_XY.X;
    pRect.i16YMin = p_Pos_Size_XY.Y;
    pRect.i16XMax = p_Pos_Size_XY.X + p_Pos_Size_XY.Xsize - 1;
    pRect.i16YMax = p_Pos_Size_XY.Y + p_Pos_Size_XY.Ysize - 1;
}

void LCD_Cell::Set_Coord(UInt32 X,UInt32 Y,UInt32 Xsize,UInt32 Ysize)
{
    p_Pos_Size_XY.X = X;
    p_Pos_Size_XY.Y = Y;
    p_Pos_Size_XY.Xsize = Xsize;
    p_Pos_Size_XY.Ysize = Ysize;
}

void LCD_Cell::PreDraw()
{
    SetRect();
    Int16 radius = (pRect.i16YMax - pRect.i16YMin)/2;
    Int16 X_sdvig=-12,Y_sdvig=2;
    //������� ������ �������� ������
    if(Rounded)
    {
        GrContextForegroundSetTranslated(&sContext, Tek_Style.Cell_Color);
        GrCircleFill(&sContext,pRect.i16XMin+radius+X_sdvig,pRect.i16YMin+radius+Y_sdvig,radius);
    }
    else
        DpyRectFill(&g_sKentec320x240x16_SSD2119,&pRect,Tek_Style.Cell_Color);

    if(Tek_Style.Border)
    {
        #define k 1
        if(Rounded)
        {
            GrContextForegroundSetTranslated(&sContext, Tek_Style.Border_Color);
            GrCircleDraw(&sContext,pRect.i16XMin+radius+X_sdvig,pRect.i16YMin+radius+Y_sdvig,radius-k);
        }
        else
        {
            // �������� ����� -k ��������
            DpyLineDrawH(&g_sKentec320x240x16_SSD2119,pRect.i16XMin+k,pRect.i16XMax-k,pRect.i16YMin+k,Tek_Style.Border_Color);
            DpyLineDrawH(&g_sKentec320x240x16_SSD2119,pRect.i16XMin+k,pRect.i16XMax-k,pRect.i16YMax-k,Tek_Style.Border_Color);
            DpyLineDrawV(&g_sKentec320x240x16_SSD2119,pRect.i16XMin+k,pRect.i16YMin+k,pRect.i16YMax-k,Tek_Style.Border_Color);
            DpyLineDrawV(&g_sKentec320x240x16_SSD2119,pRect.i16XMax-k,pRect.i16YMin+k,pRect.i16YMax-k,Tek_Style.Border_Color);
        }
    }
}

void LCD_Cell::Draw()
{
    if(!Drawed)
    {
        Tek_Style = Active_Style;
        PreDraw();
        if(strlen(p_text)>0)
        {
            //������ ��� ������� �����
            GrContextForegroundSetTranslated(&sContext, Tek_Style.Font_Color);
            GrContextFontSet(&sContext, Tek_Style.pFont);
            if(Tek_Style.Text_Centered)
                GrStringDrawCentered(&sContext, p_text, strlen(p_text), pRect.i16XMin + (pRect.i16XMax-pRect.i16XMin)/2, pRect.i16YMin + (pRect.i16YMax-pRect.i16YMin)/2, false);
            else
                GrStringDraw(&sContext, p_text, strlen(p_text), pRect.i16XMin+5, pRect.i16YMin+2, 0);
        }
        Drawed = true;
        Hided = false;
    }
}

void LCD_Cell::Hide()
{
    if(!Hided)
    {
        Tek_Style = UnActive_Style;
        PreDraw();
        if(strlen(p_text)>0)
        {
            //������ ��� ������� �����
            GrContextForegroundSetTranslated(&sContext, Tek_Style.Font_Color);
            GrContextFontSet(&sContext, Tek_Style.pFont);
            if(Tek_Style.Text_Centered)
                GrStringDrawCentered(&sContext, p_text, strlen(p_text), pRect.i16XMin + (pRect.i16XMax-pRect.i16XMin)/2, pRect.i16YMin + (pRect.i16YMax-pRect.i16YMin)/2, false);
            else
                GrStringDraw(&sContext, p_text, strlen(p_text), pRect.i16XMin+5, pRect.i16YMin+2, 0);
        }
        Drawed = false;
        Hided = true;
    }

}

void LCD_Cell::DrawTecStile()
{
        PreDraw();
        if(strlen(p_text)>0)
        {
            //������ ��� ������� �����
            GrContextForegroundSetTranslated(&sContext, Tek_Style.Font_Color);
            GrContextFontSet(&sContext, Tek_Style.pFont);
            if(Tek_Style.Text_Centered)
                GrStringDrawCentered(&sContext, p_text, strlen(p_text), pRect.i16XMin + (pRect.i16XMax-pRect.i16XMin)/2, pRect.i16YMin + (pRect.i16YMax-pRect.i16YMin)/2, false);
            else
                GrStringDraw(&sContext, p_text, strlen(p_text), pRect.i16XMin+5, pRect.i16YMin+2, 0);
        }
    }



void LCD_Cell::FastDraw(UInt32 X,UInt32 Y,UInt32 Xsize,UInt32 Ysize, char* ptext, byte Active)
{
    Set_Coord(X, Y, Xsize, Ysize);
    SetText(ptext);
    if(Active) ReDraw(); else ReHide();
}

void LCD_Cell::Listener()
{
    ReDraw();
    Task_sleep(1500);
    pDispl = pDispl->Parent;
    pDispl->Focused = true;
    return;
}

//---------------------------------- Cheak menu -------------------------------------------
CheckMenu::CheckMenu():
        c0((char*) " ", 1,1,10,10),
        c1((char*) " ", 1,1,10,10),
        c2((char*) " ", 1,1,10,10),
        c3((char*) " ", 1,1,10,10),
        c4((char*) " ", 1,1,10,10),
        c5((char*) " ", 1,1,10,10),
        c6((char*) " ", 1,1,10,10),
        c7((char*) " ", 1,1,10,10),
        c8((char*) " ", 1,1,10,10),
        c9((char*) " ", 1,1,10,10),
        c10((char*) " ", 1,1,10,10),
        c11((char*) " ", 1,1,10,10),
        c12((char*) " ", 1,1,10,10),
        c13((char*) " ", 1,1,10,10),
        c14((char*) " ", 1,1,10,10),
        c15((char*) " ", 1,1,10,10),
        connCell((char*) " ", 1,1,10,10),
        conDelay((char*) " ", 1,1,10,10)
{
        cells[0]=&c0;   cells[1]=&c1;       cells[2]=&c2;
        cells[3]=&c3;   cells[4]=&c4;       cells[5]=&c5;
        cells[6]=&c6;   cells[7]=&c7;       cells[8]=&c8;
        cells[9]=&c9;   cells[10]=&c10;     cells[11]=&c11;
        cells[12]=&c12; cells[13]=&c13;     cells[14]=&c14;
        cells[15]=&c15;
}

void CheckMenu::renderM(char* name, bool flag1, UInt8 counter)
{
    char form[20];
    strncpy(form, name,10);
    strncat(form, ": %u: %u",10);
    snprintf(buffer,20,form,flag1,counter);
    buffer[20]=0;

}
void CheckMenu::render(char* name, bool flag1, bool flag2)
{
    strncpy(buffer, name,10);
    strncat(buffer, ": ",2);
    if(flag1)
    {
        strncat(buffer, "1 ",2);
    }
    else
    {
        strncat(buffer, "0 ",2);
    }
    strncat(buffer, ": ",2);
    if(flag2)
    {
        strncat(buffer, "1 ",3);
    }
    else
    {
        strncat(buffer, "0 ",3);
    }
    buffer[20]=0;
}
void CheckMenu::Draw()
{
    UInt16 StepY;
    p_Pos_Size_XY.X = 5; p_Pos_Size_XY.Y = 30;
    p_Pos_Size_XY.Xsize = 150; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/8))-2;//35
    StepY = p_Pos_Size_XY.Ysize + 2;

    for(UInt8 i=0;i<8;i++)
    {
        render((char*)p_pult->getCheckValues()->get(i)->name, p_pult->getCheckValues()->get(i)->flagDyn, p_pult->getCheckValues()->get(i)->flagStat);
        cells[i]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                buffer, Cell_UnActive);

        render((char*)p_pult->getCheckValues()->get(i+8)->name, p_pult->getCheckValues()->get(i+8)->flagDyn, p_pult->getCheckValues()->get(i+8)->flagStat);
        cells[i+8]->FastDraw(150+p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                buffer, Cell_UnActive);
    }
    renderM("CONN-ED", p_pult->isConnected(), p_pult->getDisconnectCounter());
    this->connCell.FastDraw(p_Pos_Size_XY.X,5,150,p_Pos_Size_XY.Ysize,buffer,Cell_UnActive);
    renderM("CONN-DEL", p_pult->isConnected(), p_pult->getTransferDelay());
    this->conDelay.FastDraw(p_Pos_Size_XY.X+150,5,150,p_Pos_Size_XY.Ysize,buffer,Cell_UnActive);

}
void CheckMenu::Listener()
{

    if(Focused)
    {
        ClearDisp();
        Draw();
        Focused = false;
//      return;
    }
    if(p_pult->isErrorUpdated())
    {
        ClearDisp();
        Draw();
    }
    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        p_pult->resetStaticErrorBits();
        p_pult->resetDisconnectCounter();
    }
    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        return;
    }
}
//-----------------------------------------------------------------------------------------


LCD_Menu::LCD_Menu(char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr):
Cell_Header((char*) " ", 1,1,10,10),
Cell_1((char*) " ", 1,1,10,10),
Cell_2((char*) " ", 1,1,10,10),
Cell_3((char*) " ", 1,1,10,10),
Cell_4((char*) " ", 1,1,10,10),
Cell_5((char*) " ", 1,1,10,10),
Cell_6((char*) " ", 1,1,10,10),
Cell_7((char*) " ", 1,1,10,10),
Cell_8((char*) " ", 1,1,10,10),
Cell_9((char*) " ", 1,1,10,10),
Cell_10((char*) " ", 1,1,10,10)
{
    Counter_Cell = Count;
    Tek_Count = 1;
    Start = 1;
    Orientation = Orient;
    Menu_On_Screen = Menu_Per_Scr;
    pName = pNam;
    Menu_Link = Link;

    Table_Cell[1-1] = &Cell_1;
    Table_Cell[2-1] = &Cell_2;
    Table_Cell[3-1] = &Cell_3;
    Table_Cell[4-1] = &Cell_4;
    Table_Cell[5-1] = &Cell_5;
    Table_Cell[6-1] = &Cell_6;
    Table_Cell[7-1] = &Cell_7;
    Table_Cell[8-1] = &Cell_8;
    Table_Cell[9-1] = &Cell_9;
    Table_Cell[10-1] = &Cell_10;

    Cell_Header.Active_Style = Style_MenuHeader;
    Cell_Header.UnActive_Style = Style_MenuHeader;

    byte i;

    for(i=0;i<Counter_Cell;i++) //����������� ������� �������
        Table_Cell[i]->SetText((Menu_Link+i)->Name);

}

LCD_Menu::LCD_Menu():
Cell_Header((char*) " ", 1,1,10,10),
Cell_1((char*) " ", 1,1,10,10),
Cell_2((char*) " ", 1,1,10,10),
Cell_3((char*) " ", 1,1,10,10),
Cell_4((char*) " ", 1,1,10,10),
Cell_5((char*) " ", 1,1,10,10),
Cell_6((char*) " ", 1,1,10,10),
Cell_7((char*) " ", 1,1,10,10),
Cell_8((char*) " ", 1,1,10,10),
Cell_9((char*) " ", 1,1,10,10),
Cell_10((char*) " ", 1,1,10,10)
{
    Table_Cell[1-1] = &Cell_1;
    Table_Cell[2-1] = &Cell_2;
    Table_Cell[3-1] = &Cell_3;
    Table_Cell[4-1] = &Cell_4;
    Table_Cell[5-1] = &Cell_5;
    Table_Cell[6-1] = &Cell_6;
    Table_Cell[7-1] = &Cell_7;
    Table_Cell[8-1] = &Cell_8;
    Table_Cell[9-1] = &Cell_9;
    Table_Cell[10-1] = &Cell_10;

    Cell_Header.Active_Style = Style_MenuHeader;
    Cell_Header.UnActive_Style = Style_MenuHeader;


}


void LCD_Menu::DrawVert() //��������� ������������� ����
{
    byte i;
    Int16 StepY;

    {
        p_Pos_Size_XY.X = 3; p_Pos_Size_XY.Y = 35;
        p_Pos_Size_XY.Xsize = 314; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
        StepY = p_Pos_Size_XY.Ysize + 5;

        for(i=0;i<Menu_On_Screen;i++)
        {
            Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                    p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                    Table_Cell[Start+i-1]->p_text, Cell_UnActive);
            if( (Start+i) == Counter_Cell) break;
        }
    }
    Table_Cell[Tek_Count-1]->Draw();
}

void LCD_Menu::DrawHoriz() //��������� ��������������� ����
{
    byte i, Up;
    Int16 StepX, StepY;

    p_Pos_Size_XY.X = 5; p_Pos_Size_XY.Y = 40;
    p_Pos_Size_XY.Xsize = 154; p_Pos_Size_XY.Ysize = 60;

    if(Counter_Cell < Menu_On_Screen) Up = Counter_Cell; else Up = Menu_On_Screen;

    for(i=0;i<Up;i++)
        if(i<(Menu_On_Screen/2))
        {
            //����� ��������
            StepY = p_Pos_Size_XY.Ysize + 3;
            Table_Cell[i+Start-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                            p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                            Table_Cell[i+Start-1]->p_text, Cell_UnActive);
        }
        else
        {
            //������ ��������
            StepX = p_Pos_Size_XY.Xsize + 2;
            Table_Cell[i+Start-1]->FastDraw(p_Pos_Size_XY.X+StepX,p_Pos_Size_XY.Y+StepY*(i-(Menu_On_Screen/2)),
                                              p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                              Table_Cell[i+Start-1]->p_text, Cell_UnActive);
        }

    Table_Cell[Tek_Count-1]->Draw();
}

void LCD_Menu::Draw(byte Active) //������ ������� � ���������
{
    //ClearDisp(); //������� ������ �������� ������

    DrawHeader();

    Tek_Count = Active;

    if(Orientation==0) //���� ���������� 1*6
    {   DrawVert(); }
    else //���������� 2*3
    {   DrawHoriz(); }

    Table_Cell[Active-1]->Draw();
}

void LCD_Menu::readEEPROM() {
#ifdef joyPult
    uint32_t prestonData=EE_Working::Read(EE_LC_EXTERNAL_LENS_CONTROL);
    if (prestonData>1) {
        EE_Working::Write(EE_LC_EXTERNAL_LENS_CONTROL, 0);
        }
    else {

        if (prestonData) {
            Table_Cell[externalLensControlSistem]->p_text="EXTERNAL ZOOM ON";
            Table_Cell[externalLensControlSistem]->Active_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[externalLensControlSistem]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
        }
        else {
            Table_Cell[externalLensControlSistem]->p_text="EXTERNAL ZOOM OFF";
            Table_Cell[externalLensControlSistem]->Active_Style.Cell_Color=ClrMediumOrchid;
            Table_Cell[externalLensControlSistem]->UnActive_Style.Cell_Color=ClrMediumOrchid;
        }
        }
    p_pult->setPreston((bool)prestonData);
#endif
                    // ���������� ��� ������� �������
    }

void LCD_Menu::Plus() //���������� ����������
{
    byte prom_Count = Tek_Count;

    if(prom_Count < Counter_Cell)
    {
        if(prom_Count == Start+(Menu_On_Screen-1)) //����� �� ������� ����
            Start++;
        prom_Count++;
    }
    else {prom_Count = 1; Start = 1;}

    Draw(prom_Count);
}

void LCD_Menu::Minus() //���������� �����������
{
    byte prom_Count = Tek_Count;

    if(prom_Count > 1)
    {
        if(prom_Count == Start) //����� �� �������� ����
            Start--;
        prom_Count--;
    }
    else
    {
        prom_Count = Counter_Cell;
        if(Counter_Cell<=Menu_On_Screen)
            Start = 1;
        else Start = Counter_Cell - (Menu_On_Screen-1);
    }

    Draw(prom_Count);
}

void LCD_Menu::Right() //���������� ������ ������
{
    byte prom_Count = Tek_Count;

    if(prom_Count < 4)
    {
        prom_Count+=3;
    }
    else
    {
        if(prom_Count < Counter_Cell)
        {
            if(prom_Count == Start+(Menu_On_Screen-1)) //����� �� ������� ����
                Start++;
            prom_Count++;
        }
        else {prom_Count = 1; Start = 1;}
    }

    Draw(prom_Count);
}

void LCD_Menu::Left() //���������� ������ �����
{
    byte prom_Count = Tek_Count;

    if(prom_Count > 3)
    {
        prom_Count-=3;
    }
    else
    {
        if(prom_Count > 1)
        {
            if(prom_Count == Start) //����� �� �������� ����
                Start--;
            prom_Count--;
        }
        else
        {
            prom_Count = Counter_Cell;
            if(Counter_Cell<=Menu_On_Screen)
                Start = 1;
            else Start = Counter_Cell - (Menu_On_Screen-1);
        }
    }

    Draw(prom_Count);
}

void LCD_Menu::Select() //�����
{}

bool Menu_Listener(LCD_Listener* HoIs)
{

#define newLimitMenu

#ifdef newLimitMenu
        if (HoIs ==limitsMenuPointer){
        pDispl= (  (LCD_Menu*)HoIs )->Menu_Link[(((LCD_Menu*)HoIs )->Tek_Count)-1]. pPointSub;
                        pDispl->Parent = HoIs;
                        pDispl->Focused = true;}

#else
        if(HoIs == pTiltLimit_Menu) //���������� ����������� �������
        {
        ((LCD_Menu*)HoIs )->Table_Cell[((LCD_Menu*)HoIs )->Tek_Count-1]->Hide();
        Task_sleep(300);
        ((LCD_Menu*)HoIs )->Table_Cell[((LCD_Menu*)HoIs )->Tek_Count-1]->Draw();
        if(  (  (LCD_Menu*)HoIs )->Tek_Count == 1)
            {
            p_pult->setTiltUpLimit();
            if(mainScreenPointer->tiltLimiterStateFlag==TL_DOWN){mainScreenPointer->tiltLimiterStateFlag=TL_UP_DOWN; }
            else{mainScreenPointer->tiltLimiterStateFlag=TL_UP; }
            return true;
            }
        if(  (  (LCD_Menu*)HoIs )->Tek_Count == 2)
            {
            p_pult->setTiltDnLimit();
            if(mainScreenPointer->tiltLimiterStateFlag==TL_UP){mainScreenPointer->tiltLimiterStateFlag=TL_UP_DOWN; }
            else{mainScreenPointer->tiltLimiterStateFlag=TL_DOWN; }
            return true;
            }
        if(  (  (LCD_Menu*)HoIs )->Tek_Count == 3)
            {
            p_pult->resetTiltUpLimit();
            if(mainScreenPointer->tiltLimiterStateFlag==TL_UP_DOWN){mainScreenPointer->tiltLimiterStateFlag=TL_DOWN; return true;}
            if(mainScreenPointer->tiltLimiterStateFlag==TL_UP){mainScreenPointer->tiltLimiterStateFlag=TL_RESET; return true;}
            return true;
            }
        if(  (  (LCD_Menu*)HoIs )->Tek_Count == 4)
            {
            p_pult->resetTiltDnLimit();
            if(mainScreenPointer->tiltLimiterStateFlag==TL_UP_DOWN){mainScreenPointer->tiltLimiterStateFlag=TL_UP; return true;}
            if(mainScreenPointer->tiltLimiterStateFlag==TL_DOWN){mainScreenPointer->tiltLimiterStateFlag=TL_RESET; return true;}
            return true;
            }
        if(  (  (LCD_Menu*)HoIs )->Tek_Count == 5)
            {
            p_pult->resetTiltUpLimit();
            p_pult->resetTiltDnLimit();
            mainScreenPointer->tiltLimiterStateFlag=TL_RESET;
            return true;
            }
        }
#endif

    #ifdef objDef
    if (HoIs==lensControlObjectiveMenuPointer) {
        //if (){
        pDispl= (  (LCD_Menu*)HoIs )->Menu_Link[(((LCD_Menu*)HoIs )->Tek_Count)-1]. pPointSub;
        pDispl->Parent = HoIs;
        pDispl->Focused = true;//}
        return true;
        }
    #endif

    if(HoIs==zifReversMenuPointer ) {
        pDispl= (  (LCD_Menu*)HoIs )->Menu_Link[(((LCD_Menu*)HoIs )->Tek_Count)-1]. pPointSub;
        pDispl->Parent = HoIs;
        pDispl->Focused = true;
        return true;
        }

    if(HoIs == p_PDT_Menu_Eq) //���������� ������ �����������
        {
        if(  (  (LCD_Menu*)HoIs )->Tek_Count == 1)
            pDispl = pEqualizerPan;
        if(  (  (LCD_Menu*)HoIs )->Tek_Count == 2)
            pDispl = pEqualizerDutch;
        if(  (  (LCD_Menu*)HoIs )->Tek_Count == 3)
            pDispl = pEqualizerTilt;
        if(  (  (LCD_Menu*)HoIs )->Tek_Count == 4)
            pDispl = pEqualizerZoom;
        pDispl->Parent = HoIs;
        pDispl->Focused = true;
        return true;
        }

    if(HoIs ==  secretMenuSelectorPointer)
        {
        pDispl= (  (LCD_Menu*)HoIs )->Menu_Link[(((LCD_Menu*)HoIs )->Tek_Count)-1]. pPointSub;
        pDispl->Parent = HoIs;
        pDispl->Focused = true;
        return true;
        }
    if(HoIs ==  setupMenuPointer)
        {
        pDispl= (  (LCD_Menu*)HoIs )->Menu_Link[(((LCD_Menu*)HoIs )->Tek_Count)-1]. pPointSub;
        pDispl->Parent = HoIs;
        pDispl->Focused = true;
        return true;
        }

    if(HoIs == lensControlMenuPointer)
        {
        pDispl= (  (LCD_Menu*)HoIs )->Menu_Link[(((LCD_Menu*)HoIs )->Tek_Count)-1]. pPointSub;
        pDispl->Parent = HoIs;
        pDispl->Focused = true;
        return true;
        }

    if(HoIs == lensControlZifSetupPointer)
        {
        pDispl= (  (LCD_Menu*)HoIs )->Menu_Link[(((LCD_Menu*)HoIs )->Tek_Count)-1]. pPointSub;
        lensControlMotorActionSelectPointer->setCurrentVariable((  (LCD_Menu*)HoIs )->Tek_Count-1);
        pDispl->Parent = HoIs;
        pDispl->Focused = true;
        return true;
        }

    if(HoIs == lensControlMotorsSetupPointer)
        {
        pDispl= (  (LCD_Menu*)HoIs )->Menu_Link[(((LCD_Menu*)HoIs )->Tek_Count)-1]. pPointSub;
        lensControlMotorTypeSelectPointer->setCurrentVariable((  (LCD_Menu*)HoIs )->Tek_Count-1);
        pDispl->Parent = HoIs;
        pDispl->Focused = true;
        return true;
        }

    if(HoIs == tuningMenuPointer)
        {
        pDispl= tuningMenuText[tuningMenuPointer->Tek_Count-1].pPointSub;
        pDispl->Parent = HoIs;
        pDispl->Focused = true;
        return true;
        }

    if(HoIs == suspResonanceSelectPointer)
    {
        pDispl= suspensionResonanceSelectMenuText[suspResonanceSelectPointer->Tek_Count-1].pPointSub;
        pDispl->Parent = HoIs;
        pDispl->Focused = true;
        return true;
        }

    if(HoIs != pSub_Menu1) //��������� ����������
        {
        pDispl = pSub_Menu1;
        pDispl->Parent = HoIs;
        pDispl->Focused = true;
        return true;
        }
    return false;
}

static const char* p="WAITING!!!";
void LCD_Menu::Listener()
{
    char* resP;
    UInt32 tempColorJoyCalibration;
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }

    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        if(this==lensControlMenuPointer&&this->Tek_Count==LensCalibrateLensControlSetup)
            {

            Table_Cell[LensCalibrateLensControlSetupSistem]->Hide();

            p_pult->startLensCalibration();
            for(UInt32 i=0;i!=80;i++)
            {
                watchDogTimer.useKey(WD_KEY3);
                Task_sleep(10);
            }
            this->Table_Cell[LensCalibrateLensControlSetupSistem]->ReHide();
            this->Table_Cell[LensCalibrateLensControlSetupSistem]->ReDraw();
            return;
            }
 #ifdef joyPult
        if(this==lensControlMenuPointer&&this->Tek_Count==externalLensControl){
            return;
        }
        if(this==setupMenuPointer&&this->Tek_Count==JoystickCalibrationSetupMenu)
        {
            resP=Table_Cell[JoystickCalibrationSetupMenuSistem]->p_text;
            tempColorJoyCalibration=Table_Cell[JoystickCalibrationSetupMenuSistem]->UnActive_Style.Cell_Color;
            Table_Cell[JoystickCalibrationSetupMenuSistem]->SetText((char*)p);
            Table_Cell[JoystickCalibrationSetupMenuSistem]->UnActive_Style.Cell_Color=ClrLinen ;
            Table_Cell[JoystickCalibrationSetupMenuSistem]->Hide();

            for(UInt32 i=0;i!=300;i++)
            {
                watchDogTimer.useKey(WD_KEY3);
                Task_sleep(10);
            }

            EE_Working::WriteCalibrationData(EE_CAL_OFFSET_MAIN_PAN,    (UInt32)(p_pult->getCalibrationOffset(CALIB_OFFSET_MAIN_PAN)));
            EE_Working::WriteCalibrationData(EE_CAL_OFFSET_MAIN_DUTCH,  (UInt32)(p_pult->getCalibrationOffset(CALIB_OFFSET_MAIN_DUTCH)));
            EE_Working::WriteCalibrationData(EE_CAL_OFFSET_MAIN_TILT,   (UInt32)(p_pult->getCalibrationOffset(CALIB_OFFSET_MAIN_TILT)));
            EE_Working::WriteCalibrationData(EE_CAL_OFFSET_MAIN_ZOOM,   (UInt32)(p_pult->getCalibrationOffset(CALIB_OFFSET_MAIN_ZOOM)));
            EE_Working::WriteCalibrationData(EE_CAL_OFFSET_EXT_PAN,     (UInt32)(p_pult->getCalibrationOffset(CALIB_OFFSET_EXT_PAN)));
            EE_Working::WriteCalibrationData(EE_CAL_OFFSET_EXT_DUCTH,   (UInt32)(p_pult->getCalibrationOffset(CALIB_OFFSET_EXT_DUTCH)));
            EE_Working::WriteCalibrationData(EE_CAL_OFFSET_EXT_TILT,    (UInt32)(p_pult->getCalibrationOffset(CALIB_OFFSET_EXT_TILT)));
            EE_Working::WriteCalibrationData(EE_CAL_OFFSET_EXT2_DUCTH,  (UInt32)(p_pult->getCalibrationOffset(CALIB_OFFSET_EXT2_DUTCH)));

            Table_Cell[JoystickCalibrationSetupMenuSistem]->UnActive_Style.Cell_Color=tempColorJoyCalibration;

            Table_Cell[JoystickCalibrationSetupMenuSistem]->p_text=resP;
            this->Table_Cell[JoystickCalibrationSetupMenuSistem]->ReHide();
            this->Table_Cell[JoystickCalibrationSetupMenuSistem]->ReDraw();
            return;
        }
#endif

        if(this == pMain_Menu)
            {
            if(this->Tek_Count==MotionCtrlMainMenu)
                {
                mainScreenPointer->switchMotionMode();
                pDispl = pDispl->Parent;
                pDispl->Focused = true;
                return;
                }
            pDispl = (Menu_Link+(Tek_Count-1))->pPointSub;
            pDispl->Parent = this;
            pDispl->Focused = true;
            if(pDispl == pSet_Koeff) pDispl->Param = 100; //������ �� ����������� ���������, ����� ��� ����������
            return;
            }
        else
        {
            if(Menu_Listener(this)) return;
        }
    }

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        return;
    }
#ifdef joyPult
    if(this==lensControlMenuPointer&&this->Tek_Count==externalLensControl){
        if ((getButtonState(pult_Button_Right) == PRESSED)||(getButtonState(pult_Button_Left) == PRESSED))
            {
            p_pult->xorPreston();
            if (p_pult->getEnablePreston()) {
                Table_Cell[externalLensControlSistem]->p_text="EXTERNAL ZOOM ON";
                Table_Cell[externalLensControlSistem]->Active_Style.Cell_Color=ClrDarkSlateBlue;
                Table_Cell[externalLensControlSistem]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
                EE_Working::Write(EE_LC_EXTERNAL_LENS_CONTROL, 1);// ���������� ��� ������� �������
                }
            else {
                Table_Cell[externalLensControlSistem]->p_text="EXTERNAL ZOOM OFF";
                Table_Cell[externalLensControlSistem]->Active_Style.Cell_Color=ClrMediumOrchid;
                Table_Cell[externalLensControlSistem]->UnActive_Style.Cell_Color=ClrMediumOrchid;
                EE_Working::Write(EE_LC_EXTERNAL_LENS_CONTROL, 0);// ���������� ��� ������� �������
                }
            Table_Cell[externalLensControlSistem]->ReDraw();
            }
        }
#endif



    if (getButtonState(pult_Button_Dn) == PRESSED)  Plus();
    if (getButtonState(pult_Button_Up) == PRESSED)  Minus();

    if(Orientation==1) //���� ���������� 2*3 - ����� �������� ������ ������-�����
    {
        //������ ������
        if (getButtonState(pult_Button_Right) == PRESSED) Right();
        //������ �����
        if (getButtonState(pult_Button_Left) == PRESSED) Left();
    }
}

//----------------------------------------------------------------------------------------------------------
SetJoyDeadZone::SetJoyDeadZone(char* pNam):
     LCD_Menu()
{
    Counter_Cell = 4;
    Tek_Count = 1;
    Start = 1;
    Orientation = 0;
    Menu_On_Screen = 4;
    pName = pNam;

    values[0]=15;
    values[1]=15;
    values[2]=15;
    values[3]=15;

    //action();
}

void SetJoyDeadZone::Draw(byte Active) //������ ������� � ���������
{
    DrawHeader();
    Tek_Count = Active;
    DrawVert();
}


void SetJoyDeadZone::DrawVert() //��������� ������������� ����
{

    Int16 StepY;

    p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
    p_Pos_Size_XY.Xsize = 250; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
    StepY = p_Pos_Size_XY.Ysize + 5;

    sprintf(bufferNames[0],"PAN: %d", values[0]);
#ifdef USAEdition
    sprintf(bufferNames[1],"ROLL: %d", values[1]);
#else
    sprintf(bufferNames[1],"DUTCH: %d", values[1]);
#endif
    sprintf(bufferNames[2],"TILT: %d", values[2]);
    sprintf(bufferNames[3],"ZOOM: %d", values[3]);

    Table_Cell[0]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(0),
                                        p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                        bufferNames[0], Cell_UnActive);

    Table_Cell[1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(1),
                                        p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                        bufferNames[1], Cell_UnActive);

    Table_Cell[2]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(2),
                                        p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                        bufferNames[2], Cell_UnActive);

    Table_Cell[3]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(3),
                                        p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                        bufferNames[3], Cell_UnActive);

    Table_Cell[Tek_Count-1]->Draw();
}


void SetJoyDeadZone::upValue(UInt8 id)
{
    if(id>=4){return;}
    values[id]+=1;
    if(values[id]>200){values[id]=200;}
}
void SetJoyDeadZone::downValue(UInt8 id)
{
    if(id>=4){return;}
    values[id]-=1;
    if(values[id]<1){values[id]=1;}
}


void SetJoyDeadZone::action()
{
    p_pult->setJoyDeadZone(values[0],values[1],values[2],values[3]);
}

void SetJoyDeadZone::saveInEprom()
{
    EE_Working::Write(EE_LC_PAN_JOY_DZ,values[0]);
    EE_Working::Write(EE_LC_DUTCH_JOY_DZ,values[1]);
    EE_Working::Write(EE_LC_TILT_JOY_DZ,values[2]);
    EE_Working::Write(EE_LC_ZOOM_JOY_DZ,values[3]);
}

void SetJoyDeadZone::updateFromEeprom()
{
    UInt32 tmp[4];
    tmp[0]=EE_Working::Read(EE_LC_PAN_JOY_DZ);
    tmp[1]=EE_Working::Read(EE_LC_DUTCH_JOY_DZ);
    tmp[2]=EE_Working::Read(EE_LC_TILT_JOY_DZ);
    tmp[3]=EE_Working::Read(EE_LC_ZOOM_JOY_DZ);

    if(tmp[0]>200||tmp[1]>200||tmp[2]>200||tmp[3]>200)
    {
        values[0]=15;
        values[1]=15;
        values[2]=15;
        values[3]=15;

        saveInEprom();
        return;
    }

    values[0]=tmp[0];
    values[1]=tmp[1];
    values[2]=tmp[2];
    values[3]=tmp[3];
    action();

}
void SetJoyDeadZone::Listener()
{
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }

    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        saveInEprom();
    }

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;

        saveInEprom();

        return;
    }

    if (getButtonState(pult_Button_Dn) == PRESSED)  Plus();
    if (getButtonState(pult_Button_Up) == PRESSED)  Minus();

    if (getButtonState(pult_Button_Right) == PRESSED)
    {
        upValue(Tek_Count-1);
        action();
        Draw(Tek_Count);
        return;
    }
    if (getButtonState(pult_Button_Left) == PRESSED)
    {
        downValue(Tek_Count-1);
        action();
        Draw(Tek_Count);
        return;
    }
    if (getButtonState(pult_Button_Right) == HOLD)
    {
        upValue(Tek_Count-1);
        Draw(Tek_Count);
        action();
        Task_sleep(30);
        return;
    }
    if (getButtonState(pult_Button_Left) == HOLD)
    {
        downValue(Tek_Count-1);
        Draw(Tek_Count);
        action();
        Task_sleep(30);
        return;
    }



}
//----------------------------------------------------------------------------------------------------------

#define maxTorque 100


SetMaxTorque::SetMaxTorque(char* pNam):
     LCD_Menu()
{
    Counter_Cell = 4;
    Tek_Count = 1;
    Start = 1;
    Orientation = 0;
    Menu_On_Screen = 4;
    pName = pNam;
    eco=true;

    values[ecoModSistem]=1;
    values[panTorqueSistem]=maxTorque;
    values[dutchTorqueSistem]=maxTorque;
    values[tiltTorqueSistem]=maxTorque;
}

void SetMaxTorque::Draw(byte Active) //������ ������� � ���������
{
    DrawHeader();
    Tek_Count = Active;
    DrawVert();
}


void SetMaxTorque::DrawVert() //��������� ������������� ����
{

    Int16 StepY;

    p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
    p_Pos_Size_XY.Xsize = 250; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
    StepY = p_Pos_Size_XY.Ysize + 5;

    drawEco();
    sprintf(bufferNames[panTorqueSistem],"PAN: %d", values[panTorqueSistem]);
#ifdef USAEdition
    sprintf(bufferNames[dutchTorqueSistem],"ROOL: %d", values[dutchTorqueSistem]);
#else
    sprintf(bufferNames[dutchTorqueSistem],"DUTCH: %d", values[dutchTorqueSistem]);
#endif
    sprintf(bufferNames[tiltTorqueSistem],"TILT: %d", values[tiltTorqueSistem]);

    for(UInt32 i=0;i!=Menu_On_Screen;i++)
        {
        Table_Cell[i]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                            p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                            bufferNames[i], Cell_UnActive);     }
    Table_Cell[Tek_Count-1]->Draw();
}


void SetMaxTorque::upValue(UInt8 id)
{
    if(id>=4){return;}
    values[id]+=5;
    if(values[id]>95)
        values[id]=100;
}
void SetMaxTorque::downValue(UInt8 id)
{
    if(id>=4){return;}
    values[id]-=5;
    if(values[id]<30)
        values[id]=30;
}


void SetMaxTorque::action()
{
    p_pult->setMaxTorque(values[panTorqueSistem],values[dutchTorqueSistem],values[tiltTorqueSistem]);
    p_pult->setEcoMode(eco);
}

void SetMaxTorque::saveInEprom()
{
    EE_Working::Write(EE_LC_ECO_MOD_ON,values[ecoModSistem]);
    EE_Working::Write(EE_LC_PAN_MAX_TORQUE,values[panTorqueSistem]);
    EE_Working::Write(EE_LC_DUTCH_MAX_TORQUE,values[dutchTorqueSistem]);
    EE_Working::Write(EE_LC_TILT_MAX_TORQUE,values[tiltTorqueSistem]);
}

void SetMaxTorque::updateFromEeprom()
{
    UInt32 tmp[4];
    tmp[ecoModSistem]=EE_Working::Read(EE_LC_ECO_MOD_ON);
    tmp[panTorqueSistem]=EE_Working::Read(EE_LC_PAN_MAX_TORQUE);
    tmp[dutchTorqueSistem]=EE_Working::Read(EE_LC_DUTCH_MAX_TORQUE);
    tmp[tiltTorqueSistem]=EE_Working::Read(EE_LC_TILT_MAX_TORQUE);


    if(tmp[panTorqueSistem]>maxTorque||tmp[dutchTorqueSistem]>maxTorque||
            tmp[tiltTorqueSistem]>maxTorque||tmp[ecoModSistem]>1)
    {
        values[panTorqueSistem]=maxTorque;
        values[dutchTorqueSistem]=maxTorque;
        values[tiltTorqueSistem]=maxTorque;
        values[ecoModSistem]=1;

        saveInEprom();
        return;
    }

    values[ecoModSistem]=tmp[ecoModSistem];
    eco=(bool)values[ecoModSistem];
    values[panTorqueSistem]=tmp[panTorqueSistem];
    values[dutchTorqueSistem]=tmp[dutchTorqueSistem];
    values[tiltTorqueSistem]=tmp[tiltTorqueSistem];

    action();
}
void SetMaxTorque::Listener()
{
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }

    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        saveInEprom();
    }

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;

        saveInEprom();

        return;
    }

    if (getButtonState(pult_Button_Dn) == PRESSED)  {  Plus();  }

    if (getButtonState(pult_Button_Up) == PRESSED)  {  Minus(); }

    if (getButtonState(pult_Button_Right) == PRESSED)
        {
        if (Tek_Count==ecoMod) {
            setEco();
            }
        else{
        upValue(Tek_Count-1);
        action();}
        Draw(Tek_Count);
        return;
        }
    if (getButtonState(pult_Button_Left) == PRESSED)
    {
        if (Tek_Count==ecoMod) {
            setEco();
            }
        else{
        downValue(Tek_Count-1);
        action();}
        Draw(Tek_Count);
        return;
    }
    if (getButtonState(pult_Button_Right) == HOLD)
    {
        if (Tek_Count==ecoMod)
            return;
        upValue(Tek_Count-1);
        Draw(Tek_Count);
        action();
        Task_sleep(30);
        return;
    }
    if (getButtonState(pult_Button_Left) == HOLD)
    {
        if (Tek_Count==ecoMod)
        return;
        downValue(Tek_Count-1);
        Draw(Tek_Count);
        action();
        Task_sleep(30);
        return;
    }
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
SetMotionPrerol::SetMotionPrerol(char* pNam):
     LCD_Menu()
{
    Counter_Cell = 4;
    Tek_Count = 1;
    Start = 1;
    Orientation = 0;
    Menu_On_Screen = 4;
    pName = pNam;
    values[0]=0;
    values[1]=0;
    values[2]=0;
    values[3]=0;
}

void SetMotionPrerol::Draw(byte Active) //������ ������� � ���������
{
    DrawHeader();
    Tek_Count = Active;
    DrawVert();
}


void SetMotionPrerol::DrawVert() //��������� ������������� ����
{
    Int16 StepY;

    p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
    p_Pos_Size_XY.Xsize = 250; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
    StepY = p_Pos_Size_XY.Ysize + 5;

    sprintf(bufferNames[0],"Master: %d",  values[0]);
    sprintf(bufferNames[1],"Slave1: %d",  values[1]);
    sprintf(bufferNames[2],"Slave2: %d",  values[2]);
    sprintf(bufferNames[3],"Music: %d",   values[3]);

    for(UInt32 i=0;i!=Menu_On_Screen;i++)
    {
        Table_Cell[i]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                bufferNames[i], Cell_UnActive);
    }

    Table_Cell[Tek_Count-1]->Draw();
}


void SetMotionPrerol::upValue(UInt8 id)
{
    if(values[Tek_Count-1]>=30000){values[Tek_Count-1]=30000;}
    else{values[Tek_Count-1]+=100;}
}
void SetMotionPrerol::downValue(UInt8 id)
{
    if(values[Tek_Count-1]<=0){values[Tek_Count-1]=0;}
    else{values[Tek_Count-1]-=100;}
}


void SetMotionPrerol::action()
{
    p_pult->setMasterPrerolValue(values[0]);
    p_pult->setSlave1PrerolValue(values[1]);
    p_pult->setSlave2PrerolValue(values[2]);
    p_pult->setSlave3PrerolValue(values[3]);
}

void SetMotionPrerol::saveInEprom()
{
    EE_Working::Write(EE_LC_MOTION_MASTER_PREROL,values[0]);
    EE_Working::Write(EE_LC_MOTION_SLAVE1_PREROL,values[1]);
    EE_Working::Write(EE_LC_MOTION_SLAVE2_PREROL,values[2]);
    EE_Working::Write(EE_LC_MOTION_SLAVE3_PREROL,values[3]);
}

void SetMotionPrerol::updateFromEeprom()
{
    UInt32 tmp[4];
    tmp[0]=EE_Working::Read(EE_LC_MOTION_MASTER_PREROL);
    tmp[1]=EE_Working::Read(EE_LC_MOTION_SLAVE1_PREROL);
    tmp[2]=EE_Working::Read(EE_LC_MOTION_SLAVE2_PREROL);
    tmp[3]=EE_Working::Read(EE_LC_MOTION_SLAVE3_PREROL);


    for(UInt8 i=0;i!=4;i++)
    {
        if(tmp[i]>30000)
        {
            values[0]=0;
            values[1]=0;
            values[2]=0;
            values[3]=0;
            saveInEprom();
            return;
        }
    }
    values[0]=tmp[0];
    values[1]=tmp[1];
    values[2]=tmp[2];
    values[3]=tmp[3];
    action();

}
void SetMotionPrerol::Listener()
{
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }

    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        saveInEprom();
    }

    if (getButtonState(pult_Button_Dn) == PRESSED)  Plus();
    if (getButtonState(pult_Button_Up) == PRESSED)  Minus();

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;

        saveInEprom();

        return;
    }

    if (getButtonState(pult_Button_Right) == PRESSED)
    {
        upValue(Tek_Count-1);
        action();
        Draw(Tek_Count);
        return;
    }
    if (getButtonState(pult_Button_Left) == PRESSED)
    {
        downValue(Tek_Count-1);
        action();
        Draw(Tek_Count);
        return;
    }
    if (getButtonState(pult_Button_Right) == HOLD)
    {
        upValue(Tek_Count-1);
        Draw(Tek_Count);
        action();
        Task_sleep(30);
        return;
    }
    if (getButtonState(pult_Button_Left) == HOLD)
    {
        downValue(Tek_Count-1);
        Draw(Tek_Count);
        action();
        Task_sleep(30);
        return;
    }
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
FolowingModeMenu::FolowingModeMenu(char* pNam):
     LCD_Menu()
{
    Counter_Cell = 2;
    Tek_Count = 1;
    Start = 1;
    Orientation = 0;
    Menu_On_Screen = 2;
    pName = pNam;

    data.all=0;
//  values[0]=0;
//  values[1]=0;
//  values[2]=0;
//  values[3]=0;
}

void FolowingModeMenu::Draw(byte Active) //������ ������� � ���������
{
    DrawHeader();
    Tek_Count = Active;
    DrawVert();
}


void FolowingModeMenu::DrawVert() //��������� ������������� ����
{
    Int16 StepY;

    p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
    p_Pos_Size_XY.Xsize = 250; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
    StepY = p_Pos_Size_XY.Ysize + 5;

    switch(data.mode)
    {
        case FOLOWING_MODE_STABILIZATION:
            strcpy(bufferNames[0], "STABILIZATION");
            strcpy(bufferNames[1], "        ");

            break;

        case FOLOWING_MODE_FOLOWING:
            strcpy(bufferNames[0], "FOLOWING");
            strcpy(bufferNames[1], "       ");
            break;

        case FOLOWING_MODE_MIXED:
            strcpy(bufferNames[0], "MIXED");
            sprintf(bufferNames[1],"SECTOR: %d ",  data.sector);
            break;
        default:
            strcpy(bufferNames[0], "STABILIZATION");
            strcpy(bufferNames[1], "       ");
            break;
    }




     if(data.mode==FOLOWING_MODE_MIXED)
     {
         Table_Cell[0]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(0),
                                  p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                  bufferNames[0], Cell_UnActive);

         Table_Cell[1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(1),
                                  p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                  bufferNames[1], Cell_UnActive);

         Table_Cell[Tek_Count-1]->Draw();
     }
     else
     {
         Table_Cell[0]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(0),
                                  p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                  bufferNames[0], Cell_UnActive);

         Table_Cell[1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(1),
                                  p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                  bufferNames[1], Cell_UnActive);

         Table_Cell[1]->Clean();

         Table_Cell[0]->Draw();
     }

}


void FolowingModeMenu::upValue(UInt8 id)
{
    switch(Tek_Count-1)
    {
        case 0:
            switch(data.mode)
            {
                case FOLOWING_MODE_STABILIZATION:   data.mode=  FOLOWING_MODE_FOLOWING;     break;
                case FOLOWING_MODE_FOLOWING:        data.mode=  FOLOWING_MODE_MIXED;        break;
                case FOLOWING_MODE_MIXED:           data.mode=  FOLOWING_MODE_STABILIZATION;break;
            }
            break;
        case 1:
            if(data.sector>=FOLOWING_MODE_MAX_SECTOR)    {   data.sector=FOLOWING_MODE_MAX_SECTOR; }
            else                                         {   data.sector++;  }
            break;
    }
}
void FolowingModeMenu::downValue(UInt8 id)
{
    switch(Tek_Count-1)
    {
        case 0:
            switch(data.mode)
            {
                   case FOLOWING_MODE_STABILIZATION:   data.mode=  FOLOWING_MODE_MIXED;             break;
                   case FOLOWING_MODE_FOLOWING:        data.mode=  FOLOWING_MODE_STABILIZATION;     break;
                   case FOLOWING_MODE_MIXED:           data.mode=  FOLOWING_MODE_FOLOWING;          break;
            }
            break;
        case 1:
            if(data.sector<=1)  {   data.sector=1; }
            else                {   data.sector--;  }
            break;
    }
}


void FolowingModeMenu::action()
{
    p_pult->setPanFolowingMode(data);
}

void FolowingModeMenu::saveInEprom()
{
    EE_Working::Write(EE_LC_PAN_FOLOWING_SETTINGS,data.all);
}

void FolowingModeMenu::updateFromEeprom()
{
    FolowingModeConfigData tmp;
    tmp.all=EE_Working::Read(EE_LC_PAN_FOLOWING_SETTINGS);

    if((tmp.mode>=FOLOWING_MODE_CNT)||(tmp.sector>FOLOWING_MODE_MAX_SECTOR)||(tmp.sector<1))
    {
        tmp.all=0;
    }
    data.all=tmp.all;
    action();
}

void FolowingModeMenu::Listener()
{
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }

    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        saveInEprom();
    }

    if (getButtonState(pult_Button_Dn) == PRESSED)
    {
        if(data.mode==FOLOWING_MODE_MIXED)
        {
            Plus();
        }
        else
        {
            Tek_Count=1;
        }
    }

    if (getButtonState(pult_Button_Up) == PRESSED)
    {
        if(data.mode==FOLOWING_MODE_MIXED)
        {
            Minus();
        }
        else
        {
            Tek_Count=1;
        }
    }

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;

        saveInEprom();

        return;
    }

    if (getButtonState(pult_Button_Right) == PRESSED)
    {
        upValue(Tek_Count-1);
        action();
        Draw(Tek_Count);
        return;
    }
    if (getButtonState(pult_Button_Left) == PRESSED)
    {
        downValue(Tek_Count-1);
        action();
        Draw(Tek_Count);
        return;
    }
    if (getButtonState(pult_Button_Right) == HOLD)
    {
        upValue(Tek_Count-1);
        Draw(Tek_Count);
        action();
        Task_sleep(30);
        return;
    }
    if (getButtonState(pult_Button_Left) == HOLD)
    {
        downValue(Tek_Count-1);
        Draw(Tek_Count);
        action();
        Task_sleep(30);
        return;
    }
}

//----------------------------------------------------------------------------------------------------------
HourMeterMenu::HourMeterMenu(char* pNam):
     LCD_Menu()
{
    Counter_Cell = 2;
    Tek_Count = 1;
    Start = 1;
    Orientation = 0;
    Menu_On_Screen = 2;
    pName = pNam;

    rawValue=0;
}

void HourMeterMenu::Draw(byte Active) //������ ������� � ���������
{
    DrawHeader();
    Tek_Count = Active;
    DrawVert();
}


void HourMeterMenu::DrawVert() //��������� ������������� ����
{
    Int16 StepY;
    p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
    p_Pos_Size_XY.Xsize = 250; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
    StepY = p_Pos_Size_XY.Ysize + 5;

    sprintf(bufferName[0],"Runtime: %d", (rawValue/60));
    sprintf(bufferName[1],"OPT : %#010X", calcHash(rawValue));

    for(UInt32 i=0;i!=Menu_On_Screen;i++)
    {
        Table_Cell[i]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                            p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                            bufferName[i], Cell_Active);
    }
}

void HourMeterMenu::Listener()
{
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        return;
    }

    if(rawValue!=HourMeter_rawValue)
    {
        rawValue=HourMeter_rawValue;
        Draw(Tek_Count);
    }
}

void HourMeterMenu::setRawValue(UInt32 rawV)
{
    rawValue=rawV;
    Draw(Tek_Count);
}


UInt32 HourMeterMenu::calcHash(UInt32 val)
{
    UInt32Bytes bytes;
    UInt32Bytes newBytes;
    bytes.all=val;
    newBytes.all=0;

    newBytes.bytes.byte0=bytes.bytes.byte1;
    newBytes.bytes.byte1=bytes.bytes.byte3;
    newBytes.bytes.byte2=bytes.bytes.byte2;
    newBytes.bytes.byte3=bytes.bytes.byte0;

    UInt32 tmp=newBytes.all;
    UInt32 hb1=(tmp&0x80000000)>>29;
    UInt32 hb2=(tmp&0x40000000)>>29;
    UInt32 hb3=(tmp&0x20000000)>>29;

    tmp=tmp<<3;
    tmp=tmp|hb1|hb2|hb3;

    tmp=tmp^0xDFB242E5;
    return tmp;
}

//----------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------
SetZoomDrift::SetZoomDrift(char* pNam):
     LCD_Menu()
{
    Counter_Cell = 1;
    Tek_Count = 1;
    Start = 1;
    Orientation = 0;
    Menu_On_Screen = 1;
    pName = pNam;

    value=0;
}

void SetZoomDrift::Draw(byte Active) //������ ������� � ���������
{
    DrawHeader();
    Tek_Count = Active;
    DrawVert();
}


void SetZoomDrift::DrawVert() //��������� ������������� ����
{
    p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
    p_Pos_Size_XY.Xsize = 250; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35

    sprintf(bufferName,"ZOOM DRIFT: %d", value);

    Table_Cell[0]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+(p_Pos_Size_XY.Ysize/4),p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize/2,bufferName, Cell_Active);
    Table_Cell[0]->Draw();
}


void SetZoomDrift::upValue(UInt8 id)
{
    if(value>=100){value=100;}
    else{value+=1;}
}
void SetZoomDrift::downValue(UInt8 id)
{
    if(value<=-100){value=-100;}
    else{value-=1;}
}


void SetZoomDrift::action()
{
    p_pult->setZoomDrift(value);
}

void SetZoomDrift::saveInEprom()
{
    EE_Working::Write(EE_LC_ZOOM_DRIFT,value);
}

void SetZoomDrift::updateFromEeprom()
{
    Int32 tmp;
    tmp=EE_Working::Read(EE_LC_ZOOM_DRIFT);

    if(tmp>100||tmp<-100)
    {
        value=0;
        saveInEprom();
        return;
    }

    value=tmp;
    action();
}
void SetZoomDrift::Listener()
{
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }

    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        saveInEprom();
    }

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;

        saveInEprom();

        return;
    }

    if (getButtonState(pult_Button_Right) == PRESSED)
    {
        upValue(Tek_Count-1);
        action();
        Draw(Tek_Count);
        return;
    }
    if (getButtonState(pult_Button_Left) == PRESSED)
    {
        downValue(Tek_Count-1);
        action();
        Draw(Tek_Count);
        return;
    }
    if (getButtonState(pult_Button_Right) == HOLD)
    {
        upValue(Tek_Count-1);
        Draw(Tek_Count);
        action();
        Task_sleep(30);
        return;
    }
    if (getButtonState(pult_Button_Left) == HOLD)
    {
        downValue(Tek_Count-1);
        Draw(Tek_Count);
        action();
        Task_sleep(30);
        return;
    }
}

//----------------------------------------------------------------------------------------------------------
SetStartFluid::SetStartFluid(char* pNam):
    SetMaxTorque(pNam)
{
    Counter_Cell = 4;
    Menu_On_Screen = 4;

    values[0]=100;
    values[1]=100;
    values[2]=100;
    values[3]=100;
}


void SetStartFluid::upValue(UInt8 id)
{
    if(id>=4){return;}
    if(values[id]>=99){values[id]=100;}
    else{values[id]+=1;}
}
void SetStartFluid::downValue(UInt8 id)
{
    if(id>=4){return;}
    if(values[id]<=1){values[id]=0;}
    else{values[id]-=1;}
}


void SetStartFluid::action()
{
    p_pult->setStartFluid(values[0],values[1],values[2],values[3]);
}

void SetStartFluid::saveInEprom()
{
    EE_Working::Write(EE_LC_PAN_START_FLUID,values[0]);
    EE_Working::Write(EE_LC_DUTCH_START_FLUID,values[1]);
    EE_Working::Write(EE_LC_TILT_START_FLUID,values[2]);
    EE_Working::Write(EE_LC_ZOOM_START_FLUID,values[3]);
}

void SetStartFluid::updateFromEeprom()
{
    UInt32 tmp[4];
    tmp[0]=EE_Working::Read(EE_LC_PAN_START_FLUID);
    tmp[1]=EE_Working::Read(EE_LC_DUTCH_START_FLUID);
    tmp[2]=EE_Working::Read(EE_LC_TILT_START_FLUID);
    tmp[3]=EE_Working::Read(EE_LC_ZOOM_START_FLUID);

    if(tmp[0]>100||tmp[1]>100||tmp[2]>100||tmp[3]>100)
    {
        values[0]=100;
        values[1]=100;
        values[2]=100;
        values[3]=100;

        saveInEprom();
        return;
    }

    values[0]=tmp[0];
    values[1]=tmp[1];
    values[2]=tmp[2];
    values[3]=tmp[3];
    action();
}

//----------------------------------------------------------------------------------------------------------


//-------------------- SELECT MENU -------------------------------------------------------------------------
SelectMenu::SelectMenu (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress):
        LCD_Menu(pNam, Link, Count,0, Menu_Per_Scr),
        currentEepromAddress(eepromAddress)
{
    updateFromEEPROM();
}
void SelectMenu::Draw(byte Active)
{
    DrawHeader();
    Tek_Count = Active;
    DrawVert();
}
void SelectMenu::DrawVert()
{
    byte i;
        Int16 StepY;

        {
            p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
            p_Pos_Size_XY.Xsize = 250; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
            StepY = p_Pos_Size_XY.Ysize + 5;

            for(i=0;i<Menu_On_Screen;i++)
            {
                Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                        p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                        Table_Cell[Start+i-1]->p_text, Cell_UnActive);
                if( (Start+i) == Counter_Cell) break;
            }
        }
        Table_Cell[Tek_Count-1]->Draw();
}

void  SelectMenu::updateFromEEPROM()
{
    UInt32 tmp=EE_Working::Read(currentEepromAddress)+1;
    Tek_Count=tmp;
    if(tmp<1||tmp>Counter_Cell)
    {
        Tek_Count=1;
        saveInEEPROM();
    }
    Table_Cell[Tek_Count-1]->Active_Style.Cell_Color=ClrDarkSlateBlue;
    Table_Cell[Tek_Count-1]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
    action();
}
void  SelectMenu::saveInEEPROM()
{
    EE_Working::Write(currentEepromAddress, Tek_Count-1);
}
void  SelectMenu::action()
{

}
void  SelectMenu::Listener()
{
    if(Focused)
        {
            ClearDisp();
            Draw(Tek_Count);
            Focused = false;
        }

        if (getButtonState(pult_Button_Select) == PRESSED)
        {
            uint8_t state = EE_Working::Read(currentEepromAddress);
            if (state == (Tek_Count-1)) return;
            Table_Cell[state]->Active_Style.Cell_Color=ClrMediumOrchid;
            Table_Cell[state]->UnActive_Style.Cell_Color=ClrMediumOrchid;
            uint8_t count = Tek_Count;
            Table_Cell[state]->Hide();
            Tek_Count=count;
            Table_Cell[Tek_Count-1]->Active_Style=Style_MenuSelected;
            Draw(Tek_Count);
            Task_sleep(400);
            Table_Cell[Tek_Count-1]->Active_Style=Style_MenuActive;
            Table_Cell[Tek_Count-1]->Active_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[Tek_Count-1]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Draw(Tek_Count);
            this->saveInEEPROM();
            action();
        }

        if (getButtonState(pult_Button_ESC) == PRESSED)
        {
            pDispl = pDispl->Parent;
            pDispl->Focused = true;
            return;
        }

        if (getButtonState(pult_Button_Dn) == PRESSED)  Plus();
        if (getButtonState(pult_Button_Up) == PRESSED)  Minus();

}

SelectMenuCameraStart::SelectMenuCameraStart (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress):
        SelectMenu (pNam,Link, Count, Menu_Per_Scr, eepromAddress)
{

}
void SelectMenuCameraStart::action()
{
    switch(Tek_Count)
    {
        case 1:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:p_pult->setCameraStartOptions(CAMERA_START_FRONT);break;

        case 2:
        case 8:p_pult->setCameraStartOptions(CAMERA_START_LEVEL);break;

        default:p_pult->setCameraStartOptions(CAMERA_START_FRONT);break;

    }

}

SelectMenuSpiderSelect::SelectMenuSpiderSelect (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress,UInt32 eepromEditAddr,SuspensionResonansChannel ch):
        SelectMenu (pNam,  Link, Count,  Menu_Per_Scr, eepromAddress),
        channel(ch),
        eepromEditValueAddress(eepromEditAddr)
{
    editValue=2.5;
}
void SelectMenuSpiderSelect::action()
{
    switch(this->Tek_Count)
    {

        case 3:
            p_pult->setSuspentionResonantMode(channel,SUSPENSION_RESONANCE_MODE_DISABLE);
            p_pult->setSuspentionResonantFreq(2.5,channel);
            break;
        case 2:
            p_pult->setSuspentionResonantMode(channel,SUSPENSION_RESONANCE_MODE_ENABLE);
            p_pult->setSuspentionResonantFreq(editValue,channel);break;
        case 1:
        default:
            p_pult->setSuspentionResonantMode(channel,SUSPENSION_RESONANCE_MODE_ENABLE);
            p_pult->setSuspentionResonantFreq(2.5,channel);break;
    }
}

void  SelectMenuSpiderSelect::Listener()
{
    if(Focused)
        {
            ClearDisp();
            Draw(Tek_Count);
            Focused = false;
        }

        if (getButtonState(pult_Button_Select) == PRESSED)
        {
            saveInEEPROM();
            action();
            Draw(Tek_Count);
        }

        if (getButtonState(pult_Button_ESC) == PRESSED)
        {
            pDispl = pDispl->Parent;
            pDispl->Focused = true;
            return;
        }

        if (getButtonState(pult_Button_Dn) == PRESSED)  Plus();
        if (getButtonState(pult_Button_Up) == PRESSED)  Minus();

        if(Tek_Count==2)   //isEditaction
        {
            if (getButtonState(pult_Button_Right) == PRESSED)
            {
                if(editValue>8.9){editValue=9;}
                else{editValue+=0.1;}
                Draw(Tek_Count);
                EE_Working::Write(eepromEditValueAddress,   *((UInt32*)(&editValue)));
                action();
            }
            if (getButtonState(pult_Button_Left) == PRESSED)
            {
                if(editValue<=0.2){editValue=0.1;}
                else{editValue-=0.1;}
                Draw(Tek_Count);
                EE_Working::Write(eepromEditValueAddress,   *((UInt32*)(&editValue)));
                action();
            }
        }
}
void  SelectMenuSpiderSelect::updateFromEEPROM()
{
    UInt32 tmp=EE_Working::Read(currentEepromAddress)+1;
    if(tmp<1||tmp>Counter_Cell)
    {
        Tek_Count=1;
        saveInEEPROM();
        return;
    }
    Tek_Count=tmp;

    tmp=EE_Working::Read(eepromEditValueAddress);
    float tmp2=*((float*)(&tmp));
    if(tmp2<0.01||tmp2>10)
    {
        editValue=2.5;
        EE_Working::Write(eepromEditValueAddress,   *((UInt32*)(&editValue)));
        return;
    }
    editValue=tmp2;
    action();
}
static char selectMenuSpiderBuffer[15];
void SelectMenuSpiderSelect::DrawVert()
{
    byte i;
        Int16 StepY;

        {
            p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
            p_Pos_Size_XY.Xsize = 250; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
            StepY = p_Pos_Size_XY.Ysize + 5;

            for(i=0;i<Menu_On_Screen;i++)
            {
                if((Start+i-1)==1)
                {
                        sprintf(selectMenuSpiderBuffer, "EDIT: %3.2f", editValue);
                        Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                                                p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                                                selectMenuSpiderBuffer, Cell_UnActive);
                }
                else
                {
                        Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                                                p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                                                Table_Cell[Start+i-1]->p_text, Cell_UnActive);
                }
                if( (Start+i) == Counter_Cell) break;
            }
        }

        Table_Cell[Tek_Count-1]->Draw();

}


SelectMenuZoomSense::SelectMenuZoomSense (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress):
        SelectMenu (pNam,  Link, Count,  Menu_Per_Scr, eepromAddress)
{

}
void SelectMenuZoomSense::action()
{
    p_pult->setJoystickPresetId(ZoomJoystickPreset,Tek_Count-1);
}
//---------------------------------------------------------------
SelectMenuOverslang::SelectMenuOverslang (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress):
        SelectMenu (pNam,  Link, Count,  Menu_Per_Scr, eepromAddress)
{

}

void SelectMenuOverslang::action()
{
    p_pult->setOverslangState( Tek_Count==1 );
}
//--------------------------------------------------------------------------------------------------
SelectMenuSynchronization::SelectMenuSynchronization (char* pNam, tMenu_Link* Link, byte Count, byte Menu_Per_Scr, UInt32 eepromAddress):
        SelectMenu (pNam,  Link, Count,  Menu_Per_Scr, eepromAddress)
{

}

void SelectMenuSynchronization::action()
{
    p_pult->setSynchroSource( Tek_Count-1 );
    if (Tek_Count==3)
    {
        if ((getButtonState(pult_Button_Left)==PRESSED)||(getButtonState(pult_Button_Left)==HOLD))
        {
            camId++;
            EE_Working::Write(EE_LC_CAMERA_ID_DOPREAL, (uint32_t)camId);
 //           textDopReal
        }
        if ((getButtonState(pult_Button_Right)==PRESSED)||(getButtonState(pult_Button_Right)==HOLD))
        {
            camId--;
            EE_Working::Write(EE_LC_CAMERA_ID_DOPREAL, (uint32_t)camId);
        }
        sprintf(&textDopReal[15], "%3d", camId);
    }
}

void SelectMenuSynchronization::updateFromEEPROM()
{
    UInt32 tmp=EE_Working::Read(currentEepromAddress)+1;
        Tek_Count=tmp;
        if(tmp<1||tmp>Counter_Cell)
        {
            Tek_Count=1;
            saveInEEPROM();
        }
        Table_Cell[Tek_Count-1]->Active_Style.Cell_Color=ClrDarkSlateBlue;
        Table_Cell[Tek_Count-1]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
        uint32_t val = EE_Working::Read(EE_LC_CAMERA_ID_DOPREAL);
        if (val>0xff)
        {
            val = 0;
            EE_Working::Write(EE_LC_CAMERA_ID_DOPREAL, val);
            camId = 0;
        }
        camId=val;
        action();
}

//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
SecretMenu::SecretMenu(char* pNam,  byte Count,byte Menu_Per_Scr):
    SecretMenuView( pNam,  Count,Menu_Per_Scr)
    {
        readFromEEPROM();
        stepValue=0.0001;
    }

void SecretMenu::readFromEEPROM()
{
    UInt32 tmp2;
    for(UInt8 i=0;i!=10;i++)
    {
        tmp2=EE_Working::Read(ABS_PARAMETR0+i);
        values[i]=*((float*)(&tmp2));

        if(values[i]>129||values[i]<-129)
        {
            for(UInt8 j=0;j!=10;j++)
            {
                EE_Working::Write(ABS_PARAMETR0+j,0x00);
                values[j]=0;
            }
            return;
        }
    }
}
void SecretMenu::upValue(bool faster)
{
    if(newValue>127){newValue=-127;}

    if(faster){newValue+=1;}else{newValue+=stepValue;}

    renderCellString(names[Tek_Count-1],oldValue,newValue, true);

    Table_Cell[Tek_Count-1]->Active_Style=Style_MenuSelected;
    Table_Cell[Tek_Count-1]->SetText(bufferBig);
    Table_Cell[Tek_Count-1]->ReDraw();
}
void SecretMenu::downValue(bool faster)
{
    if(newValue<-127){newValue=127;}

    if(faster){newValue-=1;}else{newValue-=stepValue;}

    renderCellString(names[Tek_Count-1],oldValue,newValue, true);

    Table_Cell[Tek_Count-1]->Active_Style=Style_MenuSelected;
    Table_Cell[Tek_Count-1]->SetText(bufferBig);
    Table_Cell[Tek_Count-1]->ReDraw();
}

void  SecretMenu::updateDataFromPult()
{
    List<EditVar*>* varList=p_pult->getViewLists()->getEditVarList();
    Counter_Cell=varList->getSize();
    if(Counter_Cell>10){Counter_Cell=10;}
    for(UInt8 i=0;i!=Counter_Cell;i++)
    {
        (varList->get(i))->setNewValue(values[i]);
        names[i]=(char*)(varList->get(i)->name);
    }
    p_pult->sendDebugValue();
}
void SecretMenu::Listener()
{
    UInt32 tmp;
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }
    if(inputState==1)
    {
        switch(getButtonState(pult_Button_Dn))
        {
            case PRESSED:
                downValue((getButtonState(pult_Button_Left)==HOLD));
                return;
            case HOLD:
                downValue((getButtonState(pult_Button_Left)==HOLD));
                Task_sleep(30);
                return;
        }

        switch(getButtonState(pult_Button_Up))
        {
            case PRESSED:
                upValue((getButtonState(pult_Button_Left)==HOLD));
                return;
            case HOLD:
                upValue((getButtonState(pult_Button_Left)==HOLD));
                Task_sleep(30);
                return;
        }

        if (getButtonState(pult_Button_ESC) == PRESSED)
        {
            Table_Cell[Tek_Count-1]->Active_Style=Style_MenuActive;
            inputState=0;
            Draw(Tek_Count);
            return;

        }
        if (getButtonState(pult_Button_Select) == PRESSED)
        {
            values[Tek_Count-1]=newValue;
            Table_Cell[Tek_Count-1]->Active_Style=Style_MenuActive;
            inputState=0;
            tmp=*((UInt32*)(&newValue));
            EE_Working::Write(ABS_PARAMETR0+Tek_Count-1,tmp);
            tmp=EE_Working::Read(ABS_PARAMETR0+Tek_Count-1);
            values[Tek_Count-1]=*((float*)(&tmp));
            updateDataFromPult();
            Draw(Tek_Count);
        }

        if (getButtonState(pult_Button_Right)==PRESSED) {
            stepValue*=10.0;
            if (stepValue>0.1)
                stepValue=0.1;
            Task_sleep(30);
            return;
        }

        if (getButtonState(pult_Button_Left)==PRESSED) {
            stepValue*=0.1;
            if (stepValue<0.0001)
                stepValue=0.0001;
            Task_sleep(30);
            return;
            }
        return;


    }
    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        inputState=1;
        oldValue=values[Tek_Count-1];
        newValue=values[Tek_Count-1];

        renderCellString(names[Tek_Count-1],oldValue,newValue, true);

        Table_Cell[Tek_Count-1]->Active_Style=Style_MenuSelected;
        Table_Cell[Tek_Count-1]->SetText(bufferBig);
        Table_Cell[Tek_Count-1]->ReDraw();
    }



    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        return;
    }

    if (getButtonState(pult_Button_Dn) == PRESSED)  Plus();
    if (getButtonState(pult_Button_Up) == PRESSED)  Minus();

    if(Orientation==1) //���� ���������� 2*3 - ����� �������� ������ ������-�����
    {
        //������ ������
        if (getButtonState(pult_Button_Right) == PRESSED) Right();
        //������ �����
        if (getButtonState(pult_Button_Left) == PRESSED) Left();
    }
}


SecretMenuView::SecretMenuView(char* pNam,  byte Count,byte Menu_Per_Scr):
    LCD_Menu()
    {
        Counter_Cell = Count;
        Tek_Count = 1;
        Start = 1;
        Orientation = 0;
        Menu_On_Screen = 5;
        pName = pNam;
        counter=0;
        maxCount=20;
    }

void SecretMenuView::DrawVert() //��������� ������������� ����
{
    byte i;
    Int16 StepY;

    {
        p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
        p_Pos_Size_XY.Xsize = 250; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
        StepY = p_Pos_Size_XY.Ysize + 5;

        for(i=0;i<Menu_On_Screen;i++)
        {
            if(Counter_Cell==0){break;}


            renderCellString(names[Start+i-1],values[Start+i-1],0, false);

            if((Tek_Count)==(Start+i))
            {
                Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                                    p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                                    bufferBig, Cell_Active);//Table_Cell[Start+i-1]->p_text
            }

            else
            {

                Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                        p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                        bufferBig, Cell_UnActive);//Table_Cell[Start+i-1]->p_text
            }
            if( (Start+i) == Counter_Cell) break;
        }
    }
    renderCellString(names[Tek_Count-1],values[Tek_Count-1],0, false);
    Table_Cell[Tek_Count-1]->SetText(bufferBig);
    Table_Cell[Tek_Count-1]->Draw();
}

void  SecretMenuView::updateDataFromPult()
{
    List<Var*>* varList=p_pult->getViewLists()->getVarList();
    Counter_Cell=varList->getSize();
    if(Counter_Cell>10){Counter_Cell=10;}
    for(UInt8 i=0;i!=Counter_Cell;i++)
    {
        values[i]=(varList->get(i))->getValue();
        names[i]=(char*)(varList->get(i)->name);
    }
}

void  SecretMenuView::renderCellString(char* head, float var1, float var2, bool isTwoValueStyle)
{
    if(isTwoValueStyle)
        {
        strcpy(bufferBig,head);
        sprintf(bufSmall, "%8.4f", var1);
        strcat(bufferBig, bufSmall);
        strcat(bufferBig, " : ");
        sprintf(bufSmall, "%8.4f", var2);
        strcat(bufferBig, bufSmall);
        }
    else
        {
        strcpy(bufferBig,head);
        sprintf(bufSmall, "%8.4f", var1);
        strcat(bufferBig, bufSmall);
        }
}


void SecretMenuView::Draw(byte Active) //������ ������� � ���������
{
    //ClearDisp(); //������� ������ �������� ������

    DrawHeader();
    Tek_Count = Active;
    DrawVert();
    Table_Cell[Active-1]->Draw();
}


void  SecretMenuView::Listener()
{
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }

    if(counter>=maxCount)
    {
        counter=0;
    //  ClearDisp();
        this->updateDataFromPult();
        Draw(Tek_Count);
    }
    counter++;

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        return;
    }

    if (getButtonState(pult_Button_Dn) == PRESSED)  Plus();
    if (getButtonState(pult_Button_Up) == PRESSED)  Minus();

    if(Orientation==1) //���� ���������� 2*3 - ����� �������� ������ ������-�����
    {
        //������ ������
        if (getButtonState(pult_Button_Right) == PRESSED) Right();
        //������ �����
        if (getButtonState(pult_Button_Left) == PRESSED) Left();
    }

    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        p_pult->setEstimationBit();
    }
}


SwitchAxsesMenu::SwitchAxsesMenu (char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr):
    LCD_Menu(pNam, Link, Count,  Orient, Menu_Per_Scr)
    {
        updateFromEEPROM();
    }
void SwitchAxsesMenu::updateFromEEPROM()
{
    Tek_Count=EE_Working::Read(EE_JOYSTIC_CONFIGURATION)+1;
    if(Tek_Count>Counter_Cell||Tek_Count<=0)
    {
        Tek_Count=1;
        EE_Working::Write(EE_JOYSTIC_CONFIGURATION,0);
    }
    Table_Cell[Tek_Count-1]->Active_Style.Cell_Color=ClrDarkSlateBlue;
    Table_Cell[Tek_Count-1]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
    p_pult->setJoysticConfiguration((PultJoysticConfigurations)(Tek_Count-1));
}

void SwitchAxsesMenu::Listener()
{
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }
    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        uint32_t preferens=EE_Working::Read(EE_JOYSTIC_CONFIGURATION);
        if (preferens==(Tek_Count-1))
            return;
        else {
            Table_Cell[preferens]->Active_Style.Cell_Color=ClrMediumOrchid;
            Table_Cell[preferens]->UnActive_Style.Cell_Color=ClrMediumOrchid;
            uint8_t count = Tek_Count;
            Table_Cell[preferens]->Hide();
            Tek_Count = count;
            Table_Cell[Tek_Count-1]->Active_Style=Style_MenuSelected;
            Draw(Tek_Count);
            Task_sleep(400);
            Table_Cell[Tek_Count-1]->Active_Style=Style_MenuActive;
            Table_Cell[Tek_Count-1]->Active_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[Tek_Count-1]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Draw(Tek_Count);
            EE_Working::Write(EE_JOYSTIC_CONFIGURATION,Tek_Count-1);
            p_pult->setJoysticConfiguration((PultJoysticConfigurations)(Tek_Count-1));
        }
    }

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        return;
    }

    if (getButtonState(pult_Button_Dn) == PRESSED)  Plus();
    if (getButtonState(pult_Button_Up) == PRESSED)  Minus();

    if(Orientation==1) //���� ���������� 2*3 - ����� �������� ������ ������-�����
    {
        //������ ������
        if (getButtonState(pult_Button_Right) == PRESSED) Right();
        //������ �����
        if (getButtonState(pult_Button_Left) == PRESSED) Left();
    }
}
//----------------------------------------------------------------------------------------------

SwitchMotorTypeMenu::SwitchMotorTypeMenu(char* pNam,tMenu_Link* Link,UInt8 cellCount):
        SwitchMotorAction(pNam,Link,cellCount)
{
    for(UInt8 i=0;i!=10;i++)
    {
        load[i]=0;
    }
    updateFromEEPROM();
}
void SwitchMotorTypeMenu::updateFromEEPROM()
{
    load[0]=EE_Working::Read(EE_LC_MOTOR1_TYPE);
    load[1]=EE_Working::Read(EE_LC_MOTOR2_TYPE);
    load[2]=EE_Working::Read(EE_LC_MOTOR3_TYPE);

    for(UInt8 i=0;i!=3;i++)
    {
        if(load[i]>=2)
        {
            load[0]=0;
            load[1]=0;
            load[2]=0;
            EE_Working::Write(EE_LC_MOTOR1_TYPE,    load[0]);
            EE_Working::Write(EE_LC_MOTOR2_TYPE,    load[1]);
            EE_Working::Write(EE_LC_MOTOR3_TYPE,    load[2]);
            break;
        }
    }
    for(UInt8 i=0;i!=3;i++)
    {
        currentVariable=i;
        pultUpdate();
    }
    currentVariable=0;
}
void SwitchMotorTypeMenu::setValue(UInt8 motorValue)
{
    if(currentVariable>=3){return; }
    load[currentVariable]=motorValue;
}
void SwitchMotorTypeMenu::pultUpdate()
{
//  p_pult->setMotorModel(0,currentVariable);
    p_pult->setMotorType(load[currentVariable],currentVariable);
}
void SwitchMotorTypeMenu::saveInEEPROM()
{
    switch(currentVariable)
    {
        case 0:EE_Working::Write(EE_LC_MOTOR1_TYPE, load[currentVariable]);break;
        case 1:EE_Working::Write(EE_LC_MOTOR2_TYPE, load[currentVariable]);break;
        case 2:EE_Working::Write(EE_LC_MOTOR3_TYPE, load[currentVariable]);break;
    }
}
//----------------------------------------------------------------------------------------------------------


SwitchMotorAction::SwitchMotorAction(char* pNam,tMenu_Link* Link,UInt8 cellCount):
    LCD_Menu()
    {

        Counter_Cell = cellCount;
        Tek_Count = 1;
        Start = 1;
        Orientation = 0;
        Menu_On_Screen = 5;
        pName = pNam;
        Menu_Link = Link;

        currentVariable=0;

        if(Menu_On_Screen>Counter_Cell){Menu_On_Screen=Counter_Cell;}

        for(UInt8 i=0;i<Counter_Cell;i++)
        {
            load[i]=i;
            Table_Cell[i]->SetText((Menu_Link+i)->Name);
        }

        updateFromEEPROM();
    }



void  SwitchMotorAction::updateFromEEPROM()
{
    load[0]=EE_Working::Read(EE_LC_ZOOM_MN);
    load[1]=EE_Working::Read(EE_LC_IRIS_MN);
    load[2]=EE_Working::Read(EE_LC_FOCUS_MN);

    for(UInt8 i=0;i!=3;i++)
    {
        if(load[i]>=3)
        {
            load[0]=0;
            load[1]=1;
            load[2]=2;
            EE_Working::Write(EE_LC_ZOOM_MN,    load[0]);
            EE_Working::Write(EE_LC_IRIS_MN,    load[1]);
            EE_Working::Write(EE_LC_FOCUS_MN,   load[2]);
            break;
        }
    }

    if((load[0]==load[1])||(load[1]==load[2])||(load[0]==load[2]))
    {
        load[0]=0;
        load[1]=1;
        load[2]=2;
        EE_Working::Write(EE_LC_ZOOM_MN,    load[0]);
        EE_Working::Write(EE_LC_IRIS_MN,    load[1]);
        EE_Working::Write(EE_LC_FOCUS_MN,   load[2]);
    }

    pultUpdate();


}

void SwitchMotorAction::pultUpdate()
{

        p_pult->setZoomMotor(load[0]);
        p_pult->setIrisMotor(load[1]);
        p_pult->setFocusMotor(load[2]);


}

void SwitchMotorAction::saveInEEPROM()
{

        EE_Working::Write(EE_LC_ZOOM_MN,    load[0]);
        EE_Working::Write(EE_LC_IRIS_MN,    load[1]);
        EE_Working::Write(EE_LC_FOCUS_MN,   load[2]);

}

void  SwitchMotorAction::Listener()
{
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }
    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        Table_Cell[Tek_Count-1]->Active_Style=Style_MenuSelected;
        Draw(Tek_Count);
        Task_sleep(500);
        Table_Cell[Tek_Count-1]->Active_Style=Style_MenuActive;
        setValue(Tek_Count-1);
        Draw(Tek_Count);

        pultUpdate();
        saveInEEPROM();
    }

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        return;
    }

    if (getButtonState(pult_Button_Dn) == PRESSED)  Plus();
    if (getButtonState(pult_Button_Up) == PRESSED)  Minus();

    if(Orientation==1) //���� ���������� 2*3 - ����� �������� ������ ������-�����
    {
        //������ ������
        if (getButtonState(pult_Button_Right) == PRESSED) Right();
        //������ �����
        if (getButtonState(pult_Button_Left) == PRESSED) Left();
    }
}

void SwitchMotorAction::Draw(byte Active) //������ ������� � ���������
{
    //ClearDisp(); //������� ������ �������� ������
    DrawHeader();
    Tek_Count = Active;
    DrawVert();

}

void SwitchMotorAction::DrawVert() //��������� ������������� ����
{
    byte i;
    Int16 StepY;
    tCell_Style tempStyle;

    p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
    p_Pos_Size_XY.Xsize = 250; p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
    StepY = p_Pos_Size_XY.Ysize + 5;

    if(Counter_Cell<=0){return ;}
    for(i=0;i<Menu_On_Screen;i++)
    {

        if(Start+i==Tek_Count)
        {
            tempStyle=Table_Cell[Start+i-1]->Active_Style;

            if((Start+i-1)==load[currentVariable])
            {
                Table_Cell[Start+i-1]->Active_Style.Cell_Color=ClrLinen;
            }
            Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                            p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                            Table_Cell[i+Start-1]->p_text, Cell_Active);
            Table_Cell[Start+i-1]->Active_Style=tempStyle;

        }
        else
        {
            tempStyle=Table_Cell[Start+i-1]->UnActive_Style;
            if((Start+i-1)==load[currentVariable])
            {
                    Table_Cell[Start+i-1]->UnActive_Style.Cell_Color=ClrLinen;
            }

            Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                            p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                            Table_Cell[i+Start-1]->p_text, Cell_UnActive);
            Table_Cell[Start+i-1]->UnActive_Style=tempStyle;
        }

        if( (Start+i) == Counter_Cell) break;
    }
}

void SwitchMotorAction::setCurrentVariable(UInt8 v)
{
    if(v>=3){currentVariable=0;return;}
    currentVariable=v;
}


void SwitchMotorAction::setValue(UInt8 motorValue)
{
    if(currentVariable>=3){return; }
    UInt8 temp=load[currentVariable];

    for(UInt8 i=0;i!=3;i++)
    {
        if(load[i]==motorValue){load[i]=temp;}
    }
    load[currentVariable]=motorValue;
}

//----------------------------------------------------------------------------------------------------------

LCD_Menu_lcZIF::LCD_Menu_lcZIF(char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr, tMenu_Link* subtext_, UInt8 subtextCount_ ):
        LCD_Menu(pNam, Link, Count, Orient, Menu_Per_Scr)
{
     subtext=subtext_;
     subtextCount=subtextCount_;
}

void LCD_Menu_lcZIF::DrawVert() //��������� ������������� ����
{
    byte i;
    Int16 StepY;

    char str[50];
    UInt8 t=0;

    p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
    p_Pos_Size_XY.Xsize = 250;p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
    StepY = p_Pos_Size_XY.Ysize + 5;

    for(i=0;i<Menu_On_Screen;i++)
    {
        strcpy( str, Menu_Link[Start+i-1].Name);
        strcat( str, " : ");

        switch(Start+i-1)
        {
            case 0:
                t=p_pult->getZoomMotor();
                if(t<subtextCount)
                {
                    strcat( str, subtext[t].Name);
                }
                break;
            case 1:
                t=p_pult->getIrisMotor();
                if(t<subtextCount)
                {
                    strcat( str, subtext[t].Name);
                }
                break;
            case 2:
                t=p_pult->getFocusMotor();
                if(t<subtextCount)
                {
                    strcat( str, subtext[t].Name);
                }
                break;
        }

        str[49]=0;

        if((Start+i)==Tek_Count)
        {
            Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                    p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                    str, Cell_Active);
        }
        else
        {
            Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                    p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                    str, Cell_UnActive);
        }

        if( (Start+i) == Counter_Cell) break;
    }
}


void LCD_Menu_lcZIF::Draw(byte Active) //������ ������� � ���������
{
    DrawHeader();
    Tek_Count = Active;
    DrawVert();
}

//----------------------------------------------------------------------------------------------------------

LCD_Menu_lcMotrors::LCD_Menu_lcMotrors(char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr, tMenu_Link* subtext_, UInt8 subtextCount_ ):
        LCD_Menu_lcZIF(pNam, Link, Count, Orient, Menu_Per_Scr,subtext_, subtextCount_)
{

}

void LCD_Menu_lcMotrors::DrawVert() //��������� ������������� ����
{
    byte i;
    Int16 StepY;

    char str[50];

    p_Pos_Size_XY.X = 30; p_Pos_Size_XY.Y = 35;
    p_Pos_Size_XY.Xsize = 250;  p_Pos_Size_XY.Ysize = (UInt32)((VERTICAL_LEN/Menu_On_Screen))-5;//35
    StepY = p_Pos_Size_XY.Ysize + 5;

    for(i=0;i<Menu_On_Screen;i++)
    {
        strcpy( str, Menu_Link[Start+i-1].Name);
        strcat( str, " : ");
        strcat( str, subtext[p_pult->getMotorType(Start+i-1)].Name);

        str[49]=0;

        if((Start+i)==Tek_Count)
        {
            Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                    p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                    str, Cell_Active);
        }
        else
        {
            Table_Cell[Start+i-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                    p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                    str, Cell_UnActive);
        }

        if( (Start+i) == Counter_Cell) break;
    }
}

//----------------------------------------------------------------------------------------------------------
void loadEepromValueFromPult()
{
    pSet_Koeff->Read_EE_State();
    pBright_Set->readFromEEPROM();
    pEqualizerPan->Read_EE_State();
    pEqualizerDutch->Read_EE_State();
    pEqualizerTilt->Read_EE_State();
    mainScreenPointer->sendSetupValuesByPult();
    secretMenuPointer->readFromEEPROM();
    secretMenuPointer->updateDataFromPult();
    secretMenuViewPointer->updateDataFromPult();
    switchAxesPointer->updateFromEEPROM();
    lensControlMotorTypeSelectPointer->updateFromEEPROM();
    lensControlMotorActionSelectPointer->updateFromEEPROM();
    wheelMenuPointer->readEEPROM();
    setMaxTorqueMenuPointer->updateFromEeprom();
    tiltSpiderSelectMenuPointer->updateFromEEPROM();
    setupOverslangActivatePointer->updateFromEEPROM();
    lensControlCameraStartPointer->updateFromEEPROM();
#ifdef joyPult
    lensControlZoomSensePointer->updateFromEEPROM();
    lensControlZoomDriftPointer->updateFromEeprom();
    setJoyDeadZoneMenuPointer->updateFromEeprom();
    lensControlZoomSensePointer->action();
    lensControlZoomDriftPointer->action();
    setJoyDeadZoneMenuPointer->action();
#endif
    setMotionPrerolPointer->updateFromEeprom();
    folowingModePointer->updateFromEeprom();
    setMaxTorqueMenuPointer->action();
    tiltSpiderSelectMenuPointer->action();
    setupOverslangActivatePointer->action();
    lensControlCameraStartPointer->action();
    shakerRNDMenuPointer->readEEPROM();
}
//----------------------------------------------------------------------------------------------------------

ProfileSelectMenu::ProfileSelectMenu (char* pNam, tMenu_Link* Link, byte Count, byte Orient, byte Menu_Per_Scr):
    LCD_Menu(pNam, Link, Count,  Orient, Menu_Per_Scr)
    {
        Tek_Count=EE_Working::getProfileID()+1;
        if(Tek_Count>Count||Tek_Count<=0){Tek_Count=1;}
        old_position_number=Tek_Count;
        Table_Cell[old_position_number-1]->Active_Style.Cell_Color=ClrDarkSlateBlue;
        Table_Cell[old_position_number-1]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
    }
// TODO qwekqkj
void ProfileSelectMenu::Listener()
{
    if(Focused)
    {
        ClearDisp();
        Draw(Tek_Count);
        Focused = false;
    }
    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        if (this->Tek_Count==RstCurrentProfileProfileMenu) {
            char* text_need;
            UInt32 tempColorProfile;
            //todo  ����� �������� �� ���������
            tempColorProfile=Table_Cell[RstCurrentProfileProfileMenuSistem]->Active_Style.Cell_Color;//���������� �����
            text_need=Table_Cell[RstCurrentProfileProfileMenuSistem]->p_text;// ���������� �������
            Table_Cell[RstCurrentProfileProfileMenuSistem]->SetText((char*)p);// ������ ������� ��������
            Table_Cell[RstCurrentProfileProfileMenuSistem]->Active_Style.Cell_Color=ClrLinen ;// ������ �������� �����
            Draw(Tek_Count);// ������������ �������
            EE_Working::setDefaultProfile(old_position_number-1); //���������� ������� �� ������
            Task_sleep(400);
            ClearDisp();
            Table_Cell[RstCurrentProfileProfileMenuSistem]->SetText((char*)text_need);// ������������� ������� ��������
            Table_Cell[RstCurrentProfileProfileMenuSistem]->Active_Style.Cell_Color=tempColorProfile ;// ������������� ������� �����
            Draw(Tek_Count);//������������ �������
            Draw(old_position_number);//������������ ������� ���������� �������
            Tek_Count=old_position_number; //���������� ������ �������
        }
        else {
            uint8_t count = Tek_Count;
            Table_Cell[old_position_number-1]->Active_Style.Cell_Color=ClrMediumOrchid;
            Table_Cell[old_position_number-1]->UnActive_Style.Cell_Color=ClrMediumOrchid;
            Table_Cell[old_position_number-1]->Hide();
            Tek_Count = count;
            Table_Cell[Tek_Count-1]->Active_Style=Style_MenuSelected;
            Draw(Tek_Count);
            Task_sleep(400);
            Table_Cell[Tek_Count-1]->Active_Style=Style_MenuActive;
            Table_Cell[Tek_Count-1]->Active_Style.Cell_Color=ClrDarkSlateBlue;
            Table_Cell[Tek_Count-1]->UnActive_Style.Cell_Color=ClrDarkSlateBlue;
            Draw(Tek_Count);
            old_position_number=Tek_Count;
        }
        EE_Working::setProfile(Tek_Count-1);
        loadEepromValueFromPult();

    }

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        return;
    }

    if (getButtonState(pult_Button_Dn) == PRESSED)  Plus();
    if (getButtonState(pult_Button_Up) == PRESSED)  Minus();

    if(Orientation==1) //���� ���������� 2*3 - ����� �������� ������ ������-�����
    {
        //������ ������
        if (getButtonState(pult_Button_Right) == PRESSED) Right();
        //������ �����
        if (getButtonState(pult_Button_Left) == PRESSED) Left();
    }
}

LCD_Set_Koeff::LCD_Set_Koeff(char* pNam, byte Count, byte Menu_Per_Scr):
Cell_Header((char*) " ", 1,1,10,10),
Cell_1((char*) "PK", 1,1,10,10),
Cell_4((char*) "PI", 33,1,10,10),
#ifdef USAEdition
Cell_2((char*) "RK", 22,1,10,10),
Cell_3((char*) "TK", 11,1,10,10),
Cell_5((char*) "RI", 55,1,10,10),
Cell_6((char*) "TI", 44,1,10,10),
#else
Cell_2((char*) "DK", 11,1,10,10),
Cell_3((char*) "TK", 22,1,10,10),
Cell_5((char*) "DI", 44,1,10,10),
Cell_6((char*) "TI", 55,1,10,10),
#endif
Cell_7((char*) "", 66,1,10,10),
Cell_8((char*) "", 77,1,10,10),
Cell_9((char*) "", 88,1,10,10),
Cell_10((char*) "", 99,1,10,10)
{
    Counter_Cell = Count;
    Tek_Count = 1;
    Start = 1;
    Menu_On_Screen = Menu_Per_Scr;
    pName = pNam;
    Selected = false;

    Cell_Header.Active_Style = Style_MenuHeader;
    Cell_Header.UnActive_Style = Style_MenuHeader;

    Active_Style = Style_MenuActive;
    UnActive_Style = Style_MenuUnActive;
    Select_Style = Style_MenuSelected;
    Active_Style.pFont=g_psFontCmsc20;
    UnActive_Style.pFont=g_psFontCmsc20;
    Select_Style.pFont=g_psFontCmsc20;

    Table_Cell[1-1] = &Cell_1;
    Table_Cell[2-1] = &Cell_2;
    Table_Cell[3-1] = &Cell_3;
    Table_Cell[4-1] = &Cell_4;
    Table_Cell[5-1] = &Cell_5;
    Table_Cell[6-1] = &Cell_6;
    Table_Cell[7-1] = &Cell_7;
    Table_Cell[8-1] = &Cell_8;
    Table_Cell[9-1] = &Cell_9;
    Table_Cell[10-1] = &Cell_10;

    byte i;

    Read_EE_State(); //��� ����� ��������� �������� �������� �� ������

    for(i=0;i<Counter_Cell;i++) {
        Table_Cell[i]->Active_Style=Active_Style;
        Table_Cell[i]->UnActive_Style=UnActive_Style;
    }//����������� ���� �����
}

void LCD_Set_Koeff::SetText(byte num)  //����� ������ �����, �� ������ ����� ������������ �������
{
    u16_TO_buffer(&buff_Cell[num][5],Table_Cell[num]->Param,3); //���� 4 ��� 1000
    buff_Cell[num][4] = buff_Cell[num][5]; buff_Cell[num][5] = ',';
    memcpy(&buff_Cell[num][0],Table_Cell[num]->pName, 2);
    buff_Cell[num][2] = ':'; buff_Cell[num][3] = ' '; buff_Cell[num][8] = 0;
    Table_Cell[num]->p_text = &buff_Cell[num][0]; // ���� 9 ��� 1000
}

void LCD_Set_Koeff::Read_EE_State()
{
    byte i;

    for(i=0;i<Counter_Cell;i++) {
        Table_Cell[i]->Param = EE_Working::Read(i+EE_PK);//����������� ��� ��������
        if(Table_Cell[i]->Param > 100) {Table_Cell[i]->Param = 0; EE_Working::Write(i+EE_PK,0);} //����������� ��� ������ �����
        sign_Pult(i); //����� ������� ������
    }
}

void LCD_Set_Koeff::Draw(byte Active) //��������� ��������������� ����
{
    byte i, Up;
    Int16 StepX, StepY;

    DrawHeader();

    //ClearDisp(); //������� ������ �������� ������

    Tek_Count = Active;

    p_Pos_Size_XY.X = 5; p_Pos_Size_XY.Y = 45;
    p_Pos_Size_XY.Xsize = 154; p_Pos_Size_XY.Ysize = 60;

    if(Counter_Cell < Menu_On_Screen) Up = Counter_Cell; else Up = Menu_On_Screen;

    for(i=0;i<Up;i++)
        if(i<(Menu_On_Screen/2))
        {
            //����� ��������
            StepY = p_Pos_Size_XY.Ysize + 3;
            SetText(i+Start-1);
            Table_Cell[i+Start-1]->FastDraw(p_Pos_Size_XY.X,p_Pos_Size_XY.Y+StepY*(i),
                                            p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                            Table_Cell[i+Start-1]->p_text, Cell_UnActive);
        }
        else
        {
            //������ ��������
            StepX = p_Pos_Size_XY.Xsize + 2;
            SetText(i+Start-1);
            Table_Cell[i+Start-1]->FastDraw(p_Pos_Size_XY.X+StepX,p_Pos_Size_XY.Y+StepY*(i-(Menu_On_Screen/2)),
                                              p_Pos_Size_XY.Xsize,p_Pos_Size_XY.Ysize,
                                              Table_Cell[i+Start-1]->p_text, Cell_UnActive);
        }

    if(Selected) Table_Cell[Tek_Count-1]->Active_Style = Select_Style; else Table_Cell[Tek_Count-1]->Active_Style = Active_Style;
    SetText(Tek_Count-1);
    Table_Cell[Tek_Count-1]->ReDraw();
}

void LCD_Set_Koeff::DrawSelected(byte Active) //������ ������ �������� ������
{
    Tek_Count = Active;

    if(Selected) Table_Cell[Tek_Count-1]->Active_Style = Select_Style; else Table_Cell[Tek_Count-1]->Active_Style = Active_Style;
    SetText(Tek_Count-1);
    Table_Cell[Tek_Count-1]->ReDraw();
}

void LCD_Set_Koeff::sign_Pult(UInt8 num) //�������� ������ �� ��������� ���������
{
    //return;
    switch (num) {
        case EE_PK: p_pult->setAxisSetting(AXIS_SETTINGS_PK,(float)(Table_Cell[EE_PK]->Param)/100); break;
        case EE_PI: p_pult->setAxisSetting(AXIS_SETTINGS_PI,(float)(Table_Cell[EE_PI]->Param)/100); break;
        case EE_DK: p_pult->setAxisSetting(AXIS_SETTINGS_DK,(float)(Table_Cell[EE_DK]->Param)/100); break;
        case EE_DI: p_pult->setAxisSetting(AXIS_SETTINGS_DI,(float)(Table_Cell[EE_DI]->Param)/100); break;
        case EE_TK: p_pult->setAxisSetting(AXIS_SETTINGS_TK,(float)(Table_Cell[EE_TK]->Param)/100); break;
        case EE_TI: p_pult->setAxisSetting(AXIS_SETTINGS_TI,(float)(Table_Cell[EE_TI]->Param)/100); break;
        default: break;
    }
}


void LCD_Set_Koeff::Plus(UInt16 HowMatch) //���������� ����������
{
    byte prom_Count = Tek_Count;

    if(Selected) //���� ������� ��������� ���������
    {
        if(Table_Cell[Tek_Count-1]->Param < (100-HowMatch)) Table_Cell[Tek_Count-1]->Param += HowMatch;
        SetText(Tek_Count-1);
        sign_Pult(Tek_Count-1);
        DrawSelected(prom_Count);
    }
    else //������ �������� �� ����
    {
        if(prom_Count > 1)
        {
            if(prom_Count == Start) //����� �� �������� ����
                Start--;
            prom_Count--;
        }
        else
        {
            prom_Count = Counter_Cell;
            if(Counter_Cell<=Menu_On_Screen)
                Start = 1;
            else Start = Counter_Cell - (Menu_On_Screen-1);
        }
        Draw(prom_Count);
    }
}

void LCD_Set_Koeff::Minus(UInt16 HowMatch) //���������� �����������
{
    byte prom_Count = Tek_Count;

    if(Selected) //���� ������� ��������� ���������
    {
        if(Table_Cell[Tek_Count-1]->Param > HowMatch) Table_Cell[Tek_Count-1]->Param -= HowMatch;
        SetText(Tek_Count-1);
        sign_Pult(Tek_Count-1);
        DrawSelected(prom_Count);
    }
    else //������ �������� �� ����
    {
        if(prom_Count < Counter_Cell)
        {
            if(prom_Count == Start+(Menu_On_Screen-1)) //����� �� ������� ����
                Start++;
            prom_Count++;
        }
        else {prom_Count = 1; Start = 1;}
        Draw(prom_Count);
    }
}

void LCD_Set_Koeff::Right() //���������� ������ ������
{
    if(Selected) return;

    byte prom_Count = Tek_Count;

    if(prom_Count < 4) //���� ������� ��������� ���������
    {
        prom_Count+=3;
    }
    else //������ �������� �� ����
    {
        if(prom_Count < Counter_Cell)
        {
            if(prom_Count == Start+(Menu_On_Screen-1)) //����� �� ������� ����
                Start++;
            prom_Count++;
        }
        else {prom_Count = 1; Start = 1;}
    }
    Draw(prom_Count);
}

void LCD_Set_Koeff::Left() //���������� ������ �����
{
    if(Selected) return;

    byte prom_Count = Tek_Count;

    if(prom_Count > 3) //���� ������� ��������� ���������
    {
        prom_Count-=3;
    }
    else //������ �������� �� ����
    {
        if(prom_Count > 1)
        {
            if(prom_Count == Start) //����� �� �������� ����
                Start--;
            prom_Count--;
        }
        else
        {
            prom_Count = Counter_Cell;
            if(Counter_Cell<=Menu_On_Screen)
                Start = 1;
            else Start = Counter_Cell - (Menu_On_Screen-1);
        }
    }
    Draw(prom_Count);
}

void LCD_Set_Koeff::Select() //�����
{
    if(Selected) {
        Selected = false;
        EE_Working::Write(Tek_Count-1+EE_PK,Table_Cell[Tek_Count-1]->Param);
        sign_Pult(Tek_Count-1);
    }
    else
    {
        Selected=true;
        PredValue = Table_Cell[Tek_Count-1]->Param;
    }

    Draw(Tek_Count);
}

void LCD_Set_Koeff::Listener()
{
    if(Focused)
    {
        ClearDisp();
        if(Param>0 && Param<Counter_Cell+1) //���� ����� � ���������
        {
            Tek_Count = Param;
            Selected = true;
            PredValue = Table_Cell[Tek_Count-1]->Param;
        }
        else
        {
            Tek_Count = 1;
            Selected = false;
        }
        Draw(Tek_Count);
        Focused = false;
    }

    //������ ������ Select
    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        Select();
    }

    //������ ������ ESC
    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        if(Selected)
        {
            Table_Cell[Tek_Count-1]->Param = PredValue; //�������� ��� ����, ����� ��������
            Select();
        }
        else
        {
            pDispl = pDispl->Parent;
            pDispl->Focused = true;
            return;
        }
    }

    //������ ����
    if (getButtonState(pult_Button_Dn) == PRESSED)
    {
        Minus(1);
        Task_sleep(10);
    }
    if (getButtonState(pult_Button_Dn) == HOLD)
    {
        Minus(1);
        Task_sleep(50);
    }

    //������ �����
    if (getButtonState(pult_Button_Up) == PRESSED)
    {
        Plus(1);
        Task_sleep(10);
    }
    if (getButtonState(pult_Button_Up) == HOLD)
    {
        Plus(1);
        Task_sleep(50);
    }

    //������ ������
    if (getButtonState(pult_Button_Right) == PRESSED)
    {
        Right();
        Task_sleep(10);
    }

    //������ �����
    if (getButtonState(pult_Button_Left) == PRESSED)
    {
        Left();
        Task_sleep(10);
    }
}

void LCD_Set_Koeff::DrawHeader()
{
    Cell_Header.FastDraw(0,0,319,35, pName,Cell_UnActive);
}
//---------------------------------------------------------------------
LCD_Koeff::LCD_Koeff(char* pName, char* ptext,  UInt16 text_length):
Cell_Koeff((char*) "", 1,1,10,10),
Cell_String((char*) "", 1,1,10,10)
{
//  Value_Koeff = EE_Working::Read(EE_Bright);
//  if(Value_Koeff>100) {Value_Koeff=100; EE_Working::Write(EE_Bright,Value_Koeff);}
    readFromEEPROM();
    SetText(ptext);
    //Draw();
}

void LCD_Koeff::readFromEEPROM()
{
    Value_Koeff = EE_Working::Read(EE_Bright);
    if(Value_Koeff>100) {Value_Koeff=1; EE_Working::Write(EE_Bright,Value_Koeff);}
    p_pult->setBrightness((float)Value_Koeff);
}

void LCD_Koeff::Draw() //���������
{
    //ClearDisp(); //�������� ������

    //������� ��������� ������������
    static char buff_String[4];

    u16_TO_buffer(&buff_String[0],Value_Koeff,3);
    if(buff_String[0]=='0') buff_String[0] = ' ';
    buff_String[3] = 0;

    Cell_Koeff.Active_Style.Border=false;
    Cell_Koeff.UnActive_Style.Border=false;
    Cell_String.Active_Style.Border=false;
    Cell_String.UnActive_Style.Border=false;

    Cell_Koeff.UnActive_Style.Border_Color = ClrDarkSlateBlue;
    Cell_Koeff.UnActive_Style.pFont = g_psFontCmsc48;
    Cell_Koeff.Active_Style.pFont = g_psFontCmsc48;
    Cell_Koeff.FastDraw(110,85,90,70, &buff_String[0], Cell_Active);
    //������� ������ �������� ������������
    Cell_String.UnActive_Style.Border_Color = ClrChocolate;//Color_Menu_Fon_Active
    Cell_String.UnActive_Style.pFont = g_psFontCmsc18;
    Cell_String.Active_Style.pFont = g_psFontCmsc18;
    Cell_String.FastDraw(90,5,140,40, p_text, Cell_Active);
}

void LCD_Koeff::SetText(char* ptext)
{
    p_text = ptext;
}

void LCD_Koeff::Listener()
{
    if(Focused)
    {
        ClearDisp();
        Draw();
        Focused = false;
    }

    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        return;
    }

    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        pDispl = pDispl->Parent;
        pDispl->Focused = true;
        return;
    }

    char buff_String[5];
    if (getButtonState(pult_Button_Up) == PRESSED)
    {
        if(Value_Koeff<10) Value_Koeff+=1;
        else
        if(Value_Koeff+10<=100) Value_Koeff+=10;
        u16_TO_buffer(&buff_String[0],Value_Koeff,3);
        if(buff_String[0]=='0') buff_String[0] = ' ';
        buff_String[3] = 0;
        Cell_Koeff.SetText(&buff_String[0]);
        Cell_Koeff.ReDraw();
    //  SetBrightness(Value_Koeff);             � ���� ������ ����������� ���������
        EE_Working::Write(EE_Bright,Value_Koeff);
        p_pult->setBrightness((float)Value_Koeff);
    }
    if (getButtonState(pult_Button_Dn) == PRESSED)
    {
        if(Value_Koeff>=20) Value_Koeff-=10;
        else
        if(Value_Koeff>1) Value_Koeff-=1;
        u16_TO_buffer(&buff_String[0],Value_Koeff,3);
        if(buff_String[0]=='0') buff_String[0] = ' ';
        buff_String[3] = 0;
        Cell_Koeff.SetText(&buff_String[0]);
        Cell_Koeff.ReDraw();
    //  SetBrightness(Value_Koeff);
        EE_Working::Write(EE_Bright,Value_Koeff);
        p_pult->setBrightness((float)Value_Koeff);
    }

    //���� ������������ ��������� ������ ������-�����
    if ((getButtonState(pult_Button_Right) == HOLD)&&(getButtonState(pult_Button_Left) == HOLD))
    {

        pDispl = secretMenuSelectorPointer;
        pDispl->Focused = true;
        pDispl->Parent = this;
        return;
    }
}

Button_Listener::Button_Listener()
{
    UInt8 i;
    for(i=0;i<PULT_BUTTONS_COUNT;i++)
    {
        State[i]=RELESASED;
        Fix[i]=false;
        Counter_Button[i]=0;
    }
}

void Button_Listener::Button_Pressed(PultButtons Butt)
{
    Counter_Button[Butt]++;
    if(Counter_Button[Butt]>10) State[Butt]=PRESSED;
    if(Counter_Button[Butt]>50) State[Butt]=HOLD;
}

void Button_Listener::Button_Released(PultButtons Butt)
{
    State[Butt]=RELESASED;
    Fix[Butt]=false;
    Counter_Button[Butt]=0;
}

void Button_Listener::Button_Analizer()
{
    PultButtons Tek_Button;

    Tek_Button = pult_Button_ESC; //PL1 ESC
    if(!GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_1)) Button_Pressed(Tek_Button); else Button_Released(Tek_Button);

    //PL0 Fn - ���� �� �����������

    Tek_Button = pult_Button_Left; //PL2 Left
    if(!GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_2)) Button_Pressed(Tek_Button); else Button_Released(Tek_Button);

    Tek_Button = pult_Button_Dn; //PL3 Dn
    if(!GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_3)) Button_Pressed(Tek_Button); else Button_Released(Tek_Button);

    Tek_Button = pult_Button_Select; //PL4 Select
    if(!GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_4)) Button_Pressed(Tek_Button); else Button_Released(Tek_Button);

    Tek_Button = pult_Button_Up; //PL5 Up
    if(!GPIOPinRead(GPIO_PORTL_BASE,GPIO_PIN_5)) Button_Pressed(Tek_Button); else Button_Released(Tek_Button);

    Tek_Button = pult_Button_Right; //PP5 Right
    if(!GPIOPinRead(GPIO_PORTP_BASE,GPIO_PIN_5)) Button_Pressed(Tek_Button); else Button_Released(Tek_Button);
}

PultButtonStates Button_Listener::Get_Button(PultButtons Butt)
{
    return State[Butt];
}

LCD_Equalizer::LCD_Equalizer(char* pNam, byte lineCounter, EEPROM_Filds ee_start_adr,PultEqualaserType type_):
Cell_Header((char*) " ", 1,1,10,10),
Cell_X((char*) "X", 1,1,10,10),
Cell_Y((char*) "Y", 11,1,10,10),
type(type_)

{
    pName = pNam;
    Selected = false;
    Line_Counter = lineCounter;
    Tek_Num_Line = 0;
    EE_Start_Adr = ee_start_adr;

    Cell_Header.Active_Style = Style_MenuHeader;
    Cell_Header.UnActive_Style = Style_MenuHeader;
    Cell_Header.Active_Style.pFont = g_psFontCmsc20;

    Active_Style = Style_MenuActive;
    UnActive_Style = Style_MenuUnActive;
    Select_Style = Style_MenuSelected;
    Active_Style.pFont=g_psFontCmsc20;
    UnActive_Style.pFont=g_psFontCmsc20;
    Select_Style.pFont=g_psFontCmsc20;

    Cell_X.Active_Style=Active_Style;
    Cell_X.UnActive_Style=UnActive_Style;

    Cell_Y.Active_Style=Active_Style;
    Cell_Y.UnActive_Style=UnActive_Style;

    LineOff = Color_Fon_Displ;
    LineOn = Style_MenuUnActive.Border_Color;
    LineActive = Style_MenuActive.Border_Color;
    LineSelect = Style_MenuSelected.Border_Color;

    referencePoints=p_pult->getEqualaserPoints(type);
    if(referencePoints==NULL)
    {
        //���������� !!!!
    }
    currentSpeedValue=1;
    currentJoyPozition=0;

    Read_EE_State(); //��� ����� ��������� �������� �������� �� ������
}
void LCD_Equalizer::updateReferencePoints()
{

    if(referencePoints==NULL)
    {
        return;}
    UInt8 refPointSize=referencePoints->getSize();
    if(((UInt8)(refPointSize/2))!=Line_Counter)
    {
        return;}

    for(UInt8 i=0;i<(refPointSize/2);i++)
    {
        if(i>=Line_Counter){break;}
        referencePoints->get(i)->x=((float)(-lines[Line_Counter-i-1].X))/230;
        referencePoints->get(i)->y=((float)(-lines[Line_Counter-i-1].Y))/230;

    }

    for(UInt8 i=0;i<(refPointSize/2);i++)
    {
        if(i>=Line_Counter){break;}
        referencePoints->get(i+Line_Counter)->x=lines[i].X/230;
        referencePoints->get(i+Line_Counter)->y=lines[i].Y/230;

    }
    p_pult->updateAproximator(type);
}
void LCD_Equalizer::Set_X_Y(byte num)  //����� ������ �����, �� ������ ����� ������������ �������
{
    UInt32 tmp=lines[num].Y*currentSpeedValue;
    if(tmp>230){tmp=230;}
    u16_TO_buffer(&buff_Cell[0][0],(Uint16)(lines[num].X*100/230),3);
    buff_Cell[0][3] = '%';
    buff_Cell[0][4] = 0;
    Cell_X.FastDraw(203,0,55,20, &buff_Cell[0][0],Cell_Active);
    u16_TO_buffer(&buff_Cell[1][0],(Uint16)(tmp*100/230),3);
    buff_Cell[1][3] = '%';
    buff_Cell[1][4] = 0;
    Cell_Y.FastDraw(263,0,55,20, &buff_Cell[1][0],Cell_Active);
}

void LCD_Equalizer::Read_EE_State()
{
    byte i;
    UInt32 promValues;
    UInt32 promX,promY;

    for(i=0;i<Line_Counter;i++) {
        lines[i].num = i;
        promValues = EE_Working::Read(i+EE_Start_Adr);
        promX = promValues>>16;
        promY = (UInt32)((UInt16)promValues);
        if( (promX > 230) || (promY > 230) ) //���� ������ ������ ��� ������
        {
            promX = (i+1)*230/Line_Counter;
            promY = (i+1)*230/Line_Counter;
            EE_Working::Write(i+EE_Start_Adr,(Uint32)(promX<<16)+promY);
        }
        lines[i].X = (float)(promX);//X
        lines[i].Y = (float)(promY);//Y
    //  sign_Pult(i); //����� ������� ������
    }
    updateReferencePoints();
}

void LCD_Equalizer::Write_EE_State(tLine Line)
{
    UInt32 promValues;
    UInt32 promX, promY;
    byte numEE;

    promX = ((UInt32)Line.X)<<16;
    promY = (UInt32)Line.Y;

    promValues = promX + promY;
    numEE = Line.num+EE_Start_Adr;
    EE_Working::Write(numEE,promValues);
}

void LCD_Equalizer::DrawLine(tLine Line, UInt32 Color) //��������� (��� ���������) ������������ ����� � ���������
{
    UInt32 tmp=Line.Y*currentSpeedValue;
    if(tmp>230){tmp=230;}
    DpyLineDrawV(&g_sKentec320x240x16_SSD2119,(UInt16)(10+Line.X*300/230),30,230,Color);
    DpyLineDrawV(&g_sKentec320x240x16_SSD2119,(UInt16)(9+Line.X*300/230),30,230,Color);
    DpyLineDrawV(&g_sKentec320x240x16_SSD2119,(UInt16)(11+Line.X*300/230),30,230,Color);
    GrContextForegroundSetTranslated(&sContext, Color);
    GrCircleDraw(&sContext,(UInt16)(10+Line.X*300/230),(UInt16)(230 - tmp*200/230),4);
    GrCircleDraw(&sContext,(UInt16)(10+Line.X*300/230),(UInt16)(230 - tmp*200/230),5);
//  Line.Y
}
void LCD_Equalizer::DrawCurve(UInt32 Color)
{
    UInt32 tmp=0;
    UInt32 tmp2=0;
    for(UInt8 i=0;i!=230;i++)
    {
      curve[i]=230*((float)(p_pult->getEqualaserAproximatorValue(type,((float)i)/230)));
    }

    for(UInt8 i=0;i!=230;i++)
    {
        tmp=curve[i]*currentSpeedValue;
        if(tmp>230){tmp=230;}
        DpyPixelDraw(&g_sKentec320x240x16_SSD2119, (UInt16)(10+i*300/230), (UInt16)(230-200*tmp/230), Color) ;


    }

    tmp=(UInt32)(currentJoyPozition/p_pult->getJoystickMaxValue(type)*230);
    if(tmp>=230){tmp=229;}

    tmp2=curve[tmp]*currentSpeedValue;
    if(tmp2>230){tmp2=230;}

    GrContextForegroundSetTranslated(&sContext, Color);
    GrCircleDraw(&sContext,(UInt16)(10+tmp*300/230),(UInt16)(230 - tmp2*200/230),4);
}
inline void LCD_Equalizer::invalidateCurve()
{
    DrawCurve(LineOff);
    updateReferencePoints();
    DrawCurve(LineSelect);
}
void LCD_Equalizer::updateSpeedValue()
{
    float tmp=p_pult->getJoysticSpeedValue(type);
    float tmp2=p_pult->getJoystickCurrentPozition(type);
    if(tmp2<0.0){tmp2=-tmp2;}

    if
    (
            ((UInt32)(currentSpeedValue*100)!=(UInt32)(tmp*100))||
            ((UInt32)(currentJoyPozition*100)!=(UInt32)(tmp2*100))
    )
    {
        DrawCurve(LineOff);
        for(UInt8 i=0;i!=Line_Counter;i++)
        {
            DrawLine(lines[i],LineOff);
        }

        currentSpeedValue=tmp;
        currentJoyPozition=tmp2;
        DrawCurve(LineSelect);
        for(UInt8 i=0;i!=Line_Counter;i++)
        {
            currentSpeedValue=tmp;
            if(Selected)
            {
                if(i==Tek_Num_Line){    DrawLine(lines[i],LineSelect);  }
                else{                   DrawLine(lines[i],LineOn);      }

            }
            else
            {
                if(i==Tek_Num_Line){    DrawLine(lines[i],LineActive);  }
                else{                   DrawLine(lines[i],LineOn);      }

            }
        }
        Set_X_Y(Tek_Num_Line);
    }
}
void LCD_Equalizer::Draw() //
{
    byte i;

    ClearDisp();
    DrawHeader();

    for(i=0;i<Line_Counter;i++) {
        DrawLine(lines[i],LineOn);
//      Set_X_Y(i);
    }
    if(Tek_Num_Line<Line_Counter)
    {
        DrawLine(lines[Tek_Num_Line],LineActive);
        Set_X_Y(Tek_Num_Line);
    }
    else
    {
        DrawLine(lines[0],LineActive);
        Set_X_Y(0);
    }
    DrawCurve(LineSelect);
}

//void LCD_Equalizer::sign_Pult(UInt8 num) //�������� ������ �� ��������� ���������
//{
//  //return;
//
//  p_pult->setXY(pName, num, lines[num].X, lines[num].Y); //
//}


void LCD_Equalizer::Plus() //���������� ����������
{
    if(Selected) //���� ������� ��������� ���������
    {
        if(lines[Tek_Num_Line].Y < 230)
        {
            DrawLine(lines[Tek_Num_Line],LineOff);
            lines[Tek_Num_Line].Y++;
            DrawLine(lines[Tek_Num_Line],LineSelect);
            Set_X_Y(Tek_Num_Line);

            invalidateCurve();
            //sign_Pult(Tek_Num_Line); //����� ������� ������
        }
    }
}

void LCD_Equalizer::Minus() //���������� �����������
{
    if(Selected) //������� �����
    {
        if(lines[Tek_Num_Line].Y > 1)
        {
            DrawLine(lines[Tek_Num_Line],LineOff);
            lines[Tek_Num_Line].Y--;
            DrawLine(lines[Tek_Num_Line],LineSelect);
            Set_X_Y(Tek_Num_Line);

            invalidateCurve();
            //sign_Pult(Tek_Num_Line); //����� ������� ������
        }
    }
}

void LCD_Equalizer::Right() //���������� ������ ������
{

    if(Selected) //������� �����
    {
        if((Tek_Num_Line<Line_Counter-1) && (lines[Tek_Num_Line].X+6 == lines[Tek_Num_Line+1].X) ) //��� �� ��������� ����� � ��������� � ���������
            return;

        if(lines[Tek_Num_Line].X < 230)
        {
            DrawLine(lines[Tek_Num_Line],LineOff);
            lines[Tek_Num_Line].X++;
            DrawLine(lines[Tek_Num_Line],LineSelect);
            Set_X_Y(Tek_Num_Line);

            invalidateCurve();
            //sign_Pult(Tek_Num_Line); //����� ������� ������
        }
    }
    else //������� ����� �����
    if(Tek_Num_Line < Line_Counter-1)
    {
        DrawLine(lines[Tek_Num_Line],LineOn);
        Tek_Num_Line++;
        DrawLine(lines[Tek_Num_Line],LineActive);
        Set_X_Y(Tek_Num_Line);
    }
}

void LCD_Equalizer::Left() //���������� ������ �����
{

    if(Selected) //������� �����
    {
        if((Tek_Num_Line>0) && (lines[Tek_Num_Line].X == lines[Tek_Num_Line-1].X+6) ) //��� �� ������ ����� � ��������� � ����������
            return;

        if(lines[Tek_Num_Line].X > 1)
        {
            DrawLine(lines[Tek_Num_Line],LineOff);
            lines[Tek_Num_Line].X--;
            DrawLine(lines[Tek_Num_Line],LineSelect);
            Set_X_Y(Tek_Num_Line);

            invalidateCurve();
            //sign_Pult(Tek_Num_Line); //����� ������� ������
        }
    }
    else //������� ����� �����
    if(Tek_Num_Line > 0)
    {
        DrawLine(lines[Tek_Num_Line],LineOn);
        Tek_Num_Line--;
        DrawLine(lines[Tek_Num_Line],LineActive);
        Set_X_Y(Tek_Num_Line);
    }
}

void LCD_Equalizer::Select() //�����
{
    if(Selected) {
        Selected = false;
        Write_EE_State(lines[Tek_Num_Line]);
        invalidateCurve();
        //sign_Pult(Tek_Num_Line);
        DrawLine(lines[Tek_Num_Line],LineActive);
    }
    else
    {
        Selected=true;
        PredLine = lines[Tek_Num_Line]; //�������� ��� ���� ����� �������� �� �������
        DrawLine(lines[Tek_Num_Line],LineSelect);
    }
}

void LCD_Equalizer::Listener()
{
    if(Focused)
    {
        Draw();
        Focused = false;
    }

    updateSpeedValue();
    //������ ������ Select
    if (getButtonState(pult_Button_Select) == PRESSED)
    {
        Select();
    }

    //������ ������ ESC
    if (getButtonState(pult_Button_ESC) == PRESSED)
    {
        if(Selected)
        {
            DrawLine(lines[Tek_Num_Line],LineOff); //������ �����
            lines[Tek_Num_Line] = PredLine; //�������� ��� ����, ����� ��������
            Select();
        }
        else
        {
            pDispl = pDispl->Parent;
            pDispl->Focused = true;
            return;
        }
    }

    //������ ����
    if (getButtonState(pult_Button_Dn) == PRESSED)
    {   Minus();    Task_sleep(10);  }
    if (getButtonState(pult_Button_Dn) == HOLD)
    {   Minus();    Task_sleep(50);  }

    //������ �����
    if (getButtonState(pult_Button_Up) == PRESSED)
    {   Plus(); Task_sleep(10);  }
    if (getButtonState(pult_Button_Up) == HOLD)
    {   Plus(); Task_sleep(50); }

    //������ ������
    if (getButtonState(pult_Button_Right) == PRESSED)
    {   Right(); }
    if (getButtonState(pult_Button_Right) == HOLD)
    {   Right(); Task_sleep(50);  }

    //������ �����
    if (getButtonState(pult_Button_Left) == PRESSED)
    {   Left();  }
    if (getButtonState(pult_Button_Left) == HOLD)
    {   Left(); Task_sleep(50);  }
}

void LCD_Equalizer::DrawHeader()
{
    Cell_Header.FastDraw(0,0,200,25, pName,Cell_Active);
}

void TikerCell::updateFromPult()
{
    p_pult->updateWarningsList();
    List<Warning*>* warnings=p_pult->getWarnings()->getRunStrWarnings();
    stringCount=warnings->getSize();
    if(stringCount>4){stringCount=4;}

    for(UInt8 i=0;i!=stringCount;i++)   {   strings[i]=(Int8*)(warnings->get(i)->message);  }
}

TikerCell::TikerCell(char* pNam, UInt32 X,UInt32 Y,UInt32 Xsize,UInt32 Ysize):
LCD_Cell(pNam,  X, Y,Xsize, Ysize)
{
    tickerDeviderMaximum=12;
    tickerDeviderCounter=0;
    currentString=0;

    updateFromPult();
    stringPointerIterator=0;
};

void TikerCell::renderSplitString(UInt8 ind)
{

    UInt i=0;
    UInt8 a=currentString;
    UInt8 j=0;
    Uint8 spaceCounter=0;
    tempLine[25]=0x00;
    for(;;)
    {
        if(i>=25){tempLine[25]=0x00;return;}
        if(spaceCounter!=0)
        {
            tempLine[i]=' ';
            spaceCounter--;
            i++;
            continue;
        }
        if(strlen((const char*)(strings[a]+ind+j))<=0)
        {
            a=nextString(a);
            ind=0;
            j=0;
            spaceCounter=3;
            continue;
        }
        tempLine[i]=((char*)(strings[a]+ind))[j];
        i++;
        j++;
    }

}
UInt8 TikerCell::nextString(UInt8 cur)
{
    if(cur>=(stringCount-1)){return 0;}
    return cur+1;
}
void TikerCell::tick()
{
    UInt8 mainStrLen;

    if(stringCount==0)
    {
        updateFromPult();
        Hide();
        return;
    }

    if(tickerDeviderCounter>=tickerDeviderMaximum)
    {
        tickerDeviderCounter=0;
        mainStrLen=strlen((const char*)(strings[currentString]+stringPointerIterator));

        if(mainStrLen<=0)
        {
            currentString=nextString(currentString);
            stringPointerIterator=0;
            tickerDeviderCounter=tickerDeviderMaximum;
            updateFromPult();
            return;
        }

        if(mainStrLen<23)
        {
            renderSplitString(stringPointerIterator);
            this->SetText((char*)(tempLine));
            this->ReDraw();
            stringPointerIterator++;
            return;
        }

        if(mainStrLen<255)
        {
            this->SetText((char*)(strings[currentString]+stringPointerIterator));
            this->ReDraw();
            stringPointerIterator++;
            return;

        }
    }
    tickerDeviderCounter++;
}
//============================================================================================================================
//============================================================================================================================



void ClearDisp()
{
    //������� ������ �������� ������
    tRectangle pRect;

    pRect.i16XMin = 0;
    pRect.i16YMin = 0;
    pRect.i16XMax = 319;
    pRect.i16YMax = 239;
    DpyRectFill(&g_sKentec320x240x16_SSD2119,&pRect,Color_Fon_Displ);
}



// �������������� ASCII � u8
uint8_t symbol_TO_u8(uint8_t symbol)
{
    return symbol - 0x30;
}
//

// �������������� u8 � ASCII
uint8_t u8_TO_symbol(uint8_t digit)
{
    return digit + 0x30;
}
//

#define u8 uint8_t
#define u16 uint16_t

//������ �� u16 (65535) ������ "65535"
void    u16_TO_buffer(char* buffer, u16 digits, u8 base)
{
    u16 tmp16;
    u8 index_base, index=0;

    index_base=base;
    tmp16 = digits;

    if(index_base==5)
    {
        tmp16 = digits/10000;
        buffer[index++] = u8_TO_symbol((char) tmp16);
        digits -= tmp16*10000;
        index_base--;
    }
    if(index_base==4)
    {
        tmp16 = digits/1000;
        buffer[index++] = u8_TO_symbol((char) tmp16);
        digits -= tmp16*1000;
        index_base--;
    }
    if(index_base==3)
    {
        tmp16 = digits/100;
        buffer[index++] = u8_TO_symbol((char) tmp16);
        digits -= tmp16*100;
        index_base--;
    }
    if(index_base==2)
    {
        tmp16 = digits/10;
        buffer[index++] = u8_TO_symbol((char) tmp16);
        tmp16 = digits - tmp16*10;
        index_base--;
    }
    buffer[index++] = u8_TO_symbol((char) tmp16);
}
//


void LCD_PinInit()
{
    // PF6-7/PJ6/PS4-5/PR0-7 are used for the LCD.
    ROM_GPIOPinConfigure(GPIO_PF7_LCDDATA02);
    ROM_GPIOPinConfigure(GPIO_PJ6_LCDAC);
    ROM_GPIOPinConfigure(GPIO_PR0_LCDCP);
    ROM_GPIOPinConfigure(GPIO_PR1_LCDFP);
    ROM_GPIOPinConfigure(GPIO_PR2_LCDLP);
    ROM_GPIOPinConfigure(GPIO_PR3_LCDDATA03);
    ROM_GPIOPinConfigure(GPIO_PR4_LCDDATA00);
    ROM_GPIOPinConfigure(GPIO_PR5_LCDDATA01);
    ROM_GPIOPinConfigure(GPIO_PR6_LCDDATA04);
    ROM_GPIOPinConfigure(GPIO_PR7_LCDDATA05);
    ROM_GPIOPinConfigure(GPIO_PS4_LCDDATA06);
    ROM_GPIOPinConfigure(GPIO_PS5_LCDDATA07);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_6);
    ROM_GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_6, GPIO_PIN_6);
    ROM_GPIOPinTypeLCD(GPIO_PORTF_BASE, GPIO_PIN_7);
    ROM_GPIOPinTypeLCD(GPIO_PORTJ_BASE, GPIO_PIN_6);
    ROM_GPIOPinTypeLCD(GPIO_PORTR_BASE,
                       (GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 |
                        GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7));
    ROM_GPIOPinTypeLCD(GPIO_PORTS_BASE, GPIO_PIN_4 | GPIO_PIN_5);

    // Initialize the display driver.
    Kentec320x240x16_SSD2119Init(120000000);

    // Initialize the graphics context.
    GrContextInit(&sContext, &g_sKentec320x240x16_SSD2119);
    GrContextFontSet(&sContext, g_psFontCmsc24); //g_psFontCm40

    Init_Styles();
}


void CellPaint(UInt32 num_str, char* text, UInt32 text_length, uint32_t color){
tRectangle pRect;
const UInt32 Ystart = 20, Ylength = 30;
UInt32 Xstart;

    Xstart = 10;
    //������� ������ �������� ������
    pRect.i16XMin = Xstart;
    pRect.i16XMax = 310;
    pRect.i16YMin = Ystart + num_str*Ylength + 2;
    pRect.i16YMax = Ystart + (num_str+1)*Ylength;
    DpyRectFill(&g_sKentec320x240x16_SSD2119,&pRect,color); //ClrAliceBlue


    // ������ �������� ����� -2 �������
    pRect.i16XMin = pRect.i16XMin + 2;
    pRect.i16XMax = pRect.i16XMax - 2;
    pRect.i16YMin = pRect.i16YMin + 2;
    pRect.i16YMax = pRect.i16YMax - 2;
    DpyLineDrawH(&g_sKentec320x240x16_SSD2119,pRect.i16XMin,pRect.i16XMax,pRect.i16YMin,ClrWhite);
    DpyLineDrawH(&g_sKentec320x240x16_SSD2119,pRect.i16XMin,pRect.i16XMax,pRect.i16YMax,ClrWhite);
    DpyLineDrawV(&g_sKentec320x240x16_SSD2119,pRect.i16XMin,pRect.i16YMin,pRect.i16YMax,ClrWhite);
    DpyLineDrawV(&g_sKentec320x240x16_SSD2119,pRect.i16XMax,pRect.i16YMin,pRect.i16YMax,ClrWhite);

    //������ ��� ������� �����
    GrStringDraw(&sContext, text, text_length,
            Xstart+5, pRect.i16YMin + 2, 0);

}

void CellPaint_XY(UInt32 num_str, UInt32 num_col, char* text,   UInt32 text_length, uint32_t color){
tRectangle pRect;
const UInt32 Ystart = 20, Ylength = 30;
UInt32 Xstart;

    Xstart = 10*(1-num_col) + (310/2+1)*num_col;
    //������� ������ �������� ������
    pRect.i16XMin = Xstart;
    pRect.i16XMax = 310/2  + (310/2)*num_col;
    pRect.i16YMin = Ystart + num_str*Ylength + 2;
    pRect.i16YMax = Ystart + (num_str+1)*Ylength;
    DpyRectFill(&g_sKentec320x240x16_SSD2119,&pRect,color); //ClrAliceBlue


    // ������ �������� ����� -2 �������
    pRect.i16XMin = pRect.i16XMin + 2;
    pRect.i16XMax = pRect.i16XMax - 2;
    pRect.i16YMin = pRect.i16YMin + 2;
    pRect.i16YMax = pRect.i16YMax - 2;
    DpyLineDrawH(&g_sKentec320x240x16_SSD2119,pRect.i16XMin,pRect.i16XMax,pRect.i16YMin,ClrWhite);
    DpyLineDrawH(&g_sKentec320x240x16_SSD2119,pRect.i16XMin,pRect.i16XMax,pRect.i16YMax,ClrWhite);
    DpyLineDrawV(&g_sKentec320x240x16_SSD2119,pRect.i16XMin,pRect.i16YMin,pRect.i16YMax,ClrWhite);
    DpyLineDrawV(&g_sKentec320x240x16_SSD2119,pRect.i16XMax,pRect.i16YMin,pRect.i16YMax,ClrWhite);

    //������ ��� ������� �����
    GrStringDraw(&sContext, text, text_length,
            Xstart+5, pRect.i16YMin + 2, 0);

}

void debug_button_init()
{
    //����� ������ �����
    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_1);  //PL1
    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_1, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_0);  //PL0
    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_0, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_2);  //PL2
    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_2, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_3);  //PL3
    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_3, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_4);  //PL4
    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_4, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTL_BASE, GPIO_PIN_5);  //PL5
    GPIOPadConfigSet(GPIO_PORTL_BASE, GPIO_PIN_5, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    GPIOPinTypeGPIOInput(GPIO_PORTP_BASE, GPIO_PIN_5);  //PP5
    GPIOPadConfigSet(GPIO_PORTP_BASE, GPIO_PIN_5, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
}

void pultIndikatorInit()
{
    //������������� ������� � �������� �������
    LCD_PinInit();
    //������������� EEPROM
    SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
    EEPROMInit();

}

void Init_Styles()
{
    Style_MenuActive.Cell_Color = Color_Menu_Fon_Active; //���� ����
    Style_MenuActive.Border = true; //���� ��� ��� �����
    Style_MenuActive.Border_Color = Color_Menu_Text_Active; //���� �����
    Style_MenuActive.Font_Color = Color_Menu_Text_Active; //���� ������
    Style_MenuActive.pFont = g_psFontCmsc18; //������ ������
    Style_MenuActive.Text_Centered = true;

    Style_MenuUnActive.Cell_Color = Color_Menu_Fon_InActive; //���� ����
    Style_MenuUnActive.Border = true; //���� ��� ��� �����
    Style_MenuUnActive.Border_Color = Color_Menu_Text_InActive; //���� �����
    Style_MenuUnActive.Font_Color = Color_Menu_Text_InActive; //���� ������
    Style_MenuUnActive.pFont = g_psFontCmsc18; //������ ������
    Style_MenuUnActive.Text_Centered = true;

    Style_MenuSelected = Style_MenuActive; // ��� ��������, �� ����� � ����� ������� �����
    Style_MenuSelected.Border_Color = ClrYellow;
    Style_MenuSelected.Font_Color = ClrYellow;

    Style_MenuHeader = Style_MenuUnActive;  // ��� ����������, �� ��� ������� ����� � ������ �����
    Style_MenuHeader.Cell_Color = Color_Menu_Header;
    Style_MenuHeader.pFont = g_psFontCmsc20;

    Style_Error = Style_MenuHeader; // ��� ��������, �� � �������
    Style_Error.Border_Color = ClrLinen;
    Style_Error.Font_Color = ClrLinen;
    /*ClrCoral //������� � ����� - ��������
    ClrDarkSalmon //��� ��� �����������
    ClrDarkOrchid //����� �����������
    ClrDarkSlateBlue //������� ��� ������
    ClrLime //������ ��� ������
    ClrLinen //������� ��� ������
    ClrChocolate //������ - ����� ��� ����
    ClrDarkBlue // �����-����� ��� ����
    ClrDarkSalmon // ������-����� ��� ����
    ClrDarkTurquoise //������� ��� ������ ����������� ����
    */
}

tColor FullColorTable[140] =
        {
        {"ClrAliceBlue",            0x00F0F8FF},
        {"ClrAntiqueWhite",         0x00FAEBD7},
        {"ClrAqua",                 0x0000FFFF},
        {"ClrAquamarine",           0x007FFFD4},
        {"ClrAzure",                0x00F0FFFF},
        {"ClrBeige",                0x00F5F5DC},
        {"ClrBisque",               0x00FFE4C4},
        {"ClrBlack",                0x00000000},
        {"ClrBlanchedAlmond",       0x00FFEBCD},
        {"ClrBlue",                 0x000000FF},
        {"ClrBlueViolet",           0x008A2BE2},
        {"ClrBrown",                0x00A52A2A},
        {"ClrBurlyWood",            0x00DEB887},
        {"ClrCadetBlue",            0x005F9EA0},
        {"ClrChartreuse",           0x007FFF00},
        {"ClrChocolate",            0x00D2691E},
        {"ClrCoral",                0x00FF7F50},
        {"ClrCornflowerBlue",       0x006495ED},
        {"ClrCornsilk",             0x00FFF8DC},
        {"ClrCrimson",              0x00DC143C},
        {"ClrCyan",                 0x0000FFFF},
        {"ClrDarkBlue",             0x0000008B},
        {"ClrDarkCyan",             0x00008B8B},
        {"ClrDarkGoldenrod",        0x00B8860B},
        {"ClrDarkGray",             0x00A9A9A9},
        {"ClrDarkGreen",            0x00006400},
        {"ClrDarkKhaki",            0x00BDB76B},
        {"ClrDarkMagenta",          0x008B008B},
        {"ClrDarkOliveGreen",       0x00556B2F},
        {"ClrDarkOrange",           0x00FF8C00},
        {"ClrDarkOrchid",           0x009932CC},
        {"ClrDarkRed",              0x008B0000},
        {"ClrDarkSalmon",           0x00E9967A},
        {"ClrDarkSeaGreen",         0x008FBC8F},
        {"ClrDarkSlateBlue",        0x00483D8B},
        {"ClrDarkSlateGray",        0x002F4F4F},
        {"ClrDarkTurquoise",        0x0000CED1},
        {"ClrDarkViolet",           0x009400D3},
        {"ClrDeepPink",             0x00FF1493},
        {"ClrDeepSkyBlue",          0x0000BFFF},
        {"ClrDimGray",              0x00696969},
        {"ClrDodgerBlue",           0x001E90FF},
        {"ClrFireBrick",            0x00B22222},
        {"ClrFloralWhite",          0x00FFFAF0},
        {"ClrForestGreen",          0x00228B22},
        {"ClrFuchsia",              0x00FF00FF},
        {"ClrGainsboro",            0x00DCDCDC},
        {"ClrGhostWhite",           0x00F8F8FF},
        {"ClrGold",                 0x00FFD700},
        {"ClrGoldenrod",            0x00DAA520},
        {"ClrGray",                 0x00808080},
        {"ClrGreen",                0x00008000},
        {"ClrGreenYellow",          0x00ADFF2F},
        {"ClrHoneydew",             0x00F0FFF0},
        {"ClrHotPink",              0x00FF69B4},
        {"ClrIndianRed",            0x00CD5C5C},
        {"ClrIndigo",               0x004B0082},
        {"ClrIvory",                0x00FFFFF0},
        {"ClrKhaki",                0x00F0E68C},
        {"ClrLavender",             0x00E6E6FA},
        {"ClrLavenderBlush",        0x00FFF0F5},
        {"ClrLawnGreen",            0x007CFC00},
        {"ClrLemonChiffon",         0x00FFFACD},
        {"ClrLightBlue",            0x00ADD8E6},
        {"ClrLightCoral",           0x00F08080},
        {"ClrLightCyan",            0x00E0FFFF},
        {"ClrLightGoldenrodYe", 0x00FAFAD2},
        {"ClrLightGreen",           0x0090EE90},
        {"ClrLightGrey",            0x00D3D3D3},
        {"ClrLightPink",            0x00FFB6C1},
        {"ClrLightSalmon",          0x00FFA07A},
        {"ClrLightSeaGreen",        0x0020B2AA},
        {"ClrLightSkyBlue",         0x0087CEFA},
        {"ClrLightSlateGray",       0x00778899},
        {"ClrLightSteelBlue",       0x00B0C4DE},
        {"ClrLightYellow",          0x00FFFFE0},
        {"ClrLime",                 0x0000FF00},
        {"ClrLimeGreen",            0x0032CD32},
        {"ClrLinen",                0x00FAF0E6},
        {"ClrMagenta",              0x00FF00FF},
        {"ClrMaroon",               0x00800000},
        {"ClrMediumAquamarine",     0x0066CDAA},
        {"ClrMediumBlue",           0x000000CD},
        {"ClrMediumOrchid",         0x00BA55D3},
        {"ClrMediumPurple",         0x009370DB},
        {"ClrMediumSeaGreen",       0x003CB371},
        {"ClrMediumSlateBlue",      0x007B68EE},
        {"ClrMediumSpringGree",    0x0000FA9A},
        {"ClrMediumTurquoise",      0x0048D1CC},
        {"ClrMediumVioletRed",      0x00C71585},
        {"ClrMidnightBlue",         0x00191970},
        {"ClrMintCream",            0x00F5FFFA},
        {"ClrMistyRose",            0x00FFE4E1},
        {"ClrMoccasin",             0x00FFE4B5},
        {"ClrNavajoWhite",          0x00FFDEAD},
        {"ClrNavy",                 0x00000080},
        {"ClrOldLace",              0x00FDF5E6},
        {"ClrOlive",                0x00808000},
        {"ClrOliveDrab",            0x006B8E23},
        {"ClrOrange",               0x00FFA500},
        {"ClrOrangeRed",            0x00FF4500},
        {"ClrOrchid",               0x00DA70D6},
        {"ClrPaleGoldenrod",        0x00EEE8AA},
        {"ClrPaleGreen",            0x0098FB98},
        {"ClrPaleTurquoise",        0x00AFEEEE},
        {"ClrPaleVioletRed",        0x00DB7093},
        {"ClrPapayaWhip",           0x00FFEFD5},
        {"ClrPeachPuff",            0x00FFDAB9},
        {"ClrPeru",                 0x00CD853F},
        {"ClrPink",                 0x00FFC0CB},
        {"ClrPlum",                 0x00DDA0DD},
        {"ClrPowderBlue",           0x00B0E0E6},
        {"ClrPurple",               0x00800080},
        {"ClrRed",                  0x00FF0000},
        {"ClrRosyBrown",            0x00BC8F8F},
        {"ClrRoyalBlue",            0x004169E1},
        {"ClrSaddleBrown",          0x008B4513},
        {"ClrSalmon",               0x00FA8072},
        {"ClrSandyBrown",           0x00F4A460},
        {"ClrSeaGreen",             0x002E8B57},
        {"ClrSeashell",             0x00FFF5EE},
        {"ClrSienna",               0x00A0522D},
        {"ClrSilver",               0x00C0C0C0},
        {"ClrSkyBlue",              0x0087CEEB},
        {"ClrSlateBlue",            0x006A5ACD},
        {"ClrSlateGray",            0x00708090},
        {"ClrSnow",                 0x00FFFAFA},
        {"ClrSpringGreen",          0x0000FF7F},
        {"ClrSteelBlue",            0x004682B4},
        {"ClrTan",                  0x00D2B48C},
        {"ClrTeal",                 0x00008080},
        {"ClrThistle",              0x00D8BFD8},
        {"ClrTomato",               0x00FF6347},
        {"ClrTurquoise",            0x0040E0D0},
        {"ClrViolet",               0x00EE82EE},
        {"ClrWheat",                0x00F5DEB3},
        {"ClrWhite",                0x00FFFFFF},
        {"ClrWhiteSmoke",           0x00F5F5F5},
        {"ClrYellow",               0x00FFFF00},
        {"ClrYellowGreen",          0x009ACD32},
        };

