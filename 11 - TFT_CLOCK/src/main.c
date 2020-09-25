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

/* Hal */
#include "TFT_interface.h"

/**************** Prototypes *****************/
void voidClockMainScreen (void);
void counting( void );
void voidUpdateDate (void);


/**************** Data types ******************/
typedef enum {
	CLOCK_AM ,
	CLOCK_PM
}CLK_state_t;

/************* Global variables ******************/
volatile u8 global_u8SecondsCounter = 52 ;
volatile u8 global_u8MinCounter = 59;
volatile u8 global_u8HoursCounter = 11 ;
volatile u8 global_u8DaysCounter = 25 ;
volatile u8 global_u8MonCounter = 9 ;
volatile u16 global_u16YearCounter = 2020 ;
volatile u16 global_u8DayIndex = 0 ;
volatile CLK_state_t global_state = CLOCK_PM ;

const u8 *days[7] = {"FRI" , "SAT" , "SUN" , "MON" , "TUS" , "WED" , "THUR" };

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

	/* Initialize TFT */
	TFT_voidInit();

	/* Set Background */
	TFT_voidFillDisplay(0xffff);

	/* Set main screen */
	voidClockMainScreen();

	/* Set the timing task */
	STK_voidSetIntervalPeriodic(1000 , TIME_MS , counting);
	while(1){

	}

}

/* The opening screen */
void voidClockMainScreen (void)
{
	s8 loacl_s8TimeBuffer[20] ;
	s8 loacl_s8DateBuffer[20] ;
	/* Header */
	TFT_voidPrintText( "Digital"  , 0 , 0  , 3 ,TFT_BLUE , TFT_WHITE);
	TFT_voidPrintText( " CLOCK" , 0 , 30 , 3 ,TFT_BLUE,TFT_WHITE );

	/* Line */
	TFT_voidDrawRectangle(0 , 55 , 127 , 5 , TFT_GREEN);

	/* Time */
	sprintf(loacl_s8TimeBuffer, "%.2i:%.2i:%.2iAM", global_u8HoursCounter , global_u8MinCounter ,global_u8SecondsCounter );
	TFT_voidPrintText( loacl_s8TimeBuffer , 0 , 60 , 2,TFT_RED,TFT_WHITE );

	/* Line  2*/
	TFT_voidDrawRectangle(0 , 85 , 127 , 5 , TFT_GREEN);

	/* Date */
	sprintf(loacl_s8DateBuffer, "%.2i/%.2i/%.4i", global_u8DaysCounter , global_u8MonCounter ,global_u16YearCounter);
	TFT_voidPrintText( loacl_s8DateBuffer , 0 , 90 , 2,TFT_ORANGE,TFT_WHITE );

	/* Line 3 */
	TFT_voidDrawRectangle(0 , 115 , 127 , 5 , TFT_GREEN);

	/* Day */
	TFT_voidPrintText(days[global_u8DayIndex] , 40 , 120 , 2,TFT_PURPLE ,TFT_WHITE);
}

/* Counting task  , triggered by STK ISR every 1000 ms */
void counting( void ){
	global_u8SecondsCounter++;
	/* One minute passed */
	if (global_u8SecondsCounter == 60){
		global_u8MinCounter++;
		global_u8SecondsCounter = 0 ;

		/* One hour passed */
		if (global_u8MinCounter == 60){
			global_u8MinCounter = 0 ;
			global_u8HoursCounter++;

			/* One days passed */
			if (global_u8HoursCounter == 12){
				switch (global_state){
					case CLOCK_PM :
						/* Update the data values */
						voidUpdateDate();
						global_state = CLOCK_AM ;
						break ;
					case CLOCK_AM : global_state = CLOCK_PM ; break ;
				}
			}
			if (global_u8HoursCounter == 13){
				global_u8HoursCounter = 1 ;

			}
		}
	}

	/* Output the result */
	s8 loacl_s8Buffer[20] ;
	switch (global_state){
		case CLOCK_PM :
			sprintf(loacl_s8Buffer, "%.2i:%.2i:%.2iPM", global_u8HoursCounter , global_u8MinCounter ,global_u8SecondsCounter  );
			break ;
		case CLOCK_AM :
			sprintf(loacl_s8Buffer, "%.2i:%.2i:%.2iAM", global_u8HoursCounter , global_u8MinCounter ,global_u8SecondsCounter  );
			break ;
	}

	TFT_voidPrintText( loacl_s8Buffer , 0 , 60 , 2,TFT_RED,TFT_WHITE );
}



void voidUpdateDate (void)
{
	s8 loacl_s8DateBuffer[20] ;
	/* Increment to the next day name */
	global_u8DayIndex++;
	if (global_u8DayIndex == 7 ){
		global_u8DayIndex = 0 ;
	}

	global_u8DaysCounter++;
	/* Month passed */
	if (global_u8DaysCounter == 31){
		global_u8DaysCounter = 0 ;
		global_u8MonCounter++;

		/* Year passed */
		if (global_u8MonCounter == 13){
			global_u8MonCounter = 0 ;
			global_u16YearCounter++;
		}
	}

	/* Output day and date */
	sprintf(loacl_s8DateBuffer, "%.2i/%.2i/%.4i", global_u8DaysCounter , global_u8MonCounter ,global_u16YearCounter);
	TFT_voidPrintText( loacl_s8DateBuffer , 0 , 90 , 2,TFT_ORANGE,TFT_WHITE );

	TFT_voidPrintText(days[global_u8DayIndex] , 40 , 120 , 2,TFT_PURPLE,TFT_WHITE );

}






