#include "joonekas.h"

#include "Comm.h"
#include "pwm_out.h"
#include "adc.h"
#include "dma_uart.h"
#include "controller1.h"
#include "line_sens.h"

static uint32_t time = 0;
static InputMsg inMsg;

void Joonekas_SysTick(void)
{
	OutputMsg 			outMsg;
	uint16_t 				txPacketLen;	
	LineSenseOut		lineSense;
	Controller1Out 	controllerOut;
		
	/**
		* Good reflection gives ADC value ~16, bad reflection 40-60.
		*/
	outMsg.lineSensors = ADC_GetLineSensing(30);
	
	lineSense = LS_Feedback(outMsg.lineSensors);
	outMsg.pidFeedback = lineSense.feedback;
	outMsg.usedSensors = lineSense.usedLinePatt;
	
	if(Comm_NewMsg())
	{
		inMsg = Comm_LastMsg();

		switch(inMsg.cmdType)
		{
			case Stop:
				Driver_Enable(0, 0);
				PWM_Set(0, 0);
				Controller1_Reset();
				break;
			case SetPwm:
				Driver_Enable(1, 1);
				PWM_Set(inMsg.leftPwm, inMsg.rightPwm);
				outMsg.pwmLeft = inMsg.leftPwm; outMsg.pwmRight = inMsg.rightPwm;
				break;
			case SetPid:
				// Set PID params.
				// Check if stopped?
				break;
			case Run:
				Driver_Enable(1, 1);
				controllerOut = Controller1_Run(outMsg.pidFeedback);
				outMsg.pwmLeft = controllerOut.pwmLeft; outMsg.pwmRight = controllerOut.pwmRight;
				PWM_Set(outMsg.pwmLeft, outMsg.pwmRight);
				break;
		}
	}
	
	time++;
	outMsg.time = time;
	outMsg.adc1 = ADC_GetAdcReading(0);
	

#ifdef USE_STM32F4_DISCOVERY
	Comm_SendOutMsg(&outMsg);
#endif
	
	Comm_OutMsgToStr(&outMsg, (char*)Comm_TxBuffer, &txPacketLen);
	DmaUart_StartTx();
}
