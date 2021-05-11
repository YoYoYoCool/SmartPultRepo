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
#include "../ExtrenalDeviceSynchro/PanBar/PanBar.hpp"
#include "../ExtrenalDeviceSynchro/DigitalWheel/Protocol.hpp"
#include "../ExtrenalDeviceSynchro/DigitalWheel/Digital Wheel.hpp"
#include "../ExtrenalDeviceSynchro/DigitalWheel/ExchangeWheelManager.hpp"
#include "LensParam/LensPack.hpp"
#include "PULT_Indicator/ServoPwm.h"
#include "backlight.hpp"
#include "../ExtrenalDeviceSynchro/Preston/Preston.hpp"
#include "Libs/StandartElement/KalmanFilter.hpp"



#define MAX_TRANSFER_TIMEOUT 350
//#define MAX_TRANSFER_TIMEOUT 100
#define MAX_TRANSFER_TIMEOUT_ALTERNATIV_TASK 50
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
static volatile float focusReadAngleH   = 0;
static volatile float irisReadAngleH    = 0;
static volatile float panReadAngleL     = 0;
static volatile float dutchReadAngleL   = 0;
static volatile float tiltReadAngleL    = 0;
static volatile float zoomReadAngleL    = 0;
static volatile float focusReadAngleL   = 0;
static volatile float irisReadAngleL    = 0;
static volatile float versionFIZBox     = 0;


static volatile float panAngel;
static volatile float tiltAngel;
static volatile float dutchAngel;
static volatile float zoomAngel;
static volatile float panAngelGV;
static volatile float tiltAngelGV;
static volatile float dutchAngelGV;
static volatile float XCamComp;
static volatile float YCamComp;
static volatile float ZCamComp;


static volatile float panReadAngleGvH = 0;
static volatile float panReadAngleGvL = 0;
static volatile float dutchReadAngleGvH = 0;
static volatile float dutchReadAngleGvL = 0;
static volatile float tiltReadAngleGvH = 0;
static volatile float tiltReadAngleGvL = 0;
static GerconStatusBits statusBits3;

static volatile float statusGerconF = 0;

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
static const char* zoomJoystickPresetNames[JOYSTICK_FUNCS_COUNT] = {"Lin", "Sqr", "Cube",  "Eql"};
static JoyStickFunction* panFuncs[JOYSTICK_FUNCS_COUNT] =   {&panCubitorFunc,   &panQuadratorFunc,      &panLinearFunc,     &panLineralAproximator};
static JoyStickFunction* dutchFuncs[JOYSTICK_FUNCS_COUNT] = {&dutchCubitorFunc, &dutchQuadratorFunc,    &dutchLinearFunc,   &dutchLineralAproximator};
static JoyStickFunction* tiltFuncs[JOYSTICK_FUNCS_COUNT] =  {&tiltCubitorFunc,  &tiltQuadratorFunc,     &tiltLinearFunc,    &tiltLineralAproximator};
static JoyStickFunction* zoomFuncs[JOYSTICK_FUNCS_COUNT] =  {&zoomLinearFunc,   &zoomQuadratorFunc,     &zoomCubitorFunc,   &zoomLineralAproximator};


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
static PultButton virtualButtonJoysticOff, virtualZoomReversButton,virtualFocusReversButton,virtualIrisReversButton;

#define virtualButton 1

PultButton* virtualButtons[virtualButton] = {&virtualButtonJoysticOff

};

//Кнопки 1 - 16
#ifdef joyPult
static PultButton lensCalibrationButton,zoomReversButton,irisReversButton,focusReversButton,tb4;
#else
static PultButton lensCalibrationButton,rollWheelReversButton,tiltWheelReversButton,panWheelReversButton,tb4;
#endif


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
#ifdef joyPult
       &lensCalibrationButton,&zoomReversButton,&irisReversButton,&focusReversButton
#else
       &lensCalibrationButton,&rollWheelReversButton,&tiltWheelReversButton,&panWheelReversButton
#endif
};

//Кнопки 17 - 32
#define BUTTONS_17_32_COUNT 16
static PultButton gvAccButton(12), motionPlayButton(13), motionStopButton(14), motionDeleteButton(15), motionReversPlayButton(16), motionTrackSel1(17), motionTrackSel4(18), motionTrackSel2(19), motionTrackSel5(20), motionTrackSel3(21), motionTrackSel6(22), sa11(23), sa12(24), sa13(25), sa14(26), sa15(27);

static PultButton* buttons_17_32[BUTTONS_17_32_COUNT] = {&gvAccButton, &motionPlayButton, &motionStopButton, &motionDeleteButton, &motionReversPlayButton, &motionTrackSel1, &motionTrackSel4, &motionTrackSel2, &motionTrackSel5, &motionTrackSel3, &motionTrackSel6, &sa11, &sa12, &sa13, &sa14, &sa15};

//Кнопки 33-
#define BUTTONS_33_48_COUNT 16
static PultButton cameraStartButton(28), sa17(29), backlightButton, shaker, in37, in38, in39, in40, in41, in42, in43, in44, inp45, inp46, inp47;
static PultButton* buttons_33_48[BUTTONS_33_48_COUNT] = {&cameraStartButton, &sa17, &backlightButton, &shaker, &in37, &in38, &in39, &in40, &in41, &in42, &in43, &in44, &inp45, &inp46, &inp47,&tb4};

#ifdef USAEdition
static PultButton* sharedButtons[PULT_BUTTONS_COUNT] = {
        &motionPlayButton, &motionStopButton, &motionDeleteButton, &motionReversPlayButton, &motionTrackSel1, &motionTrackSel4, &motionTrackSel3, &motionTrackSel6, &motionTrackSel2, &motionTrackSel5, &sa11, &sa12, &sa13, &sa14, &sa15, &cameraStartButton, &sa17, &backlightButton//&inp47
};

static PultButton* motionButtons[MOTION_BUTTON_COUNT] = {
        &motionPlayButton, &motionStopButton, &motionDeleteButton, &motionReversPlayButton, &motionTrackSel1, &motionTrackSel2, &motionTrackSel3, &motionTrackSel4, &motionTrackSel5, &motionTrackSel6
};
#else
static PultButton* sharedButtons[PULT_BUTTONS_COUNT] = {
        &motionPlayButton, &motionStopButton, &motionDeleteButton, &motionReversPlayButton, &motionTrackSel1, &motionTrackSel4, &motionTrackSel2, &motionTrackSel5, &motionTrackSel3, &motionTrackSel6, &sa11, &sa12, &sa13, &sa14, &sa15, &cameraStartButton, &sa17, &backlightButton//&inp47
};

static PultButton* motionButtons[MOTION_BUTTON_COUNT] = {
        &motionPlayButton, &motionStopButton, &motionDeleteButton, &motionReversPlayButton, &motionTrackSel1, &motionTrackSel2, &motionTrackSel3, &motionTrackSel4, &motionTrackSel5, &motionTrackSel6
};
#endif
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

JoyChanel panExtern1Channel     (0.4,   panExtrenal1Offset,     &panChanelDeadResistor,     5,  0.015 );
JoyChanel dutchExtern1Channel   (0.4,   dutchExtrenal1Offset,   &dutchChanelDeadResistor,   5,  0.015 );
JoyChanel tiltExtern1Channel    (0.4,   tiltExtrenal1Offset,    &tiltChanelDeadResistor,    5,  0.015 );


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
        0.0000275, &zoomSpeedResistor,
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

ExtrenalDevices::PanBar panBar;

float fCLK = 500.0;
float fCLK1 = 1000.0;
float fCLK2 = 74.07;


Generation::Generator rndPanGenerator(0x0825,0.01,10.0,fCLK);
Generation::Generator rndTiltGenerator(0x0825,0.01,10.0,fCLK);
Generation::Generator rndRollPanGenerator(0x0825,0.01,10.0,fCLK);
Generation::Generator rndZoomGenerator(0x0825,0.01,10.0,fCLK);

ShakerChannel panInternalShakerChannel (10.0,PANSHAKERCHANNEL,0.01,15.0,fCLK);

ShakerChannel tiltInternalShakerChannel (10.0,TILTSHAKERCHANNEL,0.01,15.0,fCLK);

