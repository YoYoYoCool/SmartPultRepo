/*
 * Pult.cpp
 *
 *  Created on: 26 июля 2015 г.
 *      Author: Cno
 */
//testComment попробуем сделать версию 1.01

#include <Pult/Pult.h>
#include <Board.h>
#include <ti/drivers/GPIO.h>
#include <../GyConCommon/ExchangeBasic/BasicProtocol.h>
#include "MenuModels/EqualaserRefPoints.hpp"
#include "MenuModels/WeelCorrectionRefPoints.hpp"
#include "../WatchDogTimer/WatchDog.hpp"
#include <stdio.h>
#include <../GyConCommon/ControlSystem/TransferFunctions.h>

#include <xdc/cfg/global.h>
#include "../PultGlobalDefinitions.h"
#include "../ExtrenalDeviceSynchro/RS232Syncro.hpp"
#include "../ExtrenalDeviceSynchro/PanBar/CartoniPanBar.hpp"


#define MAX_TRANSFER_TIMEOUT 150
#define MAX_TRANSFER_TIMEOUT_ALTERNATIV_TASK 100
//#define PULT_DEVELOPING_BOARD


static volatile UInt32 transferDelayMaximum = 20;
static volatile UInt32 transferDelayCurrent = 0;

extern Pult pult;

#define PULT_EXCHANGE_ADDR 0x00
WatchDog watchDogTimer;
//TODO убрать это безобразие при рефакторинге
MotionControl motionControlAPI(&pult);
FileSystemState filesSystemAPI;
//-------------------------------------------------------
Warnings warnings;
ViewLists viewLists;
CheckPointList checkPoints;
// lens control
//MotorTypes lensControlMotorTapes;
//Motors lensControlMotors;

EqualaserRefPoints equRefPoints;
LineralAproximator panLineralAproximator    (equRefPoints.panReferencePointList,    -1.0,-1.0,1.0);
LineralAproximator dutchLineralAproximator  (equRefPoints.dutchReferencePointList,  -1.0,-1.0,1.0);
LineralAproximator tiltLineralAproximator   (equRefPoints.tiltReferencePointList,   -1.0,-1.0,1.0);
LineralAproximator zoomLineralAproximator   (equRefPoints.zoomReferencePointList,   -1.0,-1.0,1.0);
//-------------------------------------------------------

//   Управление светодиодами   ---------------------------------
SimpleSpiExchangeDriver spiExchange;
LedController ledControl(&spiExchange);

//---------------------------------------------------------------
//Gv/ACC PN7 Addr0

static volatile float panSpeedRef, dutchSpeedRef, tiltSpeedRef, focus, zoom;
static volatile float panSpeed = 111.5, dutchSpeed = 217.77, tiltSpeed = 187.15;
static volatile float PK = 0.0, PI=0.0, DK=0.0, DI=0.0, TK=0.0, TI=0.0, zoomControl=0.0, focusControl=0.0, tDif = 0.0;

static volatile float panWriteAngleH    = 0;
static volatile float dutchWriteAngleH  = 0;
static volatile float tiltWriteAngleH   = 0;
static volatile float zoomWriteAngleH   = 0;
static volatile float panWriteAngleL    = 0;
static volatile float dutchWriteAngleL  = 0;
static volatile float tiltWriteAngleL   = 0;
static volatile float zoomWriteAngleL   = 0;

static volatile float speedLevelCorrectionExchange = 0;

static volatile float panReadAngleH     = 0;
static volatile float dutchReadAngleH   = 0;
static volatile float tiltReadAngleH    = 0;
static volatile float zoomReadAngleH    = 0;
static volatile float panReadAngleL     = 0;
static volatile float dutchReadAngleL   = 0;
static volatile float tiltReadAngleL    = 0;
static volatile float zoomReadAngleL    = 0;

static volatile float debugValue0   =1;
static volatile float debugValue1   =2;
static volatile float debugValue2   =3;
static volatile float debugValue3   =4;
static volatile float debugValue4   =5;
static volatile float debugValue5   =6;
static volatile float debugValue6   =7;
static volatile float debugValue7   =8;



#define JOYSTICK_FUNCS_COUNT 4
static CubitorFunc panCubitorFunc   (-1.0, -1.0, 1.0);
static CubitorFunc dutchCubitorFunc (-1.0, -1.0, 1.0);
static CubitorFunc tiltCubitorFunc  (-1.0, -1.0, 1.0);
static CubitorFunc zoomCubitorFunc  (-1.0, -1.0, 1.0);


static QuadratorFunc panQuadratorFunc   (-1.0, -1.0, 1.0);
static QuadratorFunc dutchQuadratorFunc (-1.0, -1.0, 1.0);
static QuadratorFunc tiltQuadratorFunc  (-1.0, -1.0, 1.0);
static QuadratorFunc zoomQuadratorFunc  (-1.0, -1.0, 1.0);

static LinearFunc panLinearFunc     (-1.0, -1.0, 1.0);
static LinearFunc dutchLinearFunc   (-1.0, -1.0, 1.0);
static LinearFunc tiltLinearFunc    (-1.0, -1.0, 1.0);
static LinearFunc zoomLinearFunc    (-1.0, -1.0, 1.0);

static volatile float panOffset             =1860;
static volatile float dutchOffset           =1860;
static volatile float tiltOffset            =1860;
static volatile float zoomOffset            =1860;
static volatile float panExtrenal1Offset    =1860;
static volatile float dutchExtrenal1Offset  =1860;
static volatile float tiltExtrenal1Offset   =1860;

static const float panScaler        =0.13;
static const float dutchScaler      =0.13;
static const float tiltScaler       =-0.13;
static const float zoomScaler       =-(1.0/1810);

static const char* joystickPresetNames[JOYSTICK_FUNCS_COUNT] = {"Cube", "Sqr", "Lin", "Eql"};
static const char* zoomJoystickPresetNames[JOYSTICK_FUNCS_COUNT] = {"Lin", "Cube", "Sqr", "Eql"};
static JoyStickFunction* panFuncs[JOYSTICK_FUNCS_COUNT] =   {&panCubitorFunc,   &panQuadratorFunc,      &panLinearFunc,     &panLineralAproximator};
static JoyStickFunction* dutchFuncs[JOYSTICK_FUNCS_COUNT] = {&dutchCubitorFunc, &dutchQuadratorFunc,    &dutchLinearFunc,   &dutchLineralAproximator};
static JoyStickFunction* tiltFuncs[JOYSTICK_FUNCS_COUNT] =  {&tiltCubitorFunc,  &tiltQuadratorFunc,     &tiltLinearFunc,    &tiltLineralAproximator};
static JoyStickFunction* zoomFuncs[JOYSTICK_FUNCS_COUNT] =  {&zoomCubitorFunc,  &zoomQuadratorFunc,     &zoomLinearFunc,    &zoomLineralAproximator};


//Резисторы и джойстики
#define PAN_JOY_DRIFT_K     0.000035
#define DUTCH_JOY_DRIFT_K   0.00007
#define TILT_JOY_DRIFT_K    0.00007

static Resistor     panJoySpeedResistor         (0.00049,0),
                    panJoyFluidResistor         (0.0005,0),
                    panJoyDriftResistor         (PAN_JOY_DRIFT_K,1861),
                    dutchJoySpeedResistor       (0.00049,0),
                    dutchJoyFluidResistor       (0.0005,0),
                    dutchJoyDriftResistor       (DUTCH_JOY_DRIFT_K,1861),
                    tiltJoySpeedResistor        (0.00049,0),
                    tiltJoyFluidResistor        (0.0005,0),
                    tiltJoyDriftResistor        (TILT_JOY_DRIFT_K,1861),
                    IrisResistor           (1/1865.0,1860.0),
                    zoomSpeedResistor           (0.00049,0),
                    zoomFluidResistor           (0.0005,0),
                    speedLevelCorrectResistor   (1.0/4095, 0),
                    focusResistor               (-(1/1870.0),1858.0),
                    panWheelResistor,
                    dutchWheelResistor,
                    dutchPedalResistor,
                    tiltWheelResistor;
static DummyResistor zoomDeadDriftResistor;


static Resistor* resistors[15] = {&panJoySpeedResistor, &panJoyFluidResistor, &panJoyDriftResistor, &dutchJoySpeedResistor, &dutchJoyFluidResistor, &dutchJoyDriftResistor,
		                          &tiltJoySpeedResistor, &tiltJoyFluidResistor, &tiltJoyDriftResistor, &IrisResistor, &zoomSpeedResistor, &speedLevelCorrectResistor,
								  &focusResistor,
								  &dutchWheelResistor,
								  &dutchPedalResistor};
// виртуальные кнопки
static PultButton virtualButtonJoysticOff;

#define virtualButton 1

PultButton* virtualButtons[virtualButton] = {&virtualButtonJoysticOff

};

//Кнопки 1 - 16

static PultButton lensCalibrationButton,zoomReversButton,irisReversButton,focusReversButton,tb4;

#define BUTTONS_1_16_COUNT 16
static PultButton panReversButton(0), panSenseButton(1),
	   dutchReversButton(2), dutchSenseButton(3),
	   tiltReversButton(4), tiltSenseButton(5),
	   gvCalibrationButton(6), levelCorrectButton(7),
	   fastLevelCorrectButton(8), dutchLevelSetupButton(9), motorOnOffButton(10),
	   joyStickOnOffButton(11);

static PultButton* buttons_1_16[BUTTONS_1_16_COUNT] =
{
		&panReversButton, &panSenseButton,
	   &dutchReversButton, &dutchSenseButton,
	   &tiltReversButton, &tiltSenseButton,
	   &gvCalibrationButton, &levelCorrectButton,
	   &fastLevelCorrectButton, &dutchLevelSetupButton, &motorOnOffButton,
	   &joyStickOnOffButton,
	   &lensCalibrationButton,&zoomReversButton,&irisReversButton,&focusReversButton
};

//Кнопки 17 - 32
#define BUTTONS_17_32_COUNT 16
static PultButton gvAccButton(12), motionPlayButton(13), motionStopButton(14), motionDeleteButton(15), motionReversPlayButton(16), motionTrackSel1(17), motionTrackSel4(18), motionTrackSel2(19), motionTrackSel5(20), motionTrackSel3(21), motionTrackSel6(22), sa11(23), sa12(24), sa13(25), sa14(26), sa15(27);

static PultButton* buttons_17_32[BUTTONS_17_32_COUNT] = {&gvAccButton, &motionPlayButton, &motionStopButton, &motionDeleteButton, &motionReversPlayButton, &motionTrackSel1, &motionTrackSel4, &motionTrackSel2, &motionTrackSel5, &motionTrackSel3, &motionTrackSel6, &sa11, &sa12, &sa13, &sa14, &sa15};

//Кнопки 33-
#define BUTTONS_33_48_COUNT 16
static PultButton cameraStartButton(28), sa17(29), in35, in36, in37, in38, in39, in40, in41, in42, in43, in44, inp45, inp46, inp47;
static PultButton* buttons_33_48[BUTTONS_33_48_COUNT] = {&cameraStartButton, &sa17, &in35, &in36, &in37, &in38, &in39, &in40, &in41, &in42, &in43, &in44, &inp45, &inp46, &inp47,&tb4};


