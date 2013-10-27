#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

#include <stdlib.h>
#include <math.h>

static uint32_t CounterPeriod = 1000;


/*
 *	
 */
void PWM_Set(float left, float right)
{
	TIM_SetCompare3(TIM3, CounterPeriod * fmaxf(0.0, fminf(1.0, left)));
	TIM_SetCompare2(TIM3, CounterPeriod * fmaxf(0.0, fminf(1.0, right)));
}


/*
 *	Enable or Disable Half bridge drivers
 */
void Driver_Enable(unsigned int LeftOn, unsigned int RightOn)
{

		if (LeftOn) {											// ON
				GPIOB->BSRRL = GPIO_Pin_1;
    }
		else {														// OFF
			  GPIOB->BSRRH = GPIO_Pin_1;
		}
		
		if (RightOn) {										// ON
				GPIOB->BSRRL = GPIO_Pin_4;
    }
		else {														// OFF
			  GPIOB->BSRRH = GPIO_Pin_4;
		}
 
}


/*
 *	Half bridge driver Enable pins configuration
 */
void Driver_EN_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	
  /* Configure driver enable GPIO pins */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
 
}

/*
 *	Taken from main.c of TIM_PWMOutput example project
 */
void PWM_TIM_Config(void)
{
  GPIO_InitTypeDef 					GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  				TIM_OCInitStructure;
	uint16_t PrescalerValue = 0;

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* GPIOC Configuration: TIM3: left motor - TIM3_CH3 - PB0; right motor - TIM3_CH2 - PB5*/
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP ;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 

  /* Connect TIM3 pins to AF2 */  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3); 
	
	/* Compute the prescaler value */
  PrescalerValue = (uint16_t) ((SystemCoreClock /2) / 21000000) - 1;

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period 				= CounterPeriod;
  TIM_TimeBaseStructure.TIM_Prescaler 		= PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode 	= TIM_CounterMode_Up;

  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel2 */
  TIM_OCInitStructure.TIM_OCMode 			= TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse 			= 0;
  TIM_OCInitStructure.TIM_OCPolarity 	= TIM_OCPolarity_High;

  TIM_OC2Init(TIM3, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
  TIM_OCInitStructure.TIM_Pulse 			= 0;

  TIM_OC3Init(TIM3, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(TIM3, ENABLE);
	
	TIM_SetCompare3(TIM3, 0);
	TIM_SetCompare2(TIM3, 0);

  /* TIM3 enable counter */
  TIM_Cmd(TIM3, ENABLE);
}
