/*
 * Pwm.h
 *
 *  Created on: 17 џэт. 2015 у.
 *      Author: CNO
 */

#ifndef SERVOPWM_H_
#define SERVOPWM_H_

#include <stdint.h>
#include <ti/drivers/PWM.h>
#include <../GyConCommon/dataTypes.h>
class Pwm {
public:
	float sign;
	PWM_Handle pwm;
	PWM_Params params;
	Pwm(UInt8 pwmId, UInt16 periodTime);
    void setDuty(float duty);
};

#endif /* SERVOPWM_H_ */