static PultButton* sharedButtons[PULT_BUTTONS_COUNT] = {
		&motionPlayButton, &motionStopButton, &motionDeleteButton, &motionReversPlayButton, &motionTrackSel1, &motionTrackSel4, &motionTrackSel2, &motionTrackSel5, &motionTrackSel3, &motionTrackSel6, &sa11, &sa12, &sa13, &sa14, &sa15, &cameraStartButton, &sa17, &in35//&inp47
};

static PultButton* motionButtons[MOTION_BUTTON_COUNT] = {
		&motionPlayButton, &motionStopButton, &motionDeleteButton, &motionReversPlayButton, &motionTrackSel1, &motionTrackSel2, &motionTrackSel3, &motionTrackSel4, &motionTrackSel5, &motionTrackSel6
};
//-------------------------------------------------------------------------------------
//-------------- NEW JOYSTIC ----------------------------------------------------------
										//CHANELS DEFINITION
static VirtualMenuResistor panChanelDeadResistor;
static VirtualMenuResistor dutchChanelDeadResistor;
static VirtualMenuResistor tiltChanelDeadResistor;
static VirtualMenuResistor dummySpecRes(1.0);

JoyChanel panJoyChannel     (panScaler,     panOffset,      &panJoySpeedResistor ,  15,0);
JoyChanel dutchJoyChannel   (dutchScaler,   dutchOffset,    &dutchJoySpeedResistor, 15,0 );
JoyChanel tiltJoyChannel    (tiltScaler,    tiltOffset,     &tiltJoySpeedResistor,  15,0 );
JoyChanel zoomJoyChannel    (zoomScaler,    zoomOffset,     &zoomSpeedResistor,     15,0 );

JoyChanel panExtern1Channel     (0.4, 	panExtrenal1Offset,		&panChanelDeadResistor,     5,  0.015 );
JoyChanel dutchExtern1Channel   (0.4,	dutchExtrenal1Offset,	&dutchChanelDeadResistor,   5,  0.015 );
JoyChanel tiltExtern1Channel    (0.4,	tiltExtrenal1Offset,	&tiltChanelDeadResistor,    5,  0.015 );

ExtrenalDevices::CatoniPanBarChannel cartoniPanAxisChannel
(
        ExtrSyncroization::ExtrenalDevieExchDriver::dataConverter,
        ExtrenalDevices::CH_PAN,
        1.0, &panJoySpeedResistor,
       // &dummySpecRes,
        0.02,0.015,230.0
);
ExtrenalDevices::CatoniPanBarChannel cartoniTiltAxisChannel
(
        ExtrSyncroization::ExtrenalDevieExchDriver::dataConverter,
        ExtrenalDevices::CH_TILT,
        1.0, &tiltJoySpeedResistor,
        //&dummySpecRes,
        0.02,0.015,230.0
);
ExtrenalDevices::CatoniPanBarChannel cartoniDutchAxisChannel
(
        ExtrSyncroization::ExtrenalDevieExchDriver::dataConverter,
        ExtrenalDevices::CH_DUTCH,
        1.0, &dutchJoySpeedResistor,
        //&dummySpecRes,
        0.02,0.015,230.0
);
ExtrenalDevices::CatoniPanBarChannel cartoniZoomAxisChannel
(
        ExtrSyncroization::ExtrenalDevieExchDriver::dataConverter,
        ExtrenalDevices::CH_ZOOM,
        0.0000275/*0.0000305*//**/, &zoomSpeedResistor,
        15,
        0.015,
        1.0
);
ExtrenalDevices::CatoniPanBarResistor cartoniFocusAxisChannel
(
        ExtrSyncroization::ExtrenalDevieExchDriver::dataConverter,
        ExtrenalDevices::CH_FOCUS,
        0.0000307,
        1.0
);
ExtrenalDevices::CatoniPanBarResistor cartoniIrisAxisChannel
(
        ExtrSyncroization::ExtrenalDevieExchDriver::dataConverter,
        ExtrenalDevices::CH_IRIS,
        0.0000307,
        1.0
);

//--------------------------------------------------------------------

HallEffectJoyChannel dutchExtern2Channel(0.14,2110,&dutchJoySpeedResistor,100, 250,0.015);


JoyChannelIF* panChannelsArray[3]=      {&panJoyChannel,    &panExtern1Channel, &cartoniPanAxisChannel};
JoyChannelIF* dutchChannelsArray[4]=    {&dutchJoyChannel,  &dutchExtern2Channel,   &dutchExtern1Channel, &cartoniDutchAxisChannel};
JoyChannelIF* tiltChannelsArray[3]=     {&tiltJoyChannel,   &tiltExtern1Channel, &cartoniTiltAxisChannel};
JoyChannelIF* zoomChannelsArray[2]=     {&zoomJoyChannel, &cartoniZoomAxisChannel};

JoyChannels panChannals     (panChannelsArray,3);
JoyChannels dutchChannals   (dutchChannelsArray,4);
JoyChannels tiltChannals    (tiltChannelsArray,3);
JoyChannels zoomChannals    (zoomChannelsArray,2);



									//joystics

MultiJoystic panJoy     (230, &panCubitorFunc,      &panJoyDriftResistor,   &panJoyFluidResistor,   &(panChannals.channels));
MultiJoystic dutchJoy   (230, &dutchCubitorFunc,    &dutchJoyDriftResistor, &dutchJoyFluidResistor, &(dutchChannals.channels));
MultiJoystic tiltJoy    (230, &tiltCubitorFunc,     &tiltJoyDriftResistor,  &tiltJoyFluidResistor,  &(tiltChannals.channels));
MultiJoystic zoomJoy    (1.0, &zoomLinearFunc,      &zoomDeadDriftResistor, &zoomFluidResistor,     &(zoomChannals.channels));



//-------------------------------------------------------------------------------------
//-----------------------------------------------------
//--------------- ZIF structers  ---------------------
typedef union ZifControl1
{
	UInt16 all;
	struct
	{
		volatile UInt16 motor1Type:5;
		volatile UInt16 motor2Type:5;
		volatile UInt16 motor3Type:5;
	}bit;
}ZifControl1;

typedef union ZifControl2
{
	UInt16 all;
	struct
	{
		volatile UInt16 zoomMotorNO:2;
		volatile UInt16 irisMotorNO:2;
		volatile UInt16 focusMotorNO:2;
		volatile UInt16 controlBits:10;
	}bit;
}ZifControl2;

typedef union MotorControlBits
{
	UInt16 all;
	struct
	{
		volatile UInt16 cameraStart:1;
		volatile UInt16 tuningStart:1;
		volatile UInt16 cameraStartType:1;
	}bit;
}MotorControlBits;

struct ZifParams
{
	volatile float zoomRef;
	volatile float irisRef;
	volatile float focusRef;
	volatile float control1;
	volatile float control2;
};

static ZifControl1 control1;
static ZifControl2 control2;
static MotorControlBits motorControlBits;
static ZifParams zifParams;
//----------- Axces structers ------------------------
struct AxisReference
{
	volatile float panRef;
	volatile float dutchRef;
	volatile float tiltRef;
};
static AxisReference axisRef={0.0,0.0,0.0};

volatile float outputPoleCntrl       =0.0;
volatile float inputPoleCntrl        =0.0;
//-----------------------------------------------------

//Биты управления
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
		volatile Uint16 ecoMode:1;
	} bit;
} PultControlBits;

typedef union PultControlBitsA
{
    volatile  UInt16 all;
    struct
    {
        volatile FolowingModes  panSuspentionFollowingMode  : 2;
        volatile UInt16         panFollowingSectorSize      : 5;
    } bit;
} PultControlBitsA;


typedef union GyConStateBitsLCD {
   UInt16 all;
   struct {
	   UInt16 panDusFault:1;
	   UInt16 dutchDusFault:1;
	   UInt16 tiltDusFault:1;
	   UInt16 gvFault:1;
	   UInt16 encodersFault:1;
	   UInt16 pultFault:1;
	   UInt16 connectedSmartHead:1;
	   UInt16 bit7:1;
	   UInt16 bit8:1;
	   UInt16 bit9:1;
	   UInt16 bit10:1;
	   UInt16 bit11:1;
	   UInt16 stateBits:4;
	} faultBits;

   struct {
	   UInt16 faultBits:12;
	   UInt16 goToAngleComplete:1;
	   UInt16 bit13:1;
	   UInt16 bit14:1;
	   UInt16 bit15:1;
   } stateBits;
} GyConStateBits;

union TourqueValues
{
	volatile UInt16 all;
	struct
	{
		volatile UInt16 pan:5;
		volatile UInt16 dutch:5;
		volatile UInt16 tilt:5;
		volatile UInt16 bit15:1;
	}axis;
};

static TourqueValues maxTourqueValues =         {0x7FFF};
static volatile float maxTourqueValuesExchange;
static volatile float tiltSuspentionResonantFreq    =2.5;
static volatile float panSuspentionResonantFreq     =2.5;
static volatile float zoomDriftValue                =0.0;
static volatile float masterPrerolValue             =0.0;
static volatile float slave1PrerolValue             =0.0;
static volatile float slave2PrerolValue             =0.0;
static volatile float slave3PrerolValue             =0.0;

static volatile GyConStateBitsLCD gyConFaultBits       = {0};
static volatile float gyConFaultBitsExchange;
static volatile float headSupplyVoltage=0;
static SignalsReader signalsReader;
static ButtonsReader buttonsReader;
static volatile PultControlBitsLCD controlBits = {0};
static volatile PultControlBitsA controlBitsA = {0};
static volatile float controlBitsExchange = 0;
static volatile float controlBitsExchangeA = 0;
static Uint32 okCounter = 0;
static Uint32 faultCounter = 0;

static void inversLogic();
static void joySticksOnOffLogic();
static void controlLogic();
static void pultController();

#define NO_BASIC_WRITE_CMD_PERIOD 10
static Int8 noBasicWriteCmdCounter = NO_BASIC_WRITE_CMD_PERIOD;
static volatile bool transmitAxisSettingsCommand = false;
static volatile bool stopTransmitAxisSettingsCommand = false;
static volatile float panTurns = 0;

static volatile float panDriftFactor        = 0;
static volatile float dutchDriftFactor      = 0;
static volatile float tiltDriftFactor       = 0;

static volatile bool setUpTiltLimitsFlag    = 0;
static volatile bool setDwTiltLimitsFlag    = 0;
static volatile bool resetUpTiltLimitsFlag  = 0;
static volatile bool resetDwTiltLimitsFlag  = 0;
static volatile bool estimationFlag         = 0;

float getAngle(float angleH, float angleL) {return angleH + angleL;}

float getAngleH(float angle) {   return (float)((Int32)(angle));}

float getAngleL(float angle)
{
	Int32 angleH = getAngleH(angle);
	float angleL = angle - angleH;
	return angleL;
}


