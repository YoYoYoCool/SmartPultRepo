/*
 * MotionLogicController.cpp
 *
 *  Created on: 21 февр. 2018 г.
 *      Author: AhrameshinGN
 */

#include "MotionLogicController.hpp"

void MotionButtonControl::slaveSynchro()
{
    static UInt8 playCounter=0;
    if(state==MOTION_MENU_PREROL||state==MOTION_MENU_PLAY)
    {
        switch(playCounter)
        {
            case 0:
                playCounter++;
                if(playMaster.slave1Ready()){synchroModule->setSlave1PlayCommand();return;}

            case 1:
                playCounter++;
                if(playMaster.slave2Ready()){synchroModule->setSlave2PlayCommand();return;}

            case 2:
                playCounter++;
                if(playMaster.slave3Ready()){synchroModule->setSlave3PlayCommand();return;}

            case 3:
                playCounter=0;
                if(playMaster.masterReady()){synchroModule->setMasterPlayCommand();return;}
                return;

            default:
                playCounter=0;
                return;
        }
     }

    switch(state)
    {
        case MOTION_MENU_READY:             synchroModule->setSuccessCommand();     return;
        case MOTION_MENU_READY_TO_REC:      synchroModule->setSuccessCommand();     return;
        case MOTION_MENU_REC:               synchroModule->setRecCommand();         return;
        case MOTION_MENU_WAIT_READY_TO_PLAY:synchroModule->setGTACommand();         return;
        case MOTION_MENU_READY_TO_PLAY:     synchroModule->setReadyToPlayCommand(); return;
        case MOTION_MENU_PLAY_PAUSE:        synchroModule->setPauseCommand();       return;
        case MOTION_MENU_REVERS_PLAY:       synchroModule->setRevPlayCommand();     return;
        case MOTION_MENU_REVERS_PLAY_PAUSE: synchroModule->setPauseCommand();       return;
        default:                            synchroModule->setSuccessCommand();     return;
    }
}


void MotionButtonControl::ledLogic()
{
    if(!active){return;}
    for(UInt8 i=0;i!=6;i++)
    {
        switch(ledsState[i].state)
        {
            case TRACK_LED_UP:
                ledController->getData()->setLed(ledsState[i].ledID);
                ledController->invalidate();
                break;
            case TRACK_LED_DN:
                ledController->getData()->resetLed(ledsState[i].ledID);
                ledController->invalidate();
                break;
            case TRACK_LED_FAST_BLINK:
                if(phaseCounter>FAST_BLINK_TIME)
                {
                    phaseCounter=0;
                    blinkPhase=!blinkPhase;
                }
                phaseCounter++;
                if(blinkPhase)  {   ledController->getData()->setLed(ledsState[i].ledID);}
                else            {   ledController->getData()->resetLed(ledsState[i].ledID);}
                ledController->invalidate();
                break;
            case TRACK_LED_SLOW_BLINK:
                if(phaseCounter>SLOW_BLINK_TIME)
                {
                    phaseCounter=0;
                    blinkPhase=!blinkPhase;
                }
                phaseCounter++;
                if(blinkPhase)  {   ledController->getData()->setLed(ledsState[i].ledID);}
                else            {   ledController->getData()->resetLed(ledsState[i].ledID);}
                ledController->invalidate();
                break;
            case TRACK_LED_COMPLEX_BLINK:
                ledController->getData()->setLed(ledsState[i].ledID);
                ledController->invalidate();
                if(blinkPhase)
                {
                    ledController->getData()->setLed(ledsState[i].ledID);
                    ledController->invalidate();
                    if(phaseCounter>FAST_BLINK_TIME)
                    {
                        phaseCounter=0;
                        blinkPhase=!blinkPhase;
                        break;
                    }
                }
                else
                {
                    ledController->getData()->resetLed(ledsState[i].ledID);
                    ledController->invalidate();
                    if(phaseCounter>SLOW_BLINK_TIME)
                    {
                        phaseCounter=0;
                        blinkPhase=!blinkPhase;
                        break;
                    }
                }
                phaseCounter++;
                break;

            default:
                ledsState[i].state=TRACK_LED_DN;
                break;
        }
    }

}


