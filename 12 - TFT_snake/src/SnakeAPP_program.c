/*****************************************************/
 /* Author      : mosad                              */
 /* Version     : v01                                */
 /* date        : 12/9/2020                          */
/*****************************************************/

/* Library includes */
#include <stdio.h>
#include "BIT_MATH.h"
#include "STD_TYPES.h"
#include "Linked_List.h"

/* MCAL */
#include "RCC_interface.h"
#include "GPIO_interface.h"

/* HAL */
#include "TFT_interface.h"
#include "BUTTON_interface.h"

/* Services */
#include "OS_interface.h"

/* Module includes */
#include "SnakeAPP_interface.h"
#include "SnakeAPP_config.h"
#include "SnakeAPP_private.h"

/*************** Global variables ***************/
u8 global_u8Score = 0 ;

/** Snake variables **/
volatile u8 global_u8SnakeHead_X = 1 ;
volatile u8 global_u8SnakeHead_Y = 0 ;
volatile directionState_t global_dirc = TO_RIGHT ;

/** Buttons variables **/
volatile BUTTON_state_t global_UpButton = RELEASED ;
volatile BUTTON_state_t global_DownButton = RELEASED ;
volatile BUTTON_state_t global_RightButton = RELEASED ;
volatile BUTTON_state_t global_LeftButton = RELEASED ;
u8 global_u8UpButtonID = 0 ;
u8 global_u8DownButtonID = 0 ;
u8 global_u8RightButtonID = 0 ;
u8 global_u8LeftButtonID = 0 ;

static Snake_point_t *head ;
static u8 global_u8XHold = 6;
static u8 global_u8YHold = 18;



/************************** Initialization *******************/

void SnakeAPP_voidInitScreen(void)
{
	/* Initialize TFT */
	TFT_voidInit();

	/* Initialize H.W */
	global_u8UpButtonID = BUTTON_voidInit(UP_BUTTON);
	global_u8DownButtonID = BUTTON_voidInit(DOWN_BUTTON);
	global_u8RightButtonID = BUTTON_voidInit(RIGHT_BUTTON);
	global_u8LeftButtonID = BUTTON_voidInit(LEFT_BUTTON);

	/* Set the screen for game to begin */
	SnakeAPP_voidSetInitialScreen();

	/* Create tasks */
	OS_u8CreateTask(0 , 0 , 400 , SnakeAPP_voidUpdateScreen );
	OS_u8CreateTask(1 , 0 , 10 , SnakeAPP_voidCheckInput );

	/* Start OS */
	OS_voidStart();
}


/****************** Tasks *********************/
/*
 * Periodic task every 400 ms
 *
 * */
static void SnakeAPP_voidUpdateScreen(void)
{
	/** Draw snake and food **/
	TFT_voidDrawRectangle(head->x_position , head->y_position , FOOD_SIZE , FOOD_SIZE , TFT_RED );
	TFT_voidDrawRectangle(global_u8XHold, global_u8YHold , SNAKE_SIZE , SNAKE_SIZE , BACKGROUND_1 );
	Snake_point_t *node = '\0' ;
	for(node=head->next; node!= NULL; node=node->next){
		TFT_voidDrawRectangle(node->x_position , node->y_position , SNAKE_SIZE , SNAKE_SIZE , TFT_PURPLE );
	}
	/* Store position of the current tail of snake before shifting */
	global_u8XHold = (head->next)->x_position ;
	global_u8YHold = (head->next)->y_position ;

	SnakeAPP_voidUpdatePosition();
	SnakeApp_voidCheckEatFood();
	SnakeApp_voidCheckGameOver();
}



/*
 * Periodic task every 10 ms
 * check if a button was pressed and check which button then take action
 * */
static void SnakeAPP_voidCheckInput(void){
	BUTTON_voidScane();
	global_UpButton = BUTTON_voidGetState(global_u8UpButtonID);
	global_DownButton = BUTTON_voidGetState(global_u8DownButtonID);
	global_RightButton = BUTTON_voidGetState(global_u8RightButtonID);
	global_LeftButton = BUTTON_voidGetState(global_u8LeftButtonID);
	if ((global_UpButton == PRE_RELEASED) && (global_dirc != TO_DOWN)){
		global_dirc = TO_UP ;
	}
	else if ((global_DownButton == PRE_RELEASED) && (global_dirc != TO_UP)){
		global_dirc = TO_DOWN ;
	}
	else if ((global_RightButton == PRE_RELEASED)&& (global_dirc != TO_LEFT)){
		global_dirc = TO_RIGHT ;
	}
	else if ((global_LeftButton == PRE_RELEASED) && (global_dirc != TO_RIGHT)){
		global_dirc = TO_LEFT ;
	}
	else {

	}
}

/******************* Helper functions **********************/
/* Private Helper functions  */