Pult::Pult(Semaphore_Handle* s,Semaphore_Handle* sA):
		exchangeSem(s),
		exchangeAltSem(sA)
{
	connectedFlag = false;
	joysticsConfig=COUNT_JOY_CONFIG;
	control2.bit.controlBits=0x0000;
	debugMenuCmdFlag=false;
	autonegComplete=false;
	transferDelay=0;
	lensCalibrateFlag=false;
}

static UInt16 muxPosRef = 9;
//=====================================================================================================
//сея чушь есть пробный объектив;
//#define LensOtlancka
#ifdef LensOtlancka
#include "LensParam/LensDb.hpp"
#include "LensParam/LensData.hpp"
#include "LensParam/MX66L51235FDriver.hpp"
#include "LensParam/dummyDriver.hpp"
#include "Libs/Containers/List.hpp"
#include "Libs/Containers/String.hpp"
#define DummyDriverSet

#ifdef DummyDriverSet
    LensDb::DummyDriverSettings setting = {0x1234,0x5678,0x9abc};
    LensDb::DummyDriver driver(setting);

#else
    LensDb::MX66L51235FDriverSettings setting = {0x1234,0x5678,0x9abc};
    LensDb::MX66L51235FDriver driver(setting);
#endif
    char* nameDummy = "myName";
        Containers::StringStatic<64> stringNameDummy(&nameDummy[0]);
        LensDb::LensPoint zoomPoint[3]={zoomPoint[0].position=10.1,
                                        zoomPoint[0].percent=0,
                                        zoomPoint[1].position=20.5,
                                        zoomPoint[1].percent=50.4,
                                        zoomPoint[2].position=40.5,
                                        zoomPoint[2].percent=100.0};
        LensDb::LensPoint irisPoint[4]={irisPoint[0].position=10.2,
                                        irisPoint[0].percent=0,
                                        irisPoint[1].position=20.6,
                                        irisPoint[1].percent=30.7,
                                        irisPoint[2].position=50.9,
                                        irisPoint[2].percent=80.8,
                                        irisPoint[3].position=90.8,
                                        irisPoint[3].percent=100.0};
        LensDb::LensPoint focusPoint[5]={focusPoint[0].position=10.0,
                                         focusPoint[0].percent=0.0,
                                         focusPoint[1].position=20.4,
                                         focusPoint[1].percent=30.3,
                                         focusPoint[2].position=40.8,
                                         focusPoint[2].percent=45.7,
                                         focusPoint[3].position=50.9,
                                         focusPoint[3].percent=80.7,
                                         focusPoint[4].position=90.8,
                                         focusPoint[4].percent=100.0};

        Containers::List<LensDb::LensPoint> dummyZoomPointList(&zoomPoint[0],3,64);
        Containers::List<LensDb::LensPoint> dummyIrisPointList(&irisPoint[0],4,64);
        Containers::List<LensDb::LensPoint> dummyFocusPointList(&focusPoint[0],5,64);
        LensDb::LensAxis _zoom(dummyZoomPointList);
        LensDb::LensAxis _iris(dummyIrisPointList);
        LensDb::LensAxis _focus(dummyFocusPointList);
        LensDb::LensObjective dummyObjective(stringNameDummy,_zoom,_iris,_focus);

        LensDb::LensDb<64,300> lansBaseManager(driver);
#endif
//=====================================================================================================
//============================== DRIVER API  ==========================================================
#pragma CODE_SECTION(".secure")
void Pult::driverTask()
{
    watchDogTimer.registerKey(WD_KEY2);
#ifdef LensOtlancka
    lansBaseManager.store(1,dummyObjective);
    char* nameDummyNew = "";
    Containers::StringStatic<64> stringNameDummyNew(&nameDummyNew[0]);
    dummyObjective.setName(stringNameDummyNew);
    bool lensValid = lansBaseManager.load(1,dummyObjective);
    LensDb::LensAxis dummyAxis1 (dummyObjective.zoom());

    LensDb::LensPoint dymmyPoint1[3];
    for (Uint32 id=0; id<dummyAxis1.getSize(); id++) {
        dymmyPoint1[id]=dummyAxis1.point(id);
        }
    LensDb::LensAxis dummyAxis2 (dummyObjective.iris());
    LensDb::LensPoint dymmyPoint2[4];
    for (Uint32 id=0; id<dummyAxis2.getSize(); id++) {
        dymmyPoint2[id]=dummyAxis2.point(id);
        }
    LensDb::LensAxis dummyAxis3 (dummyObjective.focus());
    LensDb::LensPoint dymmyPoint3[5];
    for (Uint32 id=0; id<dummyAxis3.getSize(); id++) {
        dymmyPoint3[id]=dummyAxis3.point(id);
        }
    uint32_t a=0;
    a=10;
    a-=5;
#endif
	watchDogTimer.registerKey(WD_KEY2);
#define weelDigital

#ifdef weelDigital
	filesSystemAPI.initFS();
	motionControlAPI.init();
	Var elementPan("Pan Speed:",&cartoniPanAxisChannel.getAxisVal());
	Var elementTilt("Tilt Speed:",&cartoniTiltAxisChannel.getAxisVal());
	Var elementDutch("Dutch Speed:",&cartoniDutchAxisChannel.getAxisVal());
	viewLists.setVarList(0, &elementPan);
	viewLists.setVarList(1, &elementTilt);
	viewLists.setVarList(2, &elementDutch);
#endif
	while(true) {
		watchDogTimer.useKey(WD_KEY2);
//Обработка аналоговых сигналов
		UInt32* result = signalsReader.read();
		switch(joysticsConfig)
		    {
			case JOYSTIC_REVERS_JOY_CONFIG:
				panJoyChannel.setRef(result[SIGNAL_DUTCH]);
				dutchJoyChannel.setRef(result[SIGNAL_PAN]);
				tiltJoyChannel.setRef(result[SIGNAL_ZOOM]);
				zoomJoyChannel.setRef(result[SIGNAL_TILT]);
				break;

			case DEFAULT_JOY_CONFIG:
			default:
				panJoyChannel.setRef(result[SIGNAL_PAN]);
				dutchJoyChannel.setRef(result[SIGNAL_DUTCH]);
				tiltJoyChannel.setRef(result[SIGNAL_TILT]);
				zoomJoyChannel.setRef(result[SIGNAL_ZOOM]);
				break;
		    }

		panExtern1Channel.setRef(result[SIGNAL_PAN_WHEEL]);
		tiltExtern1Channel.setRef(result[SIGNAL_TILT_WHEEL]);
		cartoniPanAxisChannel.setData();
		cartoniTiltAxisChannel.setData();
		cartoniDutchAxisChannel.setData();
		cartoniZoomAxisChannel.setData();
        cartoniFocusAxisChannel.setData();
        cartoniIrisAxisChannel.setData();
        //запихиваем суда объектив для отлаки

		UInt8 muxPos = signalsReader.getMultiplexer();
		if(muxPos==14){	dutchExtern2Channel.setRef(result[SIGNAL_SLOW]);}
		if(muxPos==13){	dutchExtern1Channel.setRef(result[SIGNAL_SLOW]);}

		panJoy.calculate();
		dutchJoy.calculate();
		tiltJoy.calculate();
		zoomJoy.calculate();
		cartoniFocusAxisChannel.calculate(0.0);
		cartoniIrisAxisChannel.calculate(0.0);

	    resistors[muxPos]->calculate(result[SIGNAL_SLOW]);

		muxPos++;
		if (muxPos > 14) muxPos = 0;
		if (muxPos == muxPosRef) {
			muxPos = muxPosRef;
		}
		signalsReader.setMultiplexer(muxPos);
//Обработка кнопок
#ifdef PULT_DEVELOPING_BOARD
	sharedButtons[pult_Button_Up]->update(GPIO_read(EK_TM4C1294XL_SIGNAL_MUX0));
	sharedButtons[pult_Button_Left]->update(GPIO_read(EK_TM4C1294XL_SIGNAL_MUX1));
	sharedButtons[pult_Button_Select]->update(GPIO_read(EK_TM4C1294XL_BUTTON_1_16_MUX0));
	sharedButtons[pult_Button_Right]->update(GPIO_read(EK_TM4C1294XL_SIGNAL_MUX2));
	sharedButtons[pult_Button_Dn]->update(GPIO_read(EK_TM4C1294XL_BUTTON_1_16_MUX1));
	sharedButtons[ pult_Button_ESC]->update(GPIO_read(EK_TM4C1294XL_SIGNAL_MUX3));

	Task_sleep(1);

#else
		bool buttonState;
		muxPos = buttonsReader.getMultiplexer();

		buttonState = buttonsReader.read(Board_BUTTON_1_16_READ_PIN);
		if (muxPos<BUTTONS_1_16_COUNT) buttons_1_16[muxPos]->update(buttonState);

		buttonState = buttonsReader.read(Board_BUTTON_17_32_READ_PIN);
		if (muxPos<BUTTONS_17_32_COUNT) buttons_17_32[muxPos]->update(buttonState);

		buttonState = buttonsReader.read(Board_BUTTON_33_48_READ_PIN);
		if (muxPos<BUTTONS_33_48_COUNT) buttons_33_48[muxPos]->update(buttonState);
		muxPos++;
		if (muxPos > 15) muxPos = 0;
		buttonsReader.setMultiplexer(muxPos);
		Task_sleep(1);
#endif
	}
}

//=====================================================================================================
//============================== COMMUINCATION API  ===================================================

// Command definition
//=================================================

#define PULT_COMMANDS_COUNT 6

//Инициализация данных
#define WRITE_CONTROL_CMD_DATA_COUNT 5
BasicCmdValue writeControlCmdData[] = {
		{&axisRef.panRef, IQ6},
		{&axisRef.dutchRef, IQ6},
		{&axisRef.tiltRef, IQ6},
		{&speedLevelCorrectionExchange, IQ15},
		{&controlBitsExchange, IQ0},
		{&PK , IQ15},//5
		{&PI , IQ15},
		{&DK , IQ15},
		{&DI , IQ15},
		{&TK , IQ15},
		{&TI , IQ15},
		{&zifParams.zoomRef, IQ15},
		{&zifParams.focusRef, IQ15},
		{&zifParams.irisRef, IQ15},
		{&zifParams.control1, IQ0},
		{&zifParams.control2, IQ0},
		{&tiltSuspentionResonantFreq, IQ10},
		{&maxTourqueValuesExchange, IQ0},
		{&controlBitsExchangeA, IQ0},
//		{&panSuspentionResonantFreq, IQ10},
		NULL
};


BasicCmdValue writeAnglesCmdData[] = {
		{&panWriteAngleH   , IQ0},
		{&dutchWriteAngleH , IQ0},
		{&tiltWriteAngleH  , IQ0},
		{&panWriteAngleL, IQ15},
		{&dutchWriteAngleL, IQ15},
		{&tiltWriteAngleL , IQ15},
		{&PI , IQ15},
		{&DK , IQ15},
		{&DI , IQ15},
		{&TK , IQ15},
		{&TI , IQ15},
		{&zifParams.zoomRef, IQ15},
		{&zifParams.focusRef, IQ15},
		{&zifParams.irisRef, IQ15},
		{&zifParams.control1, IQ0},
		{&zifParams.control2, IQ0},
		{&zoomWriteAngleH, IQ0},
		{&zoomWriteAngleL, IQ15},
		NULL
};

