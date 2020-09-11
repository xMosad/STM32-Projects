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
#include "NVIC_interface.h"

/* HAL */
#include "AUDIO_interface.h"
#include "REMOTE_interface.h"

/* Audio data */
#include "hello.h"
#include "help.h"
#include "hungry.h"
#include "water.h"

/* Constants */
#define    HUNGRY_LEN      6612
#define    WATER_LEN       5549
#define    HELP_LEN        5195
#define    HELLO_LEN       5667

/* Global Variables */
REMOTE_buttons_t button ;

void main(void){

	/* Initialize the RCC to HSE */
	RCC_voidInitSysClock();

	/* Enable the clock for all ports */
	RCC_voidEnableClock(RCC_APB2 , IOPA_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , IOPB_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , AFIO_PERIPHERAL);

	/* Enable RCC for timer 3 and enable interrupt */
	RCC_voidEnableClock(RCC_APB1 , TIM3_PERIPHERAL);
	NVIC_voidEnableIRQ(TIM3_IRQ);

	/* H.W initialize */
	REMOTE_voidInit();
	AUDIO_voidInit();

	while(1){
		button = REMOTE_getButton();
		if (button < REMOTE_NO_DATA){
			if (button == REMOTE_RIGHT){
				AUDIO_voidSetSong(hello ,HELLO_LEN );
			}
			else if (button == REMOTE_TWO){
				AUDIO_voidSetSong(help ,HELP_LEN );
			}
			else if (button == REMOTE_ZERO){
				AUDIO_voidSetSong(hungery ,HUNGRY_LEN );
			}
			else if (button == REMOTE_VOL_DOWN){
				AUDIO_voidSetSong(water ,WATER_LEN );
			}
			else {

			}
		}
	}

}








