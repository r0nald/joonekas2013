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
		PWM_Set(inMsg.leftPwm, inMsg.rightPwm);
	}
	
	time++;
	outMsg.time = time;
	outMsg.lineSensors = 0;
	outMsg.fval = inMsg.leftPwm;
	outMsg.fval2 = (float)ADC_GetLineSensor(0) / (float)(1<<5);
	Comm_SendOutMsg(&outMsg);
	
	Comm_OutMsgToStr(&outMsg, (char*)Comm_TxBuffer, &txPacketLen);
	DmaUart_StartTx();
}
