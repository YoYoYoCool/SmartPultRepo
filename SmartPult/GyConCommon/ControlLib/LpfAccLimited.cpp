/*
 * LpfAccLimited.cpp
 *
 *  Created on: 22 окт. 2016 г.
 *      Author: I
 */

#include <ControlLib/LpfAccLimited.h>


LpfAccLimited::LpfAccLimited(float fCtf, float t0) :
		t0(t0) {
	this->k = 2 * 3.1415 * fCtf;
	out = 0;
	dwLim = -100000;
	upLim = 100000;
}

float LpfAccLimited::calc(float input) {
	float acc = k * (input - out);
	if (acc > upLim)
		acc = upLim;
	if (acc < dwLim)
		acc = dwLim;
	out = out + t0 * acc;
	return out;
}

void LpfAccLimited::reset() {
	out = 0;
}

void LpfAccLimited::setLimits(float upDwLim) {
	this->dwLim = -upDwLim;
	this->upLim = upDwLim;
}

