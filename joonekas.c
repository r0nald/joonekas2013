#include "joonekas.h"

#include "Comm.h"
#include "pwm_out.h"
#include "adc.h"
#include "dma_uart.h"

static uint32_t time = 0;
static InputMsg inMsg;

void Joonekas_SysTick(void)
{
	OutputMsg outMsg;
	uint16_t 	txPacketLen;	
	
	if(Comm_NewMsg())
	{
		inMsg = Comm_LastMsg();

		switch(inMsg.cmdType)
		{
			case Stop:
				PWM_Set(0, 0);
				// Reset PID
				break;
			case SetPwm:
				PWM_Set(inMsg.leftPwm, inMsg.rightPwm);
				break;
			case SetPid:
				// Set PID params.
				// Check if stopped?
				break;
			case Run:
				// Main controller
		}
	}
	
	time++;
	outMsg.time = time;
	outMsg.lineSensors = 0;
	outMsg.pwmLeft = inMsg.leftPwm;
	outMsg.pidF = (float)ADC_GetLineSensor(0) / (float)(1<<5);
	Comm_SendOutMsg(&outMsg);
	
	Comm_OutMsgToStr(&outMsg, (char*)Comm_TxBuffer, &txPacketLen);
	DmaUart_StartTx();
}
