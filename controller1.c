#include "controller1.h"
#include <math.h>
static Controller1 controllerObj;

Controller1Out Controller1_Run(float fb)
{
	Controller1Out out;
	
	out.uk 	= fb * controllerObj.settings.P;
	controllerObj.iSum += controllerObj.settings.I * fb; 
	out.ui = controllerObj.iSum;
	out.ud 	= (fb - controllerObj.prevErr) * controllerObj.settings.D;
	
	out.u 	= out.uk + out.ui + out.ud;
	out.u 	= fabs(out.u) * (out.u < 0 ? -1.0 : 1.0);
	
	out.pwmLeft 	= controllerObj.settings.basePwm - out.u;
	out.pwmRight	= controllerObj.settings.basePwm + out.u;
	
	return out;	
}

void Controller1_Reset(void)
{
	controllerObj.iSum 		= 0;	
	controllerObj.prevErr = 0;
}
