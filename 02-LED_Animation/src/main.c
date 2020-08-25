/*
 * main.c
 *
 *  Created on: Aug 13, 2020
 *      Author: mosad
 */

/* Library includes */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DELAY.h"

/* MCAL includes */
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "SysTick_interface.h"

/* Functions prototypes */
void Led_animation (void);

/* enum to hold the states for the finite state machine */
typedef enum {
	LED_LEFT  ,
	LED_RIGHT ,
	LED_IN    ,
	LED_OUT   ,
	LED_BLINK
}led_state_t;

/* Global variables */
led_state_t Global_state = LED_LEFT ;
u8 Global_offset = 0 ;
u8 Global_timer = 0 ;
u8 Global_counter = 0 ;
u8 up = 4 ;
u8 down = 3 ;

void main(void){

	/* Initialize the RCC to HSE */
	RCC_voidInitSysClock();

	/* Enable the clock for all ports */
	RCC_voidEnableClock(RCC_APB2 , IOPA_PERIPHERAL);

	/* initialize port  as output */
	GPIO_voidSetPortMode(PORTA , GPIO_OUTPUT_10MHZ_PP);

	/* Enable SysTick */
	SysTick_voidInit();
	while(1){

	}

}

/* Interrupt fire every 100 ms */
void SysTick_Handler(void) {
	Global_timer++;
	Led_animation();
}


void Led_animation (void){
	switch (Global_state){

		case LED_LEFT :
			/* will execute every 200 ms */
			if (Global_timer == 2){
				Global_timer = 0 ;
				GPIO_voidsetPinValue(PORTA , Global_offset    , HIGH);
				GPIO_voidsetPinValue(PORTA ,(Global_offset-1) , LOW );
				Global_offset++;
				if (Global_offset > 7){Global_state = LED_RIGHT ;}
			}
			break;

		case LED_RIGHT :
			/* will execute every 200 ms */
			if (Global_timer == 2){
				Global_timer = 0 ;
				Global_offset--;
				GPIO_voidsetPinValue(PORTA , Global_offset     , HIGH);
				GPIO_voidsetPinValue(PORTA , (Global_offset+1) , LOW );
				if (Global_offset == 0){Global_state = LED_LEFT ; Global_counter ++ ;}
				if (Global_counter == 4){
					Global_state = LED_IN ; Global_counter = 0 ;}
			}
			break;

		case LED_IN :
			/* will execute every 200 ms */
			if (Global_timer == 2){
				Global_timer = 0 ;
				GPIO_voidsetPinValue(PORTA , up   , HIGH);
				GPIO_voidsetPinValue(PORTA , down , HIGH );
				up++;
				down--;
				if (up > 7 ){Global_state = LED_OUT ; up = 7; down = 0 ;}
			}
			break ;

		case LED_OUT :
			/* will execute every 200 ms */
			if (Global_timer == 2){
				Global_timer = 0 ;
				GPIO_voidsetPinValue(PORTA , up   , LOW);
				GPIO_voidsetPinValue(PORTA , down , LOW);
				up--;
				down++;
				if (up < 4 ){Global_state = LED_IN ; Global_counter ++ ; up= 4 ; down = 3;}
				if (Global_counter == 4){
					Global_state = LED_BLINK ; Global_counter = 0 ;}
			}
			break ;

		case LED_BLINK :
			/* will execute every 500 ms */
			if (Global_timer == 5){
				Global_timer = 0 ;
				/* Toggle the 8 pins */
				GPIO_voidTogglePinValue(PORTA , 0 );
				GPIO_voidTogglePinValue(PORTA , 1 );
				GPIO_voidTogglePinValue(PORTA , 2 );
				GPIO_voidTogglePinValue(PORTA , 3 );
				GPIO_voidTogglePinValue(PORTA , 4 );
				GPIO_voidTogglePinValue(PORTA , 5 );
				GPIO_voidTogglePinValue(PORTA , 6 );
				GPIO_voidTogglePinValue(PORTA , 7 );
				Global_counter++;
				if(Global_counter > 8 ){
					Global_state = LED_LEFT ; Global_counter = 0 ;
					GPIO_voidWritePins(PORTA , 0 , PINS_8 , 0X00);
				}
			}
			break;
		default :
			/* Should not be here */
			break ;

	}
}





