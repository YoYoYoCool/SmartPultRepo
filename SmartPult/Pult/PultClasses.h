/*
 * Button.h
 *
 *  Created on: 14 июля 2015 г.
 *      Author: Cno
 */

#ifndef GYCONCOMMON_PERIPHERY_PULTCLASSES_H_
#define GYCONCOMMON_PERIPHERY_PULTCLASSES_H_
#include <../GyConCommon/DataTypes.h>
#include <../GyConCommon/Utils/Collections.h>
#include <../GyConCommon/ControlSystem/TransferFunctions.h>
#include "Board.h"


#define PULT_BUTTON_DETECT_LEVEL 3
#define PULT_BUTTON_HOLD_DETECT_LEVEL 30
//#define PULT_BUTTON_DETECT_LEVEL 2
//#define PULT_BUTTON_HOLD_DETECT_LEVEL 23


typedef enum PultButtonStates {
    PRESSED = 0, // нажата
	RELESASED, // отпущена
	HOLD

} PultButtonStates;

class PultButton {
public:
   volatile bool buttonFix;
   volatile bool clickedFlag;
   volatile bool doubleClickFlag;
   volatile bool hardRelesased;
   volatile bool hardPressed;
   volatile bool hardHold;
   volatile PultButtonStates state;
   UInt8 id;
	PultButton() {
		stateCounter = PULT_BUTTON_DETECT_LEVEL;
		holdCounter = PULT_BUTTON_HOLD_DETECT_LEVEL;
		state = RELESASED;
		buttonFix = false;
		clickedFlag = false;
		doubleClickFlag=false;
		hardRelesased=false;
		hardPressed=false;
		hardHold=false;
	}

	PultButton(UInt8 id) {
		stateCounter = PULT_BUTTON_DETECT_LEVEL;
		holdCounter = PULT_BUTTON_HOLD_DETECT_LEVEL;
		state = RELESASED;
		buttonFix = false;
		clickedFlag = false;
		doubleClickFlag=false;
        hardRelesased=false;
        hardPressed=false;
        hardHold=false;
		this->id = id;
	}


	void update(bool state)
	{
	    if (hardRelesased) {
	        this->state=RELESASED;
	        return;
	        }
        if (hardPressed) {
            this->state=PRESSED;
            return;
            }
        if (hardHold) {
            this->state=HOLD;
            return;
            }
		if (state == RELESASED)
		{
			if (this->state!=RELESASED)
			{
				if (stateCounter < PULT_BUTTON_DETECT_LEVEL){stateCounter++;}
				else
				{
					if(this->state==HOLD){doubleClickFlag=true;	}
					clickedFlag = true;
					this->state = RELESASED;
				}
			}
		}
		else
		{
			if (this->state == RELESASED)
			{
				if (stateCounter > 0) {	stateCounter--;	}
				else
				{
					buttonFix = true;
					clickedFlag = false;
					this->state = PRESSED;
				//	GPIO_write(Debug_Pin_PN5, Board_PIN_HIGH);
				}
			}
		}
		if (this->state == PRESSED)
		{
			if (holdCounter > 0) {	holdCounter--;	}
			else
			{
				this->state = HOLD;
            	clickedFlag = false;
            	doubleClickFlag=false;
             }
        }
		else
		{
            	holdCounter = PULT_BUTTON_HOLD_DETECT_LEVEL;
        }
	}
	bool isPressed() {
		return ((state == PRESSED)||(state == HOLD));
	}
	bool isClicked() {
		bool clickedFlagTmp = clickedFlag;
		clickedFlag = false;
		return clickedFlagTmp;
	}
	bool isDoubleClick()
	{
		bool tmp=doubleClickFlag;
		doubleClickFlag=false;
		return tmp;
	}
	void clearFlags()
	{
		clickedFlag = false;
		doubleClickFlag=false;
	}



private:
	Int16 stateCounter;
	Int16 holdCounter;
};


class Resistor {
public:
float offset;
float adcValue;
volatile float K;
volatile float K0;
volatile float value;
    Resistor() {
    	K=1;
    	offset = 2048;
    	value = 0;
    	K0=1;
    }
    Resistor(float K, UInt16 offset):
    	offset(offset)
	{
    	this->K=K;
    	K0=K;
    	value = 0;
    }
	Resistor(UInt16 offset):
		offset(offset) {
		K=1;
		K0=1;
		value = 0;
	}
    virtual void calculate(UInt32 adcValue) {
        this->adcValue=(float)adcValue;
    	value = K*((float)adcValue - offset);
    }
    void setInversOrientation()
    {
    	this->K = -K0;
    }
    void setNormalOrientation()
    {
    	this->K = K0;
    }
};

