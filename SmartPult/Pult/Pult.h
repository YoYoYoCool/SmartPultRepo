/*
 * Pult.h
 *
 *  Created on: 26 июля 2015 г.
 *      Author: Cno
 */
#include <Pult/PultClasses.h>
#include <Pult/MenuModels/Warnings.h>
#include <Pult/MenuModels/ViewLists.h>
#include <Pult/MenuModels/CheckPointList.hpp>
#include "Aproximators/AproximatorIF.hpp"
#include "../LedController/LedController.hpp"
#include "../MotionControl/MotionControl.hpp"
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Semaphore.h>
#include "InternalDevice/shakerInternalChannel.hpp"
#include "InternalDevice/shakerSinInternalChannel.hpp"
#include "../ExtrenalDeviceSynchro/DopReal/DopRealData.hpp"


#ifndef PULT_PULT_H_
#define PULT_PULT_H_

typedef enum GyConState {
	GyConState_Off = 0,
	GyConState_Normal = 0,
	GyConState_Fault = 0
} GyConState;

typedef enum PultButtons {
   SA1 = 0, SA2, SA3, SA4,
   pult_Button_PK, pult_Button_PI, pult_Button_DK, pult_Button_DI, pult_Button_TK, pult_Button_TI,
   pult_Button_Up, pult_Button_Left, pult_Button_Select, pult_Button_Right, pult_Button_Dn, SA16, pult_Button_ESC,
   backLightOff,
   PULT_BUTTONS_COUNT
} PultButtons;

typedef enum PultAxisSettings {
   AXIS_SETTINGS_PK = 0, //0..1
   AXIS_SETTINGS_PI,     //0..1
   AXIS_SETTINGS_DK,     //0..1
   AXIS_SETTINGS_DI,     //0..1
   AXIS_SETTINGS_TK,     //0..1
   AXIS_SETTINGS_TI,     //0..1
   AXIS_SETTINGS_Tres,     //0..1
   AXIS_SETTINGS_Tdif,     //0..1
   AXIS_SETTINGS_COUNT
} PultAxisSettings;

typedef enum PultCalibrateCoeffs { //0..1
	PultCalibrateCoefGv = 0,
	PultCalibrateCoeffsCount

} PultCalibrateCoeffs;

union GerconStatusBits {
    uint32_t all;
    struct {
        uint8_t panMotorStatus : 1;
        uint8_t dutchMotorStatus :1;
        uint8_t tiltMotorStatus :1;
    }gerconStatus;
};

typedef enum PultDebugValues { //0.001 - 1000
	PultDebugValue0 = 0,
	PultDebugValue1 = 1,
	PultDebugValue2 = 2,
	PultDebugValue3 = 3,
	PultDebugValue4 = 4,
	PultDebugValue5 = 5,
	PultDebugValue6 = 6,
	PultDebugValue7 = 7,
	PultDebugValue8 = 8,
	PultDebugValue9 = 9,
	PultDebugValuesCount
} PultDebugValues;

typedef enum PultJoystickPresets {
	PanJoystickPreset = 0,
	DutchJoystickPreset = 1,
	TiltJoystickPreset = 2,
	ZoomJoystickPreset = 3
} PultJoystickPresets;

typedef enum PultEqualaserType {
	PanJoystickEqualaser = 0,
	DutchJoystickEqualaser = 1,
	TiltJoystickEqualaser = 2,
	ZoomJoystickEqualaser=3
} PultEqualaserType;

typedef enum PultJoysticConfigurations
{
	DEFAULT_JOY_CONFIG=0,
	JOYSTIC_REVERS_JOY_CONFIG,
	COUNT_JOY_CONFIG

}PultJoysticConfigurations;

enum LensControlMotorsTypes
{
	M21VE=0,
	M26VE,
	LC_MOTORS_TYPES_COUNT
};

enum LensControlMotors
{
	LC_MOTOR1=0,
	LC_MOTOR2,
	LC_MOTOR3,
	LC_MOTORS_COUNT
};

