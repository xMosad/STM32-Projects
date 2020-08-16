/*
 *  STM32F103 project
 *
 *  Created on : Aug 16, 2020
 *  Author     : mosad
 *               sebha project
 *  Pin layout : SSD pin A0 -> pin A6
 *  			 right SSD selection pin A8
 *  			 left  SSD selection pin A9
 *  			 led pin B0
 *  			 power button pin B7
 *  			 up    button pin B5
 *  			 down  button pin B6
 */

/* Library layer */
#include "STD_TYPES.h"
#include "BIT_MATH.h"
#include "DELAY.h"

/* MCAL layer */
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "GPIO_private.h"

/* HAL layer */
#include "LED_interface.h"
#include "BUTTON_interface.h"
#include "SSD_interface.h"

/* pin layout */
#define   LED_PIN         0
#define   SSD_START_PIN   0
#define   UP_BUTTON       5
#define   DOWN_BUTTON     6
#define   POWER_BUTTON    7

/* system parameters */
#define ON_STATE  1
#define OFF_STATE 0

/*  functions prototypes  */
void sebha_system_init     (void);
void sebha_power_mangement (void);
void sebha_system_on       (void);

/*  public variables   */
LED_id_t       led1 =  {PORTB , LED_PIN      };
SSD_selectID_t SSD1 =  {PORTA , SSD_START_PIN};
u8 system_state = OFF_STATE ;
u8 counter ;
/* buttons id */
u8 power;
u8 up   ;
u8 down ;

void main(void){

	/* Initialize the system */
	sebha_system_init();

	while(1){
		/* The periodic tasks */
		sebha_power_mangement();
		if (system_state == ON_STATE){
			sebha_system_on();
		}
		else {

		}
	}
}

/*
 * Name        : sebha_system_init
 * Description : Initialize the RCC system & enable clock for ports
 * 				 & initialize LED , 3 buttons and a multiplexing SSD
 * 				 at a specific pins
 * */
void sebha_system_init (void){
	/* Initialize the RCC to HSE */
	RCC_voidInitSysClock();

	/* Enbale RCC for portb and porta */
	RCC_voidEnableClock(RCC_APB2 , IOPB_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , IOPA_PERIPHERAL);

	/* initialize led */
	LED_voidInit(led1);

	/* Initialize the SSD */
	SSD_voidInit(SSD1);

	/* initialize the sw */
	power = BUTTON_voidInit(PORTB , POWER_BUTTON);
	up    = BUTTON_voidInit(PORTB , UP_BUTTON   );
	down  = BUTTON_voidInit(PORTB , DOWN_BUTTON );

	/* Enable Pull up  */
	GPIOB_BSRR |= (1 << UP_BUTTON   ) ;
	GPIOB_BSRR |= (1 << DOWN_BUTTON ) ;
	GPIOB_BSRR |= (1 << POWER_BUTTON) ;
}

/*
 * Name        : sebha_power_mangement
 * Description : Execute the periodic function (BUTTON_voidScane) and
 * 				 check on power button every 20 ms , if the power button is pressed
 * 				 the (power_state) toggle to turn system on if system is off or
 * 				 turn it off if system is on.
 * 				 when you turn off system the ssd will be off and led will be off
 * 				 the delay is only added if the system is off because if the system is
 * 				 on the SSD update take 20 ms to execute so the check on butto will
 * 				 be every 20 ms.
 * */
void sebha_power_mangement (void ){
	BUTTON_voidScane();
	/* Get power button state */
	BUTTON_state_t power_state = BUTTON_voidGetState(power);
	if (power_state == PRE_RELEASED){
		system_state = !(system_state) ;
		if (system_state == OFF_STATE){
			/* SSD and LED are OFF */
			GPIO_voidWritePins(SSD1.SSD_portId , SSD1.SSD_startPin , PINS_7 , 0x00);
			LED_voidSetState(led1 , OFF);
			counter = 0 ;
		}
		else{
			/* LED ON */
			LED_voidSetState(led1 , ON);
		}
	}
	else {
		if (system_state == OFF_STATE){
			delay_ms(20);
		}
	}
}

/*
 * Name        : sebha_system_on
 * Description : check on the up and down buttons and change the counter then
 * 				 update the SSD
 * */
void sebha_system_on(void){
	BUTTON_state_t up_state   = BUTTON_voidGetState(up  );
	BUTTON_state_t down_state = BUTTON_voidGetState(down);
	/* Check on state of buttons */
	if      ( (up_state  == PRE_RELEASED) && (counter < 99 )){
		counter++;
	}
	else if ((down_state == PRE_RELEASED) && (counter > 0) ){
		counter--;
				}
	else {
		/* Should not be here */
	}
	SSD_voidUpdateMultiplexing(SSD1 ,counter);
}