ShakerChannel rollInternalShakerChannel (10.0,ROLLSHAKERCHANNEL,0.01,15.0,fCLK);

ShakerChannel zoomInternalShakerChannel (14.0,ZOOMSHAKERCHANNEL,0.01,15.0,fCLK);

ShakerSinChannel panInternalSinShakerChannel;

ShakerSinChannel tiltInternalSinShakerChannel;

ShakerSinChannel rollInternalSinShakerChannel;

ShakerSinChannel zoomInternalSinShakerChannel;

volatile uint32_t counterMain,counterExchang, deltaCounter;

#ifdef Garanin

ExtrenalDevices::WheelChannel digitalWheelPanPasha (230.0,1.0,&panJoySpeedResistor);

ExtrenalDevices::WheelChannel digitalWheelRollPasha (230.0,1.0,&dutchJoySpeedResistor);

ExtrenalDevices::WheelChannel digitalWheelTiltPasha (230.0,1.0,&tiltJoySpeedResistor);

ExtrenalDevices::WheelChannel* digitalWheelChannelPasha[3] = { &digitalWheelPanPasha, &digitalWheelRollPasha, &digitalWheelTiltPasha };

#else

ExtrenalDevices::WheelChannel digitalWheelPan (230.0,1.0,&panJoySpeedResistor);

ExtrenalDevices::WheelChannel digitalWheelRoll (230.0,1.0,&dutchJoySpeedResistor);

ExtrenalDevices::WheelChannel digitalWheelTilt (230.0,1.0,&tiltJoySpeedResistor);

//ExtrenalDevices::WheelChannel digitalPedal (1.0,1.0,&dutchJoySpeedResistor);

ExtrenalDevices::WheelChannel* digitalWheelChannel[3] = { &digitalWheelPan, &digitalWheelRoll, &digitalWheelTilt };

#endif


static uint64_t panGenCounter=0x116249;

//---------------------------------------------------------------------
HallEffectJoyChannel dutchExtern2Channel(0.14,2110,&dutchJoySpeedResistor,100, 250,0.015);
HallEffectJoyChannel zoomPedalChannel(0.0006,2110,&zoomSpeedResistor,50, 250,0.015);

#ifdef Garanin

JoyChannelIF* panChannelsArray[4]=      {&panJoyChannel,    &panExtern1Channel, &digitalWheelPanPasha,&panInternalShakerChannel};
JoyChannelIF* dutchChannelsArray[5]=    {&dutchJoyChannel,  &dutchExtern2Channel,   &dutchExtern1Channel,&digitalWheelRollPasha,&rollInternalShakerChannel};
JoyChannelIF* tiltChannelsArray[4]=     {&tiltJoyChannel,   &tiltExtern1Channel,&digitalWheelTiltPasha,&tiltInternalShakerChannel};
JoyChannelIF* zoomChannelsArray[3]=     {&zoomJoyChannel,&zoomInternalShakerChannel,&zoomPedalChannel};

JoyChannels panChannals     (panChannelsArray,4);
JoyChannels dutchChannals   (dutchChannelsArray,5);
JoyChannels tiltChannals    (tiltChannelsArray,4);
JoyChannels zoomChannals    (zoomChannelsArray,3);

#else

#ifdef joyPult


JoyChannelIF* panChannelsArray[6]=      {&panJoyChannel,    &panExtern1Channel, &cartoniPanAxisChannel,
                                         &digitalWheelPan,&panInternalShakerChannel,
                                         panBar.getSpeedChannel(ExtrenalDevices::panSpeedChannel)};
JoyChannelIF* dutchChannelsArray[7]=    {&dutchJoyChannel,  &dutchExtern2Channel,   &dutchExtern1Channel,
                                         &cartoniDutchAxisChannel,&digitalWheelRoll,&rollInternalShakerChannel,
                                         panBar.getSpeedChannel(ExtrenalDevices::dutchSpeedChannel)};
JoyChannelIF* tiltChannelsArray[6]=     {&tiltJoyChannel,   &tiltExtern1Channel, &cartoniTiltAxisChannel,
                                         &digitalWheelTilt,&tiltInternalShakerChannel,
                                         panBar.getSpeedChannel(ExtrenalDevices::tiltSpeedChannel)};
JoyChannelIF* zoomChannelsArray[5]=     {&zoomJoyChannel, &cartoniZoomAxisChannel,&zoomInternalShakerChannel,
                                        panBar.getSpeedChannel(ExtrenalDevices::zoomSpeedChannel),&zoomPedalChannel};

JoyChannels panChannals     (panChannelsArray,6);
JoyChannels dutchChannals   (dutchChannelsArray,7);
JoyChannels tiltChannals    (tiltChannelsArray,6);
JoyChannels zoomChannals    (zoomChannelsArray,5);

    #else
    JoyChannelIF* panChannelsArray[4]=      {&panExtern1Channel, &cartoniPanAxisChannel,&digitalWheelPan,&panInternalShakerChannel};
    JoyChannelIF* dutchChannelsArray[5]=    {&dutchExtern2Channel,   &dutchExtern1Channel, &cartoniDutchAxisChannel,&digitalWheelRoll,&rollInternalShakerChannel};
    JoyChannelIF* tiltChannelsArray[4]=     {&tiltExtern1Channel, &cartoniTiltAxisChannel,&digitalWheelTilt,&tiltInternalShakerChannel};
    JoyChannelIF* zoomChannelsArray[2]=     { &cartoniZoomAxisChannel,&zoomInternalShakerChannel};

    JoyChannels panChannals     (panChannelsArray,4);
    JoyChannels dutchChannals   (dutchChannelsArray,5);
    JoyChannels tiltChannals    (tiltChannelsArray,4);
    JoyChannels zoomChannals    (zoomChannelsArray,2);
    #endif
#endif
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
        volatile UInt16 softRegim:1;//
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
        volatile UInt16         setLeftPanLimits            : 1;
        volatile UInt16         setRightPanLimits           : 1;
        volatile UInt16         resetLeftPanLimits          : 1;
        volatile UInt16         resetRightPanLimits         : 1;
        volatile UInt16         setLeftDutchLimits          : 1;
        volatile UInt16         setRightDutchLimits         : 1;
        volatile UInt16         resetLeftDutchLimits        : 1;
        volatile UInt16         resetRightDutchLimits       : 1;
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
       UInt16 GyConProgrammVersion:5;
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

static volatile bool setRightPanLimitsFlag    = 0;
static volatile bool setLeftPanLimitsFlag    = 0;
static volatile bool resetRightPanLimitsFlag  = 0;
static volatile bool resetLeftPanLimitsFlag  = 0;

static volatile bool setRightRollLimitsFlag    = 0;
static volatile bool setLeftRollLimitsFlag    = 0;
static volatile bool resetRightRollLimitsFlag  = 0;
static volatile bool resetLefttRollLimitsFlag  = 0;

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

PrestonBoard prestonEnableDriver;
ExtrenalDevices::Preston preston(&prestonEnableDriver);


Backlight backlight;



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
    ledControl.getDriver()->init();
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

    filesSystemAPI.initFS();
    motionControlAPI.init();

    #ifdef Garanin
    Var elementPan("Pan Speed:",&digitalWheelPanPasha.getSpeedWheel());
    Var elementTilt("Tilt Speed:",&digitalWheelTiltPasha.getSpeedWheel());
    Var elementDutch("Dutch Speed:",&digitalWheelRollPasha.getSpeedWheel());
    cartoniPanAxisChannel.setSpeed(digitalWheelPanPasha.getSpeedWheelRaw());
    cartoniDutchAxisChannel.setSpeed(digitalWheelRollPasha.getSpeedWheelRaw());
    cartoniTiltAxisChannel.setSpeed(digitalWheelTiltPasha.getSpeedWheelRaw());

    #else
        #ifdef PanBarDebug
            Var elementPan ("Pan Speed: ",&cartoniPanAxisChannel.getAxisVal());
            Var elementTilt ("Tilt Speed: ",&cartoniTiltAxisChannel.getAxisVal());
            Var elementDutch ("Roll Speed: ",&cartoniDutchAxisChannel.getAxisVal());
        #else
           // Var elementPan("Pan Speed:",panBar.getPanSpeed());
           // Var elementTilt("Tilt Speed:",panBar.getTiltSpeed());
            Var elementPan("Pan GV angel:",&panAngelGV);
            Var elementTilt("Tilt GV angel:",&tiltAngelGV);
            #ifdef USAEdition
                Var elementDutch("Roll Speed:",&digitalWheelRoll.getSpeedWheel());
            #else
                //Var elementDutch("Dutch Speed:",panBar.getRollSpeed());
                Var elementDutch("Dutch GV angel:",&dutchAngelGV);
            #endif
        #endif
    #endif

    viewLists.setVarList(0, &elementPan);
    viewLists.setVarList(1, &elementTilt);
    viewLists.setVarList(2, &elementDutch);

    Pwm backlightDriver  (GyConBoard_BrightPwm,10000);
    Pwm prestonDriver (GyConBoard_PrestonPWM,3);
    backlight.setButton(sharedButtons[backLightOff]);
    backlight.setDriver(&backlightDriver);
    preston.setDriver(&prestonDriver);


