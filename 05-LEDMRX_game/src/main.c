/*
 * main.c
 *
 *  Created on: Aug 13, 2020
 *      Author: mosad
 */

/* Library  */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DELAY.h"

/* MCAL */
#include "RCC_interface.h"

/* HAL */
#include "LEDMRX_interface.h"
#include "REMOTE_interface.h"

/****** Prototypes ******/
void voidCheckRemote(void);
void voidTakeAction(void);

/***** global variables  ****/
// the led matrix frames
u8 arr [10][8] = {
		{28, 8, 42, 20, 8, 84, 20, 20},
		{28, 8, 42, 20, 8, 20, 100, 68},
		{28, 8, 42, 20, 8, 84, 36, 68},
		{28, 8, 42, 20, 72, 20, 36, 68},
		{28, 8, 42, 84, 8, 20, 36, 68},
		{28, 8, 106, 20, 8, 20, 36, 68},
		{28, 8, 42, 84, 8, 20, 36, 68},
		{28, 8, 42, 20, 72, 20, 36, 68},
		{28, 8, 42, 20, 8, 84, 36, 68},
		{28, 8, 42, 20, 8, 20, 100, 68}
};
u8 zero_arr[8];
REMOTE_buttons_t global_pressedButton;
u8 global_usPowerState;
u8 global_usPlayState ;

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
void voidCheckRemote(void)
{
	global_pressedButton = REMOTE_getButton();
	if(global_pressedButton < REMOTE_NO_DATA){
		switch (global_pressedButton){
			case REMOTE_POWER :
				global_usPowerState = ! (global_usPowerState);
				break;
			case REMOTE_RPT :
				global_usPlayState = 1 ;
				break;
			default :
				break;
		}
	}
}

/*
 * Take action based on the date came from receiver
*/
void voidTakeAction(void){
	if(global_usPowerState){
		if (global_usPlayState){
			for (u8 i = 0 ; i < 10 ; i ++){
				u8 delay = 10 ;
				while(delay--){
					LEDMRX_voidDisplay((&arr[i][0]));
				}
			}
			global_usPlayState = 0 ;
		}
		else {
			LEDMRX_voidDisplay((&arr[0][0]));
		}
	}
	else {
		LEDMRX_voidDisplay(zero_arr);
	}
}



