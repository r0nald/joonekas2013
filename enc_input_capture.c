#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"

void ENC_TIM_Config(void)
{
  GPIO_InitTypeDef 	GPIO_InitStructure;
  NVIC_InitTypeDef 	NVIC_InitStructure;
	TIM_ICInitTypeDef	TIM_ICInitStructure;
  
  /* TIM2 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* GPIOB clock enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
  /* TIM2 channel 3 pin (PB.10) configuration */
  GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_UP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* TIM2 channel 4 pin (PB.11) configuration */
  GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_11;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  /* Connect TIM pins to AF */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2);
  
  /* Enable the TIM2 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
	TIM_ICInitStructure.TIM_Channel 		= TIM_Channel_3;
  TIM_ICInitStructure.TIM_ICPolarity 	= TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	
  TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel 		= TIM_Channel_4;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);

  /* Enable the CC3&4 Interrupt Request */
  TIM_ITConfig(TIM2, TIM_IT_CC3, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC4, ENABLE);
}
