/*
 * TransferFunction2P2Z.cpp
 *
 *  Created on: 05 февр. 2015 г.
 *      Author: Александр
 */

#include <../GyConCommon/ControlSystem/TransferFunctions.h>
#define CONTROL_PERIOD 0.00083

float Controller::calculate(volatile float input) {
    return 0.0;
}
void Controller::setPI(volatile float P, volatile float I) {
}
void Controller::reset() {
}
void Controller::setMin(volatile float min) {
}
void Controller::setMax(volatile float max) {
}

FirstOrderTransferFunction::FirstOrderTransferFunction() {
	b[0] = 0;
	b[1] = 0;
	a[0] = 0;
	a[1] = 0;
	reset();
}

void FirstOrderTransferFunction::reset() {
	in[0] = 0;
	in[1] = 0;
	out[0] = 0;
	out[1] = 0;
}

void FirstOrderTransferFunction::setMin(float min) {
	this->min = min;
}

void FirstOrderTransferFunction::setMax(float max) {
	this->max = max;
}

float FirstOrderTransferFunction::calculate(float input) {
	in[0] = input;
	out[0] = b[0] * in[0] + b[1] * in[1] + a[1] * out[1];
	if (out[0] > max)
		out[0] = max;
	if (out[0] < min)
		out[0] = min;
	in[1] = in[0];
	out[1] = out[0];
	return out[0];
}

FirstOrderLpf::FirstOrderLpf():FirstOrderTransferFunction(),
		T0(0.001){
	min = -1000000;
	max = 1000000;
	setTimeConstant(0.0);
}

FirstOrderLpf::FirstOrderLpf(float T0, float T) :
		FirstOrderTransferFunction(), T0(T0) {
	min = -1000000;
	max = 1000000;
	setTimeConstant(T);
}

void FirstOrderLpf::setTimeConstant(float T) {
	b[0] = T0 / (T + T0);
	b[1] = 0;
	a[1] = T / (T + T0);
	this->T = T;
}

void FirstOrderLpf::setPeriodTime(volatile float T0){
	this->T0 = T0;
}

void FirstOrderLpf::setStartConditions(volatile float out_1) {
	out[1] = out_1;
}

SecondOrderTransferFunction::SecondOrderTransferFunction() {
	b[0] = 0;
	b[1] = 0;
	b[2] = 0;
	a[0] = 0;
	a[1] = 0;
	a[2] = 0;
	min=-100000;
	max = 100000;
}

void SecondOrderTransferFunction::reset() {
	in[0] = 0;
	in[1] = 0;
	in[2] = 0;
	out[0] = 0;
	out[1] = 0;
	out[2] = 0;
}

float SecondOrderTransferFunction::calculate(float input) {
	in[0] = input;
	out[0] = b[0]*in[0] + b[1]*in[1] + b[2]*in[2] + a[1]*out[1] + a[2]*out[2];
	if (out[0] > max)
		out[0] = max;
	if (out[0] < min)
		out[0] = min;
	in[2] = in[1];
	in[1] = in[0];
	out[2] = out[1];
	out[1] = out[0];
	return out[0];
}



void SecondOrderTransferFunction::setMin(float min) {
	this->min = min;
}
void SecondOrderTransferFunction::setMax(float max) {
	this->max = max;
}
Controller2P2Z::Controller2P2Z() {
	reset();
}
Controller2P2Z::Controller2P2Z(volatile float P, volatile float I) {
	reset();
    setPI(P,I);
}
void Controller2P2Z::setPI(volatile float P, volatile float I) {
    this->P = P;
    this->I = I;
	b[0] = 1;
	b[1] = -2 * (1 - I);
	b[2] = (1 - I) * (1 - I);
	a[0] = 0;
	a[1] = 2;
	a[2] = -1;
}
float Controller2P2Z::calculate(volatile float input) {
	in[0] = P*input;
	out[0] = b[0] * in[0] + b[1] * in[1] + b[2] * in[2] + a[1] * out[1]
			+ a[2] * out[2];
	if (out[0] > max)
		out[0] = max;
	if (out[0] < min)
		out[0] = min;
	in[2] = in[1];
	in[1] = in[0];
	out[2] = out[1];
	out[1] = out[0];
	return out[0];
}
void Controller2P2Z::reset() {
	in[0] = 0;
	in[1] = 0;
	in[2] = 0;
	out[0] = 0;
	out[1] = 0;
	out[2] = 0;
}
void Controller2P2Z::setMin(volatile float min) {
    this->min = min;
}
void Controller2P2Z::setMax(volatile float max) {
    this->max = max;
}

Controller1P1Z::Controller1P1Z() {
	reset();
}

void Controller1P1Z::setPI(volatile float P, volatile float I) {
    this->P = P;
    this->I = I;
	b[0] = P;
	b[1] = - P * (1 - I);
	a[1] = 1;
}
float Controller1P1Z::calculate(volatile float input) {
	in[0] = input;
	out[0] = b[0] * in[0] + b[1] * in[1] + a[1] * out[1];
	if (out[0] > max)
		out[0] = max;
	if (out[0] < min)
		out[0] = min;
	in[1] = in[0];
	out[1] = out[0];
	return out[0];
}
void Controller1P1Z::reset() {
	in[0] = 0;
	in[1] = 0;
	out[0] = 0;
	out[1] = 0;
}
void Controller1P1Z::setMin(volatile float min) {
    this->min = min;
}
void Controller1P1Z::setMax(volatile float max) {
    this->max = max;
}
