/**
  ******************************************************************************
  * @file    Joonekas/HW_Config.c 
  * @author  Indrek Tubalkain
  * @version V1.0.0
  * @date    30-October-2013   
  * @brief   
  ******************************************************************************  
  */ 

/* Includes ------------------------------------------------------------------*/
  #include "HW_Config.h"
  #include "pwm_out.h"
  #include "Comm.h"

/* Private typedef -----------------------------------------------------------*/
  GPIO_InitTypeDef          GPIO_InitStructure;
  TIM_TimeBaseInitTypeDef  	TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  				TIM_OCInitStructure;
	NVIC_InitTypeDef          NVIC_InitStructure;
  USART_InitTypeDef         USART_InitStructure;
 

  
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/



/* Private functions ---------------------------------------------------------*/





/**
  * @brief  Configure hardware
  * @param  None
  * @retval None
  */  
void HW_Config(void)   
{

    /* GPIO clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    

	  GPIO_Config();	
	
		GPIOB->BSRRH = GPIO_Pin_14; // BT module in uart comm mode
		GPIOB->BSRRH = GPIO_Pin_15; // BT module RESET
	
	  //ADC_Config();
	  PWM_TIM_Config();
	  //ENC_TIM_Config();
	  
		USART6_Config();
	  
	  // LED TEST
	  GPIOA->BSRRL = GPIO_Pin_8; // 
		GPIOA->BSRRL = GPIO_Pin_9; // 
		GPIOA->BSRRL = GPIO_Pin_10; // 
	
	
		Comm_Init();
    Driver_Enable(1,1);					// Enable motor drivers
		GPIOB->BSRRL = GPIO_Pin_15; // BT module RUN
}  

/**
  * @brief  Configure GPIO
  * @param  None
  * @retval None
  */
void GPIO_Config(void) {

/* GPIO A config */	
	/* Configure ADC pins as analog input ***************************************/
	// LS4, LS3, LS5, LS2, LS1, RS1, RS2, RS3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | 
	                              GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | 
																GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	/* Configure pins as digital output *****************************************/
	// LOWBAT LED, DBG_LED2, DBG_LED1
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
/* GPIO B config */	
	/* Configure pins as digital input ******************************************/
	// BOOT1(BUTTON)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure pins as timer 2 input capture **********************************/	
  // ENC2, ENC1 
  GPIO_InitStructure.GPIO_Pin 	=  GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd 	= GPIO_PuPd_DOWN;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
  /* Connect TIM2 pins to AF */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_TIM2);

	/* Configure pins as digital output *****************************************/
  // HBINH_L, HBINH_R, BT_KEY, BT_RESET
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_14 | 
																GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* Configure pins as timer 3 PWM output *************************************/
	// left motor - TIM3_CH3 - PB0; right motor - TIM3_CH2 - PB5*/
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_0 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
  /* Connect TIM3 pins to AF */  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_TIM3); 


/* GPIO C config */	
	/* Configure ADC pins as analog input ***************************************/
	// BAT_ADC, RS4, RS5
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 

	/* Configure pins as digital output *****************************************/
  // BC_LED
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* Configure pins as USART 6 RX/TX ******************************************/
	// BT_RX, BT_TX
  GPIO_InitStructure.GPIO_Pin 	= GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
  /* Connect USART pins to AF */  
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6);
	
}		


/**
  * @brief  Configure TIMER 3
  * @param  None
  * @retval None
  */
void PWM_TIM_Config(void)
{

  /* TIM3 clock enable */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period 				= TIM3_Period;
  TIM_TimeBaseStructure.TIM_Prescaler 		= TIM3_Prescaler;
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




/**
  * @brief  Configure USART 6
  * @param  None
  * @retval None
  */
void USART6_Config(void)
{
 
	/* Enable UART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
	
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_2;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART6, &USART_InitStructure);
    
  /* Enable USART */
  USART_Cmd(USART6, ENABLE);
}


/*
void ADC_Config(void) {
  
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  
  / * PCLK2 is the APB2 clock * / 
  / * ADCCLK = PCLK2/6 = 72/6 = 12MHz* / 
  //RCC_ADCCLKConfig(RCC_PCLK2_Div6); 
  
  / * Enable ADC1 clock so that we can talk to it * / 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); 
  / * Put everything back to power-on defaults * / 
  ADC_DeInit(); 
  
  

  
  
  / * ADC Common Init ********************************************************** /
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);
  
  
  / * ADC1 Init **************************************************************** /
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);
  
}
*/