class VirtualMenuResistor:public Resistor
{
public:
	 VirtualMenuResistor(): Resistor()
	 {
		 value=0.5;
	 }
	 VirtualMenuResistor(float val): Resistor()
	     {
	         value=val;
	     }
	 void setValue(float v){value=v;}
	 virtual void calculate(UInt32 adcValue)
	 {
//		 value=0.5;
	 }
};

class DummyResistor:public Resistor
{
public:
	 DummyResistor(): Resistor()
	 {
		 value=0.0;
	 }
	 virtual void calculate(UInt32 adcValue)
	 {
		 value=0.0;
	 }
};

class JoyStickFunction {
public:
	virtual float calculate(float x) {
		return 0.0;
	}
};

class LinearFunc: public JoyStickFunction {
private:
    float K;
    float min;
    float max;
public:
    LinearFunc(float K, float min, float max):
	K(K),
	min(min),
	max(max){
		K=1;
	}
    float calculate(float x) {
		float res = K*x;
		if (res>max) {
			res = max;
		}
		if (res<min){
			res = min;
		}
        return res;
	}
};


class CubitorFunc: public JoyStickFunction {
private:
    float K;
    float min;
    float max;
public:
	CubitorFunc(float K, float min, float max):
	K(K),
	min(min),
	max(max){
		K=1;
	}
    float calculate(float x) {
		float res = K*x*x*x;
		if (res>max) {
			res = max;
		}
		if (res<min){
			res = min;
		}
        return res;
	}
};

class QuadratorFunc: public JoyStickFunction {
public:
	float K;
	const float min;
	const float max;
	QuadratorFunc(float K, float min, float max):
		    K(K),
			min(min),
			max(max) {
	}
	virtual float calculate(float x) {

		float sign = 1;
		if (x<0) sign = -1;
		float res = K*(x*x)*sign;
		if (res>max) {
			res = max;
		}
		if (res<min){
			res = min;
		}
        return res;
	}
};

#define JoyStick_controlPeriod 0.01
class JoyStick {
public:
	JoyStickFunction* volatile func;
	Resistor* speedControl;
	Resistor* driftControl;
	Resistor* fluidControl;
	volatile float offset;
	const float maxValue;
	float kSpeed0;
	float kSpeedMax;
	float Kcontrol;
	const float minFluid;
	volatile float K;
	volatile bool enabled;
	FirstOrderLpf fluidLpf;
	volatile float K0;
	volatile bool joyUsed;
	JoyStick(float K, float offset, float maxValue, JoyStickFunction* func, Resistor* speedControl, Resistor* driftControl,
			Resistor* fluidControl):
		func(func),
		offset(offset),
		speedControl(speedControl),
		driftControl(driftControl),
		fluidControl(fluidControl),
		minFluid(0.001),
		fluidLpf(JoyStick_controlPeriod, minFluid),
		maxValue(maxValue){
//		speedControl->offset = 0;
//		speedControl->K = 0.00037;
//		driftControl->offset = 1861;
//		driftControl->K = 0.0005;
//		fluidControl->offset = 0;
//		fluidControl->K = 0.001;
		kSpeed0 = 0.5;
		kSpeedMax = 2.0;
		this->K0 = K;
		this->K = K;
		enabled = false;
		joyUsed=false;
	}

	//TODO нужно вызывать предидущий конструктор
	JoyStick(float K, float offset, float maxValue, JoyStickFunction* func, Resistor* speedControl, Resistor* driftControl, float Kdrift,
			Resistor* fluidControl):
				func(func),
						offset(offset),
						speedControl(speedControl),
						driftControl(driftControl),
						fluidControl(fluidControl),
						minFluid(0.01),
						fluidLpf(JoyStick_controlPeriod, minFluid),
						maxValue(maxValue)
	{
//		speedControl->offset = 0;
//		speedControl->K = 0.00037;
//		driftControl->offset = 1861;
//		driftControl->K = Kdrift;
//		fluidControl->offset = 0;
//		fluidControl->K = 0.001;
		kSpeed0 = 0.5;
		kSpeedMax = 2.0;
		this->K0 = K;
		this->K = K;
		this->enabled = false;
		joyUsed=false;
	}
	bool getUsedFlag(){return joyUsed;}
	void setOffset(float val)	{	this->offset=val;		}
	float getOffsset()			{	return this->offset;	}

	float getValue(float var)
	{
		var-=driftControl->value;
        if (var>maxValue){ var = maxValue;}
        if (var<-maxValue){ var = -maxValue;}
        return var;
	}
	float getValue()
	{
		value-=driftControl->value;
        if (value>maxValue){ value = maxValue;}
        if (value<-maxValue){ value = -maxValue;}
        return value;
	}

