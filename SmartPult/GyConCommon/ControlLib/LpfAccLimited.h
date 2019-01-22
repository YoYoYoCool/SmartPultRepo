/*
 * LpfAccLimited.h
 *
 *  Created on: 22 окт. 2016 г.
 *      Author: I
 */

#ifndef GYCONCOMMON_CONTROLSYSTEM_LPFACCLIMITED_H_
#define GYCONCOMMON_CONTROLSYSTEM_LPFACCLIMITED_H_

class LpfAccLimited {
private:
	float k;
	float t0;
	float out;
	float upLim;
	float dwLim;
public:
	LpfAccLimited(float fCtf, float T0);
	float calc(float input);
	void reset();
	void setLimits(float upDwLim);
};

#endif /* GYCONCOMMON_CONTROLSYSTEM_LPFACCLIMITED_H_ */
