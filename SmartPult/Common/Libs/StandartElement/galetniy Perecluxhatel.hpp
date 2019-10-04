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

enum  {
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
	int8_t min;
	int8_t max;
	int8_t actualPosition;


public:
	GaletniyPerecluxhatel(int8_t min=off, int8_t max=twelve)  {
	    this->actualPosition=min;
	    this->min=min;
	    this->max=max;
		}

	void stepLeft () {

		actualPosition--;
		if (actualPosition<min)
		    actualPosition=min;

		}

	void stepRight () {
		actualPosition++;
		if (actualPosition>max)
		    actualPosition=max;


		}

	bool setNeedPosition (int8_t needPosition) {
		if (needPosition< min)
			return false;
		if (needPosition> max)
			return false;
		actualPosition=needPosition;
		return true;
		}

	inline int8_t getActualPosition () { return actualPosition;}

	};

}


#endif /* STANDATRNIEELEMENTI_GALETNIY_PERECLUXHATEL_HPP_ */
