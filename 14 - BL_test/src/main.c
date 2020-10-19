/*
 * main.c
 *
 *  Created on: Oct 19, 2020
 *      Author: mosad
 */
//D:\xBlank\COM\ARM\IMT_projects\04 - review\blink_test.txt
/* Library includes */
#include "STD_TYPES.h"
#include "BIT_MATH.h"

/* MCAL includes */
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "UART_interface.h"
#include "STK_interface.h"
#include "FPEC_interface.h"

/* Constants */
#define    BL_START_PAGE      8
#define    BL_MC_PAGES        64

/* Global Variables */
u8 global_u8ReceiveFlag ;
u8 global_u8Data[50];
u8 global_u8Index ;
u8 global_FirstRecordFlag = 1 ;

typedef void (*Function_t)(void);
Function_t addr_to_call = 0;

/* Proto types */
void BL_voidEraseApp (void);
void voidJumbToApp (void);
void Parser_voidParseRecord(u8* Copy_u8BufData);

void main(void){

	/* Initialize the RCC to HSE */
	RCC_voidInitSysClock();

	/* Enable the clock for all ports */
	RCC_voidEnableClock(RCC_APB2 , IOPA_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , USART1_PERIPHERAL);

	/* Initialize pins for UART module */
	GPIO_voidSetPinMode(PORTA , PIN9  , GPIO_OUTPUT_10MHZ_AFPP);  //TX
	GPIO_voidSetPinMode(PORTA , PIN10 , GPIO_INPUT_FLOATING);     //RX

	/* Initialize the stm32 used modules */
	UART_voidInit();
	STK_voidInit();

	STK_voidSetIntervalSingle(15000 , TIME_MS , voidJumbToApp);

	while(1)
	{
		global_u8ReceiveFlag = UART_voidRecDataConditional(UART1 , &(global_u8Data[global_u8Index]));

		if (global_u8ReceiveFlag)
		{
			/* Stop timer till processing the data */
			STK_voidStop();

			/* '\n' indicate the end of the record */
			if (global_u8Data[global_u8Index] == '\n')
			{
				/* With the coming of the first record will erase the App section */
				if (global_FirstRecordFlag)
				{
					BL_voidEraseApp();
					global_FirstRecordFlag = 0 ;
				}
				/* Parse Record */
				Parser_voidParseRecord(global_u8Data);
				/* Inform the burning app the record has come to send the next */
				UART_voidSendDataSynch(UART1 , "ok");
				/* Initialize the index for the new record */
				global_u8Index = 0 ;
			}
			else
			{
				global_u8Index++;
			}
			STK_voidSetIntervalSingle(15000 , TIME_MS , voidJumbToApp);
		}
		else
		{

		}


	}

}

/* To Erase the APP section before burning the new APP*/
void BL_voidEraseApp (void)
{
	for (u8 i = BL_START_PAGE ; i < BL_MC_PAGES ; i++){
		FPEC_voidErasePage(i);
	}
}

/* This function will be called when timeout happens to jump to
 * the APP section */
void voidJumbToApp (void)
{
	#define SCB_VTOR   *((volatile u32*)0xE000ED08)

	/* Relocate the vector table */
	SCB_VTOR = 0x08002000;

	/* Jump to application */
	addr_to_call = *(Function_t*)(0x08002004);
	addr_to_call();
}








