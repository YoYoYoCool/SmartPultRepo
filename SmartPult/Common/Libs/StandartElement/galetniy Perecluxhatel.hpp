/*
 * galetniy Perecluxhatel.hpp
 *
 *  Created on: 14 мая 2019 г.
 *      Author: User9
 */

#ifndef STANDATRNIEELEMENTI_GALETNIY_PERECLUXHATEL_HPP_
#define STANDATRNIEELEMENTI_GALETNIY_PERECLUXHATEL_HPP_




#include "stddef.h"
#include "stdint.h"
#include "stdio.h"

namespace StandartElement {

enum SwitchPosition {
	off=0,
	one,
	two,
	three,
	four,
	five,
	six,
	seven,
	eight,
	nine,
	ten,
	eleven,
	twelve
	};

class GaletniyPerecluxhatel {

private:
	SwitchPosition min;
	SwitchPosition max;
	SwitchPosition actualPosition;
	SwitchPosition oldStepPosition;

public:
	GaletniyPerecluxhatel(int8_t min=(int8_t)off, int8_t max=(int8_t)twelve)  {
	    this->actualPosition=((SwitchPosition)min);
	    this->min=((SwitchPosition)min);
	    this->max=((SwitchPosition)max);
		}

	void stepLeft () {
		oldStepPosition=actualPosition;
		int8_t newPosition=(int8_t)actualPosition;
		newPosition--;
		if (newPosition<(int8_t)min)
		    newPosition=(int8_t)min;
		actualPosition=(SwitchPosition)newPosition;
		}

	void stepRight () {
		oldStepPosition=actualPosition;
		int8_t newPosition=(int8_t)actualPosition;
		newPosition++;
		if (newPosition>(int8_t)max)
		    newPosition=(int8_t)max;
		actualPosition=(SwitchPosition)newPosition;

		}

	bool setNeedPosition (int8_t needPosition) {
		oldStepPosition=actualPosition;
		if (needPosition< min)
			return false;
		if (needPosition> max)
			return false;
		actualPosition=(SwitchPosition)needPosition;
		return true;
		}

	inline uint8_t getActualPosition () { return (uint8_t)actualPosition;}

	};

}


#endif /* STANDATRNIEELEMENTI_GALETNIY_PERECLUXHATEL_HPP_ */
