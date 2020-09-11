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

/* HAL */
#include "AUDIO_interface.h"
#include "LEDMRX_interface.h"

/* services */
#include "OS_interface.h"

/* Prototypes */
void voidDance(void);

/* user defined */
typedef enum {
	TO_RIGHT,
	TO_LEFT
}state_t;

/* Global variables */
u8 Frames[4][8] = {
		{24, 214, 62, 214, 48, 0, 0, 0},
		{0, 176, 86, 62, 86, 152, 0, 0},
		{0, 0, 24, 214, 62, 214, 48, 0},
		{0, 0, 0, 176, 86, 62, 86, 152}
};
volatile u8 global_u8Index = 0 ;
volatile state_t global_state ;

void main(void){

	/* Initialize the RCC to HSE */
	RCC_voidInitSysClock();
	/* Enable the clock for all ports */
	RCC_voidEnableClock(RCC_APB2 , IOPA_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , IOPB_PERIPHERAL);

	/* H.W initialize */
	LEDMRX_voidInit();
	AUDIO_voidInit();

	/* Initialize the os */
	OS_voidStart();

	/* Tasks */
	OS_u8CreateTask(0  , 0 , 125 , AUDIO_voidSetDAC );
	OS_u8CreateTask(1  , 250 , 2500 , LEDMRX_voidDisplay );
	OS_u8CreateTask(2  , 0 , 500000 , voidDance );

	while(1){
		OS_voidDispatcher();
	}

}

/* Periodic task to make the dance on led matrix*/
void voidDance(void){
	LEDMRX_voidSetData( (&Frames[global_u8Index][0] ));

	switch (global_state){
		case TO_RIGHT :
			global_u8Index++;
			if (global_u8Index == 3) {
				global_state = TO_LEFT ;
			}
			break ;
		case TO_LEFT:
			global_u8Index--;
			if (global_u8Index == 0) {
				global_state = TO_RIGHT ;
			}
			break ;
	}
}