enum PanAxisDirection
{
	PAN_AXIS_UP,
	PAN_AXIS_DOWN,
	PAN_AXIS_STOP
};
enum MotionButtons
{
	MOTION_BUTTON_PLAY=0,
	MOTION_BUTTON_STOP,
	MOTION_BUTTON_DELETE,
	MOTION_BUTTON_REVERS_PLAY,
	MOTION_BUTTON_T1S,
	MOTION_BUTTON_T2S,
	MOTION_BUTTON_T3S,
	MOTION_BUTTON_T4S,
	MOTION_BUTTON_T5S,
	MOTION_BUTTON_T6S,
	MOTION_BUTTON_COUNT
};
enum CameraStartType
{
	CAMERA_START_FRONT=0,
	CAMERA_START_LEVEL,
	CAMERA_START_COUNT
};

enum CalibratedJoyChannel
{
	CALIB_OFFSET_MAIN_PAN=0,
	CALIB_OFFSET_MAIN_DUTCH,
	CALIB_OFFSET_MAIN_TILT,
	CALIB_OFFSET_MAIN_ZOOM,
	CALIB_OFFSET_EXT_PAN,
	CALIB_OFFSET_EXT_DUTCH,
	CALIB_OFFSET_EXT_TILT,
	CALIB_OFFSET_EXT2_DUTCH,
	CALIB_OFFSET_COUNT
};

enum SuspensionResonansChannel
{
	SUSPENSION_RESONANCE_CHANNEL_TILT=0,
	SUSPENSION_RESONANCE_CHANNEL_PAN,
	SUSPENSION_RESONANCE_CHANNEL_COUNT
};

enum SuspensionResonansMode
{
	SUSPENSION_RESONANCE_MODE_ENABLE=0,
	SUSPENSION_RESONANCE_MODE_DISABLE
};


enum FolowingModes
{
    FOLOWING_MODE_STABILIZATION=0,
    FOLOWING_MODE_FOLOWING,
    FOLOWING_MODE_MIXED,
    FOLOWING_MODE_CNT
};

union FolowingModeConfigData
{
    volatile UInt32 all;
    struct
    {
        volatile FolowingModes mode :2;
        volatile UInt32 sector      :5;
    };
};






class Pult:public MotionControlListener{
	Semaphore_Handle* exchangeSem;
	Semaphore_Handle* exchangeAltSem;
public:

	void motionOnOff(bool dir)
	{
		autonegComplete=false;
	}

	void motionSetTransferDelay(UInt32 delay)
	{
		setMotionModeTransferDelay(delay);
	}
	void useAutonegatation()
	{
		motionTransferModeActivate();
	}

	bool isAutonegComplete()
	{
		return autonegComplete;
	}

	bool motionCompareEndSetTransferDelay(UInt32 delay)
	{
		if(!autonegComplete)
		{
			if(!motionTransferModeActivate()){return false;}
		}

		if(getMotionModeTransferDelay()>delay){return false;}

		setMotionModeTransferDelay(delay);
		return true;

	}

	UInt32 getMiCurrentTransferDelay()
	{
		return getMotionModeTransferDelay();
	}

	volatile PultJoysticConfigurations joysticsConfig;
	volatile bool connectedFlag;
	volatile bool debugMenuCmdFlag;
	volatile bool autonegComplete;
	volatile bool lensCalibrateFlag;

	volatile UInt32 transferDelay;
	Pult(Semaphore_Handle* s,Semaphore_Handle* sA);
	PultButton* getButton(PultButtons btn); //Возвращает кнопку btn
	PultButton* getButton_1_16(uint8_t ID); //Возвращает кнопку передней панели
	PultButton* getButtonVirtual(uint8_t ID); //Возвращает виртуальную кнопку
	void startTransmitAxisSettings(); //Начать периодическую передачу настроек
	void setAxisSetting(PultAxisSettings setting, float value); //Установить значение настройки
	void stopTransmitAxisSettings(); //Остановить периодическую передачу настроек
    void driverTask();
    void exchangeTask();
    void exchangeAlternativeTask();

    float getPanTurns(); //Получить количество оборотов
    bool isConnected();
    bool isJoySticksEnabled();
    bool isMotorsEnabled();
    bool isBackLightEnabled();
    GyConState getGyConState();
    char* getFaultMessage();

    char* getPanPreset(); //тексты для отображения пресетов
    char* getDutchPreset(); //тексты для отображения пресетов
    char* getTiltPreset(); //тексты для отображения пресетов
    void initPanPreset(char* preset); //тексты для отображения пресетов
    void initDutchPreset(char* preset); //тексты для отображения пресетов
    void initTiltPreset(char* preset); //тексты для отображения пресетов

