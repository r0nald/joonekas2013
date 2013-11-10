#include "stm32f4xx_adc.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_dma.h"

#define DMA_CHANNELx        			DMA_Channel_0
#define DMA_STREAMx         			DMA2_Stream0
#define ADCx                			ADC1

#define ADCx_DR_Address    ((uint32_t)0x4001204C) // ADC1_BASE + 0x4C?
#define BufferLenght       10

__IO uint16_t ADC1ConvertedValue[BufferLenght];

uint32_t	ADC_GetLineSensing(uint16_t threshold)
{
	uint16_t i;
	uint32_t lineSens = 0;
	
	for(i = 0 ; i < BufferLenght ; i++)
	{
		if(ADC1ConvertedValue[i] > threshold)
		{
			lineSens |= (1<<i);
		}
	}
	
	return lineSens;	
}

uint16_t 	ADC_GetAdcReading(uint8_t channel)
{
	return ADC1ConvertedValue[channel];
}

void ADC_Config(void)
{
	ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef   		DMA_InitStructure;
	GPIO_InitTypeDef      GPIO_InitStructure;
	
	/* Enable ADCx, DMA and GPIO clocks ****************************************/ 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	
	/* Configure GPIO **********************************************************/ 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
   /* DMA2 channel0 configuration ---------------------------------------------*/
  DMA_DeInit(DMA_STREAMx);
	DMA_InitStructure.DMA_Channel 						= DMA_CHANNELx;
  DMA_InitStructure.DMA_PeripheralBaseAddr 	= ADCx_DR_Address;
  DMA_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)ADC1ConvertedValue;
  DMA_InitStructure.DMA_DIR 								= DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize 					= BufferLenght;
  DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc 					= DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize 	= DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode 								= DMA_Mode_Circular; 
  DMA_InitStructure.DMA_Priority 						= DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode 						= DMA_FIFOMode_Disable; 				// Good choice?
	DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_HalfFull;		// Relevant with FIFO disabled?
	DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst 		= DMA_PeripheralBurst_Single;
  DMA_Init(DMA_STREAMx, &DMA_InitStructure);

  /* Enable DMA channel */
  DMA_Cmd(DMA_STREAMx, ENABLE);

  /* ADC common configuration -------------------------------------------------*/
	ADC_CommonInitStructure.ADC_Mode 						= ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler 			= ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode 	= ADC_DMAAccessMode_1;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay= ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  /* ADC Init *****************************************************************/
  ADC_InitStructure.ADC_Resolution 						= ADC_Resolution_6b;
  ADC_InitStructure.ADC_ScanConvMode 					= ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode 		= ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge 	= ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_ExternalTrigConv 			= ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign 						= ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion 			= BufferLenght;
  ADC_Init(ADCx, &ADC_InitStructure);

  /* ... */ 
  ADC_RegularChannelConfig(ADCx, ADC_Channel_2, 1, ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(ADCx, ADC_Channel_0, 2, ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(ADCx, ADC_Channel_1, 3, ADC_SampleTime_3Cycles);
  ADC_RegularChannelConfig(ADCx, ADC_Channel_3, 4, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_4, 5, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_5, 6, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_6, 7, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_7, 8, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_14, 9, ADC_SampleTime_3Cycles);
	ADC_RegularChannelConfig(ADCx, ADC_Channel_15, 10, ADC_SampleTime_3Cycles);
	
	/* Enable DMA request after last transfer (Single-ADC mode) */
  ADC_DMARequestAfterLastTransferCmd(ADCx, ENABLE);
		
  /* Enable ADC1 DMA */
	ADC_DMACmd(ADCx, ENABLE);

  /* Enable ADC1 */
  ADC_Cmd(ADCx, ENABLE);

  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConv(ADCx);
}
