/*
 * main.c
 *
 *  Created on: Aug 13, 2020
 *      Author: mosad
 */

/* Library  */
#include "STD_TYPES.h"
#include "BIT_MATH.h"

/* MCAL */
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "TIMER_interface.h"
#include "NVIC_interface.h"

/* HAL */
#include "AUDIO_interface.h"
#include "REMOTE_interface.h"


/**/
REMOTE_buttons_t button ;
volatile u16 global_u16AudioFreq = 125 ;

void main(void){

	/* Initialize the RCC to HSE */
	RCC_voidInitSysClock();

	/* Enable the clock for all ports */
	RCC_voidEnableClock(RCC_APB2 , IOPA_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , IOPB_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , AFIO_PERIPHERAL);

	/* Enable RCC for timer and enable interrupt */
	RCC_voidEnableClock(RCC_APB1 , TIM3_PERIPHERAL);
	NVIC_voidEnableIRQ(TIM3_IRQ);

	/* MCAL initialize */
	TIMER_voidInit();

	/* H.W initialize */
	REMOTE_voidInit();
	AUDIO_voidInit();

	/* Set Audio as periodic task managed bt Timer 3*/
	TIMER_voidSetPeriodic(TIM3 , global_u16AudioFreq , AUDIO_voidSetDAC);

	while(1){
		button = REMOTE_getButton();
		if (button < REMOTE_NO_DATA){
			if (button == REMOTE_RIGHT){
				global_u16AudioFreq += 8 ;
				TIMER_voidSetPeriodic(TIM3 , global_u16AudioFreq , AUDIO_voidSetDAC);
			}
			else if (button == REMOTE_LEFT){
				global_u16AudioFreq -= 8 ;
				TIMER_voidSetPeriodic(TIM3 , global_u16AudioFreq , AUDIO_voidSetDAC);
			}
		}
	}

}








