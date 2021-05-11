/*
 * PanBar.hpp
 *
 *  Created on: 15 июл. 2020 г.
 *      Author: Кирилл Чеверев
 */

#ifndef EXTRENALDEVICESYNCHRO_PANBAR_PANBAR_HPP_
#define EXTRENALDEVICESYNCHRO_PANBAR_PANBAR_HPP_


#include <Pult/PultClasses.h>

#include <stdint.h>


namespace ExtrenalDevices {

union ButtonPanBar {
    uint32_t all;
    struct {
        uint8_t panBarError             :1;
        uint8_t motorsBtn               :1;
        uint8_t levelSetupBtn           :1;
        uint8_t irisEnBtn               :1;
        uint8_t focusEnBtn              :1;
    }BtnData;
};

class IPanBarChannel {
public:
    virtual void setInputValue(int32_t* data) = 0;
};

class PanBarSpeedChannel:public JoyChannelBase,public IPanBarChannel  {
private:

    float inputValue;

public:

    void setInputValue(int32_t* data) {
        this->inputValue=*((float*)(&data[0]));
    }

     virtual float getCurrentAdcValue() {
         return inputValue;
     }

     float * inpVal() {
         return &inputValue;
     }

     virtual float getCalibrationOffsetValue() {
         return 0.0;
     }

};

class PanBarPositionChannel:public IPanBarChannel {
private:

    float inputValue;
    bool enable;

public:

    void setInputValue(int32_t* data) {
            this->inputValue=*((float*)(&data[0]));
        }

    float getValue() {
        return inputValue;}

    bool isEnable() {   return enable;  }

    void _enable () {    enable= true;   }

    void disable () {   enable= false;  }

};



enum BtnId {
    AxisButton=0, MotorsButton,ShakerButton, CameraStartButton,LensCalibrateButton,
    LevelCorrectButton,LevelSetupButton
};

class PanBarButtonChannel:public IPanBarChannel {
private:
    ButtonPanBar inputValue;
    bool panBarError, motorsBtn,LevelSetupBtn,irisEnableBtn,focusEnableButton;

public:

    PanBarButtonChannel():panBarError(false),motorsBtn(false),LevelSetupBtn(false),
    irisEnableBtn(false),focusEnableButton(false)
    {

    }

    void setInputValue(int32_t* data) {
            this->inputValue.all = data[0];
            panBarError = (bool)inputValue.BtnData.panBarError;
            motorsBtn = (bool)inputValue.BtnData.motorsBtn;
            LevelSetupBtn=(bool)inputValue.BtnData.levelSetupBtn;
            irisEnableBtn=(bool)inputValue.BtnData.irisEnBtn;
            focusEnableButton=(bool)inputValue.BtnData.focusEnBtn;
        }

    bool panBarIsError()    {   return panBarError;     }

    bool motorsBtnIsEnable()    {   return motorsBtn;     }

    bool LevelSetupBtnEnable()    {   return LevelSetupBtn;     }

    bool irisEnableBtnEnable()    {   return irisEnableBtn;     }

    bool focusEnableButtonEnable()    {   return focusEnableButton;     }

};

enum PanBarChannel {
    panChannel=0,tiltChannel,dutchChannel,zoomChannel,focusChannel,irisChannel,btnChannel,offsetChannel
};

enum PanBarSpeedChannels {
    panSpeedChannel=0,tiltSpeedChannel,dutchSpeedChannel,zoomSpeedChannel,offsetSpeedChannel
};

enum PanBarPositionChannels {
    focusPositionChannel=5,irisPositionChannel,offsetPositionChannel
};

class IPanBarEZ {
public:
    virtual void updateEz() = 0;
};

class PanBarEnable: public IPanBarEZ {

private:
    PanBarSpeedChannel& pan;
    PanBarSpeedChannel& tilt;
    PanBarSpeedChannel& dutch;
    PanBarSpeedChannel& zoom;
    PanBarPositionChannel& focus;
    PanBarPositionChannel& iris;
public:
    PanBarEnable(PanBarSpeedChannel& pan,
    PanBarSpeedChannel& tilt,
    PanBarSpeedChannel& dutch,
    PanBarSpeedChannel& zoom,
    PanBarPositionChannel& focus,
    PanBarPositionChannel& iris):pan(pan),tilt(tilt),
    dutch(dutch),zoom(zoom),focus(focus),iris(iris)
    {

    }

