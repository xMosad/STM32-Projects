/*
 * main.c
 *
 *  Created on: Aug 13, 2020
 *      Author: mosad
 */

/* Library */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DELAY.h"

/* MCAL */
#include "RCC_interface.h"
#include "GPIO_interface.h"

/* HAL */
#include "LEDMRX_interface.h"
#include "REMOTE_interface.h"


/****** Prototypes *******/
void voidCheckRemote(void);
void voidTakeAction(void);

/***** Global variables ******/
u8 arr [30] = {0, 0, 124, 2, 62, 2, 124, 0, 0, 60, 66, 66, 60, 0, 0, 76, 82, 50, 0, 0, 124, 18, 18, 124, 0, 126, 66, 36, 24, 0};
u8 arr_zero[8] ={0 };
u16 global_u16Delay  = 200;
u8  global_power_flag = 0 ;
REMOTE_buttons_t global_pressedButton ;


void main(void){

	/* Initialize the RCC to HSE */
	RCC_voidInitSysClock();

	/* Enable the clock for all ports */
	RCC_voidEnableClock(RCC_APB2 , IOPA_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , IOPB_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , AFIO_PERIPHERAL);

	/* Initialize the hardware */
	LEDMRX_voidInit();
	REMOTE_voidInit();

	while(1){
		voidCheckRemote();
		voidTakeAction();
	}

}

/*
 * This function to observe the receiver in case some data came and notify the system
 * of the current state of the system
*/
void voidCheckRemote(void){
	global_pressedButton = REMOTE_getButton();
	if(global_pressedButton < REMOTE_NO_DATA){
		switch (global_pressedButton){
			case REMOTE_POWER :
				global_power_flag = 1 ;
				break;
			case REMOTE_P_RESUME :
				global_power_flag = 0 ;
				break;
			case REMOTE_VOL_UP :
				global_u16Delay += 100;
				break ;
			case REMOTE_VOL_DOWN :
				global_u16Delay -= 50;
				break ;
			default :
				break ;
		}
	}
}

/*
 * the system stay most of the time here and will manage what will happen in each state
*/
void voidTakeAction(void){
	if (global_power_flag){
		LEDMRX_voidDisplayShiftText(arr , global_u16Delay , 24);
	}
	else if (!(global_power_flag)){
		LEDMRX_voidDisplay (arr_zero);
	}
	else{

	}
}