static void SnakeAPP_voidSetInitialScreen(void)
{
	/* Set background */
	TFT_voidFillDisplay(BACKGROUND_1);

	/* Set score bar and borders */
	TFT_voidDrawRectangle( 0 , 0 , TFT_WIDTH , 18 ,SCORE_BAR);
	TFT_voidDrawRectangle( 0 , 0 , BORDER_THICK , TFT_HIGHT ,BORDERS);
	TFT_voidDrawRectangle( (TFT_WIDTH - BORDER_THICK) , 0 , BORDER_THICK , TFT_HIGHT ,BORDERS);
	TFT_voidDrawRectangle( 0 , (TFT_HIGHT - BORDER_THICK) , TFT_WIDTH , 8 ,BORDERS);
	TFT_voidPrintText("00" , 12 , 0 , 2 , TFT_WHITE ,SCORE_BAR );
	TFT_voidDrawRectangle(45 , 3 , FOOD_SIZE , FOOD_SIZE , TFT_RED );

	/* Create the initial snake */
	appendNode( 5 , 5);   //first dot
	appendNode(global_u8SnakeHead_X-1 , global_u8SnakeHead_Y ); //snake
	appendNode(global_u8SnakeHead_X , global_u8SnakeHead_Y ); //snake

	/* get head of linked list */
	head = getHead();

}

static void SnakeAPP_voidUpdatePosition(void)
{
	switch (global_dirc){
		case TO_DOWN :
			global_u8SnakeHead_Y++;
			if (global_u8SnakeHead_Y >= 11){global_u8SnakeHead_Y = 0 ;}
			break ;

		case TO_UP :
			if (global_u8SnakeHead_Y < 1){global_u8SnakeHead_Y = 11 ;}
			global_u8SnakeHead_Y--;
			break;

		case TO_LEFT :
			if (global_u8SnakeHead_X < 1){global_u8SnakeHead_X = 9 ;}
			global_u8SnakeHead_X--;
			break;
		case TO_RIGHT :
			global_u8SnakeHead_X++;
			if (global_u8SnakeHead_X >= 9){global_u8SnakeHead_X = 0 ;}
			break;

		default :  /* should not be here */ break;

	}
}

static void SnakeApp_voidCheckEatFood(void)
{
	u8 local_u8Flag = updateNodes(global_u8SnakeHead_X , global_u8SnakeHead_Y );
	/* If reached the dot */
	if (local_u8Flag){
		if (global_dirc == TO_DOWN) {
			global_u8SnakeHead_Y++;
			if (global_u8SnakeHead_Y ==9 ){global_u8SnakeHead_Y = 0 ;}
			appendNode(global_u8SnakeHead_X , global_u8SnakeHead_Y);
		}
		else if (global_dirc == TO_UP){
			if (global_u8SnakeHead_Y ==0 ){global_u8SnakeHead_Y = 11 ;}
			else {global_u8SnakeHead_Y--;}
			appendNode(global_u8SnakeHead_X , global_u8SnakeHead_Y);
		}
		else if (global_dirc == TO_LEFT){
			if (global_u8SnakeHead_X == 0 ){global_u8SnakeHead_X = 9 ;}
			else {global_u8SnakeHead_X--;}
			appendNode(global_u8SnakeHead_X , global_u8SnakeHead_Y);
		}
		else if (global_dirc == TO_RIGHT) {
			global_u8SnakeHead_X++;
			if (global_u8SnakeHead_Y ==11 ){global_u8SnakeHead_X = 0 ;}
			appendNode(global_u8SnakeHead_X , global_u8SnakeHead_Y);
		}
		else {

		}
		global_u8Score++;
		s8 local_buffer[3];
		sprintf(local_buffer ,"%.2i" , global_u8Score);
		TFT_voidPrintText( local_buffer, 12 , 0 , 2 , TFT_WHITE ,SCORE_BAR );
	}
}

static void SnakeApp_voidCheckGameOver(void)
{
	u8 local_u8CheckGame = checkForCollison ();
	if (local_u8CheckGame){
		/* Set background */
		TFT_voidFillDisplay(BACKGROUND_1);

		/* Set score bar and borders */
		TFT_voidDrawRectangle( 0 , 0 , TFT_WIDTH , 18 ,SCORE_BAR);
		TFT_voidDrawRectangle( 0 , 0 , BORDER_THICK , TFT_HIGHT ,BORDERS);
		TFT_voidDrawRectangle( (TFT_WIDTH - BORDER_THICK) , 0 , BORDER_THICK , TFT_HIGHT ,BORDERS);
		TFT_voidDrawRectangle( 0 , (TFT_HIGHT - BORDER_THICK) , TFT_WIDTH , 8 ,BORDERS);
		s8 local_buffer[3];
		sprintf(local_buffer ,"%.2i" , global_u8Score);
		TFT_voidPrintText(local_buffer , 12 , 0 , 2 , TFT_WHITE ,SCORE_BAR );
		TFT_voidDrawRectangle(45 , 3 , FOOD_SIZE , FOOD_SIZE , TFT_RED );
		TFT_voidPrintText("Game" , 30 , 50, 3 , TFT_BLACK ,BACKGROUND_1 );
		TFT_voidPrintText("OVER" , 30 , 80 , 3 , TFT_BLACK ,BACKGROUND_1 );
		OS_voidStop();
	}
}





