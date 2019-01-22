/*
 * TransferFunction2P2Z.h
 *
 *  Created on: 05 февр. 2015 г.
 *      Author: Александр
 */

#ifndef TRANSFERFUNCTION2P2Z_H_
#define TRANSFERFUNCTION2P2Z_H_

class Controller {
public:
	virtual float calculate(volatile float input);
	virtual void setPI(volatile float P, volatile float I);
	virtual void reset();
	virtual void setMin(volatile float min);
	virtual void setMax(volatile float max);
};

class FirstOrderTransferFunction {
protected:
	volatile float in[2];
	volatile float out[2];
	volatile float b[2];
	volatile float a[2];
	volatile float min;
	volatile float max;
public:
    FirstOrderTransferFunction();
    virtual float calculate(volatile float input);
    void setMin(volatile float min);
    void setMax(volatile float max);
    void reset();
};

class FirstOrderLpf:public FirstOrderTransferFunction {
private:
    volatile float T0;
public:
    volatile float T;
    FirstOrderLpf();
	FirstOrderLpf(volatile float T0, volatile float T);
    void setTimeConstant(volatile float T);
    void setPeriodTime(volatile float T0);
    void setStartConditions(volatile float out_1);
};

class Median3Lpf {
private:
	volatile float in[3];
public:
    Median3Lpf();
    float calculate(volatile float input);
    void reset();
};

class SecondOrderTransferFunction {
protected:
	volatile float in[3];
	volatile float out[3];
	volatile float b[3];
	volatile float a[3];
	volatile float min;
	volatile float max;
public:
    SecondOrderTransferFunction();
    float calculate(volatile float input);
    void reset();
    void setMin(volatile float min);
    void setMax(volatile float max);
};



class Controller2P2Z:public Controller {
protected:
	volatile float P;
	volatile float I;
	volatile float in[3];
	volatile float out[3];
	volatile float b[3];
	volatile float a[3];
public:
	volatile float min;
	volatile float max;
	Controller2P2Z();
	Controller2P2Z(volatile float P, volatile float I);
	void setPI(volatile float P, volatile float I);
	float calculate(volatile float input);
	void reset();
	void setMin(volatile float min);
	void setMax(volatile float max);
};

class Controller1P1Z:public Controller {
protected:
	volatile float P;
	volatile float I;
	volatile float in[2];
	volatile float out[2];
	volatile float b[2];
	volatile float a[2];
public:
	volatile float min;
	volatile float max;
	Controller1P1Z();
	void setPI(volatile float P, volatile float I);
	float calculate(volatile float input);
	void reset();
	void setMin(volatile float min);
	void setMax(volatile float max);
};

class Controller2P2ZDry:public Controller2P2Z {
public:
	float calculate(volatile float input);
};

class Controller3P3Z:public Controller {
protected:
	volatile float P;
	volatile float I;
	volatile float in[4];
	volatile float out[4];
	volatile float b[4];
	volatile float a[4];
	volatile float min;
	volatile float max;
public:
	Controller3P3Z();
	void setPI(volatile float P, volatile float I);
	float calculate(volatile float input);
	void reset();
	void setMin(volatile float min);
	void setMax(volatile float max);
};


class TiltController:public Controller {
private:
	Controller2P2Z ctrl1P1Z;
	Controller2P2Z ctrl1P1Z2;

	FirstOrderLpf lpf1;
	FirstOrderLpf lpf2;

	float input1;
public:
	TiltController();
	void setPI(volatile float P, volatile float I);
	float calculate(volatile float input);
	void reset();
	void setMin(volatile float min);
	void setMax(volatile float max);
};



#endif /* TRANSFERFUNCTION2P2Z_H_ */