#ifdef WhellSmartPult
    panJoyChannel.disable();
    dutchJoyChannel.disable();
    tiltJoyChannel.disable();
    zoomJoyChannel.disable();
#endif
    while(true) {
        watchDogTimer.useKey(WD_KEY2);
//Обработка аналоговых сигналов
        UInt32* result = signalsReader.read();
#ifdef joyPult
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
#endif
 //       counterMain++;
 //       rndGenerator.calcFilter();
        panInternalShakerChannel.setInputeValue(rndPanGenerator.getOutValue());
        tiltInternalShakerChannel.setInputeValue(rndPanGenerator.getOutValue());
        rollInternalShakerChannel.setInputeValue(rndPanGenerator.getOutValue());
        zoomInternalShakerChannel.setInputeValue(rndPanGenerator.getOutValue());
        panExtern1Channel.setRef(result[SIGNAL_PAN_WHEEL]);
        tiltExtern1Channel.setRef(result[SIGNAL_TILT_WHEEL]);
        cartoniPanAxisChannel.setData();
        cartoniTiltAxisChannel.setData();
        cartoniDutchAxisChannel.setData();


#ifdef Garanin
#else
        cartoniZoomAxisChannel.setData();
#endif
        cartoniFocusAxisChannel.setData();
        cartoniIrisAxisChannel.setData();
        //запихиваем суда объектив для отлаки

        UInt8 muxPos = signalsReader.getMultiplexer();
        if(muxPos==14){
            dutchExtern2Channel.setRef(result[SIGNAL_SLOW]);
            zoomPedalChannel.setRef(result[SIGNAL_SLOW]);}
        if(muxPos==13){ dutchExtern1Channel.setRef(result[SIGNAL_SLOW]);}

        panJoy.calculate();
        dutchJoy.calculate();
        tiltJoy.calculate();
        zoomJoy.calculate();
        cartoniFocusAxisChannel.calculate(0.0);
        cartoniIrisAxisChannel.calculate(0.0);

        resistors[muxPos]->calculate(result[SIGNAL_SLOW]);

        backlight.update();
        preston.update(zoomJoy.getValue());

        muxPos++;
        if (muxPos > 14) muxPos = 0;
        if (muxPos == muxPosRef) {
            muxPos = muxPosRef;
        }
        signalsReader.setMultiplexer(muxPos);

      /*signalsReader.prestonOn(state);
        signalsReader.cameraStartFront(state);
        signalsReader.cameraStartLevel(state);*/
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

#define PULT_COMMANDS_COUNT 7

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
//      {&panSuspentionResonantFreq, IQ10},
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
        {&focusReadAngleH, IQ0},
        {&focusReadAngleL, IQ15 },
        {&irisReadAngleH, IQ0},
        {&irisReadAngleL, IQ15},
        {&versionFIZBox,IQ0},
        NULL
};