void MotionButtonControl::mainLogic()
{
    if(!active){return;}

    for(UInt8 i=0;i!=6;i++){    compareTrackSize(i);}
//    switch(mode)
//    {

        switch(state)
        {
        //---------------------------------------------------------------------------------------------------
            case MOTION_MENU_READY:
                    //State definition logic
                    ledsState[motionControlSubsystem->getCurrentTrackID()].state=TRACK_LED_SLOW_BLINK;
                    if(!motionControlSubsystem->isReady()){ motionControlSubsystem->resetModule();  }

                    ledController->getData()->resetLed(syncroDryContact);
                    ledController->invalidate();

                    // ButtonControlLogic
                    //---------------------------------------------------------
                    for(UInt8 i=MOTION_BUTTON_T1S;i<=MOTION_BUTTON_T6S;i++)
                    {
                        if(motionButtons[i]->state==HOLD||synchroModule->isRecComand())
                        {
                            motionControlSubsystem->selectCurrentTrackID(i-MOTION_BUTTON_T1S);
                            if(!motionControlSubsystem->isReadyToRec()){return;}
                            state=MOTION_MENU_READY_TO_REC;
                            return;
                        }
                    }
                    //---------------------------------------------------------
                    for(UInt8 i=MOTION_BUTTON_T1S;i<=MOTION_BUTTON_T6S;i++)
                    {
                        if(motionButtons[i]->isClicked())
                        {
                            motionControlSubsystem->selectCurrentTrackID(i-MOTION_BUTTON_T1S);
                            return;
                        }
                    }
                    //---------------------------------------------------------
                    if(motionButtons[MOTION_BUTTON_PLAY]->isClicked()||synchroModule->isGTAComand())
                    {
                        if(!motionControlSubsystem->play()){return;}
                        state=MOTION_MENU_WAIT_READY_TO_PLAY;
                        goToAngleTimeout=0;
                        return;
                    }
                    //---------------------------------------------------------
                    if(motionButtons[MOTION_BUTTON_REVERS_PLAY]->isClicked())
                    {
                        if(!motionControlSubsystem->reversPlay()){return;}
                        state=MOTION_MENU_WAIT_READY_TO_PLAY;
                        goToAngleTimeout=0;
                        return;
                    }
                    //---------------------------------------------------------
                    if(motionButtons[MOTION_BUTTON_DELETE]->state==HOLD)
                    {
                        if(holdCounterDelete>0){holdCounterDelete--;return;}
                        holdCounterDelete=100;
                        motionControlSubsystem->clearCurrentTrack();
                        return;
                    }
                    //---------------------------------------------------------
                    resetButtonsClick();


                    return;
        //---------------------------------------------------------------------------------------------------
                    //Play logic
            case MOTION_MENU_PLAY:
                ledsState[motionControlSubsystem->getCurrentTrackID()].state=TRACK_LED_FAST_BLINK;


                ledController->getData()->setLed(syncroDryContact);
                ledController->invalidate();

                if(!motionControlSubsystem->isPlay())
                {
                    motionControlSubsystem->resetModule();
                    state=MOTION_MENU_READY;
                    return;
                }

                if(motionButtons[MOTION_BUTTON_PLAY]->isClicked()||synchroModule->isPauseComand())
                {
                    motionControlSubsystem->pause();
                    state=MOTION_MENU_PLAY_PAUSE;
                    return;
                }
                if(motionButtons[MOTION_BUTTON_STOP]->isClicked()||synchroModule->isSuccessComand())
                {
                    motionControlSubsystem->stopPlay();
                    state=MOTION_MENU_READY;
                    return;
                }
                resetButtonsClick();

               return;

            case MOTION_MENU_REVERS_PLAY:
                ledsState[motionControlSubsystem->getCurrentTrackID()].state=TRACK_LED_FAST_BLINK;

                ledController->getData()->setLed(syncroDryContact);
                ledController->invalidate();

                if(!motionControlSubsystem->isPlay())
                {
                    motionControlSubsystem->resetModule();
                    state=MOTION_MENU_READY;
                    return;
                }

                if(motionButtons[MOTION_BUTTON_REVERS_PLAY]->isClicked())
                {
                    motionControlSubsystem->pause();
                    state=MOTION_MENU_REVERS_PLAY_PAUSE;
                    return;
                }
                if(motionButtons[MOTION_BUTTON_STOP]->isClicked())
                {
                    motionControlSubsystem->stopPlay();
                    state=MOTION_MENU_READY;
                    return;
                }
                resetButtonsClick();

                return;

            case MOTION_MENU_WAIT_READY_TO_PLAY:
                ledsState[motionControlSubsystem->getCurrentTrackID()].state=TRACK_LED_FAST_BLINK;


                ledController->getData()->resetLed(syncroDryContact);
                ledController->invalidate();

                if(goToAngleTimeout>6000)
                {
                    motionControlSubsystem->resetModule();
                    state=MOTION_MENU_READY;
                    goToAngleTimeout=0;
                    return;
                }
                goToAngleTimeout++;
                if(motionControlSubsystem->readyToPlay())
                {
                    state=MOTION_MENU_READY_TO_PLAY;
                    return;
                }
                if(motionButtons[MOTION_BUTTON_STOP]->isClicked()||synchroModule->isSuccessComand())
                {
                    motionControlSubsystem->resetModule();
                    state=MOTION_MENU_READY;
                    return;
                }


                resetButtonsClick();

               break;

            case MOTION_MENU_PREROL:
                ledsState[motionControlSubsystem->getCurrentTrackID()].state=TRACK_LED_FAST_BLINK;

                ledController->getData()->setLed(syncroDryContact);
                ledController->invalidate();

                if(playMaster.masterReady()||synchroModule->isSlave1PlayComand())
                {
                    if(motionControlSubsystem->getReversPlayFlag())
                    {
                        motionControlSubsystem->playActivation();
                        state=MOTION_MENU_REVERS_PLAY;
                        return;
                    }
                    else
                    {
                        motionControlSubsystem->playActivation();
                        state=MOTION_MENU_PLAY;
                        return;
                    }
                }
                return;

            case MOTION_MENU_READY_TO_PLAY:
                ledsState[motionControlSubsystem->getCurrentTrackID()].state=TRACK_LED_FAST_BLINK;

                ledController->getData()->resetLed(syncroDryContact);
                ledController->invalidate();

                if(
                        motionButtons[MOTION_BUTTON_REVERS_PLAY]->isClicked()||
                        motionButtons[MOTION_BUTTON_PLAY]->isClicked()||
                        synchroModule->isSlave1PlayComand()
                  )
                {
                    state=MOTION_MENU_PREROL;
                    return;
                }

                if(motionButtons[MOTION_BUTTON_STOP]->isClicked()||synchroModule->isSuccessComand())
                {
                    motionControlSubsystem->resetModule();
                    state=MOTION_MENU_READY;
                    return;
                }
                resetButtonsClick();


                break;

            case MOTION_MENU_PLAY_PAUSE:
                ledsState[motionControlSubsystem->getCurrentTrackID()].state=TRACK_LED_COMPLEX_BLINK;

                ledController->getData()->resetLed(syncroDryContact);
                ledController->invalidate();

                if(!motionControlSubsystem->isPaused())
                {
                    state=MOTION_MENU_READY;
                    motionControlSubsystem->resetModule();
                    return;
                }
                if(motionButtons[MOTION_BUTTON_PLAY]->isClicked()||synchroModule->isMasterPlayComand()||synchroModule->isSlave1PlayComand())
                {
                    motionControlSubsystem->pause();
                    state=MOTION_MENU_PLAY;
                    return;
                }
                if(motionButtons[MOTION_BUTTON_STOP]->isClicked()||synchroModule->isSuccessComand())
                {
                    motionControlSubsystem->stopPlay();
                    state=MOTION_MENU_READY;
                    return;
                }
                resetButtonsClick();


                return;

            case MOTION_MENU_REVERS_PLAY_PAUSE:
                ledsState[motionControlSubsystem->getCurrentTrackID()].state=TRACK_LED_COMPLEX_BLINK;

                ledController->getData()->resetLed(syncroDryContact);
                ledController->invalidate();

                if(!motionControlSubsystem->isPaused())
                {
                    state=MOTION_MENU_READY;
                    motionControlSubsystem->resetModule();
                    return;
                }
                if(motionButtons[MOTION_BUTTON_REVERS_PLAY]->isClicked())
                {
                        motionControlSubsystem->pause();
                        state=MOTION_MENU_REVERS_PLAY;
                        return;
                }
                if(motionButtons[MOTION_BUTTON_STOP]->isClicked())
                {
                    motionControlSubsystem->stopPlay();
                    state=MOTION_MENU_READY;
                    return;
                }
                resetButtonsClick();


                return;
        //---------------------------------------------------------------------------------------------------
                //rec logic
            case MOTION_MENU_READY_TO_REC:
                ledsState[motionControlSubsystem->getCurrentTrackID()].state=TRACK_LED_COMPLEX_BLINK;



                ledController->getData()->resetLed(syncroDryContact);
                ledController->invalidate();

                if(!motionControlSubsystem->isReady())
                {
                    motionControlSubsystem->resetModule();
                }
                if(motionButtons[MOTION_BUTTON_STOP]->isClicked())
                {
                    state=MOTION_MENU_READY;
                    return;
                }
                if((pult.joysticUsed())||(motionButtons[MOTION_BUTTON_PLAY]->isClicked())||synchroModule->isRecComand())
                {
                    motionControlSubsystem->rec();
                    state=MOTION_MENU_REC;
                    return;
                }
                resetButtonsClick();



                return;

            case MOTION_MENU_REC:
                ledsState[motionControlSubsystem->getCurrentTrackID()].state=TRACK_LED_FAST_BLINK;

                ledController->getData()->setLed(syncroDryContact);
                ledController->invalidate();

                if(!motionControlSubsystem->isRec())
                {
                    motionControlSubsystem->resetModule();
                    state=MOTION_MENU_READY;
                }
                if(motionButtons[MOTION_BUTTON_STOP]->isClicked()||synchroModule->isSuccessComand())
                {
                    motionControlSubsystem->stopRec();
                    state=MOTION_MENU_READY;
                    return;
                }
                resetButtonsClick();


                return;
            default:
                motionControlSubsystem->resetModule();
                ledController->getData()->resetLed(syncroDryContact);
                ledController->invalidate();
                state=MOTION_MENU_READY;
                resetButtonsClick();
                return;
        }

}


