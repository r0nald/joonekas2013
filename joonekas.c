#include "joonekas.h"

#include "Comm.h"
#include "pwm_out.h"

#include "stm32f4xx_gpio.h"

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
	outMsg.lineSensors = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)<<4 
													| GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_10);
	
	Comm_SendOutMsg(&outMsg);
}
