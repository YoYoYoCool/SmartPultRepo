#ifndef _PWM_H_
#define _PWM_H_


#include <stdbool.h>
#include <stdint.h>

#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"
#include "driverlib/sysctl.h"
#include <PULT_Indicator/ServoPwm.h>
#include "Board.h"



void init_PWM_Bright(Pwm* pwmBright);
void SetBrightness(uint8_t val);

#endif //_PWM_H_
