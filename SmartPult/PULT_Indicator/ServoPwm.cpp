/*
 *  Pwm.cpp
 *  Created on: 17 џэт. 2015 у.
 *      Author: CNO
 */

#include <PULT_Indicator/ServoPwm.h>
#include <xdc/runtime/System.h>

#define PWM_SCALE 65535.0f

Pwm::Pwm(UInt8 pwmId, UInt16 periodTime) {
	PWM_Params_init(&params);
	params.period = periodTime;
	params.dutyMode = PWM_DUTY_SCALAR;

	pwm = PWM_open(pwmId, &params);
	if (pwm == NULL) {
		System_abort("PWM0 did not open");
	}
	sign = 1.0;
	setDuty(0.0);
}

void Pwm::setDuty(float duty) {
    UInt16 resultDuty;
    resultDuty = (UInt16)(PWM_SCALE*duty);
    PWM_setDuty(pwm, resultDuty);
}
