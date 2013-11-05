#include "joonekas.h"

#include "Comm.h"
#include "pwm_out.h"
#include "adc.h"

#include "stm32f4xx.h"

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
	
	/* Clear DMA Transfer Complete Flags */
  DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
	DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_HTIF6);
  /* Clear USART Transfer Complete Flags */
  USART_ClearFlag(USART6,USART_FLAG_TC);
	/* DMA transfer should start now */
	DMA_Cmd(DMA2_Stream6, ENABLE);
}
