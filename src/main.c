#include "stm32f4xx.h"
#include "usbd_cdc_core.h"
#include "usbd_cdc_vcp.h"
#include "usbd_usr.h"
#include "usbd_desc.h"

#include "enc_input_capture.h"
#include "pwm_out.h"
#include "Comm.h"
#include "HW_Config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;


/* Extern variables ----------------------------------------------------------*/

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTick);
uint16_t VCP_DataTx   (uint8_t* Buf, uint32_t Len);

/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : main.
* Description    : Main routine.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/

extern uint8_t usb_open;

int main(void)
{
//	InputMsg inMsg;
//	char sendText[5] = "test";
	
  uint32_t i = 0;  
	
	HW_Config();

	
	
	/* 1ms period SysTick interrupt */
	if (SysTick_Config(SystemCoreClock / 100))
  { 
    /* Capture error */ 
    while (1);
  }
	

  USBD_Init(&USB_OTG_dev,
#ifdef USE_USB_OTG_HS 
  USB_OTG_HS_CORE_ID,
#else            
  USB_OTG_FS_CORE_ID,
#endif  
  &USR_desc, 
  &USBD_CDC_cb,
  &USR_cb);

// PWM out test
  //PWM_Set(0.3, 0.3);

  while (1)
  {
    if (i++ == 0x100000)
    {
    	//STM32F4_Discovery_LEDToggle(LED4);  /* GREEN LED TOGGLE */
    	i = 0;

			/*
			if(usb_open)
			{
				VCP_DataTx((uint8_t*)sendText, 4);
			}
			*/
    }
  }
}

/**
  * @brief  
  * @param 
  * @retval : 
  */
void Delay(__IO uint32_t nTick)
{
  for(; nTick != 0; nTick--);
}


#ifdef USE_FULL_ASSERT
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {}
}
#endif

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