BasicCmdValue askAnglesCmdData[] = {
		{&panReadAngleH, IQ0},
		{&dutchReadAngleH, IQ0},
		{&tiltReadAngleH, IQ0},
		{&panReadAngleL, IQ15},
		{&dutchReadAngleL, IQ15},
		{&tiltReadAngleL, IQ15},
		NULL
};

BasicCmdValue askAnglesCmdDataZIF[] = {
		{&zoomReadAngleH, IQ0},
		{&zoomReadAngleL, IQ15},
		NULL
};


#define ASK_BASIC_CMD_DATA_COUNT 3
static BasicCmdValue askBasicCmdData[] = {
		{&panTurns,                 IQ12},
		{&gyConFaultBitsExchange,   IQ6},
		{&headSupplyVoltage,        IQ7},
		{&dutchDriftFactor,         IQ15},
		{&tiltDriftFactor,          IQ15},
		{&panDriftFactor,           IQ15},
		NULL
	};

static BasicCmdValue writeDebugVarsCmdData[] = {
		{&debugValue0,IQ10},
		{&debugValue1,IQ10},
		{&debugValue2,IQ10},
		{&debugValue3,IQ10},
		{&debugValue4,IQ10},
		{&debugValue5,IQ10},
		{&debugValue6,IQ10},
		{&debugValue7,IQ10},
		NULL
	};

static BasicCmd commands[] = {
		{writeControlCmdData, WRITE_CONTROL_CMD_DATA_COUNT},
		{askBasicCmdData, ASK_BASIC_CMD_DATA_COUNT},
		{writeDebugVarsCmdData, AXIS_SETTINGS_COUNT},
		{writeAnglesCmdData, 0},
		{askAnglesCmdData, 0},
		{askAnglesCmdDataZIF, 0}

	};
static BasicCmdList cmdList = {commands, PULT_COMMANDS_COUNT};
//=================================================
// ALT command definition
//=================================================

   #define PULT_COMMANDS_COUNT_ALT             2
   #define WRITE_CONTROL_CMD_DATA_COUNT_ALT    1
   #define READ_CONTROL_CMD_DATA_COUNT_ALT     1



   BasicCmdValue writeControlCmdDataAlt[] =
   {
        {&outputPoleCntrl, IQ15},
        NULL
   };

   BasicCmdValue askControlCmdDataAlt[] =
   {
        {&inputPoleCntrl  , IQ15},
        NULL
   };

   BasicCmd commandsAlt[] =
   {
        {writeControlCmdDataAlt,   WRITE_CONTROL_CMD_DATA_COUNT_ALT},
        {askControlCmdDataAlt,     READ_CONTROL_CMD_DATA_COUNT_ALT},
   };

   static BasicCmdList cmdListAlt = {commandsAlt, PULT_COMMANDS_COUNT_ALT};

//============================== FAST CLOCK  ===================================================

static volatile UInt32 counter=0;
static volatile UInt32 minuteCounter=0;
static volatile UInt32 timeToStart=0;

extern void ExtrenalDevieExchDriver_fastClockInt();
extern "C"
{
    void fastClockInterrupt()
    {
        ExtrenalDevieExchDriver_fastClockInt();
//-------- HOUR METER ------------------
        minuteCounter++;
        if(minuteCounter>=60000)
        {
            minuteCounter=0;
            timeToStart++;
            if(timeToStart>=0xFFFFFFF0){timeToStart=0xFFFFFFF0;}
        }
//--------------------------------------
        counter++;
        if(counter>100){counter=100;}

        if(transferDelayMaximum>MAX_TRANSFER_TIMEOUT||transferDelayMaximum<20){transferDelayMaximum=20;}

        if(transferDelayCurrent>=transferDelayMaximum)
        {
            transferDelayCurrent=1;
            Semaphore_post(exchangeTaskSemafore);
            Semaphore_post(exchangeAltSem);
            return;
        }
        transferDelayCurrent++;

    }
}

//============================== EXCHANGE  ===================================================

inline void dataRenderLogic()
{
	PultControlBitsLCD contBitsTemp;
	PultControlBitsLCD contBitsTempMotion;

	contBitsTemp.all=controlBits.all;
	controlBitsExchange = controlBits.all;
	controlBitsExchangeA = controlBitsA.all;

	maxTourqueValuesExchange=maxTourqueValues.all;
	speedLevelCorrectionExchange=speedLevelCorrectResistor.value;

	axisRef.panRef=panJoy.getValue();
	axisRef.dutchRef=dutchJoy.getValue();
	axisRef.tiltRef=tiltJoy.getValue();

	control2.bit.controlBits= motorControlBits.all;


    cartoniFocusAxisChannel.setData();
    cartoniIrisAxisChannel.setData();

	if(cartoniFocusAxisChannel.isActive())  {	    zifParams.focusRef=cartoniFocusAxisChannel.value;	}
	else                                    {       zifParams.focusRef=focusResistor.value;             }
	if(cartoniIrisAxisChannel.isActive())   {	    zifParams.irisRef=cartoniIrisAxisChannel.value;     }
	else                                    {       zifParams.irisRef=IrisResistor.value;       	}

	zifParams.zoomRef=zoomJoy.getValue();

	zifParams.control1=control1.all;
	zifParams.control2=control2.all;


	outputPoleCntrl=inputPoleCntrl;

	 if(motionControlAPI.isRec())
	 {
		 motionControlAPI.getCurrentPoint()->panRef=panJoy.getRawValue();
		 motionControlAPI.getCurrentPoint()->dutchRef=dutchJoy.getRawValue();
		 motionControlAPI.getCurrentPoint()->tiltRef=tiltJoy.getRawValue();
		 motionControlAPI.getCurrentPoint()->zoomRef=zoomJoy.getRawValue();

		 motionControlAPI.getCurrentPoint()->irisRef=zifParams.irisRef;
		 motionControlAPI.getCurrentPoint()->focusRef=zifParams.focusRef;

		 motionControlAPI.getCurrentPoint()->levelCorrectionSpeed=speedLevelCorrectionExchange;
		 motionControlAPI.getCurrentPoint()->controlBits=controlBits.all;

		 motionControlAPI.getCurrentPoint()->poleRef=inputPoleCntrl;

		 motionControlAPI.strobe();

	 }

	 if(motionControlAPI.isPlay())
	 {
		 motionControlAPI.strobe();

	     switch(motionControlAPI.getMixMode())
	     {
	     	 case MOTION_MIX_PDTZ:
	     		 axisRef.panRef=	panJoy.getValue(	motionControlAPI.getCurrentPoint()->panRef+panJoy.getRawValue());
	     		 axisRef.dutchRef=	dutchJoy.getValue(	motionControlAPI.getCurrentPoint()->dutchRef+dutchJoy.getRawValue());
	     		 axisRef.tiltRef=	tiltJoy.getValue(	motionControlAPI.getCurrentPoint()->tiltRef+tiltJoy.getRawValue());
	     		 zifParams.zoomRef=	zoomJoy.getValue(	motionControlAPI.getCurrentPoint()->zoomRef+zoomJoy.getRawValue());

	     		 zifParams.irisRef=motionControlAPI.getCurrentPoint()->irisRef;;
   				 zifParams.focusRef=motionControlAPI.getCurrentPoint()->focusRef;

   				 speedLevelCorrectionExchange=motionControlAPI.getCurrentPoint()->levelCorrectionSpeed;
   				 contBitsTempMotion.all=(motionControlAPI.getCurrentPoint()->controlBits&0x0000FFFF);
   				 contBitsTemp.bit.fastLevelCorrect=contBitsTempMotion.bit.fastLevelCorrect;
   				 contBitsTemp.bit.levelCorrect=contBitsTempMotion.bit.levelCorrect;
   				 contBitsTemp.bit.levelSetup=contBitsTempMotion.bit.levelSetup;
   				 controlBitsExchange=contBitsTemp.all;

   				 outputPoleCntrl=motionControlAPI.getCurrentPoint()->poleRef+inputPoleCntrl;
	     		 break;
	     	 case MOTION_MIX_ALL:
	     		 axisRef.panRef=	panJoy.getValue(	motionControlAPI.getCurrentPoint()->panRef+panJoy.getRawValue());
	     		 axisRef.dutchRef=	dutchJoy.getValue(	motionControlAPI.getCurrentPoint()->dutchRef+dutchJoy.getRawValue());
	     		 axisRef.tiltRef=	tiltJoy.getValue(	motionControlAPI.getCurrentPoint()->tiltRef+tiltJoy.getRawValue());
	     		 zifParams.zoomRef=	zoomJoy.getValue(	motionControlAPI.getCurrentPoint()->zoomRef+zoomJoy.getRawValue());

   				 speedLevelCorrectionExchange=motionControlAPI.getCurrentPoint()->levelCorrectionSpeed;
   				 contBitsTempMotion.all=(motionControlAPI.getCurrentPoint()->controlBits&0x0000FFFF);
   				 contBitsTemp.bit.fastLevelCorrect=contBitsTempMotion.bit.fastLevelCorrect;
   				 contBitsTemp.bit.levelCorrect=contBitsTempMotion.bit.levelCorrect;
   				 contBitsTemp.bit.levelSetup=contBitsTempMotion.bit.levelSetup;
   				 controlBitsExchange=contBitsTemp.all;

   				 outputPoleCntrl=motionControlAPI.getCurrentPoint()->poleRef+inputPoleCntrl;
	     		 break;
	     	 case MOTION_MIX_TRACK_ONLY:
	     	 default:
	     		 axisRef.panRef=	panJoy.getValue(	motionControlAPI.getCurrentPoint()->panRef);
	     		 axisRef.dutchRef=	dutchJoy.getValue(	motionControlAPI.getCurrentPoint()->dutchRef);
	     		 axisRef.tiltRef=	tiltJoy.getValue(	motionControlAPI.getCurrentPoint()->tiltRef);
	     		 zifParams.zoomRef=	zoomJoy.getValue(	motionControlAPI.getCurrentPoint()->zoomRef);

	     		 zifParams.irisRef=motionControlAPI.getCurrentPoint()->irisRef;;
				 zifParams.focusRef=motionControlAPI.getCurrentPoint()->focusRef;

				 speedLevelCorrectionExchange=motionControlAPI.getCurrentPoint()->levelCorrectionSpeed;
				 contBitsTempMotion.all=(motionControlAPI.getCurrentPoint()->controlBits&0x0000FFFF);
				 contBitsTemp.bit.fastLevelCorrect=contBitsTempMotion.bit.fastLevelCorrect;
				 contBitsTemp.bit.levelCorrect=contBitsTempMotion.bit.levelCorrect;
				 contBitsTemp.bit.levelSetup=contBitsTempMotion.bit.levelSetup;
				 controlBitsExchange=contBitsTemp.all;

				 outputPoleCntrl=motionControlAPI.getCurrentPoint()->poleRef;
	     }

	 }

	 	zifParams.zoomRef+= zoomDriftValue;
		if(zifParams.zoomRef>=0.99){zifParams.zoomRef=0.99;}
		if(zifParams.zoomRef<=-0.99){zifParams.zoomRef=-0.99;}

		if(zifParams.focusRef>=0.99){zifParams.focusRef=0.99;}
		if(zifParams.focusRef<=-0.99){zifParams.focusRef=-0.99;}

		if(zifParams.irisRef>=0.99){zifParams.irisRef=0.99;}
		if(zifParams.irisRef<=-0.99){zifParams.irisRef=-0.99;}

}