    virtual void updateEz() {
        pan.enable();
        tilt.enable();
        dutch.enable();
        zoom.enable();
        focus._enable();
        iris._enable();
    }
};

class PanBarDisable: public IPanBarEZ {

private:
    PanBarSpeedChannel& pan;
    PanBarSpeedChannel& tilt;
    PanBarSpeedChannel& dutch;
    PanBarSpeedChannel& zoom;
    PanBarPositionChannel& focus;
    PanBarPositionChannel& iris;

public:

    PanBarDisable(PanBarSpeedChannel& pan,
    PanBarSpeedChannel& tilt,
    PanBarSpeedChannel& dutch,
    PanBarSpeedChannel& zoom,
    PanBarPositionChannel& focus,
    PanBarPositionChannel& iris):pan(pan),tilt(tilt),
    dutch(dutch),zoom(zoom),focus(focus),iris(iris)
    {

    }

    virtual void updateEz() {
        pan.disable();
        tilt.disable();
        dutch.disable();
        zoom.disable();
        focus.disable();
        iris.disable();
    }
};

class PanBar:public JoyChannelBase {
private:
    PanBarSpeedChannel pan;
    PanBarSpeedChannel tilt;
    PanBarSpeedChannel dutch;
    PanBarSpeedChannel zoom;
    PanBarPositionChannel focus,iris;
    PanBarButtonChannel button;
    PanBarEnable enebleChannel;
    PanBarDisable disableChannel;
    IPanBarChannel* panBarChannels[7];
    JoyChannelIF* panbarSpeedChannel[4];
    IPanBarEZ * ezChannel[2];
    uint8_t counter;
    bool connect;

public:

    PanBar():button(),enebleChannel(pan,tilt,dutch,zoom,focus,iris),
    disableChannel(pan,tilt,dutch,zoom,focus,iris)
    {
        panBarChannels[panChannel]=&pan;
        panBarChannels[tiltChannel]=&tilt;
        panBarChannels[dutchChannel]=&dutch;
        panBarChannels[zoomChannel]=&zoom;
        panBarChannels[focusChannel]=&focus;
        panBarChannels[irisChannel]=&iris;
        panBarChannels[btnChannel]=&button;
        panbarSpeedChannel[panSpeedChannel]=&pan;
        panbarSpeedChannel[tiltSpeedChannel]=&tilt;
        panbarSpeedChannel[dutchSpeedChannel]=&dutch;
        panbarSpeedChannel[zoomSpeedChannel]=&zoom;
        ezChannel[0] = &disableChannel;
        ezChannel[1] = &enebleChannel;
    }

    void setInputValue(int32_t* data) {
        for (uint8_t i = panChannel;i<offsetChannel;i++ ) {
            panBarChannels[i]->setInputValue(&data[i]);
        }
    }

    JoyChannelIF* getSpeedChannel (PanBarSpeedChannels ID) {
        return panbarSpeedChannel[ID];
    }

    PanBarButtonChannel& getButton() {
        return button;
    }

    float getFocusPosition ()
    {
        focus.getValue();
    }

    float getIrisPosition ()
        {
            iris.getValue();
        }

    void updateState (bool state) {
        ezChannel[state]->updateEz();
        isEnable=state;
    }

    virtual float getCurrentAdcValue()         {
        return 0.0;
    }
    virtual float getCalibrationOffsetValue()   {
        return 0.0;
    }

    bool isError()  {return button.panBarIsError();}

    bool isConnect () {return connect;}

    float* getPanSpeed()
    {
        return pan.inpVal();
    }

    float* getTiltSpeed()
    {
        return tilt.inpVal();
    }

    float* getRollSpeed()
    {
        return dutch.inpVal();
    }

    inline void updateConnect (uint32_t flagConnect)
        {
            if (flagConnect==1)
            {
                counter+=5;
                if (counter>100)
                {
                    counter=100;
                    connect=true;
                }
            }
            else
            {
                counter--;
                if (counter<1)
                {
                    counter=1;
                    connect=false;
                }
            }
        }

    virtual void enable() {
        this->isEnable=true;
        pan.enable();
        tilt.enable();
        dutch.enable();
        zoom.enable();
    }

    virtual void disable() {
        this->isEnable=false;
        pan.disable();
        tilt.disable();
        dutch.disable();
        zoom.disable();
    }

};

}


#endif /* EXTRENALDEVICESYNCHRO_PANBAR_PANBAR_HPP_ */
