#include "PWMLed.h"
void LEDInit()
{
	PWM0_P12_OUTPUT_ENABLE;//red
	PWM1_P11_OUTPUT_ENABLE;//green
	PWM2_P10_OUTPUT_ENABLE;//blue
  PWM_IMDEPENDENT_MODE;
	PWM_CLOCK_DIV_8;
	PWMPH = 0x30;
	PWMPL = 0x0f;
	/*-------- PWM start run--------------*/ 
	set_PWMCON0_LOAD;
	set_PWMCON0_PWMRUN;
}

void LEDChange(UINT8 red,UINT8 green,UINT8 blue)
{
	PWM0H=255-red;
	PWM1H=255-green;
	PWM2H=255-blue;
}