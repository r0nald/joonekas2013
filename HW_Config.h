/**
  ******************************************************************************
  * @file    Joonekas/HW_Config.h 
  * @author  Indrek Tubalkain
  * @version V1.0.0
  * @date    30-October-2013   
  * @brief   
  ******************************************************************************  
  */ 

/* Includes ----------------------------------------------------------------- */
  #include "stm32f4xx.h"

/* Defines ------------------------------------------------------------------ */
  #define CoreClk           (168000000)
	
	#define TIM3_Period     	(5000)          
  #define TIM3_Prescaler  	((uint16_t) ((CoreClk /2) / 21000000) - 1)           







/* Function prototypes ------------------------------------------------------ */
  void HW_Config(void);


  //void ADC_Config(void);
		
	void GPIO_Config(void);		
  void PWM_TIM_Config(void);
	void USART6_Config(void);
	