	float getRawValue()
	{
		return value;
	}
	inline float getSpeed()
	{
		float Ktmp = kSpeed0 + speedControl->value;
		if (Ktmp>kSpeedMax)
		{
		      	Ktmp = kSpeedMax;
		}
		return Ktmp;
	}

	void calculate(UInt32 adcValue)
	{
		float res = ((float)adcValue - offset);
		res = deadZone(res, 30);
        res = calcFluid(res);
		res = res*K;

		if (!enabled) res = 0.0;

        if(res!=0.0)
        {
        	joyUsed=true;
        }
        else{joyUsed=false;}

        float tmpValue = func->calculate((getSpeed()*res)/maxValue)*maxValue;
        if (tmpValue>maxValue) tmpValue = maxValue;
        if (tmpValue<-maxValue) tmpValue = -maxValue;
        value = tmpValue;
	}
	void setInversOrientation() {	this->K = -K0;	}
	void setNormalOrientation() {	this->K = K0;	}
	void enable() {		enabled = true;	}
	void disable() {    enabled = false;}
	void relativeInvers()
	{
		K0=-K0;
		K=-K;
	}


private:
	volatile float value;
	float calcFluid(float input)
	{
		float fluid = minFluid + fluidControl->value;
		fluidLpf.setTimeConstant(fluid);
		return fluidLpf.calculate(input);
	}
	float deadZone(float value, float dz)
	{
		float sign = 1.0;
		if (value < 0) {
			sign = -1.0;
			value = -value;
		}
		if (value < dz) {
			value = 0;
		} else {
			value = value - dz;
		}
		return sign * value;
	}
};

//------------- MultiJoystic -------------------
class JoyChannelIF
{
    public:
        virtual float getCurrentAdcValue()          =0;
        virtual float getCalibrationOffsetValue()   =0;

        virtual bool isEnable_()    =0;
        virtual void enable()       =0;
        virtual void disable()      =0;
};

class JoyChanel:public JoyChannelIF
{
protected:
	volatile float K;
	volatile float K0;
	volatile float speed0;
	volatile float speedMax;
	volatile float offset;
	volatile float calibrationOffset;
	volatile float deadZoneValue;
	volatile bool isEnable;
	Resistor* speedControl;

	FirstOrderLpf filter;
	FirstOrderLpf calibrationFilter;


	volatile float adcValue;

	float deadZone(float value, float dz) {
			float sign = 1.0;
			if (value < 0) {
				sign = -1.0;
				value = -value;
			}
			if (value < dz) {
				value = 0;
			} else {
				value = value - dz;
			}
			return sign * value;
		}

public:


	JoyChanel(float K, float offset,Resistor* speedControl,float deadZone, float T):
		offset(offset),
		speedControl(speedControl),
		deadZoneValue(deadZone),
		filter(0.002,T),
		calibrationFilter(JoyStick_controlPeriod, 0.75)
	{
		this->K=K;
		this->K0=K;
		speed0 = 0.0;//last 0.5
		speedMax = 2.0;
		isEnable=true;
		calibrationOffset=1860;
	}

	bool isEnable_(){return this->isEnable;}
	void enable(){this->isEnable=true;}
	void disable(){this->isEnable=false;}
	void setInversOrientation() {K=-K0;}
	void setNormalOrientation() {K=K0;}
	void setOffset(float val)	{	offset=val;		}
	float getOffsset()			{	return offset;	}
	void setDeadZone(float dz)	{	deadZoneValue=dz;}
	float getDeadZone()			{	return deadZoneValue;}
	void relativeInvers()
	    {
		K0=-K0;
		K=-K;
	    }

	void setRef(UInt32 ref)
	{
		adcValue=(float)ref;
		calibrationOffset=calibrationFilter.calculate((float)(adcValue));
	}

	inline float getSpeed()
	{
		float Ktmp = speed0 + speedControl->value;
		if (Ktmp>speedMax)
		{
		      	Ktmp = speedMax;
		}
		return Ktmp;
	}
	virtual float getCurrentAdcValue()
	{
		//float rez=adcValue-offset;
		float rez=filter.calculate(adcValue-offset);
		rez=deadZone(rez, deadZoneValue);
		rez=rez*K*getSpeed();
		if(!this->isEnable){rez=0.0;}
		return rez;
	}

	virtual float getCalibrationOffsetValue()
	{
		offset=calibrationOffset;
		return calibrationOffset;
	}
};