#pragma CODE_SECTION(".secure")
void Pult::exchangeAlternativeTask()
{

    BasicProtocolParams params (
            3,
            &cmdListAlt,
            Board_PULT_ALT_UART,
            MAX_TRANSFER_TIMEOUT_ALTERNATIV_TASK,
            Board_PULTALT_RS485RW
    );
    BasicProtocolMaster protocol(&params);
    protocol.writeCmdId = 0;
    protocol.askCmdId = 1;
    GPIO_write(Board_PULTALT_RS485RW, Board_RS485_WRITE_MODE);

    while(true)
    {
        //      Wait synchronization
        if(motionControlAPI.isActive())
        {
            if(!Semaphore_pend( *exchangeAltSem,BIOS_WAIT_FOREVER)) {  }
        }
        else
        {
            Task_sleep(3);
        }

        if (protocol.transaction(3, BASIC_PROTOCOL_BROADCAST_ADDRESS))
        {
            if(protocol.askCmdId == 1)
            {
                outputPoleCntrl=inputPoleCntrl;
            }
        }
    }
}

static volatile bool gtaComplite=false;
#pragma CODE_SECTION(".secure")
void Pult::exchangeTask()
{
	watchDogTimer.registerKey(WD_KEY1);
	ExtrSyncroization::ExtrenalDevieExchDriver::init();

//Инициализация протокола
	BasicProtocolParams params (
			1,
			&cmdList,
			Board_PULT_GYCON_UART,
			MAX_TRANSFER_TIMEOUT,//33
			Board_PULT_RS485RW
	);
	BasicProtocolMaster protocol(&params);
	protocol.writeCmdId = 0;
	protocol.askCmdId = 1;
	GPIO_write(Board_PULT_RS485RW, Board_RS485_WRITE_MODE);

	while (true) {
		watchDogTimer.useKey(WD_KEY1);


//Поготовка комманды записи
		if (transmitAxisSettingsCommand) {
			if (!noBasicWriteCmdCounter) {
				protocol.writeCmdId = 2;
				noBasicWriteCmdCounter = NO_BASIC_WRITE_CMD_PERIOD;
				if (stopTransmitAxisSettingsCommand) {
					transmitAxisSettingsCommand = false;
					stopTransmitAxisSettingsCommand  = false;
				}
			}
		}
//Обмен
		dataRenderLogic();

		transferDelay=counter;
		counter=0;

		if(*motionControlAPI.getAnglesFlag()==MOTION_GET_ANGLE)
		{
			protocol.askCmdId = 4;
			*motionControlAPI.getAnglesFlag()=MOTION_ANGLE_WAIT;
		}

		if(*motionControlAPI.getAnglesFlag()==MOTION_GET_ANGLE_ZIF)
		{
			protocol.askCmdId = 5;
			*motionControlAPI.getAnglesFlag()=MOTION_ANGLE_WAIT;
		}

		if(*motionControlAPI.getAnglesFlag()==MOTION_SET_ANGLE)
		{
			protocol.writeCmdId = 3;


			panWriteAngleH=		getAngleH(motionControlAPI.getAnglesData()->panRef);
			dutchWriteAngleH=	getAngleH(motionControlAPI.getAnglesData()->dutchRef);
			tiltWriteAngleH=	getAngleH(motionControlAPI.getAnglesData()->tiltRef);
			zoomWriteAngleH=	getAngleH(motionControlAPI.getAnglesData()->zoomRef);

			panWriteAngleL=		getAngleL(motionControlAPI.getAnglesData()->panRef);
			dutchWriteAngleL=	getAngleL(motionControlAPI.getAnglesData()->dutchRef);
			tiltWriteAngleL=	getAngleL(motionControlAPI.getAnglesData()->tiltRef);
			zoomWriteAngleL=	getAngleL(motionControlAPI.getAnglesData()->zoomRef);

			*motionControlAPI.getAnglesFlag()=MOTION_ANGLE_WAIT;
		}
		if(*motionControlAPI.getAnglesFlag()==MOTION_APPLY_SET_ANGLE)
		{
			controlBits.bit.goToAngels=1;
			*motionControlAPI.getAnglesFlag()=MOTION_APPLY_SET_WAIT;
		}
		if(*motionControlAPI.getAnglesFlag()==MOTION_APPLY_SET_WAIT)
		{
			if((gyConFaultBits.stateBits.goToAngleComplete==1)&&(!gtaComplite))
			{
				controlBits.bit.goToAngels=0;
				*motionControlAPI.getAnglesFlag()=MOTION_ANGLE_READY;
			}
			else
			{
				controlBits.bit.goToAngels=1;
			}
		}
		if(*motionControlAPI.getAnglesFlag()==MOTION_ANGLE_RESET)
		{
			controlBits.bit.goToAngels=0;
			protocol.writeCmdId = 0;
			protocol.askCmdId = 1;
			*motionControlAPI.getAnglesFlag()=MOTION_ANGLE_READY;
		}

		if(gyConFaultBits.stateBits.goToAngleComplete==1){	gtaComplite=true;}else{	gtaComplite=false;	}


		if((*motionControlAPI.getAnglesFlag()==MOTION_ANGLE_READY)&&debugMenuCmdFlag)
		{
			debugValue0=viewLists.getEditVarList()->get(0)->getValue();
			debugValue1=viewLists.getEditVarList()->get(1)->getValue();
			debugValue2=viewLists.getEditVarList()->get(2)->getValue();
			debugValue3=viewLists.getEditVarList()->get(3)->getValue();
			debugValue4=viewLists.getEditVarList()->get(4)->getValue();
			debugValue5=viewLists.getEditVarList()->get(5)->getValue();
			debugValue6=viewLists.getEditVarList()->get(6)->getValue();
			debugValue7=viewLists.getEditVarList()->get(7)->getValue();
			protocol.writeCmdId=2;
			protocol.askCmdId = 1;
		}


//		Wait synchronization
		if(motionControlAPI.isActive())
		{
			if(!Semaphore_pend(	*exchangeSem,BIOS_WAIT_FOREVER))
			{
			uint32_t a=10;
			a++;
			//TODO Критическая ошибка надо обработать
			}
		}



//TODO могут быть нюансы при отсуствии ОС,
//система может бесконечно пытаться передать альтернативную комманду
		if (protocol.transaction(PULT_EXCHANGE_ADDR, BASIC_PROTOCOL_BROADCAST_ADDRESS))
		{
			if(protocol.askCmdId == 4)
			{
				motionControlAPI.getAnglesData()->panRef=	getAngle(panReadAngleH,panReadAngleL);
				motionControlAPI.getAnglesData()->dutchRef=	getAngle(dutchReadAngleH,dutchReadAngleL);
				motionControlAPI.getAnglesData()->tiltRef=	getAngle(tiltReadAngleH,tiltReadAngleL);
//				motionControlAPI.getAnglesData()->zoomRef=	getAngle(zoomReadAngleH,zoomReadAngleL);
				*motionControlAPI.getAnglesFlag()=MOTION_ANGLE_READY;
			}

			if(protocol.askCmdId == 5)
			{
				motionControlAPI.getAnglesData()->zoomRef=	getAngle(zoomReadAngleH,zoomReadAngleL);
				*motionControlAPI.getAnglesFlag()=MOTION_ANGLE_READY;
			}

			if(protocol.writeCmdId == 3)
			{
				*motionControlAPI.getAnglesFlag()=MOTION_ANGLE_READY;
			}

			if(protocol.writeCmdId == 2)
			{
				debugMenuCmdFlag=false;
			}

			protocol.writeCmdId = 0; //Восстановление стандартной комманды записи
			protocol.askCmdId = 1;
//			readAngles = false;
//			writeAngles = false;
			if (noBasicWriteCmdCounter)
				noBasicWriteCmdCounter--;
			gyConFaultBits.all = gyConFaultBitsExchange;
			okCounter++;
		} else {

			faultCounter++;
		}

		if(!motionControlAPI.isActive())
		{
			Task_sleep(3);
		}
		pultController();
		connectedFlag = protocol.isConnected();

		checkErrorLogic();

	}
}

//====================================================================================================
// Pult Logic

static void inversLogic() {
	switch (panReversButton.state) {
	case PRESSED:
		panJoy.setInversOrientation();
		break;
	case RELESASED:
		panJoy.setNormalOrientation();
		break;
	}
	switch (dutchReversButton.state) {
	case PRESSED:
		dutchJoy.setInversOrientation();
		break;
	case RELESASED:
		dutchJoy.setNormalOrientation();
		break;
	}
	switch (tiltReversButton.state) {
	case PRESSED:
		tiltJoy.setInversOrientation();
		break;
	case RELESASED:
		tiltJoy.setNormalOrientation();
		break;
	}
	//ZIF REVERS
	switch (zoomReversButton.state)
	{
		case PRESSED:
			zoomJoy.setInversOrientation();
			break;
		case RELESASED:
			zoomJoy.setNormalOrientation();
			break;
	}
	switch (irisReversButton.state)
	{
		case PRESSED:
		    IrisResistor.setInversOrientation();
			break;
		case RELESASED:
		    IrisResistor.setNormalOrientation();
			break;
	}
	switch (focusReversButton.state)
	{
		case PRESSED:
			focusResistor.setInversOrientation();
			break;
		case RELESASED:
			focusResistor.setNormalOrientation();
			break;
	}
}


static void joySticksOnOffLogic() {
	switch (joyStickOnOffButton.state) {
	case PRESSED:

		panJoyChannel.enable();
		dutchJoyChannel.enable();
		tiltJoyChannel.enable();
		zoomJoyChannel.enable();
		controlBits.bit.joysticOn=1;
		break;
	case RELESASED:
		panJoyChannel.disable();
		dutchJoyChannel.disable();
		tiltJoyChannel.disable();
		zoomJoyChannel.disable();
		controlBits.bit.joysticOn=0;
		break;
	}
	if (virtualButtonJoysticOff.state==RELESASED) {
	    zoomJoy.enable();
	    panJoy.enable();
	    dutchJoy.enable();
	    tiltJoy.enable();}
	else {
	    zoomJoy.disable();
	    panJoy.disable();
	    dutchJoy.disable();
	    tiltJoy.disable();
	}
}

static UInt8 panSensId = 0;
static UInt8 dutchSensId = 0;
static UInt8 tiltSensId = 0;
static UInt8 zoomSensId = 0;

