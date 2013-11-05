#include "stm32f4xx.h"

void DmaUart_StartTx(void)
{
	/* Clear DMA Transfer Complete Flags */
  DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_TCIF6);
	DMA_ClearFlag(DMA2_Stream6,DMA_FLAG_HTIF6);
  /* Clear USART Transfer Complete Flags */
  USART_ClearFlag(USART6,USART_FLAG_TC);
	/* DMA transfer should start now */
	DMA_Cmd(DMA2_Stream6, ENABLE);
}
