/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 12/9/2020                          */
/*****************************************************/

/* Library includes */
#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "Linked_List.h"


/* MCAL */
#include "RCC_interface.h"
#include "GPIO_interface.h"

/* HAL */
#include "LEDMRX_interface.h"
#include "REMOTE_interface.h"

/* Services */
#include "OS_interface.h"

/* Module includes */
#include "SnakeAPP_interface.h"
#include "SnakeAPP_config.h"
#include "SnakeAPP_private.h"

/* Global variables */
u8 global_u8Frame[8];
volatile u8 global_u8SnakeHead_X = 0 ;
volatile u8 global_u8SnakeHead_Y = 0 ;
volatile directionState_t global_dirc = TO_RIGHT ;
volatile REMOTE_buttons_t global_button ;

/************************** Initialization *******************/

void SnakeAPP_voidInitSys(void)
{

	/* Initialize the RCC to HSE */
	RCC_voidInitSysClock();
	/* Enable the clock for all ports */
	RCC_voidEnableClock(RCC_APB2 , IOPA_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , IOPB_PERIPHERAL);
	RCC_voidEnableClock(RCC_APB2 , AFIO_PERIPHERAL);

	/* Initialize one led  */
	GPIO_voidSetPinMode(PORTB , 15 , GPIO_OUTPUT_2MHZ_PP);

	/* Initialize H.W */
	LEDMRX_voidInit();
	REMOTE_voidInit();

	/* Create the initial snake */
	appendNode( 5 , 5);   //first dot
	appendNode(global_u8SnakeHead_X , global_u8SnakeHead_Y ); //snake

	/* Create tasks */
	OS_u8CreateTask(1 , 0 , 3000 , LEDMRX_voidDisplay );
	OS_u8CreateTask(2 , 0 , 400000 , SnakeAPP_voidUpdateScreen );
	OS_u8CreateTask(0 , 1000 , 300000 , SnakeAPP_voidCheckRemote );
}

/**********************  Tasks  ************************/

/*
 * Periodic task every 400 ms
 * handle  1 - updating the frame for led matrix
 *         2 - check for collision
 *         3 - check if snake ate a new dot to increase its length
 * */
void SnakeAPP_voidUpdateScreen(void){
	getFrame(global_u8Frame);
	LEDMRX_voidSetData(global_u8Frame);
	u8 local_u8CollisionFlag = checkForCollison();
	if (local_u8CollisionFlag){
		SnakeAPP_voidAbortGame();
	}
	SnakeAPP_voidUpdatePosition();
	u8 local_u8Flag = updateNodes(global_u8SnakeHead_X , global_u8SnakeHead_Y );
	/* If reached the dot */
	if (local_u8Flag){
		if (global_dirc == TO_RIGHT) {
			global_u8SnakeHead_Y++;
			if (global_u8SnakeHead_Y ==8 ){global_u8SnakeHead_Y = 0 ;}
			appendNode(global_u8SnakeHead_X , global_u8SnakeHead_Y);
		}
		else if (global_dirc == TO_LEFT){
			if (global_u8SnakeHead_Y ==0 ){global_u8SnakeHead_Y = 7 ;}
			else {global_u8SnakeHead_Y--;}
			appendNode(global_u8SnakeHead_X , global_u8SnakeHead_Y);
		}
		else if (global_dirc == TO_UP){
			if (global_u8SnakeHead_X == 0 ){global_u8SnakeHead_X = 7 ;}
			else {global_u8SnakeHead_X--;}
			appendNode(global_u8SnakeHead_X , global_u8SnakeHead_Y);
		}
		else if (global_dirc == TO_DOWN) {
			global_u8SnakeHead_X++;
			if (global_u8SnakeHead_Y ==8 ){global_u8SnakeHead_X = 0 ;}
			appendNode(global_u8SnakeHead_X , global_u8SnakeHead_Y);
		}
		else {

		}
	}
}

/*
 * Periodic task every 300 ms
 * check if a button was pressed and check which button then take action
 * */
void SnakeAPP_voidCheckRemote(void){
	global_button = REMOTE_getButton();
	if (global_button < REMOTE_NO_DATA){
		switch (global_button){
			case REMOTE_POWER :
				SnakeAPP_voidStartGameOver();
				break;
			case REMOTE_TWO :
				if (global_dirc != TO_DOWN){
					global_dirc = TO_UP;
				}
				break ;

			case REMOTE_EIGHT :
				if (global_dirc != TO_UP){
					global_dirc = TO_DOWN;
				}
				break;

			case REMOTE_SIX :
				if (global_dirc != TO_LEFT){
					global_dirc = TO_RIGHT;
				}
				break ;

			case REMOTE_FOUR :
				if (global_dirc != TO_RIGHT){
					global_dirc = TO_LEFT;
				}
				break;

			case REMOTE_ONE :
				GPIO_voidTogglePinValue(PORTB , 15);
				break;

			default :
				break;

		}
	}
}

/* Private Helper functions  */

static void SnakeAPP_voidUpdatePosition(void)
{
	switch (global_dirc){
		case TO_RIGHT :
			global_u8SnakeHead_Y++;
			if (global_u8SnakeHead_Y > 7){global_u8SnakeHead_Y = 0 ;}
			break ;

		case TO_LEFT :
			if (global_u8SnakeHead_Y < 1){global_u8SnakeHead_Y = 8 ;}
			global_u8SnakeHead_Y--;
			break;

		case TO_UP :
			if (global_u8SnakeHead_X < 1){global_u8SnakeHead_X = 8 ;}
			global_u8SnakeHead_X--;
			break;
		case TO_DOWN :
			global_u8SnakeHead_X++;
			if (global_u8SnakeHead_X > 7){global_u8SnakeHead_X = 0 ;}
			break;

		default :  /* should not be here */ break;

	}
}

static void SnakeAPP_voidAbortGame(void)
{
	OS_voidSetTaskState(1 , OS_TASK_SUSPENDE);
	OS_voidSetTaskState(2 , OS_TASK_SUSPENDE);
}

static void SnakeAPP_voidStartGameOver (void)
{
	OS_voidSetTaskState(1 , OS_TASK_READY);
	OS_voidSetTaskState(2 , OS_TASK_READY);
	global_u8SnakeHead_X = 0 ;
	global_u8SnakeHead_Y = 0 ;
	restorInitSnake();
}