volatile static UInt16 stErrorBits=0;
volatile static UInt8 disconnectedCounter=0;
volatile static bool discTrigger=true;

volatile static bool erUpdated=true;
volatile static UInt16 f1Last=0;
volatile static UInt16 f2Last=0;
volatile static bool trLast=true;

volatile static UInt8 transCounterLast=0;
//volatile static UInt32 maxTransferDelay=0;

void Pult::checkErrorLogic()
{
	stErrorBits=stErrorBits|(gyConFaultBits.all);
	checkPoints.parseBytes(gyConFaultBits.all,stErrorBits);

	if(!connectedFlag)
	{
//		autonegComplete=false;
		if(discTrigger)
		{
			discTrigger=false;
			disconnectedCounter++;
		}
	}
	else
	{
		if(!discTrigger){discTrigger=true;}
	}

	if(f1Last!=gyConFaultBits.all)
	{
		f1Last=gyConFaultBits.all;
		erUpdated=true;
	}

	if(f2Last!=stErrorBits)
	{
		f2Last=stErrorBits;
		erUpdated=true;
	}

	if(trLast!=discTrigger)
	{
		trLast=discTrigger;
		erUpdated=true;
	}

	if(transCounterLast!=transferDelay)
	{
		transCounterLast=transferDelay;
		erUpdated=true;

//		if(transferDelay>maxTransferDelay)
//		{
//			maxTransferDelay=transferDelay;
//		}

	}
}

static void joyStickSensLogic() {
   if (panSenseButton.isClicked()) {
	   panSensId++;
	   if (panSensId >= JOYSTICK_FUNCS_COUNT) panSensId = 0;
	   panJoy.func = panFuncs[panSensId];
   }
   if (dutchSenseButton.isClicked()) {
	   dutchSensId++;
	   if (dutchSensId >= JOYSTICK_FUNCS_COUNT) dutchSensId = 0;
	   dutchJoy.func = dutchFuncs[dutchSensId];
   }
   if (tiltSenseButton.isClicked()) {
	   tiltSensId++;
	   if (tiltSensId >= JOYSTICK_FUNCS_COUNT) tiltSensId = 0;
	   tiltJoy.func = tiltFuncs[tiltSensId];
   }
}

static void controlLogic() {
	static UInt8 setUpTiltLimitsCounter = 0;
	static UInt8 setDwTiltLimitsCounter = 0;
	static UInt8 resetUpTiltLimitsCounter = 0;
	static UInt8 resetDwTiltLimitsCounter = 0;
	static UInt8 lensCalibrationCounter = 0;
	static UInt8 estimationBitCounter=0;

    controlBits.bit.onOffMotors = motorOnOffButton.isPressed();
    controlBits.bit.levelCorrect = levelCorrectButton.isPressed();
    controlBits.bit.levelSetup = dutchLevelSetupButton.isPressed();
    controlBits.bit.gvCalibration = gvCalibrationButton.isPressed();
	controlBits.bit.fastLevelCorrect = fastLevelCorrectButton.isPressed();

	if (setUpTiltLimitsFlag == true) {setUpTiltLimitsCounter = 3; setUpTiltLimitsFlag = false;};
	if (setDwTiltLimitsFlag == true) {setDwTiltLimitsCounter = 3; setDwTiltLimitsFlag = false;};
	if (resetUpTiltLimitsFlag == true) {resetUpTiltLimitsCounter = 3; resetUpTiltLimitsFlag = false;};
	if (resetDwTiltLimitsFlag == true) {resetDwTiltLimitsCounter = 3; resetDwTiltLimitsFlag = false;};
	if (lensCalibrationButton.isClicked()||pult.lensCalibrtionClicked()){lensCalibrationCounter = 3;}
	if (estimationFlag==true){estimationBitCounter=3;estimationFlag=false;}

	if (estimationBitCounter){
		estimationBitCounter--;
		//todo set true
	}
	else
		// set false;

	if (lensCalibrationCounter) {
		lensCalibrationCounter--;
		motorControlBits.bit.tuningStart = true;
	} else
		motorControlBits.bit.tuningStart = false;
	if (setUpTiltLimitsCounter) {
		setUpTiltLimitsCounter--;
		controlBits.bit.setUpTiltLimits = true;
	} else
		controlBits.bit.setUpTiltLimits = false;

	if (setDwTiltLimitsCounter) {
		setDwTiltLimitsCounter--;
		controlBits.bit.setDwTiltLimits = true;
	} else
		controlBits.bit.setDwTiltLimits = false;

	if (resetUpTiltLimitsCounter) {
		resetUpTiltLimitsCounter--;
		controlBits.bit.resetUpTiltLimits = true;
	} else
		controlBits.bit.resetUpTiltLimits = false;

	if (resetDwTiltLimitsCounter) {
		resetDwTiltLimitsCounter--;
		controlBits.bit.resetDwTiltLimits = true;
	} else
		controlBits.bit.resetDwTiltLimits = false;


	if (gvAccButton.isClicked()) {
		controlBits.bit.gvAcc = !controlBits.bit.gvAcc;
	}
	if (cameraStartButton.isClicked()) {
		motorControlBits.bit.cameraStart=!motorControlBits.bit.cameraStart;
	}
}


static void pultController()
{
	controlLogic();
	joySticksOnOffLogic();
	inversLogic();
	joyStickSensLogic();
}

PultButton* Pult::getButton(PultButtons btn) {
	if (btn<PULT_BUTTONS_COUNT) {
		return sharedButtons[btn];
	} else {
		return NULL;
	}
}

PultButton* Pult::getButton_1_16(uint8_t ID) {
    if (ID<BUTTONS_1_16_COUNT) {
        return buttons_1_16[ID];
    } else {
        return NULL;
    }
}

PultButton* Pult::getButtonVirtual(uint8_t ID) {
    if (ID<virtualButton) {
        return virtualButtons[ID];
    } else {
        return NULL;
    }
}

void Pult::setAxisSetting(PultAxisSettings setting, float value) {
	if (setting<AXIS_SETTINGS_COUNT) {
		 *writeControlCmdData[setting+5].ptr=value;
	}
}

void Pult::startTransmitAxisSettings() {
	transmitAxisSettingsCommand = true;
}

void Pult::stopTransmitAxisSettings() {
	stopTransmitAxisSettingsCommand = true;
}

float Pult::getPanTurns()
{
	return (float)panTurns; //   /100
}

GyConState Pult::getGyConState() {
	return GyConState_Normal;
}

char* Pult::getFaultMessage() {
	return "";
}

bool Pult::isConnected() {
	return connectedFlag;
}

bool Pult::isJoySticksEnabled() {
	return joyStickOnOffButton.isPressed();
}

bool Pult::isMotorsEnabled() {
	return motorOnOffButton.isPressed();
}
bool Pult::isBackLightEnabled()
{
	return sharedButtons[backLightOff]->isPressed();
}

char* Pult::getPanPreset() {
	if(panSensId==0) return "Cube";
	if(panSensId==1) return "Sqr";
	if(panSensId==2) return "Lin";
	return "PS ";
}

char* Pult::getDutchPreset() {
	if(dutchSensId==0) return "Cube";
	if(dutchSensId==1) return "Sqr";
	if(dutchSensId==2) return "Lin";
	return "PS ";
}

char* Pult::getTiltPreset() {
	if(tiltSensId==0) return "Cube";
	if(tiltSensId==1) return "Sqr";
	if(tiltSensId==2) return "Lin";
	return "PS ";
}

void Pult::setTiltUpLimit() {
	setUpTiltLimitsFlag = true;
}

void Pult::setTiltDnLimit() {
	setDwTiltLimitsFlag = true;
}

void Pult::resetTiltUpLimit() {
	resetUpTiltLimitsFlag = true;
}

void Pult::resetTiltDnLimit() {
	resetDwTiltLimitsFlag = true;
}

void Pult::setPlatform(UInt8 num) {
/*	if (num<4) {
		controlBits.bit.algorithmNum = num;
	}*/
}

char* Pult::getGvAcc() {
	if (!controlBits.bit.gvAcc) {
		return "GV";
	} else {
		return "ACC";
	}
}

void Pult::setXY(char* pName, UInt8 num, float x, float y) {
}

void Pult::initPanPreset(char* preset) {

}

void Pult::initDutchPreset(char* preset) {
}

void Pult::initTiltPreset(char* preset) {
}

void Pult::setPultCalibratesCoeff(PultCalibrateCoeffs name, float value) {
}

float Pult::getPultDebugValue(PultDebugValues name) {
	return 0.1;
}


Warnings* Pult::getWarnings() {
	return &warnings;
}

ViewLists* Pult::getViewLists() {
    return &viewLists;
}

Warning headDisconnectedWarning("Head disconnected!",	WT_WARNING);
Warning pDusFail("Pan ARS Fail", 						WT_WARNING);
Warning dDusFail("Dutch ARS Fail", 						WT_WARNING);
Warning tDusFail("Tilt ARS Fail", 						WT_WARNING);
Warning gvFail("GV Fail", 								WT_WARNING);
Warning encoderFail("Encoder Fail", 					WT_WARNING);
Warning pultFail("Pult Fail", 							WT_WARNING);
Warning pultGVCalibrat ("Giro vertical calibration", WT_INFO);

void Pult::updateWarningsList()
{
	warnings.getRunStrWarnings()->removeAll();

	if(!connectedFlag){	warnings.getRunStrWarnings()->add(&headDisconnectedWarning);}
	if(gyConFaultBits.faultBits.panDusFault!=0){	warnings.getRunStrWarnings()->add(&pDusFail);}
	if(gyConFaultBits.faultBits.dutchDusFault!=0){	warnings.getRunStrWarnings()->add(&dDusFail);}
	if(gyConFaultBits.faultBits.tiltDusFault!=0){	warnings.getRunStrWarnings()->add(&tDusFail);}
	if(gyConFaultBits.faultBits.gvFault!=0){	warnings.getRunStrWarnings()->add(&gvFail);}
	if(gyConFaultBits.faultBits.encodersFault!=0){	warnings.getRunStrWarnings()->add(&encoderFail);}
	if(gyConFaultBits.faultBits.pultFault!=0){	warnings.getRunStrWarnings()->add(&pultFail);}
	//здесь дописываем индикацию калибровки ГВ
	if (controlBits.bit.gvCalibration) {warnings.getRunStrWarnings()->add(&pultGVCalibrat);}
}

const char* Pult::getJoystickPresetName(PultJoystickPresets preset) {
	switch (preset) {
	case PanJoystickPreset:
		return joystickPresetNames[panSensId];
	case DutchJoystickPreset:
		return joystickPresetNames[dutchSensId];
	case TiltJoystickPreset:
		return joystickPresetNames[tiltSensId];
	case ZoomJoystickPreset:
	        return zoomJoystickPresetNames[zoomSensId];
	}
	return "";
}

UInt8 Pult::getJoystickPresetId(PultJoystickPresets preset) {
	switch (preset) {
	case PanJoystickPreset:
		return panSensId;
	case DutchJoystickPreset:
		return dutchSensId;
	case TiltJoystickPreset:
		return tiltSensId;
	case ZoomJoystickPreset:
	    return zoomSensId;
	}
	return 0;
}

