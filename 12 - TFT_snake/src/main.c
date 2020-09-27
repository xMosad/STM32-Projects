/*
 * main.c
 *
 *  Created on: Aug 13, 2020
 *      Author: mosad
 */

/************* Includes ***************/
/* Library */
#include <stdio.h>
#include "STD_TYPES.h"
#include "BIT_MATH.h"

/* MCAL */
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "STK_interface.h"
#include "SPI_interface.h"

/* APP */
#include "SnakeAPP_interface.h"

/* Services */
#include "OS_interface.h"


/************** APP functions ************/
void main(void){

	/* Initialize the RCC to HSE */
	RCC_voidInitSysClock();
	/* Enable the clock for all ports */
	RCC_voidEnableClock(RCC_APB2 , IOPA_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , SPI1_PERIPHERAL);

	/* Configure pins for SPI */
	GPIO_voidSetPinMode(PORTA , PIN7 , GPIO_OUTPUT_10MHZ_AFPP); //MOSI
	GPIO_voidSetPinMode(PORTA , PIN6 , GPIO_INPUT_FLOATING); //MISO
	GPIO_voidSetPinMode(PORTA , PIN5 , GPIO_OUTPUT_10MHZ_AFPP); //SCK

	/* Configure pins for TFT */
	GPIO_voidSetPinMode(PORTA , PIN1 , GPIO_OUTPUT_10MHZ_PP);
	GPIO_voidSetPinMode(PORTA , PIN2 , GPIO_OUTPUT_10MHZ_PP);

	/* Initialize STK */
	STK_voidInit();

	// master - idle high - write first - msb - 8bit format - sw m slave ,full duplex , f/2 / disable ISR
	SPI_config_t SPI1_config =
		{ 1 , 1 , 1 , 0 , 0 , 1, 0 ,0 ,0 };
	SPI_u8ConfigureCh(SPI1 , &SPI1_config );

	/* Initialization for the snake game */
	SnakeAPP_voidInitScreen();

	while(1){
		/* Call dispatcher */
		OS_voidDispatcher();
	}

}







