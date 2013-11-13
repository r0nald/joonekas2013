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
	controllerObj.prevErr = fb;
	
	out.u 	= out.uk + out.ui + out.ud;
	
	out.pwmLeft 	= controllerObj.settings.basePwm - out.u;
	out.pwmRight	= controllerObj.settings.basePwm + out.u;
	
	return out;	
}

void Controller1_Reset(void)
{
	controllerObj.iSum 		= 0;	
	controllerObj.prevErr = 0;
}

void 	Controller1_Init(void)
{
	// For 2C LiPo
	//controllerObj.settings.basePwm = 0.4;
	//controllerObj.settings.P = 0.07;
	//controllerObj.settings.I = 0.00005;
	//controllerObj.settings.D = 0.7;
	
	controllerObj.settings.basePwm = 0.25;
	controllerObj.settings.P = 0.06;
	controllerObj.settings.I = 0.00005;
	controllerObj.settings.D = 0.15;
}

void 	Controller1_SetConf(float basePwm, float P, float I, float D)
{
	controllerObj.settings.basePwm = basePwm;
	controllerObj.settings.P = P;
	controllerObj.settings.I = I;
	controllerObj.settings.D = D;
}