void Pult::setJoystickPresetId(PultJoystickPresets preset, UInt8 presetId) {
	if (presetId< JOYSTICK_FUNCS_COUNT) {
		switch (preset) {
		case PanJoystickPreset:
			if (presetId >= JOYSTICK_FUNCS_COUNT)   {panJoy.func = panFuncs[0];				panSensId=0;}
			else									{panJoy.func = panFuncs[presetId];		panSensId=presetId;}
			return;
		case DutchJoystickPreset:
			if (presetId >= JOYSTICK_FUNCS_COUNT)   {dutchJoy.func = dutchFuncs[0];			dutchSensId=0;}
			else									{dutchJoy.func = dutchFuncs[presetId];	dutchSensId=presetId;}
			return;
		case TiltJoystickPreset:
			if (presetId >= JOYSTICK_FUNCS_COUNT)   {tiltJoy.func = tiltFuncs[0];			tiltSensId=0;}
			else									{tiltJoy.func = tiltFuncs[presetId];	tiltSensId=presetId;}
			return;
		case ZoomJoystickPreset:
			if (presetId >= JOYSTICK_FUNCS_COUNT)   {zoomJoy.func = zoomFuncs[0];           zoomSensId=0;}
			else									{zoomJoy.func = zoomFuncs[presetId];    zoomSensId=presetId;}
			return;
		}
	}
}

List<Point*>* Pult::getEqualaserPoints(PultEqualaserType type)
{
	switch(type)
	{
		case PanJoystickEqualaser:
			return &(equRefPoints.panReferencePointList);
		case DutchJoystickEqualaser:
			return &(equRefPoints.dutchReferencePointList);
		case TiltJoystickEqualaser:
			return &(equRefPoints.tiltReferencePointList);
		case ZoomJoystickEqualaser:
			return &(equRefPoints.zoomReferencePointList);
		default:return NULL;
	}
}
void Pult::updateAproximator(PultEqualaserType type)
{
	switch(type)
		{
			case PanJoystickEqualaser:
				 panLineralAproximator.updateReferencePoints();
				return ;

			case DutchJoystickEqualaser:
				dutchLineralAproximator.updateReferencePoints();
				return ;

			case TiltJoystickEqualaser:
				tiltLineralAproximator.updateReferencePoints();
				return ;

			case ZoomJoystickEqualaser:
				zoomLineralAproximator.updateReferencePoints();
				return ;
			default:return ;
		}
}
float Pult::getEqualaserAproximatorValue(PultEqualaserType type, float x)
{
	switch(type)
	{
		case PanJoystickEqualaser:
			if(!panLineralAproximator.isComputed_()){return 0;}
			return -panLineralAproximator.calculate(x);

		case DutchJoystickEqualaser:
			if(!dutchLineralAproximator.isComputed_()){return 0;}
			return -dutchLineralAproximator.calculate(x);

		case TiltJoystickEqualaser:
			if(!tiltLineralAproximator.isComputed_()){return 0;}
			return -tiltLineralAproximator.calculate(x);
		case ZoomJoystickEqualaser:
			if(!zoomLineralAproximator.isComputed_()){return 0;}
			return -zoomLineralAproximator.calculate(x);
		default:return 0;
	}
}

float Pult::getJoysticSpeedValue(PultEqualaserType type)
{
	switch(type)
		{
			case PanJoystickEqualaser:
				return panJoyChannel.getSpeed();

			case DutchJoystickEqualaser:
				return dutchJoyChannel.getSpeed();

			case TiltJoystickEqualaser:
				return tiltJoyChannel.getSpeed();

			case ZoomJoystickEqualaser:
				return zoomJoyChannel.getSpeed();
			default:return 1;
		}
}
float Pult::getJoystickCurrentPozition(PultEqualaserType type)
{
	switch(type)
		{
			case PanJoystickEqualaser:
				return panJoyChannel.getCurrentAdcValue();
			case DutchJoystickEqualaser:
				return dutchJoyChannel.getCurrentAdcValue();
			case TiltJoystickEqualaser:
				return tiltJoyChannel.getCurrentAdcValue();
			case ZoomJoystickEqualaser:
				return zoomJoyChannel.getCurrentAdcValue();
			default:return 1;
		}
}

float Pult::getJoystickMaxValue(PultEqualaserType type)
{
	switch(type)
		{
			case PanJoystickEqualaser:
				return panJoy.maxValue;
			case DutchJoystickEqualaser:
				return dutchJoy.maxValue;
			case TiltJoystickEqualaser:
				return tiltJoy.maxValue;
			case ZoomJoystickEqualaser:
				return zoomJoy.maxValue;
			default:return 1;
		}
}
LedController* Pult::getLedController()
{
	return &ledControl;
}

void Pult::setJoysticConfiguration(PultJoysticConfigurations preset)
{

	if(joysticsConfig==COUNT_JOY_CONFIG&&preset!=JOYSTIC_REVERS_JOY_CONFIG)
	{

		panJoyChannel.relativeInvers();
		dutchJoyChannel.relativeInvers();
		tiltJoyChannel.relativeInvers();
		zoomJoyChannel.relativeInvers();

	}

	joysticsConfig=preset;


	switch(joysticsConfig)
	{
		case JOYSTIC_REVERS_JOY_CONFIG:
			panJoyChannel.setOffset(dutchOffset);
			dutchJoyChannel.setOffset(panOffset);
			tiltJoyChannel.setOffset(zoomOffset);
			zoomJoyChannel.setOffset(tiltOffset);

			panJoyChannel.relativeInvers();
			dutchJoyChannel.relativeInvers();
			tiltJoyChannel.relativeInvers();
			zoomJoyChannel.relativeInvers();
			break;

		case DEFAULT_JOY_CONFIG:
		default:
			panJoyChannel.setOffset(panOffset);
			dutchJoyChannel.setOffset(dutchOffset);
			tiltJoyChannel.setOffset(tiltOffset);
			zoomJoyChannel.setOffset(zoomOffset);

			panJoyChannel.relativeInvers();
			dutchJoyChannel.relativeInvers();
			tiltJoyChannel.relativeInvers();
			zoomJoyChannel.relativeInvers();
			break;
	}
}

//--------------------------------- ZIF API  ------------------------------------

void Pult::setMotorType(UInt8 type, UInt8 motorNumber){
	switch(motorNumber)
	{
		case LC_MOTOR1:control1.bit.motor1Type=type;break;
		case LC_MOTOR2:control1.bit.motor2Type=type;break;
		case LC_MOTOR3:control1.bit.motor3Type=type;break;
	}
}
void Pult::setZoomMotor(UInt8 motorNumber)
{
	if(motorNumber>=LC_MOTORS_COUNT){return;}
	control2.bit.zoomMotorNO=motorNumber;

}
void Pult::setIrisMotor(UInt8 motorNumber)
{
	if(motorNumber>=LC_MOTORS_COUNT){return;}
	control2.bit.irisMotorNO=motorNumber;
}
void Pult::setFocusMotor(UInt8 motorNumber)
{
	if(motorNumber>=LC_MOTORS_COUNT){return;}
	control2.bit.focusMotorNO=motorNumber;
}

UInt8 Pult::getZoomMotor(){return control2.bit.zoomMotorNO;}
UInt8 Pult::getIrisMotor(){return control2.bit.irisMotorNO;}
UInt8 Pult::getFocusMotor(){return control2.bit.focusMotorNO;}

UInt8 Pult::getMotorType(UInt8 no)
{
	switch(no)
	{
		case LC_MOTOR1:return control1.bit.motor1Type;
		case LC_MOTOR2:return control1.bit.motor2Type;
		case LC_MOTOR3:return control1.bit.motor3Type;
		default:return 0;
	}
}
//-------------------------------------------------------------------------------
float  Pult::getHeadVoltage()
{
	if(!connectedFlag){return 0;}
	return headSupplyVoltage;
}
PanAxisDirection Pult::getPanAxisDirection()
{
	if(axisRef.panRef>0.2){return PAN_AXIS_UP;}
	if(axisRef.panRef<-0.2){return PAN_AXIS_DOWN;}
	return PAN_AXIS_STOP;
}
//-------------------------------------------------------------------------------
void Pult::initWatchDog()
{
	watchDogTimer.init(true, 0x06FFFFFF);
}
//-------------------------------------------------------------------------------
PultButton** Pult::getMotionControlButtons()
{
	return motionButtons;
}
MotionControl* Pult::getMotionSubsystem()
{
	return &motionControlAPI;
}

bool Pult::joysticUsed()
{
	if(panJoy.getUsedFlag()||dutchJoy.getUsedFlag()||tiltJoy.getUsedFlag()||zoomJoy.getUsedFlag())
	{
		return true;
	}
	return false;
}
void Pult::resetCentralButtons()
{
	sharedButtons[pult_Button_Up]->clearFlags();
	sharedButtons[pult_Button_Dn]->clearFlags();
	sharedButtons[pult_Button_Left]->clearFlags();
	sharedButtons[pult_Button_Right]->clearFlags();
	sharedButtons[pult_Button_Select]->clearFlags();
}
UInt8 Pult::getTransferDelay()
{
	return transferDelay;
}

//-------------------------------------------------------------------------------
void Pult::setCameraStartOptions(CameraStartType option)
{
	switch(option)
	{
		case CAMERA_START_FRONT:
			motorControlBits.bit.cameraStartType=1;
			break;
		case CAMERA_START_LEVEL:
			motorControlBits.bit.cameraStartType=0;
			break;
		default:
			motorControlBits.bit.cameraStartType=1;
	}
}
//-------------------------------------------------------------------------------
void Pult::setOverslangState(bool on)
{
	if(on)
	{
		controlBits.bit.overslangUnderslang=0;
	}
	else
	{
		controlBits.bit.overslangUnderslang=1;
	}
}
//-------------------------------------------------------------------------------
void Pult::setEstimationBit()
{
	estimationFlag=true;
}
//-------------------------------------------------------------------------------
List<CheckPoint*>* Pult::getCheckValues()
{
	return checkPoints.getList();
}
void Pult::resetStaticErrorBits()
{
	stErrorBits=0;
}
void Pult::resetDisconnectCounter()
{
	disconnectedCounter=0;
//	maxTransferDelay=0;
}
UInt8 Pult::getDisconnectCounter()
{
	return disconnectedCounter;
}
bool Pult::isErrorUpdated()
{
	bool tmp=erUpdated;
	erUpdated=false;
	return tmp;
}
void Pult::sendDebugValue()
{
	debugMenuCmdFlag=true;
}
//-------------------------------------------------------------------------------
UInt32 Pult::getMotionModeTransferDelay()
{
	return transferDelayMaximum;
}
void Pult::setMotionModeTransferDelay(UInt32 delay)
{
	transferDelayMaximum=delay;
}
bool Pult::motionTransferModeActivate()
{
	if(autonegComplete){return true;}
	if((!connectedFlag)||transferDelay>MAX_TRANSFER_TIMEOUT)
	{
		autonegComplete=false;
		return false;
	}
	if(transferDelay<20)
	{
		setMotionModeTransferDelay(20);
		autonegComplete=true;
		return true;
	}
	else
	{
		setMotionModeTransferDelay(transferDelay+5);
		autonegComplete=true;
		return true;
	}
}