    void setTiltUpLimit(); //верхнее ограничение тангажа
    void setTiltDnLimit(); //нижнее ограничение тангажа
    void resetTiltUpLimit();
    void resetTiltDnLimit();

    void setLeftPanLimit();
    void setRightPanLimit();
    void resetLeftPanLimit();
    void resetRightPanLimit();

    void setLeftRollLimit();
    void setRightRollLimit();
    void resetLeftRollLimit();
    void resetRightRollLimit();

    void setPlatform(UInt8 num); //выбор платформы (1 или 2)
    char* getGvAcc();
    bool getGVEnable();

    float getPanAngel();
    float getTiltAngel();
    float getRollAngel();
    float getZoomAngel();

    void setXY(char* pName, UInt8 num, float x, float y);

    //Пресеты джойстиков
    const char* getJoystickPresetName(PultJoystickPresets preset);
    UInt8 getJoystickPresetId(PultJoystickPresets preset);
    void setJoystickPresetId(PultJoystickPresets preset, UInt8 presetId);

    void setPultCalibratesCoeff(PultCalibrateCoeffs name, float value);
    float getPultDebugValue(PultDebugValues name);

    //-------------- новое апи ---------------
    Warnings* getWarnings();
	void updateWarningsList();
    ViewLists* getViewLists();
    //----------------------------------------
    //АПИ Эквалайзера
    List<Point*>* getEqualaserPoints(PultEqualaserType type);
    void updateAproximator(PultEqualaserType type);
    float getEqualaserAproximatorValue(PultEqualaserType type, float x);
    float getJoysticSpeedValue(PultEqualaserType type);
    float getJoystickCurrentPozition(PultEqualaserType type);
    float getJoystickMaxValue(PultEqualaserType type);

    //---------------------- АПИ светодиодов ---------------------------------
    LedController* getLedController();
    //----------------------                 ---------------------------------
    //---------------------- Переназначение джойстиков -----------------------
    void setJoysticConfiguration(PultJoysticConfigurations preset);
    //------------------------------------------------------------------------
//  lens control 		API
    void setMotorType(UInt8 type, UInt8 motorNumber);
    void setZoomMotor(UInt8 motorNumber);
    void setIrisMotor(UInt8 motorNumber);
    void setFocusMotor(UInt8 motorNumber);

    UInt8 getZoomMotor();
    UInt8 getIrisMotor();
    UInt8 getFocusMotor();

    UInt8 getMotorType(UInt8 no);
    //------------------------------------------------------------------------
    float getHeadVoltage();
	PanAxisDirection getPanAxisDirection();
    //------------------------------------------------------------------------
    void initWatchDog();
    //------------------------------------------------------------------------
    PultButton** getMotionControlButtons();
    MotionControl* getMotionSubsystem();
    bool joysticUsed();
    void resetCentralButtons();
    UInt8 getTransferDelay();

    //------------------------------------------------------------------------
    void setCameraStartOptions(CameraStartType option);
    //------------------------------------------------------------------------
    void setOverslangState(bool on);
    //------------------------------------------------------------------------
    void setEstimationBit();
    //------------------------------------------------------------------------
    List<CheckPoint*>* getCheckValues();
    void resetStaticErrorBits();
    void resetDisconnectCounter();
    UInt8 getDisconnectCounter();
    void checkErrorLogic();
    bool isErrorUpdated();
    void sendDebugValue();
    //------------------------------------------------------------------------
    UInt32 getMotionModeTransferDelay();
    void setMotionModeTransferDelay(UInt32 delay);
    bool motionTransferModeActivate();
//    bool motionTransferModeActivateByValue(UInt32 delay);
    //------------------------------------------------------------------------
    void setPanWheelSpeed(float val);
    void setTiltWheelSpeed(float val);
    void setDutchWheelSpeed(float val);
    void enablePanAnalogWheel();
    void enableTiltAnalogWheel();
    void enableRollAnalogWheel();
    void enableDutchPadal();
    void enableZoomPadal();
    void disablePanAnalogWheel();
    void disableTiltAnalogWheel();
    void disableRollAnalogWheel();
    void disablePadal();

