/*
 * main.c
 *
 *  Created on: Aug 13, 2020
 *      Author: mosad
 */

/* Library includes */
#include "STD_TYPES.h"
#include "BIT_MATH.h"

/* MCAL includes */
#include "RCC_interface.h"
#include "STK_interface.h"


#include "STP_interface.h"

void LEDMEX_show (u8 *arr);

u8 arr[8] = {0, 0, 124, 2, 62, 2, 124, 0};
void main(void){

	/* Initialize the RCC to HSE */
	RCC_voidInitSysClock();

	/* Enable the clock for all ports */
	RCC_voidEnableClock(RCC_APB2 , IOPA_PERIPHERAL);

	/**/
	STK_voidInit();

	/* Initialize the STP */
	STP_voidInit();

	while(1){
		LEDMEX_show(arr);
		//STP_voidSendSynch(32515);
	}

}

void LEDMEX_show (u8 *arr){
	u16 local_u16Num ;
	u8 hold ;
	for (u8 i = 0 ; i < 8 ; i++){
		hold = 255 &  (~(1 << i));
		local_u16Num = arr[i] + (hold << 8);
		STP_voidSendSynch(local_u16Num);
		STK_voidSetBusyWait(2 , TIME_MS);
	}
}