//-------------------------------------------------------------------------------
void Pult::setPanWeelSpeed(float val)
{
	panChanelDeadResistor.setValue(val);

}
void Pult::setTiltWeelSpeed(float val)
{
	tiltChanelDeadResistor.setValue(val);
}
void Pult::setDutchWeelSpeed(float val)
{
	dutchChanelDeadResistor.setValue(val);
}

//-------------------------------------------------------------------------------

float Pult::getCalibrationOffset(CalibratedJoyChannel ch)
{
	if(joysticsConfig!=DEFAULT_JOY_CONFIG)
	{
		switch(ch)
		{
			case CALIB_OFFSET_MAIN_PAN:		return dutchJoyChannel.getCalibrationOffsetValue();
			case CALIB_OFFSET_MAIN_DUTCH:	return panJoyChannel.getCalibrationOffsetValue();
			case CALIB_OFFSET_MAIN_TILT:	return zoomJoyChannel.getCalibrationOffsetValue();
			case CALIB_OFFSET_MAIN_ZOOM:	return tiltJoyChannel.getCalibrationOffsetValue();
			case CALIB_OFFSET_EXT_PAN:		return panExtern1Channel.getCalibrationOffsetValue();
			case CALIB_OFFSET_EXT_DUTCH:	return dutchExtern1Channel.getCalibrationOffsetValue();
			case CALIB_OFFSET_EXT_TILT:		return tiltExtern1Channel.getCalibrationOffsetValue();
			case CALIB_OFFSET_EXT2_DUTCH:	return dutchExtern2Channel.getCalibrationOffsetValue();
			default:return 1860;
		}
	}
	else
	{
		switch(ch)
		{
			case CALIB_OFFSET_MAIN_PAN:		return panJoyChannel.getCalibrationOffsetValue();
			case CALIB_OFFSET_MAIN_DUTCH:	return dutchJoyChannel.getCalibrationOffsetValue();
			case CALIB_OFFSET_MAIN_TILT:	return tiltJoyChannel.getCalibrationOffsetValue();
			case CALIB_OFFSET_MAIN_ZOOM:	return zoomJoyChannel.getCalibrationOffsetValue();
			case CALIB_OFFSET_EXT_PAN:		return panExtern1Channel.getCalibrationOffsetValue();
			case CALIB_OFFSET_EXT_DUTCH:	return dutchExtern1Channel.getCalibrationOffsetValue();
			case CALIB_OFFSET_EXT_TILT:		return tiltExtern1Channel.getCalibrationOffsetValue();
			case CALIB_OFFSET_EXT2_DUTCH:	return dutchExtern2Channel.getCalibrationOffsetValue();
			default:return 1860;
		}
	}
}

void  Pult::setCalibrationOffset(CalibratedJoyChannel ch, UInt32 offset)
{
	if(joysticsConfig!=DEFAULT_JOY_CONFIG)
	{
		switch(ch)
		{
			case CALIB_OFFSET_MAIN_PAN:		dutchJoyChannel.setOffset(offset);		panOffset=offset;		break;
			case CALIB_OFFSET_MAIN_DUTCH:	panJoyChannel.setOffset(offset);		dutchOffset=offset;		break;
			case CALIB_OFFSET_MAIN_TILT:	zoomJoyChannel.setOffset(offset);		tiltOffset=offset;		break;
			case CALIB_OFFSET_MAIN_ZOOM:	tiltJoyChannel.setOffset(offset);		zoomOffset=offset;		break;
			case CALIB_OFFSET_EXT_PAN:		panExtern1Channel.setOffset(offset);	break;
			case CALIB_OFFSET_EXT_DUTCH:	dutchExtern1Channel.setOffset(offset);	break;
			case CALIB_OFFSET_EXT_TILT:		tiltExtern1Channel.setOffset(offset);	break;
			case CALIB_OFFSET_EXT2_DUTCH:	dutchExtern2Channel.setOffset(offset);	break;
			default:return;
		}
	}
	else
	{
		switch(ch)
		{
			case CALIB_OFFSET_MAIN_PAN:		panJoyChannel.setOffset(offset);		panOffset=offset;		break;
			case CALIB_OFFSET_MAIN_DUTCH:	dutchJoyChannel.setOffset(offset);		dutchOffset=offset;		break;
			case CALIB_OFFSET_MAIN_TILT:	tiltJoyChannel.setOffset(offset);		tiltOffset=offset;		break;
			case CALIB_OFFSET_MAIN_ZOOM:	zoomJoyChannel.setOffset(offset);		zoomOffset=offset;		break;
			case CALIB_OFFSET_EXT_PAN:		panExtern1Channel.setOffset(offset);	break;
			case CALIB_OFFSET_EXT_DUTCH:	dutchExtern1Channel.setOffset(offset);	break;
			case CALIB_OFFSET_EXT_TILT:		tiltExtern1Channel.setOffset(offset);	break;
			case CALIB_OFFSET_EXT2_DUTCH:	dutchExtern2Channel.setOffset(offset);	break;
			default:return;
		}
	}
}
//-------------------------------------------------------------------------------
void Pult::setSuspentionResonantFreq(float freq,SuspensionResonansChannel channel)
{
	switch(channel)
	{
		case SUSPENSION_RESONANCE_CHANNEL_TILT:
			tiltSuspentionResonantFreq=freq;
			break;
		case SUSPENSION_RESONANCE_CHANNEL_PAN:
			panSuspentionResonantFreq=freq;
			break;
	}
}

void Pult::setSuspentionResonantMode(SuspensionResonansChannel channel,SuspensionResonansMode mode)
{
	switch(channel)
	{
		case SUSPENSION_RESONANCE_CHANNEL_TILT:

			break;
		case SUSPENSION_RESONANCE_CHANNEL_PAN:
			controlBits.bit.panSuspensionResonatorDisable=mode;
			break;
	}

}
//-------------------------------------------------------------------------------
void Pult::setMaxTorque(UInt32 pan,UInt32 dutch,UInt32 tilt)
{
	maxTourqueValues.axis.pan=pan/5;
	maxTourqueValues.axis.dutch=dutch/5;
	maxTourqueValues.axis.tilt=tilt/5;
	maxTourqueValues.axis.bit15=0;
}

void Pult::setEcoMode(bool eco) {
    controlBits.bit.ecoMode=(UInt16)eco;
}

void Pult::setJoyDeadZone(UInt32 pan,UInt32 dutch,UInt32 tilt,UInt32 zoom)
{
	panJoyChannel.setDeadZone(pan);
	dutchJoyChannel.setDeadZone(dutch);
	tiltJoyChannel.setDeadZone(tilt);
	zoomJoyChannel.setDeadZone(zoom);

	panExtern1Channel.setDeadZone(pan);
	dutchExtern1Channel.setDeadZone(dutch);
	tiltExtern1Channel.setDeadZone(tilt);


}
//-------------------------------------------------------------------------------
void Pult::startLensCalibration()
{
	lensCalibrateFlag=true;
}
bool Pult::lensCalibrtionClicked()
{
	bool tmp=lensCalibrateFlag;
	lensCalibrateFlag=false;
	return tmp;
}
//-------------------------------------------------------------------------------
void Pult::setStartFluid(UInt32 valuePan,UInt32 valueDutch,UInt32 valueTilt,UInt32 valueZoom)
{
	panJoy.setStartFluid(valuePan);
	dutchJoy.setStartFluid(valueDutch);
	tiltJoy.setStartFluid(valueTilt);
	zoomJoy.setStartFluid(valueZoom);
}
//-------------------------------------------------------------------------------
void Pult::setZoomDrift(Int32 zoomDrift)
{
	zoomDriftValue=zoomDrift/500.0;
	if(zoomDriftValue>0.2)  {zoomDriftValue=0.2;}
	if(zoomDriftValue<-0.2) {zoomDriftValue=-0.2;}
}
//-------------------------------------------------------------------------------
void Pult::setMasterPrerolValue(Int32 value)
{
    if(value>30000){value=30000;}
    masterPrerolValue=value;
}

UInt32 Pult::getMasterPrerolValue()
{
    return masterPrerolValue;
}

void   Pult::setSlave1PrerolValue(Int32 value)
{
    if(value>30000){value=30000;}
    slave1PrerolValue=value;
}
UInt32 Pult::getSlave1PrerolValue()
{
    return slave1PrerolValue;
}

void   Pult::setSlave2PrerolValue(Int32 value)
{
    if(value>30000){value=30000;}
    slave2PrerolValue=value;
}
UInt32 Pult::getSlave2PrerolValue()
{
    return slave2PrerolValue;
}

void   Pult::setSlave3PrerolValue(Int32 value)
{
    if(value>30000){value=30000;}
    slave3PrerolValue=value;
}
UInt32 Pult::getSlave3PrerolValue()
{
    return slave3PrerolValue;
}

//-------------------------------------------------------------------------------
UInt32 Pult::getTimeToStart()
{
	return timeToStart;
}
//-------------------------------------------------------------------------------
float Pult::getPanDriftFactor()
{
    return (float)panDriftFactor;
}

float Pult::getDutchDriftFactor()
{
    return (float)dutchDriftFactor; //   /100
}

float Pult::getTiltDriftFactor()
{
    return (float)tiltDriftFactor; //   /100
}
float* Pult::getPanDriftFactorUk()
{
    return (float*)&panDriftFactor;
}

float* Pult::getDutchDriftFactorUk()
{
    return (float*)&dutchDriftFactor; //   /100
}

float* Pult::getTiltDriftFactorUk()
{
    return (float*)&tiltDriftFactor; //   /100
}

//-------------------------------------------------------------------------------
UInt16 Pult::getControlBits ()
{
    return controlBits.all;
}
//-------------------------------------------------------------------------------
UInt16 Pult::getGyConFaultBits ()
{
    return gyConFaultBits.stateBits.faultBits;
}
//-------------------------------------------------------------------------------
void Pult::setDriftStopperMode(bool val)
{
    if(val)
    {
        controlBits.bit.driftStopperOn=1;
    }
    else
    {
        controlBits.bit.driftStopperOn=0;
    }
}
//-------------------------------------------------------------------------------
void Pult::setPanFolowingMode(FolowingModeConfigData val)
{
    controlBitsA.bit.panSuspentionFollowingMode =val.mode;
    controlBitsA.bit.panFollowingSectorSize     =val.sector;
}
//-------------------------------------------------------------------------------
void   Pult::setSynchroSource(UInt32 value)
{
    switch(value)
    {
        case 1:
            ExtrSyncroization::ExtrenalDevieExchDriver::selectMode(ExtrSyncroization::EXT_DEV_PAN_BAR);
            break;
        case 0:
            ExtrSyncroization::ExtrenalDevieExchDriver::selectMode(ExtrSyncroization::EXT_DEV_MOTION);
            break;
    }
}