    //------------------------------------------------------------------------
    float getCalibrationOffset(CalibratedJoyChannel ch);
    void  setCalibrationOffset(CalibratedJoyChannel ch, UInt32 offset);
    //------------------------------------------------------------------------
    void setSuspentionResonantFreq(float freq,SuspensionResonansChannel channel);
    void setSuspentionResonantMode(SuspensionResonansChannel channel,SuspensionResonansMode mode);
    //------------------------------------------------------------------------
    void setMaxTorque(UInt32 pan,UInt32 ducth,UInt32 tilt);
    //------------------------------------------------------------------------
    void setEcoMode(bool eco);

    void setSoftRegim(bool regim);
    //------------------------------------------------------------------------
    void setJoyDeadZone(UInt32 pan,UInt32 ducth,UInt32 tilt,UInt32 zoom);
    //------------------------------------------------------------------------
    void startLensCalibration();
    bool lensCalibrtionClicked();
    //------------------------------------------------------------------------
    void setStartFluid(UInt32 valuePan,UInt32 valueDutch,UInt32 valueTilt,UInt32 valueZoom);
    //------------------------------------------------------------------------
    void setZoomDrift(Int32 zoomDrift);
    //------------------------------------------------------------------------
    void   setMasterPrerolValue(Int32 value);
    UInt32 getMasterPrerolValue();

    void   setSlave1PrerolValue(Int32 value);
    UInt32 getSlave1PrerolValue();

    void   setSlave2PrerolValue(Int32 value);
    UInt32 getSlave2PrerolValue();

    void   setSlave3PrerolValue(Int32 value);
    UInt32 getSlave3PrerolValue();
    //------------------------------------------------------------------------
    void   setSynchroSource(UInt32 value);
    //------------------------------------------------------------------------
    float getPanDriftFactor();
    float getDutchDriftFactor();
    float getTiltDriftFactor();
    float* getPanDriftFactorUk();
    float* getDutchDriftFactorUk();
    float* getTiltDriftFactorUk();
    //------------------------------------------------------------------------
    void setDriftStopperMode(bool val);
    //------------------------------------------------------------------------
    UInt16  getControlBits ();
    //------------------------------------------------------------------------
    UInt16  getGyConFaultBits ();
        //------------------------------------------------------------------------
    void setPanFolowingMode(FolowingModeConfigData val);
    //------------------------------------------------------------------------
    UInt32 getTimeToStart();
    //-----------------------------------------------------------------------------
    void digitalWheelPanStepLeft();
    void digitalWheelTiltStepLeft();
    void digitalWheelRollStepLeft();
    void digitalWheelPanStepRight();
    void digitalWheelTiltStepRight();
    void digitalWheelRollStepRight();
    int8_t digitalWheelPanGetFunction();
    int8_t digitalWheelTiltGetFunction();
    int8_t digitalWheelRollGetFunction();
    void digitalWhellPanEnable();
    void digitalWhellPanDisable();
    void digitalWhellTiltEnable();
    void digitalWhellTiltDisable();
    void digitalWhellRollEnable();
    void digitalWhellRollDisable();
    void digitalWheelPanSetFunction(int8_t pan,int8_t tilt, int8_t roll);
    void disableAllDigitalWheel();
    void digitalWhellSetPanDeadZone(float deadZone);
    void digitalWhellSetTiltDeadZone(float deadZone);
    void digitalWhellSetRollDeadZone(float deadZone);
    void analogWhellSetPanDeadZone(float deadZone);
    void analogWhellSetTiltDeadZone(float deadZone);
    void analogWhellSetRollDeadZone(float deadZone);
    //------------------------------------------------------------
    float getSpeedPasha();
    //-------------------------------------------------

    void setZIFRevers(bool zoom, bool focus, bool iris);

    //------------------------------------------------------------

    void setPreston(bool on);

    void xorPreston ();

    bool getEnablePreston();

    bool getPanGerconStatus();
    bool getTiltGerconStatus();
    bool getRollGerconStatus();

    //------------------------------------------------------------

    void setBrightness (float persent);

    //------------------------------------------------------------
    IShaker* _panAxisShaker();
    IShaker* _tiltAxisShaker();
    IShaker* _rollAxisShaker();
    IShaker* _zoomAxisShaker();


    IShakerSin* _panAxisShakerSin();
    IShakerSin* _tiltAxisShakerSin();
    IShakerSin* _rollAxisShakerSin();
    IShakerSin* _zoomAxisShakerSin();

    //-------------------------------------------------------------
    void setCamIDDopReal(uint8_t CamId);

};


#endif /* PULT_PULT_H_ */
