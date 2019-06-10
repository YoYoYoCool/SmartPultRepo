/*
 * TrigonometricFunction.hpp
 *
 *  Created on: 14 мая 2019 г.
 *      Author: User9
 */

#ifndef OBRABOTCHIC_TRIGONOMETRICFUNCTION_HPP_
#define OBRABOTCHIC_TRIGONOMETRICFUNCTION_HPP_


#include "StandartElement/galetniy Perecluxhatel.hpp"
#include "math.h"



namespace Function {

enum functionConnect {
    off=0,
    sqrtConnect,
	lineConnect,
	qrtConnect,
	qubConnect
	};

class TrigonometricBase {

public:
	virtual float calculate (float data) {
		return data;
		}
};

class TrigonometricYZero:public TrigonometricBase {

public:
    virtual float calculate (float data) {
        return 0.0;
        }
};

class TrigonometricSQRT:public TrigonometricBase {

public:

    virtual float calculate (float data) {
        return sqrt(data);
        }

};


class TrigonometricLine:public TrigonometricBase {

public:

	virtual float calculate (float data) {
		return data;
		}

};

class TrigonometricQrt: public TrigonometricBase {

	virtual float calculate (float data) {

			if (data<0)
				return data*data*-1;

			return data*data;
			}
	};

class TrigonometricQub: public TrigonometricBase {

	virtual float calculate (float data) {
		return data*data*data;
		}
	};

enum {maxFunction=5};
class Trigonometric {
private:
	TrigonometricBase* functionActive;
	TrigonometricYZero yZero;
	TrigonometricLine line;
	TrigonometricSQRT sqrt;
	TrigonometricQrt qrt;
	TrigonometricQub qub;
	TrigonometricBase* function[maxFunction];
	StandartElement::GaletniyPerecluxhatel galetnic;

public:

	Trigonometric(int8_t actualPosition=(int8_t)lineConnect): galetnic((int8_t)off,(int8_t)qubConnect ) {
	    galetnic.setNeedPosition(actualPosition);
		function[0]=&yZero;
		function[1]=&sqrt;
		function[2]=&line;
		function[3]=&qrt;
		function[4]=&qub;
		updateFunction();
		}

	void stepLeft() {
		galetnic.stepLeft();
		updateFunction ();
		}

	void stepRight() {
		galetnic.stepRight();
		updateFunction ();
		}

	void setFunction (int8_t functionName) {
		galetnic.setNeedPosition((int8_t)functionName);
		updateFunction ();
		}

	inline int8_t getFunction() {
	    return (int8_t)galetnic.getActualPosition();
	    }

	inline float updateData(float data) {
		return functionActive->calculate(data);
		}

private:

	void updateFunction () {
	    functionActive=function[(uint8_t)galetnic.getActualPosition()];
		}


	};

}

#endif /* OBRABOTCHIC_TRIGONOMETRICFUNCTION_HPP_ */
