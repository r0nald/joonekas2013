
#include "HW_Config.h"
	
#include <stdlib.h>
#include <math.h>




/*
 *	Set motors PWM
 */
void PWM_Set(float left, float right)
{
	TIM_SetCompare3(TIM3, TIM3_Period * fmaxf(0.1, fminf(0.8, left)));
	TIM_SetCompare2(TIM3, TIM3_Period * fmaxf(0.1, fminf(0.8, right)));
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



	
 



