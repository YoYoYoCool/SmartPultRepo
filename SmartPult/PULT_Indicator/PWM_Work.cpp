#include <PULT_Indicator/PWM_Work.h>

static Pwm* pwmBright;

void init_PWM_Bright(Pwm* pwmBright)
{
	::pwmBright = pwmBright;

//    //SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_PLL);
//
//    // Set the PWM clock to the system clock.
//    SysCtlPWMClockSet(SYSCTL_PWMDIV_16);
//
//    // The PWM peripheral must be enabled for use.
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
//
//    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
//
//    GPIOPinConfigure(GPIO_PF1_M0PWM1);
//    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1);
//
//    // Configure the PWM0 to count up/down without synchronization.
//    PWMGenConfigure(PWM0_BASE, PWM_GEN_0, PWM_GEN_MODE_UP_DOWN |
//                    PWM_GEN_MODE_NO_SYNC);
//
//    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_0, 64000);
//    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 64000 / 2);
//
//    // Enable the PWM0 Bit0 (PD0) output signal.
//    //
//    PWMOutputState(PWM0_BASE, PWM_OUT_1_BIT, true);
//
//    // Enable the PWM generator block.
//    PWMGenEnable(PWM0_BASE, PWM_GEN_0);
}

void SetBrightness(uint8_t val)
{
	if(val>99) val=99;
	pwmBright->setDuty((float)val/100);
    //PWMPulseWidthSet(PWM0_BASE, PWM_OUT_1, 64000*val / 100);
}