#define ASK_BASIC_CMD_DATA_COUNT 3
static BasicCmdValue askBasicCmdData[] = {
        {&panTurns,                 IQ12},
        {&gyConFaultBitsExchange,   IQ0},
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

static BasicCmdValue askReadGvAnglesAndOther[] = {
    {&panReadAngleH, IQ0},
    {&panReadAngleL, IQ15},
    {&dutchReadAngleH, IQ0},
    {&dutchReadAngleL, IQ15},
    {&tiltReadAngleH,  IQ0},
    {&tiltReadAngleL,  IQ15},
    {&panReadAngleGvH, IQ0},
    {&panReadAngleGvL, IQ15},
    {&dutchReadAngleGvH, IQ0},
    {&dutchReadAngleGvL, IQ15},
    {&tiltReadAngleGvH,  IQ0},
    {&tiltReadAngleGvL,  IQ15},
    {&statusGerconF,   IQ0},
    NULL
};

static BasicCmd commands[] = {
        {writeControlCmdData, WRITE_CONTROL_CMD_DATA_COUNT},
        {askBasicCmdData, ASK_BASIC_CMD_DATA_COUNT},
        {writeDebugVarsCmdData, AXIS_SETTINGS_COUNT},
        {writeAnglesCmdData, 0},
        {askAnglesCmdData, 0},
        {askAnglesCmdDataZIF, 0},
        {askReadGvAnglesAndOther, 0}
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
        if (minuteCounter>1000) {
            timeToStart++;
            minuteCounter=0;
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

    if(cartoniFocusAxisChannel.isActive())
    {
        zifParams.focusRef=cartoniFocusAxisChannel.value;
    }
    else
    {
#ifdef myPanBar
    if ((panBar.isEnable_())&&(panBar.isConnect()))
    {
        if (panBar.getButton().focusEnableButtonEnable())
            zifParams.focusRef = panBar.getFocusPosition();
        else
            zifParams.focusRef=focusResistor.value;
    }
    else
        zifParams.focusRef=focusResistor.value;
#else
    zifParams.focusRef=focusResistor.value;
#endif
    }
    if(cartoniIrisAxisChannel.isActive())   {
        zifParams.irisRef=cartoniIrisAxisChannel.value;
    }
    else
    {
#ifdef myPanBar
    if ((panBar.isEnable_())&&(panBar.isConnect()))
        {
        if (panBar.getButton().irisEnableBtnEnable())
            zifParams.irisRef = panBar.getIrisPosition();
        else
            zifParams.irisRef=IrisResistor.value;
         }
         else
            zifParams.irisRef=IrisResistor.value;

#else
        zifParams.irisRef=IrisResistor.value;
#endif
    }

#ifdef myPanBar
    if ((panBar.isEnable_())&&(panBar.isConnect()))
    {
        if (panBar.getButton().focusEnableButtonEnable())
            zifParams.focusRef = panBar.getFocusPosition();
        zifParams.irisRef = panBar.getIrisPosition();
    }
#endif

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
                 axisRef.panRef=    panJoy.getValue(    motionControlAPI.getCurrentPoint()->panRef+panJoy.getRawValue());
                 axisRef.dutchRef=  dutchJoy.getValue(  motionControlAPI.getCurrentPoint()->dutchRef+dutchJoy.getRawValue());
                 axisRef.tiltRef=   tiltJoy.getValue(   motionControlAPI.getCurrentPoint()->tiltRef+tiltJoy.getRawValue());
                 zifParams.zoomRef= zoomJoy.getValue(   motionControlAPI.getCurrentPoint()->zoomRef+zoomJoy.getRawValue());

                 zifParams.irisRef=motionControlAPI.getCurrentPoint()->irisRef;;
                 zifParams.focusRef=motionControlAPI.getCurrentPoint()->focusRef;

                 speedLevelCorrectionExchange=motionControlAPI.getCurrentPoint()->levelCorrectionSpeed;
                 contBitsTempMotion.all=(motionControlAPI.getCurrentPoint()->controlBits&0x0000FFFF);
                 contBitsTemp.bit.softRegim=contBitsTempMotion.bit.softRegim;
                 contBitsTemp.bit.levelCorrect=contBitsTempMotion.bit.levelCorrect;
                 contBitsTemp.bit.levelSetup=contBitsTempMotion.bit.levelSetup;
                 controlBitsExchange=contBitsTemp.all;

                 outputPoleCntrl=motionControlAPI.getCurrentPoint()->poleRef+inputPoleCntrl;
                 break;
             case MOTION_MIX_ALL:
                 axisRef.panRef=    panJoy.getValue(    motionControlAPI.getCurrentPoint()->panRef+panJoy.getRawValue());
                 axisRef.dutchRef=  dutchJoy.getValue(  motionControlAPI.getCurrentPoint()->dutchRef+dutchJoy.getRawValue());
                 axisRef.tiltRef=   tiltJoy.getValue(   motionControlAPI.getCurrentPoint()->tiltRef+tiltJoy.getRawValue());
                 zifParams.zoomRef= zoomJoy.getValue(   motionControlAPI.getCurrentPoint()->zoomRef+zoomJoy.getRawValue());

                 speedLevelCorrectionExchange=motionControlAPI.getCurrentPoint()->levelCorrectionSpeed;
                 contBitsTempMotion.all=(motionControlAPI.getCurrentPoint()->controlBits&0x0000FFFF);
                 contBitsTemp.bit.softRegim=contBitsTempMotion.bit.softRegim;
                 contBitsTemp.bit.levelCorrect=contBitsTempMotion.bit.levelCorrect;
                 contBitsTemp.bit.levelSetup=contBitsTempMotion.bit.levelSetup;
                 controlBitsExchange=contBitsTemp.all;

                 outputPoleCntrl=motionControlAPI.getCurrentPoint()->poleRef+inputPoleCntrl;
                 break;
             case MOTION_MIX_TRACK_ONLY:
             default:
                 axisRef.panRef=    panJoy.getValue(    motionControlAPI.getCurrentPoint()->panRef);
                 axisRef.dutchRef=  dutchJoy.getValue(  motionControlAPI.getCurrentPoint()->dutchRef);
                 axisRef.tiltRef=   tiltJoy.getValue(   motionControlAPI.getCurrentPoint()->tiltRef);
                 zifParams.zoomRef= zoomJoy.getValue(   motionControlAPI.getCurrentPoint()->zoomRef);

                 zifParams.irisRef=motionControlAPI.getCurrentPoint()->irisRef;;
                 zifParams.focusRef=motionControlAPI.getCurrentPoint()->focusRef;

                 speedLevelCorrectionExchange=motionControlAPI.getCurrentPoint()->levelCorrectionSpeed;
                 contBitsTempMotion.all=(motionControlAPI.getCurrentPoint()->controlBits&0x0000FFFF);
                 contBitsTemp.bit.softRegim=contBitsTempMotion.bit.softRegim;
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
    watchDogTimer.registerKey(WD_KEY4);
#ifdef Garanin

    while(true)
        {
        watchDogTimer.useKey(WD_KEY4);
        Task_sleep(100);
        }
#else

    BasicProtocolParams params (
            3,
            &cmdListAlt,
            Board_PULT_ALT_UART,
            MAX_TRANSFER_TIMEOUT_ALTERNATIV_TASK,
            Board_PULTALT_RS485RW

    );
    GPIO_write(Board_PULTALT_RS485RW, Board_RS485_WRITE_MODE);
    ProtocolWheel::WheelProtocol protokol;
    ExtrenalDevices::DataOut panData;
    ExtrenalDevices::DataOut tiltData;
    ExtrenalDevices::DataOut rollData;
//    ExtrenalDevices::DataOut pedalData;
    ExtrenalDevices::DataOut panBarData;
    LensDb::LensPackStatic<256> packRx;
    LensDb::LensPackStatic<256> packTx;
    int32_t panWheelData, tiltWheelData,rollWheelData;
    panData.dataInput=&panWheelData;
    tiltData.dataInput=&tiltWheelData;
    rollData.dataInput=&rollWheelData;
    JoyChannelIF* cnannel[5];
    cnannel[0]=&digitalWheelPan;
    cnannel[1]=&digitalWheelTilt;
    cnannel[2]=&digitalWheelRoll;
    cnannel[3]=&panBar;
    int32_t panBarDataBuf[7];
    int32_t pedalDat;
    panBarData.dataInput=&panBarDataBuf[0];
 //   pedalData.dataInput = &pedalDat;
    Rs485Driver2 driverWhell(params.uartId, 115200, params.recieveTimeout, params.txEnablePin);
    ExtrenalDevices::DigitalWheelManager digitalWheelManager(driverWhell,protokol,packRx,packTx);
    while(true)
    {
        watchDogTimer.useKey(WD_KEY4);
        bool exchange =false;
        if (digitalWheelPan.isEnable_())
        {
            digitalWheelPan.updateConnect(digitalWheelManager.exchenge(ProtocolWheel::wheelPan,ProtocolWheel::wheelSpeedRequest,panData));
            if (!digitalWheelPan.isConnect())
                panWheelData=0;
            digitalWheelPan.getSpeedWheelRaw()[0]=panWheelData;
            exchange = true;
        }

        if (digitalWheelTilt.isEnable_())
        {
            digitalWheelTilt.updateConnect(digitalWheelManager.exchenge(ProtocolWheel::wheelTilt ,ProtocolWheel::wheelSpeedRequest,tiltData));
            if (!digitalWheelTilt.isConnect())
                tiltWheelData=0;
            digitalWheelTilt.getSpeedWheelRaw()[0]=tiltWheelData;
            exchange = true;
        }

        if (digitalWheelRoll.isEnable_())
        {
            digitalWheelRoll.updateConnect(digitalWheelManager.exchenge(ProtocolWheel::wheelRoll,ProtocolWheel::wheelSpeedRequest,rollData));
            if (!digitalWheelRoll.isConnect())
                rollWheelData=0;
            digitalWheelRoll.getSpeedWheelRaw()[0]=rollWheelData;
            exchange = true;
        }

        //if(digitalPedal.isEnable_())
//        digitalWheelManager.exchenge(ProtocolWheel::pedal, ProtocolWheel::wheelSpeedRequest, pedalData);

        if (panBar.isEnable_())
        {
            panBar.updateConnect(digitalWheelManager.exchenge(ProtocolWheel::panBar,ProtocolWheel::wheelSpeedRequest,panBarData));
            if (!panBar.isConnect()) {
                for (uint8_t i=0;i<7;i++)
                {
                    panBarDataBuf[i]=0;
                }
            }
            panBar.setInputValue(&panBarDataBuf[0]);
            exchange = true;
        }
        if (!exchange)
            Task_sleep(5);
        else
            Task_sleep(5);
    }
#endif
}

static volatile bool gtaComplite=false;

static const uint8_t commandAskCikl[3] = {1,6,5};
static uint8_t askID=0;
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
        rndPanGenerator.generate();
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


            panWriteAngleH=     getAngleH(motionControlAPI.getAnglesData()->panRef);
            dutchWriteAngleH=   getAngleH(motionControlAPI.getAnglesData()->dutchRef);
            tiltWriteAngleH=    getAngleH(motionControlAPI.getAnglesData()->tiltRef);
            zoomWriteAngleH=    getAngleH(motionControlAPI.getAnglesData()->zoomRef);

            panWriteAngleL=     getAngleL(motionControlAPI.getAnglesData()->panRef);
            dutchWriteAngleL=   getAngleL(motionControlAPI.getAnglesData()->dutchRef);
            tiltWriteAngleL=    getAngleL(motionControlAPI.getAnglesData()->tiltRef);
            zoomWriteAngleL=    getAngleL(motionControlAPI.getAnglesData()->zoomRef);

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

        if(gyConFaultBits.stateBits.goToAngleComplete==1){
            gtaComplite=true;}
        else{   gtaComplite=false;  }


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


//      Wait synchronization
        if(motionControlAPI.isActive())
        {
            if(!Semaphore_pend( *exchangeSem,BIOS_WAIT_FOREVER))
            {
            uint32_t a=10;
            a++;
            //TODO Критическая ошибка надо обработать
            }
        }
//TODO могут быть нюансы при отсуствии ОС,
//система может бесконечно пытаться передать альтернативную комманду
 /*       if (dutchLevelSetupButton.state!=RELESASED)
        {

        }
        else */if (protocol.transaction(PULT_EXCHANGE_ADDR, BASIC_PROTOCOL_BROADCAST_ADDRESS))
        {
            if (protocol.askCmdId == 6)
            {
                panAngel = getAngle(panReadAngleH,panReadAngleL);
                tiltAngel = getAngle(tiltReadAngleH,tiltReadAngleL);
                dutchAngel = getAngle(dutchReadAngleH,dutchReadAngleL);
                panAngelGV = getAngle(panReadAngleGvH,panReadAngleGvL);
                tiltAngelGV = getAngle(tiltReadAngleGvH,tiltReadAngleGvL);
                dutchAngelGV = getAngle(dutchReadAngleGvH,dutchReadAngleGvL);
                statusBits3.all =statusGerconF;
                ExtrSyncroization::ExtrenalDevieExchDriver::dataDopReal.setPanAngel(panAngelGV);
                ExtrSyncroization::ExtrenalDevieExchDriver::dataDopReal.setTiltAngel(tiltAngelGV);
                ExtrSyncroization::ExtrenalDevieExchDriver::dataDopReal.setRollAngel(dutchAngelGV);
            }
            if(protocol.askCmdId == 4)
            {
                motionControlAPI.getAnglesData()->panRef=   getAngle(panReadAngleH,panReadAngleL);
                motionControlAPI.getAnglesData()->dutchRef= getAngle(dutchReadAngleH,dutchReadAngleL);
                motionControlAPI.getAnglesData()->tiltRef=  getAngle(tiltReadAngleH,tiltReadAngleL);
                *motionControlAPI.getAnglesFlag()=MOTION_ANGLE_READY;
            }

            if(protocol.askCmdId == 5)
            {
                motionControlAPI.getAnglesData()->zoomRef=  getAngle(zoomReadAngleH,zoomReadAngleL);
                float zoomAngelAbs = getAngle(zoomReadAngleH,zoomReadAngleL);
                zoomAngelAbs+=1.0;
                zoomAngelAbs*=0.5;
                zoomAngel=zoomAngelAbs;
                ExtrSyncroization::ExtrenalDevieExchDriver::dataDopReal.setZoomAngel(getAngle(zoomReadAngleH,zoomReadAngleL));
                ExtrSyncroization::ExtrenalDevieExchDriver::dataDopReal.setFocusAngel(getAngle(focusReadAngleH,focusReadAngleL));
                ExtrSyncroization::ExtrenalDevieExchDriver::dataDopReal.setIrisAngel(getAngle(irisReadAngleH,irisReadAngleL));
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
            if (noBasicWriteCmdCounter)
                noBasicWriteCmdCounter--;
            gyConFaultBits.all = gyConFaultBitsExchange;
            okCounter++;
        }
        else
            {
            faultCounter++;
            }
        protocol.askCmdId = commandAskCikl[askID];
        askID++;
        uint8_t commandCounter;
#ifdef joyPult
        if (preston.getEnable())
            commandCounter=1;
        else
            commandCounter=2;
#else
        commandCounter=1;
#endif
        if (askID>commandCounter)
            askID=0;

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
#ifdef joyPult
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
#else
//  &rollWheelReversButton,&tiltWheelReversButton,&panWheelReversButton
    if(panReversButton.isPressed()) {
        panJoy.enable();
        panJoy.setNormalOrientation();
        }
    else if (!panReversButton.isPressed()&&!panWheelReversButton.isPressed()) {
        panJoy.disable();
        }

    else {
        panJoy.enable();
        panJoy.setInversOrientation();
        }

    if(dutchReversButton.isPressed()) {
        dutchJoy.enable();
        dutchJoy.setNormalOrientation();
        }
    else if (!dutchReversButton.isPressed()&&!rollWheelReversButton.isPressed()) {
        dutchJoy.disable();
        }

    else {
        dutchJoy.enable();
        dutchJoy.setInversOrientation();
        }

    if(tiltReversButton.isPressed()) {
        tiltJoy.enable();
        tiltJoy.setNormalOrientation();
        }
    else if (!tiltReversButton.isPressed()&&!tiltWheelReversButton.isPressed()) {
        tiltJoy.disable();
        }

    else {
        tiltJoy.enable();
        tiltJoy.setInversOrientation();
        }

#endif
    //ZIF REVERS
#ifdef joyPultRussian
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
#else
    switch (virtualZoomReversButton.state)
    {
        case PRESSED:
            zoomJoy.setInversOrientation();
            break;
        case RELESASED:
            zoomJoy.setNormalOrientation();
            break;
    }
    switch (virtualIrisReversButton.state)
    {
        case PRESSED:
            IrisResistor.setInversOrientation();
            break;
        case RELESASED:
            IrisResistor.setNormalOrientation();
            break;
    }
    switch (virtualFocusReversButton.state)
    {
        case PRESSED:
            focusResistor.setInversOrientation();
            break;
        case RELESASED:
            focusResistor.setNormalOrientation();
            break;
    }
#endif
}


static void joySticksOnOffLogic() {
#ifdef joyPult
    switch (joyStickOnOffButton.state) {
    case PRESSED:
        panJoyChannel.enable();
        dutchJoyChannel.enable();
        tiltJoyChannel.enable();
        zoomJoyChannel.enable();
        ledControl.getData()->resetLed(LED_JOYSTIC);
        ledControl.invalidate();
        controlBits.bit.joysticOn=1;
        break;
    case RELESASED:
        panJoyChannel.disable();
        dutchJoyChannel.disable();
        tiltJoyChannel.disable();
        zoomJoyChannel.disable();
        ledControl.getData()->setLed(LED_JOYSTIC);
        ledControl.invalidate();
        controlBits.bit.joysticOn=0;
        break;
        }
#else
    switch (joyStickOnOffButton.state) {
    case PRESSED:
        digitalWheelPan.enable();
        digitalWheelRoll.enable();
        digitalWheelTilt.enable();
        ledControl.getData()->resetLed(LED_JOYSTIC);
        ledControl.invalidate();
        controlBits.bit.joysticOn=1;
        break;
    case RELESASED:
        digitalWheelPan.disable();
        digitalWheelRoll.disable();
        digitalWheelTilt.disable();
        ledControl.getData()->setLed(LED_JOYSTIC);
        ledControl.invalidate();
        controlBits.bit.joysticOn=0;
        break;
        }
#endif
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
//      autonegComplete=false;
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

//      if(transferDelay>maxTransferDelay)
//      {
//          maxTransferDelay=transferDelay;
//      }

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

static bool shakerEnable=false;

static void controlLogic() {
    static UInt8 setUpTiltLimitsCounter = 0;
    static UInt8 setDwTiltLimitsCounter = 0;
    static UInt8 resetUpTiltLimitsCounter = 0;
    static UInt8 resetDwTiltLimitsCounter = 0;

    static UInt8 setLeftPanLimitsCounter = 0;
    static UInt8 setRightPanLimitsCounter = 0;
    static UInt8 resetLeftPanLimitsCounter = 0;
    static UInt8 resetRightPanLimitsCounter = 0;

    static UInt8 setLeftRollLimitsCounter = 0;
    static UInt8 setRightRollLimitsCounter = 0;
    static UInt8 resetLeftRollLimitsCounter = 0;
    static UInt8 resetRightRollLimitsCounter = 0;

    static UInt8 lensCalibrationCounter = 0;
    static UInt8 estimationBitCounter=0;

    if ((panBar.isEnable_())&&(panBar.isConnect()))
    {
        controlBits.bit.onOffMotors = (bool)panBar.getButton().motorsBtnIsEnable();
    }
    else
        controlBits.bit.onOffMotors = motorOnOffButton.isPressed();
    if (controlBits.bit.onOffMotors) {
        ledControl.getData()->setLed(LED_MOTOR);
        ledControl.invalidate();
        }
    else {
        ledControl.getData()->resetLed(LED_MOTOR);
        ledControl.invalidate();
        }
    controlBits.bit.levelCorrect = levelCorrectButton.isPressed();
    controlBits.bit.levelSetup = dutchLevelSetupButton.isPressed();
    controlBits.bit.gvCalibration = gvCalibrationButton.isPressed();
 //   controlBits.bit.fastLevelCorrect = fastLevelCorrectButton.isPressed();

    if (fastLevelCorrectButton.isClicked()){
        if (shakerEnable){
            panInternalShakerChannel.disable();
            tiltInternalShakerChannel.disable();
            rollInternalShakerChannel.disable();
            zoomInternalShakerChannel.disable();
            shakerEnable=false;
       }
        else{
            panInternalShakerChannel.enable();
            tiltInternalShakerChannel.enable();
            rollInternalShakerChannel.enable();
            zoomInternalShakerChannel.enable();
            shakerEnable=true;
       }
    }

    if (setUpTiltLimitsFlag == true) {setUpTiltLimitsCounter = 10; setUpTiltLimitsFlag = false;};
    if (setDwTiltLimitsFlag == true) {setDwTiltLimitsCounter = 10; setDwTiltLimitsFlag = false;};
    if (resetUpTiltLimitsFlag == true) {resetUpTiltLimitsCounter = 10; resetUpTiltLimitsFlag = false;};
    if (resetDwTiltLimitsFlag == true) {resetDwTiltLimitsCounter = 10; resetDwTiltLimitsFlag = false;};

    if (setRightPanLimitsFlag == true) {setRightPanLimitsCounter = 10; setRightPanLimitsFlag = false;};
    if (setLeftPanLimitsFlag == true) {setLeftPanLimitsCounter = 10; setLeftPanLimitsFlag = false;};
    if (resetRightPanLimitsFlag == true) {resetRightPanLimitsCounter = 10; resetRightPanLimitsFlag = false;};
    if (resetLeftPanLimitsFlag == true) {resetLeftPanLimitsCounter = 10; resetLeftPanLimitsFlag = false;};

    if (setRightRollLimitsFlag == true) {setRightRollLimitsCounter = 10; setRightRollLimitsFlag = false;};
    if (setLeftRollLimitsFlag == true) {setLeftRollLimitsCounter = 10; setLeftRollLimitsFlag = false;};
    if (resetRightRollLimitsFlag == true) {resetRightRollLimitsCounter = 10; resetRightRollLimitsFlag = false;};
    if (resetLefttRollLimitsFlag == true) {resetLeftRollLimitsCounter = 10; resetLefttRollLimitsFlag = false;};

    #ifdef USAEdition || WhellSmartPult
    if (pult.lensCalibrtionClicked()){lensCalibrationCounter = 10;}
#else
    if (lensCalibrationButton.isClicked()||pult.lensCalibrtionClicked()){lensCalibrationCounter = 10;}
#endif
    if (estimationFlag==true){estimationBitCounter=10;estimationFlag=false;}

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

    if (setLeftPanLimitsCounter) {
        setLeftPanLimitsCounter--;
        controlBitsA.bit.setLeftPanLimits = true;
    } else
        controlBitsA.bit.setLeftPanLimits = false;

    if (setRightPanLimitsCounter) {
        setRightPanLimitsCounter--;
        controlBitsA.bit.setRightPanLimits = true;
    } else
        controlBitsA.bit.setRightPanLimits = false;

    if (resetLeftPanLimitsCounter) {
        resetLeftPanLimitsCounter--;
        controlBitsA.bit.resetLeftPanLimits = true;
    } else
        controlBitsA.bit.resetLeftPanLimits = false;

    if (resetRightPanLimitsCounter) {
        resetRightPanLimitsCounter--;
        controlBitsA.bit.resetRightPanLimits = true;
    } else
        controlBitsA.bit.resetRightPanLimits = false;

    if (setLeftRollLimitsCounter) {
        setLeftRollLimitsCounter--;
        controlBitsA.bit.setLeftDutchLimits = true;
    } else
        controlBitsA.bit.setLeftDutchLimits = false;

    if (setRightRollLimitsCounter) {
        setRightRollLimitsCounter--;
        controlBitsA.bit.setRightDutchLimits = true;
    } else
        controlBitsA.bit.setRightDutchLimits = false;

    if (resetLeftRollLimitsCounter) {
        resetLeftRollLimitsCounter--;
        controlBitsA.bit.resetLeftDutchLimits = true;
    } else
        controlBitsA.bit.resetLeftDutchLimits = false;

    if (resetRightRollLimitsCounter) {
        resetRightRollLimitsCounter--;
        controlBitsA.bit.resetRightDutchLimits = true;
    } else
        controlBitsA.bit.resetRightDutchLimits = false;

#ifdef joyPult
    if (gvAccButton.isClicked()) {
        controlBits.bit.gvAcc = !controlBits.bit.gvAcc;
    }
#else
    if (gvAccButton.isPressed())
        controlBits.bit.gvAcc = 1;
    else
        controlBits.bit.gvAcc = 0;
#endif
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
    return controlBits.bit.onOffMotors;
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

void Pult::setTiltUpLimit() {   setUpTiltLimitsFlag = true;}

void Pult::setTiltDnLimit() {   setDwTiltLimitsFlag = true;}

void Pult::resetTiltUpLimit() { resetUpTiltLimitsFlag = true;}

void Pult::resetTiltDnLimit() { resetDwTiltLimitsFlag = true;}

void Pult::setLeftPanLimit() {   setLeftPanLimitsFlag = true;}

void Pult::setRightPanLimit() {   setRightPanLimitsFlag = true;}

void Pult::resetLeftPanLimit() { resetLeftPanLimitsFlag = true;}

void Pult::resetRightPanLimit() { resetRightPanLimitsFlag = true;}

void Pult::setLeftRollLimit() {   setLeftRollLimitsFlag = true;}

void Pult::setRightRollLimit() {   setRightRollLimitsFlag = true;}

void Pult::resetLeftRollLimit() { resetLefttRollLimitsFlag = true;}

void Pult::resetRightRollLimit() { resetRightRollLimitsFlag = true;}

void Pult::setPlatform(UInt8 num) {
/*  if (num<4) {
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

bool Pult::getGVEnable()    {   return controlBits.bit.levelCorrect; }

float Pult::getPanAngel()   {   return panAngelGV;           }

float Pult::getTiltAngel()   {   return tiltAngelGV;           }

float Pult::getRollAngel()   {   return dutchAngelGV;           }

float Pult::getZoomAngel()   {   return zoomAngel;           }


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

Warning headDisconnectedWarning("Head disconnected!",   WT_WARNING);
Warning pDusFail("Pan ARS Fail",                        WT_WARNING);
Warning dDusFail("Dutch ARS Fail",                      WT_WARNING);
Warning tDusFail("Tilt ARS Fail",                       WT_WARNING);
Warning gvFail("GV Fail",                               WT_WARNING);
Warning encoderFail("Encoder Fail",                     WT_WARNING);
Warning pultFail("Pult Fail",                           WT_WARNING);
Warning pultGVCalibrat ("Vertical gyro calibration", WT_WARNING);
Warning panWheelDisconnect ("Pan wheel disconnect", WT_WARNING);
Warning tiltWheelDisconnect ("Tilt wheel disconnect", WT_WARNING);
#ifdef USAEdition
Warning rollWheelDisconnect ("Roll wheel disconnect", WT_WARNING);
#else
Warning rollWheelDisconnect ("Dutch wheel disconnect", WT_WARNING);
#endif
#ifdef myPanBar
Warning panBarDisconnect ("Pan bar disconnect", WT_WARNING);
Warning panBarError ("Pan bar has error", WT_WARNING);
#endif

void Pult::updateWarningsList()
{
    warnings.getRunStrWarnings()->removeAll();

    if(!connectedFlag){
        warnings.getRunStrWarnings()->add(&headDisconnectedWarning);}
    if (gyConFaultBits.all!=0) {
        int a=0;
        a++;
        }
    if(gyConFaultBits.faultBits.panDusFault!=0){    warnings.getRunStrWarnings()->add(&pDusFail);}
    if(gyConFaultBits.faultBits.dutchDusFault!=0){  warnings.getRunStrWarnings()->add(&dDusFail);}
    if(gyConFaultBits.faultBits.tiltDusFault!=0){   warnings.getRunStrWarnings()->add(&tDusFail);}
    if(gyConFaultBits.faultBits.gvFault!=0){    warnings.getRunStrWarnings()->add(&gvFail);}
    if(gyConFaultBits.faultBits.encodersFault!=0){  warnings.getRunStrWarnings()->add(&encoderFail);}
    if(gyConFaultBits.faultBits.pultFault!=0){  warnings.getRunStrWarnings()->add(&pultFail);}
    //здесь дописываем индикацию калибровки ГВ
    if (controlBits.bit.gvCalibration) {warnings.getRunStrWarnings()->add(&pultGVCalibrat);}
#ifndef Garanin
    if ((digitalWheelPan.isEnable_())&&(!digitalWheelPan.isConnect()))   {warnings.getRunStrWarnings()->add(&panWheelDisconnect);}
    if ((digitalWheelTilt.isEnable_())&&(!digitalWheelTilt.isConnect()))   {warnings.getRunStrWarnings()->add(&tiltWheelDisconnect);}
    if ((digitalWheelRoll.isEnable_())&&(!digitalWheelRoll.isConnect()))   {warnings.getRunStrWarnings()->add(&rollWheelDisconnect);}
#endif
#ifdef myPanBar
    if ((panBar.isEnable_())&&(!panBar.isConnect()))   {warnings.getRunStrWarnings()->add(&panBarDisconnect);}
    if ((panBar.isEnable_())&&(panBar.isConnect())&&(panBar.isError())) {   warnings.getRunStrWarnings()->add(&panBarError);}
#endif
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
            if (presetId >= JOYSTICK_FUNCS_COUNT)   {panJoy.func = panFuncs[0];             panSensId=0;}
            else                                    {panJoy.func = panFuncs[presetId];      panSensId=presetId;}
            return;
        case DutchJoystickPreset:
            if (presetId >= JOYSTICK_FUNCS_COUNT)   {dutchJoy.func = dutchFuncs[0];         dutchSensId=0;}
            else                                    {dutchJoy.func = dutchFuncs[presetId];  dutchSensId=presetId;}
            return;
        case TiltJoystickPreset:
            if (presetId >= JOYSTICK_FUNCS_COUNT)   {tiltJoy.func = tiltFuncs[0];           tiltSensId=0;}
            else                                    {tiltJoy.func = tiltFuncs[presetId];    tiltSensId=presetId;}
            return;
        case ZoomJoystickPreset:
            if (presetId >= JOYSTICK_FUNCS_COUNT)   {zoomJoy.func = zoomFuncs[0];           zoomSensId=0;}
            else                                    {zoomJoy.func = zoomFuncs[presetId];    zoomSensId=presetId;}
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
//  maxTransferDelay=0;
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
void Pult::setPanWheelSpeed(float val)
{
    panChanelDeadResistor.setValue(val);

}
void Pult::setTiltWheelSpeed(float val)
{
    tiltChanelDeadResistor.setValue(val);
}
void Pult::setDutchWheelSpeed(float val)
{
    dutchChanelDeadResistor.setValue(val);
}

void Pult::enablePanAnalogWheel()     {  panExtern1Channel.enable();      }

void Pult::enableTiltAnalogWheel()    {  tiltExtern1Channel.enable();     }

void Pult::enableRollAnalogWheel()    {  dutchExtern1Channel.enable();    }

void Pult::enableDutchPadal()        {  dutchExtern2Channel.enable();
                                        zoomPedalChannel.disable();}

void Pult::enableZoomPadal()        {  zoomPedalChannel.enable();
                                        dutchExtern2Channel.disable();}

void Pult::disablePanAnalogWheel()    {  panExtern1Channel.disable();     }

void Pult::disableTiltAnalogWheel()   {  tiltExtern1Channel.disable();    }

void Pult::disableRollAnalogWheel()   {  dutchExtern1Channel.disable();   }

void Pult::disablePadal()       {
    dutchExtern2Channel.disable();
    zoomPedalChannel.disable();}

void Pult::digitalWheelPanStepLeft() {
    #ifdef Garanin
    digitalWheelPanPasha.channal.stepLeft();
    #else
    digitalWheelPan.channal.stepLeft();
    #endif
    }

void Pult::digitalWheelTiltStepLeft(){
     #ifdef Garanin
     digitalWheelTiltPasha.channal.stepLeft();
     #else
     digitalWheelTilt.channal.stepLeft();
     #endif
     }

void Pult::digitalWheelRollStepLeft(){
    #ifdef Garanin
    digitalWheelRollPasha.channal.stepLeft();
    #else
    digitalWheelRoll.channal.stepLeft();
    #endif
    }

void Pult::digitalWheelPanStepRight(){
#ifdef Garanin
    digitalWheelPanPasha.channal.stepRight();
#else
    digitalWheelPan.channal.stepRight();
#endif
}

void Pult::digitalWheelTiltStepRight(){
#ifdef Garanin
    digitalWheelTiltPasha.channal.stepRight();
#else
    digitalWheelTilt.channal.stepRight();
#endif
    }

void Pult::digitalWheelRollStepRight(){
#ifdef Garanin
    digitalWheelRollPasha.channal.stepRight();
#else
    digitalWheelRoll.channal.stepRight();
#endif
    }

int8_t Pult:: digitalWheelPanGetFunction() {
#ifdef Garanin
    return digitalWheelPanPasha.channal.getFunction();
#else
    return digitalWheelPan.channal.getFunction();
#endif
     }

int8_t Pult::digitalWheelTiltGetFunction() {
#ifdef Garanin
    return digitalWheelTiltPasha.channal.getFunction();
#else
    return digitalWheelTilt.channal.getFunction();
#endif
    }

int8_t Pult::digitalWheelRollGetFunction() {
#ifdef Garanin
    return digitalWheelRollPasha.channal.getFunction();
#else
    return digitalWheelRoll.channal.getFunction();
#endif
    }

void Pult::digitalWhellPanEnable()         {
#ifdef Garanin
    digitalWheelPanPasha.enable();
#else
    digitalWheelPan.enable();
#endif
      }

void Pult::digitalWhellPanDisable()        {
#ifdef Garanin
    digitalWheelPanPasha.disable();
#else
    digitalWheelPan.disable();
#endif
     }

void Pult::digitalWhellTiltEnable()        {
#ifdef Garanin
    digitalWheelTiltPasha.enable();
#else
    digitalWheelTilt.enable();
#endif
     }

void Pult::digitalWhellTiltDisable()       {
#ifdef Garanin
    digitalWheelTiltPasha.disable();
#else
    digitalWheelTilt.disable();
#endif

    }

void Pult::digitalWhellRollEnable()        {
#ifdef Garanin
    digitalWheelRollPasha.enable();
#else
    digitalWheelRoll.enable();
#endif
     }

void Pult::digitalWhellRollDisable()       {
#ifdef Garanin
    digitalWheelRollPasha.disable();
#else
    digitalWheelRoll.disable();
#endif
     }

void Pult::disableAllDigitalWheel()  {
#ifdef Garanin
    digitalWheelPanPasha.disable();
    digitalWheelTiltPasha.disable();
    digitalWheelRollPasha.disable();
#else
    digitalWheelPan.disable();
    digitalWheelTilt.disable();
    digitalWheelRoll.disable();
#endif


     }

void Pult:: digitalWhellSetPanDeadZone(float deadZone) {
#ifdef Garanin
    digitalWheelPanPasha.setDeadZone(deadZone);
#else
    digitalWheelPan.setDeadZone(deadZone);
#endif
      }

void Pult:: digitalWhellSetTiltDeadZone(float deadZone) {
#ifdef Garanin
    digitalWheelTiltPasha.setDeadZone(deadZone);
#else
    digitalWheelTilt.setDeadZone(deadZone);
#endif
      }

void Pult:: digitalWhellSetRollDeadZone(float deadZone) {
#ifdef Garanin
    digitalWheelRollPasha.setDeadZone(deadZone);
#else
    digitalWheelRoll.setDeadZone(deadZone);
#endif
      }

void Pult:: analogWhellSetPanDeadZone(float deadZone)   {
    panExtern1Channel.setDeadZone(deadZone);  }

void Pult:: analogWhellSetTiltDeadZone(float deadZone)  {
    tiltExtern1Channel.setDeadZone(deadZone);  }

void Pult:: analogWhellSetRollDeadZone(float deadZone)  {
    panExtern1Channel.setDeadZone(deadZone);  }

float Pult::getSpeedPasha() {
#ifdef panWheelDebug
    return panJoy.getRawValue();
#endif
#ifdef rollWheelDebug
    return dutchJoy.getRawValue();
#endif
#ifdef tiltWheelDebug
    return tiltJoy.getRawValue();
#endif
}

void Pult::digitalWheelPanSetFunction(int8_t pan,int8_t tilt, int8_t roll) {
#ifdef Garanin
    digitalWheelPanPasha.channal.setFunction(pan);
    digitalWheelTiltPasha.channal.setFunction(tilt);
    digitalWheelRollPasha.channal.setFunction(roll);
#else
    digitalWheelPan.channal.setFunction(pan);
    digitalWheelTilt.channal.setFunction(tilt);
    digitalWheelRoll.channal.setFunction(roll);
#endif


     }

//-------------------------------------------------------------------------------

void Pult::setZIFRevers(bool zoom, bool focus, bool iris) {
    virtualZoomReversButton.state=(PultButtonStates)zoom;
    virtualFocusReversButton.state=(PultButtonStates)focus;
    virtualIrisReversButton.state=(PultButtonStates)iris;
}

//-------------------------------------------------------------------------------

void Pult::setPreston(bool on) { preston.setEnable(on);       }

void Pult::xorPreston ()    {    preston.xorEnable();    }

bool Pult::getEnablePreston() {     return preston.getEnable();    }

//-----------------------------------------------------------------------------

void Pult::setBrightness (float brightness) {   backlight.setBrightness(brightness);    }


//---------------------------------------------------------------------------------

bool Pult::getPanGerconStatus()
{
    return (bool)statusBits3.gerconStatus.panMotorStatus;
}
bool Pult::getTiltGerconStatus()
{
    return (bool)statusBits3.gerconStatus.tiltMotorStatus;
}
bool Pult::getRollGerconStatus()
{
    return (bool)statusBits3.gerconStatus.dutchMotorStatus;
}

//-------------------------------------------------------------------------------

float Pult::getCalibrationOffset(CalibratedJoyChannel ch)
{
    if(joysticsConfig!=DEFAULT_JOY_CONFIG)
    {
        switch(ch)
        {
            case CALIB_OFFSET_MAIN_PAN:     return dutchJoyChannel.getCalibrationOffsetValue();
            case CALIB_OFFSET_MAIN_DUTCH:   return panJoyChannel.getCalibrationOffsetValue();
            case CALIB_OFFSET_MAIN_TILT:    return zoomJoyChannel.getCalibrationOffsetValue();
            case CALIB_OFFSET_MAIN_ZOOM:    return tiltJoyChannel.getCalibrationOffsetValue();
            case CALIB_OFFSET_EXT_PAN:      return panExtern1Channel.getCalibrationOffsetValue();
            case CALIB_OFFSET_EXT_DUTCH:    return dutchExtern1Channel.getCalibrationOffsetValue();
            case CALIB_OFFSET_EXT_TILT:     return tiltExtern1Channel.getCalibrationOffsetValue();
            case CALIB_OFFSET_EXT2_DUTCH:   return dutchExtern2Channel.getCalibrationOffsetValue();
            default:return 1860;
        }
    }
    else
    {
        switch(ch)
        {
            case CALIB_OFFSET_MAIN_PAN:     return panJoyChannel.getCalibrationOffsetValue();
            case CALIB_OFFSET_MAIN_DUTCH:   return dutchJoyChannel.getCalibrationOffsetValue();
            case CALIB_OFFSET_MAIN_TILT:    return tiltJoyChannel.getCalibrationOffsetValue();
            case CALIB_OFFSET_MAIN_ZOOM:    return zoomJoyChannel.getCalibrationOffsetValue();
            case CALIB_OFFSET_EXT_PAN:      return panExtern1Channel.getCalibrationOffsetValue();
            case CALIB_OFFSET_EXT_DUTCH:    return dutchExtern1Channel.getCalibrationOffsetValue();
            case CALIB_OFFSET_EXT_TILT:     return tiltExtern1Channel.getCalibrationOffsetValue();
            case CALIB_OFFSET_EXT2_DUTCH:   return dutchExtern2Channel.getCalibrationOffsetValue();
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
            case CALIB_OFFSET_MAIN_PAN:     dutchJoyChannel.setOffset(offset);      panOffset=offset;       break;
            case CALIB_OFFSET_MAIN_DUTCH:   panJoyChannel.setOffset(offset);        dutchOffset=offset;     break;
            case CALIB_OFFSET_MAIN_TILT:    zoomJoyChannel.setOffset(offset);       tiltOffset=offset;      break;
            case CALIB_OFFSET_MAIN_ZOOM:    tiltJoyChannel.setOffset(offset);       zoomOffset=offset;      break;
            case CALIB_OFFSET_EXT_PAN:      panExtern1Channel.setOffset(offset);    break;
            case CALIB_OFFSET_EXT_DUTCH:    dutchExtern1Channel.setOffset(offset);  break;
            case CALIB_OFFSET_EXT_TILT:     tiltExtern1Channel.setOffset(offset);   break;
            case CALIB_OFFSET_EXT2_DUTCH:   dutchExtern2Channel.setOffset(offset);  break;
            default:return;
        }
    }
    else
    {
        switch(ch)
        {
            case CALIB_OFFSET_MAIN_PAN:     panJoyChannel.setOffset(offset);        panOffset=offset;       break;
            case CALIB_OFFSET_MAIN_DUTCH:   dutchJoyChannel.setOffset(offset);      dutchOffset=offset;     break;
            case CALIB_OFFSET_MAIN_TILT:    tiltJoyChannel.setOffset(offset);       tiltOffset=offset;      break;
            case CALIB_OFFSET_MAIN_ZOOM:    zoomJoyChannel.setOffset(offset);       zoomOffset=offset;      break;
            case CALIB_OFFSET_EXT_PAN:      panExtern1Channel.setOffset(offset);    break;
            case CALIB_OFFSET_EXT_DUTCH:    dutchExtern1Channel.setOffset(offset);  break;
            case CALIB_OFFSET_EXT_TILT:     tiltExtern1Channel.setOffset(offset);   break;
            case CALIB_OFFSET_EXT2_DUTCH:   dutchExtern2Channel.setOffset(offset);  break;
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

void Pult::setSoftRegim(bool regim) {
    controlBits.bit.softRegim=(UInt16)regim;
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
    //return gyConFaultBits.stateBits.faultBits;
    return gyConFaultBits.all;
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
#ifdef crane
    ExtrSyncroization::ExtrenalDevieExchDriver::selectMode(ExtrSyncroization::EXT_DEV_DOPREAL);
    return;
#endif
    switch(value)
    {
        case 2:
            ExtrSyncroization::ExtrenalDevieExchDriver::selectMode(ExtrSyncroization::EXT_DEV_DOPREAL);
            break;
        case 1:
            ExtrSyncroization::ExtrenalDevieExchDriver::selectMode(ExtrSyncroization::EXT_DEV_PAN_BAR);
#ifdef myPanBar
            panBar.enable();
#endif
            break;
        case 0:
            ExtrSyncroization::ExtrenalDevieExchDriver::selectMode(ExtrSyncroization::EXT_DEV_MOTION);
#ifdef myPanBar
            panBar.disable();
#endif
            break;
    }
}

//-------------------------------------------------------------------------------

IShaker* Pult::_panAxisShaker() {   return &panInternalShakerChannel;    }
IShaker* Pult::_tiltAxisShaker(){   return &tiltInternalShakerChannel;    }
IShaker* Pult::_rollAxisShaker(){   return &rollInternalShakerChannel;    }
IShaker* Pult::_zoomAxisShaker(){   return &zoomInternalShakerChannel;    }

IShakerSin* Pult::_panAxisShakerSin() {   return &panInternalSinShakerChannel;    }
IShakerSin* Pult::_tiltAxisShakerSin(){   return &tiltInternalSinShakerChannel;    }
IShakerSin* Pult::_rollAxisShakerSin(){   return &rollInternalSinShakerChannel;    }
IShakerSin* Pult::_zoomAxisShakerSin(){   return &zoomInternalSinShakerChannel;    }

void Pult::setCamIDDopReal(uint8_t camId) {ExtrSyncroization::ExtrenalDevieExchDriver::dataDopReal.setCamId(camId);}



