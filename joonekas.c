#include "joonekas.h"

#include "Comm.h"
#include "pwm_out.h"

static uint32_t time = 0;
static InputMsg inMsg;

void Joonekas_SysTick(void)
{
	OutputMsg outMsg;
	
	
	if(Comm_NewMsg())
	{
		inMsg = Comm_LastMsg();
		PWM_Set(inMsg.leftPwm, inMsg.rightPwm);
	}
	
	time++;
	outMsg.time = time;
	outMsg.lineSensors = 0;
	outMsg.fval = inMsg.leftPwm;
	Comm_SendOutMsg(&outMsg);
}
