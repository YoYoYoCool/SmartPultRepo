/*
 * MotionLogicController.hpp
 *
 *  Created on: 21 февр. 2018 г.
 *      Author: AhrameshinGN
 */

#ifndef MOTIONCONTROL_LOGICCONTROLLER_MOTIONLOGICCONTROLLER_HPP_
#define MOTIONCONTROL_LOGICCONTROLLER_MOTIONLOGICCONTROLLER_HPP_


#include <Pult/Pult.h>
#include "ExtrenalDeviceSynchro/RS232Syncro.hpp"

enum MotionMenuState
{
    MOTION_MENU_READY=0,
    MOTION_MENU_READY_TO_REC,
    MOTION_MENU_REC,
    MOTION_MENU_PLAY,
    MOTION_MENU_WAIT_READY_TO_PLAY,
    MOTION_MENU_READY_TO_PLAY,
    MOTION_MENU_PLAY_PAUSE,
    MOTION_MENU_REVERS_PLAY,
    MOTION_MENU_REVERS_PLAY_PAUSE,
    MOTION_MENU_PREROL


};

enum TrackLedBlinkState
{
    TRACK_LED_UP=0,
    TRACK_LED_DN,
    TRACK_LED_FAST_BLINK,
    TRACK_LED_SLOW_BLINK,
    TRACK_LED_COMPLEX_BLINK
};

struct TrackLedState
{
    volatile TrackLedBlinkState state;
    volatile LedID ledID;
};

class MotionPlayMaster
{
    public:
    MotionPlayMaster(Pult& p, MotionMenuState& st):
        pult(p),
        logicalState(st),
        masterReadyToStart(false),
        slave1ReadyToStart(false),
        slave2ReadyToStart(false),
        slave3ReadyToStart(false)
    {

    }

    bool masterReady()          {   return masterReadyToStart;   }
    bool slave1Ready()          {   return slave1ReadyToStart;   }
    bool slave2Ready()          {   return slave2ReadyToStart;   }
    bool slave3Ready()          {   return slave3ReadyToStart;   }

    void clk()
    {
        if(logicalState==MOTION_MENU_PREROL||logicalState==MOTION_MENU_PLAY||logicalState==MOTION_MENU_REVERS_PLAY)
        {
            if(timerSlave1>=(pult.getSlave1PrerolValue()/10))   {    slave1ReadyToStart=true;   }
            else                                                {    timerSlave1++;             }

            if(timerSlave2>=(pult.getSlave2PrerolValue()/10))   {    slave2ReadyToStart=true;   }
            else                                                {    timerSlave2++;             }

            if(timerSlave3>=(pult.getSlave3PrerolValue()/10))   {    slave3ReadyToStart=true;   }
            else                                                {    timerSlave3++;             }

            if(timerMy>=(pult.getMasterPrerolValue()/10))       {    masterReadyToStart=true;   }
            else                                                {    timerMy++;                 }
        }
        else
        {
            timerSlave1=0;
            timerSlave2=0;
            timerSlave3=0;
            timerMy=0;
            masterReadyToStart=false;
            slave1ReadyToStart=false;
            slave2ReadyToStart=false;
            slave3ReadyToStart=false;
        }
    }

    private:
        Pult& pult;
        MotionMenuState& logicalState;
        volatile bool masterReadyToStart;
        volatile bool slave1ReadyToStart;
        volatile bool slave2ReadyToStart;
        volatile bool slave3ReadyToStart;

        UInt32 timerSlave1;
        UInt32 timerSlave2;
        UInt32 timerSlave3;
        UInt32 timerMy;


};

#define FAST_BLINK_TIME 10
#define SLOW_BLINK_TIME 50
class MotionButtonControl
{
    private:
        MotionSynchroTransmiter* synchroModule;
        Pult& pult;
        LedController* ledController;
        PultButton** motionButtons;
        MotionMenuState state;
        MotionControl* motionControlSubsystem;
        volatile bool active;

        bool blinkPhase;
        UInt16 phaseCounter;
        TrackLedState* ledsState;

        UInt8 holdCounterDelete;
        UInt32 goToAngleTimeout;

        LedID syncroDryContact;
        MotionPlayMaster playMaster;

        inline void compareTrackSize(UInt8 id)
        {
            if(id>=6){return;}
            if(motionControlSubsystem->isEmptyTrack(id)){ledsState[id].state=TRACK_LED_DN;}
            else{ledsState[id].state=TRACK_LED_UP;  }
        }

        inline void resetButtonsClick()
        {
            for(UInt8 i=0;i!=MOTION_BUTTON_COUNT;i++)
            {
                 motionButtons[i]->clearFlags();
            }
        }
    public:
        MotionButtonControl(TrackLedState* ledStartup, LedID dryContactLed, Pult& pult_):
            ledsState(ledStartup),
            syncroDryContact(dryContactLed),
            pult(pult_),
            playMaster(pult, state)
            {
                    motionButtons=pult.getMotionControlButtons();
                    motionControlSubsystem=pult.getMotionSubsystem();
                    ledController=pult.getLedController();
                    state=MOTION_MENU_READY;
                    active=false;
                    blinkPhase=false;
                    phaseCounter=0;
                    holdCounterDelete=0;

            }

        void slaveSynchro();
        void mainLogic();
        void ledLogic();
        void actionManager();
        inline void clocking()
        {
            mainLogic();
            ledLogic();
            playMaster.clk();
//            slaveSynchro();
        }
        inline void initMotionControl()
        {
            motionControlSubsystem->init();
        }

        void setMotionSynchroModule(MotionSynchroTransmiter* mst)
        {
            synchroModule=mst;
        }

        void activate()
        {
            resetLeds();
            resetButtonsClick();
            motionControlSubsystem->activate();
            active=true;
            synchroModule->activateModule();
            synchroModule->resetModule();

        }
        void deactivate()
        {
            motionControlSubsystem->resetModule();
            motionControlSubsystem->deactivated();
            resetLeds();
            resetButtonsClick();
            active=false;
            synchroModule->deactivateModule();
            synchroModule->resetModule();

        }

        inline void resetLeds()
        {
            for(UInt8 i=0;i!=6;i++)
            {
                ledController->getData()->resetLed(ledsState[i].ledID);
            }
            ledController->invalidate();
        }

        MotionMenuState getState(){ return state;}


};




#endif /* MOTIONCONTROL_LOGICCONTROLLER_MOTIONLOGICCONTROLLER_HPP_ */