class HallEffectJoyChannel:public JoyChanel
{
const float desconnectedValue;
public:
	 HallEffectJoyChannel(float K, float offset,Resistor* speedControl,float deadZone, float discValue, float T):
		 JoyChanel(K,offset,speedControl,deadZone,T),
		 desconnectedValue(discValue){}
	virtual float getCurrentAdcValue()
	{
		if(adcValue<desconnectedValue)
		{
			return 0.0;
		}
//		float rez=adcValue-offset;
		float rez=filter.calculate(adcValue-offset);
		rez=deadZone(rez, deadZoneValue);
		rez=rez*K*getSpeed();
		if(!this->isEnable){rez=0.0;}
		return rez;
	}
	float getCalibrationOffsetValue()
	{
		if(calibrationOffset<desconnectedValue)
		{
			offset=2110;
			return offset;
		}
		offset=calibrationOffset;
		return calibrationOffset;
	}
};


class WeelJoyChannel:public JoyChanel
{
	JoyStickFunction* corrector;
public:
	WeelJoyChannel(float K, float offset,Resistor* speedControl,float deadZone,JoyStickFunction* cor):
		 JoyChanel(K,offset,speedControl,deadZone,0),
		 corrector(cor)
		{

		}
	virtual float getCurrentAdcValue()
	{
		float tmp;
		if(adcValue<deadZoneValue){return 0.0;}
		float rez=adcValue-offset;
		rez=deadZone(rez, deadZoneValue);
		if(rez<0.0){tmp=-rez;}
		else{tmp=rez;}
		rez=rez*(K+1*corrector->calculate(tmp));
		rez*=getSpeed();
		if(!this->isEnable){rez=0.0;}
		return rez;
	}
};

class MultiJoystic
{
public:
	List<JoyChannelIF*>* channels;
	JoyStickFunction* volatile func;
	Resistor* driftControl;
	Resistor* fluidControl;
	const float maxValue;
	const float minFluid;
	volatile bool enabled;
	FirstOrderLpf fluidLpf;
	volatile float revers;
	volatile bool joyUsed;
	volatile UInt32 startFluid;//Percents

	MultiJoystic(float maxValue, JoyStickFunction* func, Resistor* driftControl,Resistor* fluidControl,List<JoyChannelIF*>* channels):
		func(func),
		driftControl(driftControl),
		fluidControl(fluidControl),
		minFluid(0.001),
		fluidLpf(JoyStick_controlPeriod, minFluid),
		maxValue(maxValue),
		channels(channels),
		driftLpf(0.01, 0.333)
	{
//		driftControl->offset = 1861;
//		driftControl->K = 0.0005;
//		fluidControl->offset = 0;
//		fluidControl->K = 0.001;
		enabled = false;
		revers=1.0;
		joyUsed=false;
		driftValueFiltred=0.0;
		startFluid=50;
	}

	bool getUsedFlag(){return joyUsed;}
	void setStartFluid(UInt32 value)
	{
		if(value>=100){startFluid=100;return;}
		startFluid=value;
	}

	void calculate(){
		float res=0;
		for(UInt8 i=0;i!=channels->getSize();i++)
		{
			if(channels->get(i)->isEnable_())
			{
				res+=channels->get(i)->getCurrentAdcValue();
			}
		}

        res = calcFluid(res);
        res*=revers;
        if (!enabled) res = 0.0;

        if(res>0.2||res<-0.2)
        {
        	joyUsed=true;
        }
        else
        {
        	joyUsed=false;
        }
        value = func->calculate(res/maxValue)*maxValue;
        driftValueFiltred=this->calcDrift(driftControl->value);
	}
	void setInversOrientation() {	revers=-1.0;}
	void setNormalOrientation() {	revers=1.0; }
	void relativeRevers(){revers=-revers;}
	void enable() {	enabled = true;	}
	void disable() {enabled = false;}
	float getValue(float var)
	{

		var=var-driftValueFiltred;
        if (var>maxValue){ var = maxValue;}
        if (var<-maxValue){ var = -maxValue;}
        return var;
	}
	float getValue()
	{
		float value=this->value - driftValueFiltred;;
        if (value>maxValue){ value = maxValue;}
        if (value<-maxValue){ value = -maxValue;}
        return value;
	}
	float getRawValue(){return value;}

private:
	FirstOrderLpf driftLpf;
	volatile float driftValueFiltred;
	volatile float value;
	float calcFluid(float input)
	{
		float fluid = minFluid + fluidControl->value;
		fluidLpf.setTimeConstant(fluid);
		return fluidLpf.calculate(input);
	}
	float calcDrift(float input) {
		return driftLpf.calculate(input);
	}
};

class JoyChannels
{
public:
	StaticList<JoyChannelIF*,5> channels;
	JoyChannels(JoyChannelIF** array, UInt32 count);
};



#endif /* GYCONCOMMON_PERIPHERY_PULTCLASSES_H_ */
